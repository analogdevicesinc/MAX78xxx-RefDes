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
#include <spi.h>
#include <stdio.h>
#include <string.h>

#include "lcd.h"
#include "max32666_debug.h"
#include "maxcam_definitions.h"
#include "qspi.h"
#include "spi_dma.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "qspi"


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
const gpio_cfg_t video_int_pin = MAX32666_VIDEO_INT_PIN;
const gpio_cfg_t video_cs_pin  = MAX32666_VIDEO_CS_PIN;
const gpio_cfg_t audio_int_pin = MAX32666_AUDIO_INT_PIN;
const gpio_cfg_t audio_cs_pin  = MAX32666_AUDIO_CS_PIN;

static volatile int qspi_video_int_flag;
static volatile int qspi_audio_int_flag;


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
void MAX32666_QSPI_DMA_CHANNEL_IRQ_HAND(void)
{
    spi_dma_int_handler(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
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

    GPIO_OutSet(&video_cs_pin);
    GPIO_Config(&video_cs_pin);

    GPIO_OutSet(&audio_cs_pin);
    GPIO_Config(&audio_cs_pin);

    qspi_master_cfg.map = MAX32666_QSPI_MAP;
    qspi_master_cfg.ss0 = Disable;
    qspi_master_cfg.ss1 = Disable;
    qspi_master_cfg.ss2 = Disable;
    qspi_master_cfg.num_io = 4;

    spi_dma_master_init(MAX32666_QSPI, MAX32666_QSPI_ID, qspi_master_cfg, QSPI_SPEED, 1);

    NVIC_EnableIRQ(MAX32666_QSPI_DMA_CHANNEL_IRQ);

    qspi_video_int_flag = 0;
    GPIO_Config(&video_int_pin);
    GPIO_RegisterCallback(&video_int_pin, qspi_video_int, NULL);
    GPIO_IntConfig(&video_int_pin, MAX32666_VIDEO_INT_MODE);
    GPIO_IntEnable(&video_int_pin);
    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(video_int_pin.port));

    qspi_audio_int_flag = 0;
    GPIO_Config(&audio_int_pin);
    GPIO_RegisterCallback(&audio_int_pin, qspi_audio_int, NULL);
    GPIO_IntConfig(&audio_int_pin, MAX32666_AUDIO_INT_MODE);
    GPIO_IntEnable(&audio_int_pin);
    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(audio_int_pin.port));

    return E_NO_ERROR;
}

int qspi_worker(void)
{
    if (qspi_video_int_flag) {
        qspi_video_int_flag = 0;
        qspi_header_t qspi_header = {0};

        GPIO_OutClr(&video_cs_pin);
        spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, NULL, (uint8_t *) &qspi_header, sizeof(qspi_header_t), MAX32666_QSPI_DMA_REQSEL_SPIRX, NULL);
        spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
        GPIO_OutSet(&video_cs_pin);

        if (qspi_header.start_symbol != QSPI_START_SYMBOL) {
            PR_ERROR("Invalid QSPI start byte 0x%08hhX", qspi_header.start_symbol);
            return E_BAD_STATE;
        }

        while(!qspi_video_int_flag);
        qspi_video_int_flag = 0;

        if (qspi_header.data_type == QSPI_TYPE_RESPONSE_VIDEO_DATA) {
            if ((qspi_header.data_len == 0) || (qspi_header.data_len > LCD_DATA_SIZE)) {
                PR_ERROR("Invalid QSPI data len %u", qspi_header.data_len);
                return E_BAD_PARAM;
            }

            GPIO_OutClr(&video_cs_pin);
            spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, NULL, (void *)lcd_data, qspi_header.data_len, MAX32666_QSPI_DMA_REQSEL_SPIRX, NULL);
            spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
            GPIO_OutSet(&video_cs_pin);

            PR_INFO("video %u", qspi_header.data_len);

            return QSPI_TYPE_RESPONSE_VIDEO_DATA;
        } else if (qspi_header.data_type == QSPI_TYPE_RESPONSE_VIDEO_RESULT) {
            if ((qspi_header.data_len == 0) || (qspi_header.data_len > sizeof(lcd_subtitle))) {
                PR_ERROR("Invalid QSPI data len %u", qspi_header.data_len);
                return E_BAD_PARAM;
            }

            memset(lcd_subtitle, 0, sizeof(lcd_subtitle));

            GPIO_OutClr(&video_cs_pin);
            spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, NULL, (void *)lcd_subtitle, qspi_header.data_len, MAX32666_QSPI_DMA_REQSEL_SPIRX, NULL);
            spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
            GPIO_OutSet(&video_cs_pin);

            PR_INFO("video result %u %s", qspi_header.data_len, lcd_subtitle);

            return QSPI_TYPE_RESPONSE_VIDEO_RESULT;
        }
    }

    if (qspi_audio_int_flag) {
        qspi_audio_int_flag = 0;

        qspi_header_t qspi_header = {0};

        GPIO_OutClr(&audio_cs_pin);
        spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, NULL, (uint8_t *) &qspi_header, sizeof(qspi_header_t), MAX32666_QSPI_DMA_REQSEL_SPIRX, NULL);
        spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
        GPIO_OutSet(&audio_cs_pin);

        if (qspi_header.start_symbol != QSPI_START_SYMBOL) {
            PR_ERROR("Invalid QSPI start byte 0x%08hhX", qspi_header.start_symbol);
            return E_BAD_STATE;
        }

        while(!qspi_audio_int_flag);
        qspi_audio_int_flag = 0;

        if (qspi_header.data_type == QSPI_TYPE_RESPONSE_AUDIO_RESULT) {
            memset(lcd_toptitle, 0, sizeof(lcd_toptitle));

            if ((qspi_header.data_len == 0) || (qspi_header.data_len > sizeof(lcd_toptitle))) {
                PR_ERROR("Invalid QSPI data len %u", qspi_header.data_len);
                return E_BAD_PARAM;
            }

            GPIO_OutClr(&audio_cs_pin);
            spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, NULL, (void *)lcd_toptitle, qspi_header.data_len, MAX32666_QSPI_DMA_REQSEL_SPIRX, NULL);
            spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
            GPIO_OutSet(&audio_cs_pin);

            PR_INFO("audio result %u %s", qspi_header.data_len, lcd_toptitle);

            return QSPI_TYPE_RESPONSE_AUDIO_RESULT;
        }
    }

    return QSPI_TYPE_NO_DATA;
}
