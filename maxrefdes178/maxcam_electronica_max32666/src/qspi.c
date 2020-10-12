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
#include <stdio.h>
#include <string.h>
#include "spi.h"
#include "tmr_utils.h"

#include "config.h"
#include "qspi.h"
#include "faceid_definitions.h"
#include "maxcam_debug.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "qspi"

#define SPI_DMA_COUNTER_MAX  0xffff
#define QSPI_TIMEOUT_CNT     100000000


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
const gpio_cfg_t ai85_video_int_pin = {PORT_0, PIN_30, GPIO_FUNC_IN, GPIO_PAD_PULL_UP};
const gpio_cfg_t ai85_video_cs_pin  = {PORT_0, PIN_8, GPIO_FUNC_OUT, GPIO_PAD_NONE};
const gpio_cfg_t ai85_audio_cs_pin  = {PORT_0, PIN_14, GPIO_FUNC_OUT, GPIO_PAD_NONE};
const gpio_cfg_t ai85_audio_int_pin = {PORT_1, PIN_13, GPIO_FUNC_IN, GPIO_PAD_PULL_UP};

uint8_t qspi_image_buff[IMAGE_SIZE];
char video_result_string[RESULT_MAX_SIZE];
char audio_result_string[RESULT_MAX_SIZE];
static volatile int qspi_video_int_flag;
static volatile int qspi_audio_int_flag;
static volatile uint8_t dma_done_flag = 0;


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static void qspi_dma_master_rx(uint8_t* in, unsigned int len);
static int qspi_dma_wait(void);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
void QSPI_DMA_CHANNEL_IRQ_HAND(void)
{
    int ch = QSPI_DMA_CHANNEL;
    if (MXC_DMA->intr & (0x1 << ch)) {
        if (MXC_DMA->ch[ch].st & (MXC_F_DMA_ST_TO_ST | MXC_F_DMA_ST_BUS_ERR)) {
            PR_ERROR("dma error %d", MXC_DMA->ch[ch].st);
        }

        if (MXC_DMA->ch[ch].st & MXC_F_DMA_ST_RLD_ST) {
            // reload occurred, do nothing
        } else {
            if (MXC_DMA->ch[ch].cnt) {
                PR_WARN("dma is not empty %d", MXC_DMA->ch[ch].cnt);
            }

//            // Stop DMA
//            MXC_DMA->ch[ch].cfg &= ~MXC_F_DMA_CFG_CHEN;
//
//            // Stop SPI
//            MXC_SPI->ctrl0 &= ~MXC_F_SPI17Y_CTRL0_EN;
//
//            // Disable SPI DMA, flush FIFO
//            MXC_SPI->dma = (MXC_F_SPI17Y_DMA_TX_FIFO_CLEAR | MXC_F_SPI17Y_DMA_RX_FIFO_CLEAR);

            dma_done_flag = 1;
        }

        // Clear DMA int flags
        MXC_DMA->ch[ch].st =  MXC_DMA->ch[ch].st;
    }
}

