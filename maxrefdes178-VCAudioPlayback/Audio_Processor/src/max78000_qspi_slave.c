/*******************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All rights Reserved.
 *
 * This software is protected by copyright laws of the United States and
 * of foreign countries. This material may also be protected by patent laws
 * and technology transfer regulations of the United States and of foreign
 * countries. This software is furnished under a license agreement and/or a
 * nondisclosure agreement and may only be used or reproduced in accordance
 * with the terms of those agreements. Dissemination of this information to
 * any party or parties not specified in the license agreement and/or
 * nondisclosure agreement is expressly prohibited.
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <board.h>
#include <mxc.h>
#include <spi.h>
#include <stdint.h>
#include <stdio.h>

#include "max78000_debug.h"
#include "max78000_qspi_slave.h"
#include "maxrefdes178_definitions.h"
#include "maxrefdes178_utility.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "qspi"

#define SPI_DMA_COUNTER_MAX  0xffff


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
static volatile uint8_t *g_tx_data = NULL;
static volatile uint8_t *g_rx_data = NULL;
static volatile uint32_t g_tx_data_size = 0;
static volatile uint32_t g_rx_data_size = 0;
static volatile qspi_packet_header_t g_qspi_packet_header_tx = {0};
static volatile qspi_state_e g_qspi_state_tx = QSPI_STATE_IDLE;
static volatile qspi_packet_header_t g_qspi_packet_header_rx = {0};
static volatile qspi_state_e g_qspi_state_rx = QSPI_STATE_IDLE;

#if defined(MAXREFDES178_MAX78000_AUDIO)
static const mxc_gpio_cfg_t qspi_int_pin = MAX78000_AUDIO_HOST_INT_PIN;
static const mxc_gpio_cfg_t qspi_cs_pin  = MAX78000_AUDIO_HOST_CS_PIN;
static const mxc_gpio_cfg_t qspi_rw_pin  = MAX78000_AUDIO_HOST_IO_PIN;
static const uint8_t qspi_ch             = MAX78000_AUDIO_QSPI_DMA_CHANNEL;
static mxc_spi_regs_t *qspi              = MAX78000_AUDIO_QSPI;
#elif defined(MAXREFDES178_MAX78000_VIDEO)
static const mxc_gpio_cfg_t qspi_int_pin = MAX78000_VIDEO_HOST_INT_PIN;
static const mxc_gpio_cfg_t qspi_cs_pin  = MAX78000_VIDEO_HOST_CS_PIN;
static const mxc_gpio_cfg_t qspi_rw_pin  = MAX78000_VIDEO_HOST_IO_PIN;
static const uint8_t qspi_ch             = MAX78000_VIDEO_QSPI_DMA_CHANNEL;
static mxc_spi_regs_t *qspi              = MAX78000_VIDEO_QSPI;
#else
#error MAX78000 AUDIO or MAX78000 VIDEO flag should be set
#endif


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static int qspi_slave_dma(uint8_t *tx_data, uint8_t *rx_data, uint32_t data_size);
static int qspi_slave_wait_tx(qspi_state_e qspi_state);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
#if defined(MAXREFDES178_MAX78000_AUDIO)
void MAX78000_AUDIO_QSPI_DMA_IRQ_HAND(void)
#else
void MAX78000_VIDEO_QSPI_DMA_IRQ_HAND(void)
#endif
{
    if (MXC_DMA->intfl & (0x1 << qspi_ch)) {
        if (MXC_DMA->ch[qspi_ch].status & (MXC_F_DMA_STATUS_TO_IF | MXC_F_DMA_STATUS_BUS_ERR)) {
            PR_ERROR("dma error %d", MXC_DMA->ch[qspi_ch].status);
        }

        if (MXC_DMA->ch[qspi_ch].status & MXC_F_DMA_STATUS_RLD_IF) {
            // reload interrupt, do nothing
        } else {
            if (MXC_DMA->ch[qspi_ch].cnt) {
                PR_ERROR("dma is not empty %d", MXC_DMA->ch[qspi_ch].cnt);
            }
        }

        // Clear DMA int flags
        MXC_DMA->ch[qspi_ch].status = MXC_DMA->ch[qspi_ch].status;
    }
}

void qspi_slave_cs_handler(void *cbdata)
{
    if (MXC_GPIO_InGet(qspi_cs_pin.port, qspi_cs_pin.mask)) {
        // CS deassert

        // Stop DMA and SPI
        MXC_DMA->ch[qspi_ch].ctrl &= ~MXC_F_DMA_CTRL_EN;
        qspi->ctrl0 &= ~MXC_F_SPI_CTRL0_EN;
        qspi->dma = (MXC_F_SPI_DMA_TX_FLUSH | MXC_F_SPI_DMA_RX_FLUSH);

        if (qspi_rw_pin.port->in & qspi_rw_pin.mask) {
            // TX request
            switch (g_qspi_state_tx) {
            case QSPI_STATE_CS_ASSERTED_HEADER:
                if (g_qspi_packet_header_tx.info.packet_size) {
                    g_qspi_state_tx = QSPI_STATE_CS_DEASSERTED_HEADER;
                } else {
                    // Completed if no data to send
                    g_qspi_state_tx = QSPI_STATE_COMPLETED;
                }
                break;
            case QSPI_STATE_CS_ASSERTED_DATA:
                g_qspi_state_tx = QSPI_STATE_COMPLETED;
                break;
            default:
                PR_ERROR("invalid tx state %d", g_qspi_state_tx);
                break;
            }
        } else {
            // RX request
            switch (g_qspi_state_rx) {
            case QSPI_STATE_CS_ASSERTED_HEADER:
                if (g_qspi_packet_header_rx.start_symbol != QSPI_START_SYMBOL) {
                    PR_ERROR("Invalid start %x", g_qspi_packet_header_rx.start_symbol);
                    g_qspi_state_rx = QSPI_STATE_IDLE;
                } else if (g_qspi_packet_header_rx.header_crc16 != crc16_sw((uint8_t *) &g_qspi_packet_header_rx.info, sizeof(g_qspi_packet_header_rx.info))) {
                    PR_ERROR("Invalid header crc %x", g_qspi_packet_header_rx.start_symbol);
                    g_qspi_state_rx = QSPI_STATE_IDLE;
                } else if (g_qspi_packet_header_rx.info.packet_size) {
                    g_qspi_state_rx = QSPI_STATE_CS_DEASSERTED_HEADER;
                } else {
                    // Completed if no data to send
                    g_qspi_state_rx = QSPI_STATE_COMPLETED;
                }
                break;
            case QSPI_STATE_CS_ASSERTED_DATA:
                if (g_qspi_packet_header_rx.payload_crc16 != crc16_sw((uint8_t *) g_rx_data, g_rx_data_size)) {
                    PR_ERROR("Invalid payload crc %x", g_qspi_packet_header_rx.payload_crc16);
                    g_qspi_state_rx = QSPI_STATE_IDLE;
                } else {
                    g_qspi_state_rx = QSPI_STATE_COMPLETED;
                }
                break;
            default:
                PR_ERROR("invalid rx state %d", g_qspi_state_rx);
                break;
            }
        }
    } else {
        // CS assert

        if (qspi_rw_pin.port->in & qspi_rw_pin.mask) {
            // TX request
            switch (g_qspi_state_tx) {
            case QSPI_STATE_STARTED:
                g_qspi_state_tx = QSPI_STATE_CS_ASSERTED_HEADER;
                qspi_slave_dma((uint8_t *) &g_qspi_packet_header_tx, NULL, sizeof(g_qspi_packet_header_tx));
                break;
            case QSPI_STATE_CS_DEASSERTED_HEADER:
                if (g_tx_data) {
                    qspi_slave_dma((uint8_t *) g_tx_data, NULL, g_tx_data_size);
                    g_tx_data = NULL;
                    g_tx_data_size = 0;
                    g_qspi_state_tx = QSPI_STATE_CS_ASSERTED_DATA;
                } else {
                    PR_ERROR("no data to tx");
                    g_qspi_state_tx = QSPI_STATE_IDLE;
                }
                break;
            default:
                PR_ERROR("invalid tx state %d", g_qspi_state_tx);
                break;
            }
        } else {
            // RX request
            switch (g_qspi_state_rx) {
            case QSPI_STATE_IDLE:
                qspi_slave_dma(NULL, (uint8_t *) &g_qspi_packet_header_rx, sizeof(g_qspi_packet_header_rx));
                g_qspi_state_rx = QSPI_STATE_CS_ASSERTED_HEADER;
                break;
            case QSPI_STATE_CS_DEASSERTED_HEADER:
                if (g_rx_data) {
                    qspi_slave_dma(NULL, (uint8_t *) g_rx_data, g_rx_data_size);
                    g_qspi_state_rx = QSPI_STATE_CS_ASSERTED_DATA;
                } else {
                    PR_ERROR("no data to rx");
                    g_qspi_state_rx = QSPI_STATE_IDLE;
                }
                break;
            default:
                PR_ERROR("invalid rx state %d", g_qspi_state_rx);
                break;
            }
        }
    }
}

int qspi_slave_init(void)
{
    int ret;

    mxc_spi_pins_t qspi_pins;
    qspi_pins.clock = TRUE;
    qspi_pins.miso = TRUE;
    qspi_pins.mosi = TRUE;
    qspi_pins.sdio2 = TRUE;
    qspi_pins.sdio3 = TRUE;
    qspi_pins.ss0 = TRUE;
    qspi_pins.ss1 = FALSE;
    qspi_pins.ss2 = FALSE;
    qspi_pins.vddioh = FALSE;

    ret = MXC_SPI_Init(qspi, 0, 1, 0, 0, 0, qspi_pins);
    if (ret != E_NO_ERROR) {
        PR_ERROR("MXC_SPI_Init fail %d", ret);
        return ret;
    }

    // Host interrupt output pin
    GPIO_SET(qspi_int_pin);
    MXC_GPIO_Config(&qspi_int_pin);

    // Host cs interrupt input pin
    MXC_GPIO_Config(&qspi_cs_pin);
    MXC_GPIO_RegisterCallback(&qspi_cs_pin, qspi_slave_cs_handler, NULL);
    MXC_GPIO_IntConfig(&qspi_cs_pin, MXC_GPIO_INT_BOTH);
    MXC_GPIO_EnableInt(qspi_cs_pin.port, qspi_cs_pin.mask);
    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(MXC_GPIO_GET_IDX(qspi_cs_pin.port)));

    // Host read/write input pin
    MXC_GPIO_Config(&qspi_rw_pin);

    // Set data size
    MXC_SETFIELD(qspi->ctrl2, MXC_F_SPI_CTRL2_NUMBITS, (8 << MXC_F_SPI_CTRL2_NUMBITS_POS));

    ret = MXC_SPI_SetWidth(qspi, SPI_WIDTH_QUAD);
    if (ret != E_NO_ERROR) {
        PR_ERROR("MXC_SPI_SetWidth fail");
        return ret;
    }

#if defined(MAXREFDES178_MAX78000_AUDIO)
    NVIC_SetVector(MAX78000_AUDIO_QSPI_DMA_IRQ, MAX78000_AUDIO_QSPI_DMA_IRQ_HAND);
    NVIC_EnableIRQ(MAX78000_AUDIO_QSPI_DMA_IRQ);
#else
    NVIC_EnableIRQ(MAX78000_VIDEO_QSPI_DMA_IRQ);
#endif

//    MXC_LP_EnableGPIOWakeup((mxc_gpio_cfg_t *)&qspi_cs_pin);

    return E_NO_ERROR;
}

static int qspi_slave_dma(uint8_t *tx_data, uint8_t *rx_data, uint32_t data_size)
{
    // Stop SPI
    qspi->ctrl0 &= ~(MXC_F_SPI_CTRL0_EN);

    // Setup SPI
    qspi->ctrl1 = 0;

    // Number of characters to transmit from TX or RX FIFO.
    if (tx_data) {
        if (data_size > SPI_DMA_COUNTER_MAX) {
            MXC_SETFIELD(qspi->ctrl1, MXC_F_SPI_CTRL1_TX_NUM_CHAR, (SPI_DMA_COUNTER_MAX << MXC_F_SPI_CTRL1_TX_NUM_CHAR_POS));
        } else {
            MXC_SETFIELD(qspi->ctrl1, MXC_F_SPI_CTRL1_TX_NUM_CHAR, (data_size << MXC_F_SPI_CTRL1_TX_NUM_CHAR_POS));
        }
    }
    if (rx_data) {
        if (data_size > SPI_DMA_COUNTER_MAX) {
            MXC_SETFIELD(qspi->ctrl1, MXC_F_SPI_CTRL1_RX_NUM_CHAR, (SPI_DMA_COUNTER_MAX << MXC_F_SPI_CTRL1_RX_NUM_CHAR_POS));
        } else {
            MXC_SETFIELD(qspi->ctrl1, MXC_F_SPI_CTRL1_RX_NUM_CHAR, (data_size << MXC_F_SPI_CTRL1_RX_NUM_CHAR_POS));
        }
    }

    // TX FIFO Enabled, clear TX and RX FIFO
    if (tx_data) {
        qspi->dma = (MXC_F_SPI_DMA_TX_FIFO_EN |
                    MXC_F_SPI_DMA_TX_FLUSH |
                    MXC_F_SPI_DMA_RX_FLUSH);
    }
    if (rx_data) {
        qspi->dma = (MXC_F_SPI_DMA_RX_FIFO_EN |
                    MXC_F_SPI_DMA_TX_FLUSH |
                    MXC_F_SPI_DMA_RX_FLUSH);
    }

    // Set SPI DMA TX and RX Thresholds
    if (tx_data) {
        MXC_SETFIELD(qspi->dma, MXC_F_SPI_DMA_TX_THD_VAL, (4 << MXC_F_SPI_DMA_TX_THD_VAL_POS));
        MXC_SETFIELD(qspi->dma, MXC_F_SPI_DMA_RX_THD_VAL, (0 << MXC_F_SPI_DMA_RX_THD_VAL_POS));
    }
    if (rx_data) {
        MXC_SETFIELD(qspi->dma, MXC_F_SPI_DMA_TX_THD_VAL, (0 << MXC_F_SPI_DMA_TX_THD_VAL_POS));
        MXC_SETFIELD(qspi->dma, MXC_F_SPI_DMA_RX_THD_VAL, (0 << MXC_F_SPI_DMA_RX_THD_VAL_POS));
    }

    // Enable SPI
    qspi->ctrl0 |= (MXC_F_SPI_CTRL0_EN);

    // Enable SPI DMA
    if (tx_data) {
        qspi->dma |= MXC_F_SPI_DMA_DMA_TX_EN;
    }
    if (rx_data) {
        qspi->dma |= MXC_F_SPI_DMA_DMA_RX_EN;
    }

    // Clear DMA int flags
    MXC_DMA->ch[qspi_ch].status = MXC_DMA->ch[qspi_ch].status;

    // Enable SRC increment, set request, set source and destination width, enable channel, Count-To-Zero int enable
    if (tx_data) {
        MXC_DMA->ch[qspi_ch].ctrl = (MXC_F_DMA_CTRL_SRCINC |
                                MXC_S_DMA_CTRL_REQUEST_SPI0TX |
                                MXC_S_DMA_CTRL_SRCWD_WORD |
                                MXC_S_DMA_CTRL_SRCWD_WORD |
                                MXC_F_DMA_CTRL_CTZ_IE);
    }
    if (rx_data) {
        MXC_DMA->ch[qspi_ch].ctrl = (MXC_F_DMA_CTRL_DSTINC |
                                MXC_S_DMA_CTRL_REQUEST_SPI0RX |
                                MXC_S_DMA_CTRL_SRCWD_WORD |
                                MXC_S_DMA_CTRL_SRCWD_WORD |
                                MXC_F_DMA_CTRL_CTZ_IE);
    }

    // Set DMA source, destination, counter
    MXC_DMA->ch[qspi_ch].cnt = data_size;
    MXC_DMA->ch[qspi_ch].cntrld = 0;

    if (tx_data) {
        MXC_DMA->ch[qspi_ch].src = (unsigned int) tx_data;
        MXC_DMA->ch[qspi_ch].dst = 0;
    }
    if (rx_data) {
        MXC_DMA->ch[qspi_ch].src = 0;
        MXC_DMA->ch[qspi_ch].dst = (unsigned int) rx_data;
    }

    // if too big, set reload registers
    if (data_size > SPI_DMA_COUNTER_MAX) {
        MXC_DMA->ch[qspi_ch].cnt = SPI_DMA_COUNTER_MAX;
        if (tx_data) {
            MXC_DMA->ch[qspi_ch].srcrld = (unsigned int) (tx_data + SPI_DMA_COUNTER_MAX);
            MXC_DMA->ch[qspi_ch].dstrld = 0;
        }
        if (rx_data) {
            MXC_DMA->ch[qspi_ch].srcrld = 0;
            MXC_DMA->ch[qspi_ch].dstrld = (unsigned int) (rx_data + SPI_DMA_COUNTER_MAX);
        }
        MXC_DMA->ch[qspi_ch].cntrld = data_size - SPI_DMA_COUNTER_MAX;
    }

    // Enable DMA int
    MXC_DMA->inten |= (1 << qspi_ch);

    // Enable DMA
    if (data_size > SPI_DMA_COUNTER_MAX) {
        MXC_DMA->ch[qspi_ch].ctrl |= (MXC_F_DMA_CTRL_EN | MXC_F_DMA_CTRL_RLDEN);
    } else {
        MXC_DMA->ch[qspi_ch].ctrl |= MXC_F_DMA_CTRL_EN;
    }

    return E_NO_ERROR;
}

int qspi_slave_trigger(void)
{
    // Send interrupt to master
    MXC_GPIO_OutClr(qspi_int_pin.port, qspi_int_pin.mask);
    MXC_GPIO_OutSet(qspi_int_pin.port, qspi_int_pin.mask);

    return E_NO_ERROR;
}

int qspi_slave_wait_rx(void)
{
    uint32_t cnt = SPI_TIMEOUT_CNT;

    while((g_qspi_state_rx != QSPI_STATE_COMPLETED) &&
          (g_qspi_state_rx != QSPI_STATE_IDLE) && cnt) {
        cnt--;
    }

    if (g_qspi_state_rx == QSPI_STATE_IDLE) {
        PR_WARN("rx fail");
        return E_BAD_STATE;
    }

    if (cnt == 0) {
        PR_WARN("timeout");
        return E_TIME_OUT;
    }

    return E_NO_ERROR;
}

static int qspi_slave_wait_tx(qspi_state_e qspi_state)
{
    uint32_t cnt = SPI_TIMEOUT_CNT * 10;

    while((g_qspi_state_tx != qspi_state) && cnt) {
        cnt--;
    }

    if (cnt == 0) {
        PR_WARN("timeout %d", qspi_state);
        return E_TIME_OUT;
    }

    return E_NO_ERROR;
}

int qspi_slave_send_packet(uint8_t *data, uint32_t data_size, uint8_t data_type)
{
    if (g_qspi_state_tx != QSPI_STATE_IDLE) {
        PR_WARN("qspi is not idle %d", g_qspi_state_tx);
        return E_BUSY;
    }

    if (g_qspi_state_rx != QSPI_STATE_IDLE) {
        PR_WARN("qspi is not idle %d", g_qspi_state_rx);
        return E_BUSY;
    }

    PR_DEBUG("spi tx started %d", data_type);

    g_qspi_packet_header_tx.start_symbol = QSPI_START_SYMBOL;
    g_qspi_packet_header_tx.info.packet_size = data_size;
    g_qspi_packet_header_tx.info.packet_type = data_type;
    g_qspi_packet_header_tx.header_crc16 = crc16_sw((uint8_t *) &g_qspi_packet_header_tx.info, sizeof(g_qspi_packet_header_tx.info));
//    g_qspi_packet_header_tx.payload_crc16 = crc16_sw(data, data_size);
    g_tx_data = data;
    g_tx_data_size = data_size;

    g_qspi_state_tx = QSPI_STATE_STARTED;

    // Send header
    qspi_slave_trigger();

    if (data_size) {
        if (qspi_slave_wait_tx(QSPI_STATE_CS_DEASSERTED_HEADER) != E_NO_ERROR) {
            g_qspi_state_tx = QSPI_STATE_IDLE;
            PR_ERROR("wait fail %d %d", data_size, data_type);

            return E_BAD_STATE;
        }

        // Send data
        qspi_slave_trigger();
    }

    if (qspi_slave_wait_tx(QSPI_STATE_COMPLETED) != E_NO_ERROR) {
        g_qspi_state_tx = QSPI_STATE_IDLE;
        PR_ERROR("wait fail %d %d", data_size, data_type);
        return E_BAD_STATE;
    }

    g_qspi_state_tx = QSPI_STATE_IDLE;

    PR_DEBUG("spi tx completed %d", data_type);

    return E_NO_ERROR;
}

void qspi_slave_set_rx_state(qspi_state_e rx_state)
{
    g_qspi_state_rx = rx_state;
}

qspi_state_e qspi_slave_get_rx_state(void)
{
    return g_qspi_state_rx;
}

qspi_packet_header_t qspi_slave_get_rx_header(void)
{
    return g_qspi_packet_header_rx;
}

int qspi_slave_set_rx_data(uint8_t *data, uint32_t data_size)
{
    g_rx_data = data;
    g_rx_data_size = data_size;

    return E_NO_ERROR;
}
