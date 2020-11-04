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

#define S_MODULE_NAME   "spi_dma"


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdint.h>
#include <mxc.h>
#include "board.h"
#include "spi.h"
#include "spi_dma.h"

#include "maxcam_debug.h"
#include "faceid_definitions.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define SPI_DMA_COUNTER_MAX  0xffff
#define SPI_TIMEOUT_CNT      100000000


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
static volatile uint8_t dma_busy_flag[MXC_DMA_CHANNELS] = {0};


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
void spi_dma_int_handler(uint8_t ch, mxc_spi_regs_t *spi)
{
    if (MXC_DMA->intfl & (0x1 << ch)) {
        if (MXC_DMA->ch[ch].status & (MXC_F_DMA_STATUS_TO_IF | MXC_F_DMA_STATUS_BUS_ERR)) {
            PR_ERROR("dma error %d", MXC_DMA->ch[ch].status);
        }

        if (MXC_DMA->ch[ch].status & MXC_F_DMA_STATUS_RLD_IF) {
            // reload interrupt, do nothing
        } else {
            if (MXC_DMA->ch[ch].cnt) {
                PR_ERROR("dma is not empty %d", MXC_DMA->ch[ch].cnt);
            }

            // Stop DMA
            MXC_DMA->ch[ch].ctrl &= ~MXC_F_DMA_CTRL_EN;

            // Stop SPI
            spi->ctrl0 &= ~MXC_F_SPI_CTRL0_EN;

            // Disable SPI DMA, flush FIFO
            spi->dma = (MXC_F_SPI_DMA_TX_FLUSH | MXC_F_SPI_DMA_RX_FLUSH);

            dma_busy_flag[ch] = 0;
        }

        // Clear DMA int flags
        MXC_DMA->ch[ch].status = MXC_DMA->ch[ch].status;
    }
}


void spi_dma_slave_init(mxc_spi_regs_t *spi, mxc_spi_pins_t spi_pins)
{
    if (MXC_SPI_Init(spi, 0, 1, 0, 0, 0, spi_pins) != E_NO_ERROR) {
        PR_ERROR("SPI INITIALIZATION ERROR");
    }

    // Set data size
    MXC_SETFIELD(spi->ctrl2, MXC_F_SPI_CTRL2_NUMBITS, (8 << MXC_F_SPI_CTRL2_NUMBITS_POS));

    if (MXC_SPI_SetWidth(spi, SPI_WIDTH_QUAD) != E_NO_ERROR) {
        PR_ERROR("SPI SET WIDTH ERROR");
    }
}

void spi_dma_tx(uint8_t ch, mxc_spi_regs_t *spi, uint8_t *data, uint32_t len, mxc_gpio_cfg_t *spi_int)
{
    // Setup SPI
    spi->ctrl1 = 0;

    // Number of characters to transmit from TX FIFO.
    if (len > SPI_DMA_COUNTER_MAX) {
        MXC_SETFIELD(spi->ctrl1, MXC_F_SPI_CTRL1_TX_NUM_CHAR, (SPI_DMA_COUNTER_MAX << MXC_F_SPI_CTRL1_TX_NUM_CHAR_POS));
    } else {
        MXC_SETFIELD(spi->ctrl1, MXC_F_SPI_CTRL1_TX_NUM_CHAR, (len << MXC_F_SPI_CTRL1_TX_NUM_CHAR_POS));
    }

    // TX FIFO Enabled, clear TX and RX FIFO
    spi->dma = (MXC_F_SPI_DMA_TX_FIFO_EN |
                MXC_F_SPI_DMA_TX_FLUSH |
                MXC_F_SPI_DMA_RX_FLUSH);


    // Set SPI DMA TX and RX Thresholds
    MXC_SETFIELD(spi->dma, MXC_F_SPI_DMA_TX_THD_VAL, (4 << MXC_F_SPI_DMA_TX_THD_VAL_POS));
    MXC_SETFIELD(spi->dma, MXC_F_SPI_DMA_RX_THD_VAL, (0 << MXC_F_SPI_DMA_RX_THD_VAL_POS));

    // Enable SPI DMA
    spi->dma |= MXC_F_SPI_DMA_DMA_TX_EN;

    // Enable SPI
    spi->ctrl0 |= (MXC_F_SPI_CTRL0_EN);

    // Setup DMA
    dma_busy_flag[ch] = 1;

    // Clear DMA int flags
    MXC_DMA->ch[ch].status = MXC_DMA->ch[ch].status;

    // Enable SRC increment, set request, set source and destination width, enable channel, Count-To-Zero int enable
    MXC_DMA->ch[ch].ctrl = (MXC_F_DMA_CTRL_SRCINC |
                            MXC_S_DMA_CTRL_REQUEST_SPI0TX |
                            MXC_S_DMA_CTRL_SRCWD_WORD |
                            MXC_S_DMA_CTRL_SRCWD_WORD |
                            MXC_F_DMA_CTRL_CTZ_IE);

    // Set DMA source, destination, counter
    MXC_DMA->ch[ch].cnt = len;
    MXC_DMA->ch[ch].src = (unsigned int) data;
    MXC_DMA->ch[ch].dst = 0;

    // if too big, set reload registers
    if (len > SPI_DMA_COUNTER_MAX) {
        MXC_DMA->ch[ch].cnt = SPI_DMA_COUNTER_MAX;
        MXC_DMA->ch[ch].srcrld = (unsigned int) (data + SPI_DMA_COUNTER_MAX);
        MXC_DMA->ch[ch].dstrld = 0;
        MXC_DMA->ch[ch].cntrld = len - SPI_DMA_COUNTER_MAX;
    }

    // Enable DMA int
    MXC_DMA->inten |= (1 << ch);

    // Enable DMA
    if (len > SPI_DMA_COUNTER_MAX) {
        MXC_DMA->ch[ch].ctrl |= (MXC_F_DMA_CTRL_EN | MXC_F_DMA_CTRL_RLDEN);
    } else {
        MXC_DMA->ch[ch].ctrl |= MXC_F_DMA_CTRL_EN;
    }

    // Send interrupt to master
    spi_int->port->out_clr = spi_int->mask;
    spi_int->port->out_set = spi_int->mask;
}

int spi_dma_wait(uint8_t ch)
{
    uint32_t cnt = SPI_TIMEOUT_CNT;

    while(dma_busy_flag[ch] && cnt) {
        cnt--;
    }

    if (cnt == 0) {
        PR_WARN("timeout");
        return 1;
    }

    return 0;
}

void spi_dma_send_packet(uint8_t ch, mxc_spi_regs_t *spi, uint8_t *data, uint32_t len, uint8_t data_type, mxc_gpio_cfg_t *spi_int)
{
    qspi_header_t header;
    header.start_symbol = QSPI_START_SYMBOL;
    header.data_len = len;
    header.data_type = data_type;

    if (dma_busy_flag[ch]) {
        PR_ERROR("dma is busy");
    }

    PR_INFO("spi tx started %d", data_type);

    spi_dma_tx(ch, spi, (uint8_t*) &header, sizeof(qspi_header_t), spi_int);
    spi_dma_wait(ch);

    spi_dma_tx(ch, spi, data, len, spi_int);
//    spi_dma_wait(ch);

    PR_INFO("spi tx completed %d", data_type);
}