static void qspi_dma_master_rx(uint8_t* in, unsigned int len)
{
    int ch = QSPI_DMA_CHANNEL;

    // Initialize the CTRL1 register
    QSPI->ctrl1 = 0;

    // Set how many to characters to send, or when in quad mode how many characters to receive
    if (len > SPI_DMA_COUNTER_MAX) {
        QSPI->ctrl1 |= (SPI_DMA_COUNTER_MAX << MXC_F_SPI17Y_CTRL1_RX_NUM_CHAR_POS);
    } else {
        QSPI->ctrl1 |= (len << MXC_F_SPI17Y_CTRL1_RX_NUM_CHAR_POS);
    }

    // RX FIFO Enabled, clear TX and RX FIFO
    QSPI->dma = (MXC_F_SPI17Y_DMA_RX_DMA_EN |
                    MXC_F_SPI17Y_DMA_TX_FIFO_CLEAR |
                    MXC_F_SPI17Y_DMA_RX_FIFO_CLEAR);

    // Set SPI DMA TX and RX Thresholds
    QSPI->dma |= (0 << MXC_F_SPI17Y_DMA_TX_FIFO_LEVEL_POS);

    // Enable SPI DMA
    QSPI->dma |= MXC_F_SPI17Y_DMA_RX_FIFO_EN;

    // Setup DMA
    dma_done_flag = 0;

    // Clear DMA int flags
    MXC_DMA->ch[ch].st =  MXC_DMA->ch[ch].st;

    // Prepare DMA to fill TX FIFO.
    MXC_DMA->ch[ch].cfg = (MXC_F_DMA_CFG_DSTINC |
                           QSPI_DMA_REQSEL_SPIRX |
                           (MXC_V_DMA_CFG_SRCWD_BYTE << MXC_F_DMA_CFG_SRCWD_POS) |
                           (MXC_V_DMA_CFG_SRCWD_BYTE << MXC_F_DMA_CFG_DSTWD_POS) |
                           MXC_F_DMA_CFG_CHDIEN |
                           MXC_F_DMA_CFG_CTZIEN);

    // Set DMA source, destination, counter
    MXC_DMA->ch[ch].cnt = len;
    MXC_DMA->ch[ch].src = 0;
    MXC_DMA->ch[ch].dst = (unsigned int) in;

    // if too big, set reload registers
    if (len > SPI_DMA_COUNTER_MAX) {
        MXC_DMA->ch[ch].cnt = SPI_DMA_COUNTER_MAX;
        MXC_DMA->ch[ch].src_rld = 0;
        MXC_DMA->ch[ch].dst_rld = (unsigned int) (in + SPI_DMA_COUNTER_MAX);
        MXC_DMA->ch[ch].cnt_rld = len - SPI_DMA_COUNTER_MAX;
    }

    // Enable DMA int
    MXC_DMA->cn |= (1 << ch);

    // Enable DMA
    if (len > SPI_DMA_COUNTER_MAX) {
        MXC_DMA->ch[ch].cfg |= (MXC_F_DMA_CFG_CHEN | MXC_F_DMA_CFG_RLDEN);
    } else {
        MXC_DMA->ch[ch].cfg |= MXC_F_DMA_CFG_CHEN;
    }

    // Start the SPI transaction
    QSPI->ctrl0 |= (MXC_F_SPI17Y_CTRL0_EN | MXC_F_SPI17Y_CTRL0_START);
}

static int qspi_dma_wait(void)
{
    uint32_t cnt = QSPI_TIMEOUT_CNT;

    while(!dma_done_flag && cnt) {
        cnt--;
    }

    if (cnt == 0) {
        PR_ERROR("timeout");
        return 1;
    }

    return 0;
}


void qspi_video_int(void *cbdata)
{
    qspi_video_int_flag = 1;
}

void qspi_audio_int(void *cbdata)
{
    qspi_audio_int_flag = 1;
}

int qspi_init()
{
    sys_cfg_spi_t qspi_master_cfg;

    GPIO_OutSet(&ai85_video_cs_pin);
    GPIO_Config(&ai85_video_cs_pin);

    GPIO_OutSet(&ai85_audio_cs_pin);
    GPIO_Config(&ai85_audio_cs_pin);

    qspi_master_cfg.map = MAP_B;
    qspi_master_cfg.ss0 = Disable;
    qspi_master_cfg.ss1 = Disable;
    qspi_master_cfg.ss2 = Disable;
    qspi_master_cfg.num_io = 4;

    SPI_Shutdown(QSPI_ID);

    // Configure the peripheral
    if (SPI_Init(QSPI_ID, 0, QSPI_SPEED, qspi_master_cfg) != 0) {
        PR_ERROR("Error configuring QSPI_ID");
    }

    // Switch SPI to master mode, else the state of the SS pin could cause the SPI periph to appear 'BUSY';
    QSPI->ctrl0 |= MXC_F_SPI17Y_CTRL0_MASTER;

    // Initialize the CTRL2 register
    QSPI->ctrl2 = (MXC_S_SPI17Y_CTRL2_DATA_WIDTH_QUAD) |
                      (8 << MXC_F_SPI17Y_CTRL2_NUMBITS_POS);

    QSPI->ss_time = (4 << MXC_F_SPI17Y_SS_TIME_PRE_POS) |
                        (8 << MXC_F_SPI17Y_SS_TIME_POST_POS) |
                        (16 << MXC_F_SPI17Y_SS_TIME_INACT_POS);

    NVIC_EnableIRQ(QSPI_DMA_CHANNEL_IRQ);

    qspi_video_int_flag = 0;
    GPIO_Config(&ai85_video_int_pin);
    GPIO_RegisterCallback(&ai85_video_int_pin, qspi_video_int, NULL);
    GPIO_IntConfig(&ai85_video_int_pin, GPIO_INT_EDGE, GPIO_INT_FALLING);
    GPIO_IntEnable(&ai85_video_int_pin);
    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(ai85_video_int_pin.port));

    qspi_audio_int_flag = 0;
    GPIO_Config(&ai85_audio_int_pin);
    GPIO_RegisterCallback(&ai85_audio_int_pin, qspi_audio_int, NULL);
    GPIO_IntConfig(&ai85_audio_int_pin, GPIO_INT_EDGE, GPIO_INT_FALLING);
    GPIO_IntEnable(&ai85_audio_int_pin);
    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(ai85_audio_int_pin.port));

    return E_NO_ERROR;
}

int qspi_worker(void)
{
    if (qspi_video_int_flag) {
        qspi_video_int_flag = 0;
        qspi_header_t qspi_header = {0};

        GPIO_OutClr(&ai85_video_cs_pin);
        qspi_dma_master_rx((uint8_t *) &qspi_header, sizeof(qspi_header_t));
        qspi_dma_wait();
        GPIO_OutSet(&ai85_video_cs_pin);

        if (qspi_header.start_symbol != QSPI_START_SYMBOL) {
            PR_ERROR("Invalid QSPI_ID start byte 0x%08hhX", qspi_header.start_symbol);
            return E_BAD_STATE;
        }

        while(!qspi_video_int_flag);
        qspi_video_int_flag = 0;

        if (qspi_header.data_type == QSPI_TYPE_RESPONSE_VIDEO_DATA) {
            if ((qspi_header.data_len == 0) || (qspi_header.data_len > IMAGE_SIZE)) {
                PR_ERROR("Invalid QSPI_ID data len %u", qspi_header.data_len);
                return E_BAD_PARAM;
            }

            GPIO_OutClr(&ai85_video_cs_pin);
            qspi_dma_master_rx((void *)qspi_image_buff, qspi_header.data_len / 2);
            qspi_dma_wait();
            qspi_dma_master_rx((void *)&(qspi_image_buff[qspi_header.data_len/2]), qspi_header.data_len / 2);
            qspi_dma_wait();
            GPIO_OutSet(&ai85_video_cs_pin);

            PR_INFO("video %u", qspi_header.data_len);

            return QSPI_TYPE_RESPONSE_VIDEO_DATA;
        } else if (qspi_header.data_type == QSPI_TYPE_RESPONSE_VIDEO_RESULT) {
            if ((qspi_header.data_len == 0) || (qspi_header.data_len > sizeof(video_result_string))) {
                PR_ERROR("Invalid QSPI_ID data len %u", qspi_header.data_len);
                return E_BAD_PARAM;
            }

            memset(video_result_string, 0, sizeof(video_result_string));

            GPIO_OutClr(&ai85_video_cs_pin);
            qspi_dma_master_rx((void *)video_result_string, qspi_header.data_len);
            qspi_dma_wait();
            GPIO_OutSet(&ai85_video_cs_pin);

            PR_INFO("video result %u %s", qspi_header.data_len, video_result_string);

            return QSPI_TYPE_RESPONSE_VIDEO_RESULT;
        }
    }

    if (qspi_audio_int_flag) {
        qspi_audio_int_flag = 0;

        qspi_header_t qspi_header = {0};

        GPIO_OutClr(&ai85_audio_cs_pin);
        qspi_dma_master_rx((uint8_t *) &qspi_header, sizeof(qspi_header_t));
        qspi_dma_wait();
        GPIO_OutSet(&ai85_audio_cs_pin);

        if (qspi_header.start_symbol != QSPI_START_SYMBOL) {
            PR_ERROR("Invalid QSPI_ID start byte 0x%08hhX", qspi_header.start_symbol);
            return E_BAD_STATE;
        }

        while(!qspi_audio_int_flag);
        qspi_audio_int_flag = 0;

        if (qspi_header.data_type == QSPI_TYPE_RESPONSE_AUDIO_RESULT) {
            memset(audio_result_string, 0, sizeof(audio_result_string));

            if ((qspi_header.data_len == 0) || (qspi_header.data_len > sizeof(audio_result_string))) {
                PR_ERROR("Invalid QSPI_ID data len %u", qspi_header.data_len);
                return E_BAD_PARAM;
            }

            GPIO_OutClr(&ai85_audio_cs_pin);
            qspi_dma_master_rx((void *)audio_result_string, qspi_header.data_len);
            qspi_dma_wait();
            GPIO_OutSet(&ai85_audio_cs_pin);

            PR_INFO("audio result %u %s", qspi_header.data_len, audio_result_string);

            return QSPI_TYPE_RESPONSE_AUDIO_RESULT;
        }
    }

    return QSPI_TYPE_NO_DATA;
}
