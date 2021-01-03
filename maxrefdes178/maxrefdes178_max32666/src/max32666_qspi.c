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
#include <gpio.h>
#include <string.h>

#include "max32666_debug.h"
#include "max32666_data.h"
#include "max32666_lcd.h"
#include "max32666_qspi.h"
#include "max32666_spi_dma.h"
#include "maxrefdes178_definitions.h"


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
static const mxc_gpio_cfg_t video_int_pin = MAX32666_VIDEO_INT_PIN;
static const mxc_gpio_cfg_t video_cs_pin  = MAX32666_VIDEO_CS_PIN;
static const mxc_gpio_cfg_t audio_int_pin = MAX32666_AUDIO_INT_PIN;
static const mxc_gpio_cfg_t audio_cs_pin  = MAX32666_AUDIO_CS_PIN;

static volatile int qspi_video_int_flag = 0;
static volatile int qspi_audio_int_flag = 0;

static qspi_packet_header_t qspi_packet_header = {0};


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
void MAX32666_QSPI_DMA_IRQ_HAND(void)
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

int qspi_init(void)
{
    GPIO_SET(video_cs_pin);
    MXC_GPIO_Config(&video_cs_pin);

    GPIO_SET(audio_cs_pin);
    MXC_GPIO_Config(&audio_cs_pin);

    spi_dma_master_init(MAX32666_QSPI, MAX32666_QSPI_MAP, QSPI_SPEED, 1);

    NVIC_EnableIRQ(MAX32666_QSPI_DMA_IRQ);

    qspi_video_int_flag = 0;
    MXC_GPIO_Config(&video_int_pin);
    MXC_GPIO_RegisterCallback(&video_int_pin, qspi_video_int, NULL);
    MXC_GPIO_IntConfig(&video_int_pin, MAX32666_VIDEO_INT_MODE);
    MXC_GPIO_EnableInt(video_int_pin.port, video_int_pin.mask);
    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(MXC_GPIO_GET_IDX(video_int_pin.port)));

    qspi_audio_int_flag = 0;
    MXC_GPIO_Config(&audio_int_pin);
    MXC_GPIO_RegisterCallback(&audio_int_pin, qspi_audio_int, NULL);
    MXC_GPIO_IntConfig(&audio_int_pin, MAX32666_AUDIO_INT_MODE);
    MXC_GPIO_EnableInt(audio_int_pin.port, audio_int_pin.mask);
    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(MXC_GPIO_GET_IDX(audio_int_pin.port)));

    return E_NO_ERROR;
}

qspi_status_e qspi_worker(qspi_packet_type_e *qspi_packet_type)
{
    if (qspi_video_int_flag) {
        qspi_video_int_flag = 0;

        GPIO_CLR(video_cs_pin);
        spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, NULL, (uint8_t *) &qspi_packet_header, sizeof(qspi_packet_header_t), MAX32666_QSPI_DMA_REQSEL_SPIRX, NULL);
        spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
        GPIO_SET(video_cs_pin);
        *qspi_packet_type = qspi_packet_header.packet_type;

        if (qspi_packet_header.start_symbol != QSPI_START_SYMBOL) {
            PR_ERROR("Invalid QSPI start byte 0x%08hhX", qspi_packet_header.start_symbol);
            return QSPI_STATUS_ERROR_RX;
        }

        while(!qspi_video_int_flag);
        qspi_video_int_flag = 0;

        switch(qspi_packet_header.packet_type) {
        case QSPI_PACKET_TYPE_VIDEO_DATA_RES:
            if (qspi_packet_header.packet_size != LCD_DATA_SIZE) {
                PR_ERROR("Invalid QSPI data len %u", qspi_packet_header.packet_size);
                return QSPI_STATUS_ERROR_RX;
            }

            GPIO_CLR(video_cs_pin);
            spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, NULL, (void *)lcd_data.buffer, qspi_packet_header.packet_size, MAX32666_QSPI_DMA_REQSEL_SPIRX, NULL);
            spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
            GPIO_SET(video_cs_pin);

            PR_DEBUG("video %u", qspi_packet_header.packet_size);

            return QSPI_STATUS_SUCCESS_RX;
            break;
        case QSPI_PACKET_TYPE_VIDEO_CLASSIFICATION_RES:
            if (qspi_packet_header.packet_size != sizeof(classification_result_t)) {
                PR_ERROR("Invalid QSPI data len %u", qspi_packet_header.packet_size);
                return QSPI_STATUS_ERROR_RX;
            }

            GPIO_CLR(video_cs_pin);
            spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, NULL, (uint8_t *) &device_status.classification_video, qspi_packet_header.packet_size, MAX32666_QSPI_DMA_REQSEL_SPIRX, NULL);
            spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
            GPIO_SET(video_cs_pin);

            PR_INFO("video result %s %d %0.1f", device_status.classification_video.result, device_status.classification_video.classification, (double)device_status.classification_video.probabily);

            return QSPI_STATUS_SUCCESS_RX;
            break;
        case QSPI_PACKET_TYPE_VIDEO_STATISTICS_RES:
            if (qspi_packet_header.packet_size != sizeof(max78000_statistics_t)) {
                PR_ERROR("Invalid QSPI data len %u", qspi_packet_header.packet_size);
                return QSPI_STATUS_ERROR_RX;
            }

            GPIO_CLR(video_cs_pin);
            spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, NULL, (uint8_t *) &device_status.statistics.max78000_video, qspi_packet_header.packet_size, MAX32666_QSPI_DMA_REQSEL_SPIRX, NULL);
            spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
            GPIO_SET(video_cs_pin);

            PR_DEBUG("video capture : %lu", device_status.statistics.max78000_video.capture_duration_us);
            PR_DEBUG("video cnn     : %lu", device_status.statistics.max78000_video.cnn_duration_us);
            PR_DEBUG("video qspi    : %lu", device_status.statistics.max78000_video.communication_duration_us);
            PR_DEBUG("video total   : %lu", device_status.statistics.max78000_video.total_duration_us);

            return QSPI_STATUS_SUCCESS_RX;
            break;
        case QSPI_PACKET_TYPE_VIDEO_VERSION_RES:
            if (qspi_packet_header.packet_size != sizeof(version_t)) {
                PR_ERROR("Invalid QSPI data len %u", qspi_packet_header.packet_size);
                return QSPI_STATUS_ERROR_RX;
            }

            GPIO_CLR(video_cs_pin);
            spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, NULL, (uint8_t *) &device_info.device_version.max78000_video, qspi_packet_header.packet_size, MAX32666_QSPI_DMA_REQSEL_SPIRX, NULL);
            spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
            GPIO_SET(video_cs_pin);

            PR_INFO("MAX78000 Video v%d.%d.%d", device_info.device_version.max78000_video.major, device_info.device_version.max78000_video.minor, device_info.device_version.max78000_video.build);

            return QSPI_STATUS_SUCCESS_RX;
            break;
        case QSPI_PACKET_TYPE_VIDEO_SERIAL_RES:
            if (qspi_packet_header.packet_size != sizeof(serial_num_t)) {
                PR_ERROR("Invalid QSPI data len %u", qspi_packet_header.packet_size);
                return QSPI_STATUS_ERROR_RX;
            }

            GPIO_CLR(video_cs_pin);
            spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, NULL, (uint8_t *) &device_info.device_serial_num.max78000_video, qspi_packet_header.packet_size, MAX32666_QSPI_DMA_REQSEL_SPIRX, NULL);
            spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
            GPIO_SET(video_cs_pin);

            PR_INFO("MAX78000 Video serial number: ");
            for (int i = 0; i < sizeof(device_info.device_serial_num.max78000_video); i++) {
                PR("%02X", device_info.device_serial_num.max78000_video[i]);
            }
            PR("\n");

            return QSPI_STATUS_SUCCESS_RX;
            break;
        case QSPI_PACKET_TYPE_VIDEO_FACEID_EMBED_UPDATE_RES:
            if (qspi_packet_header.packet_size != sizeof(faceid_embed_update_status_e)) {
                PR_ERROR("Invalid QSPI data len %u", qspi_packet_header.packet_size);
                return QSPI_STATUS_ERROR_RX;
            }

            GPIO_CLR(video_cs_pin);
            spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, NULL, (uint8_t *) &device_status.faceid_embed_update_status, qspi_packet_header.packet_size, MAX32666_QSPI_DMA_REQSEL_SPIRX, NULL);
            spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
            GPIO_SET(video_cs_pin);

            PR_INFO("FaceID embeddings update status: %d", device_status.faceid_embed_update_status);

            return QSPI_STATUS_SUCCESS_RX;
            break;
        default:
            PR_ERROR("Unknown qspi video packet");
            return QSPI_STATUS_ERROR_RX;
            break;
        }
    }

    if (qspi_audio_int_flag) {
        qspi_audio_int_flag = 0;

        GPIO_CLR(audio_cs_pin);
        spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, NULL, (uint8_t *) &qspi_packet_header, sizeof(qspi_packet_header_t), MAX32666_QSPI_DMA_REQSEL_SPIRX, NULL);
        spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
        GPIO_SET(audio_cs_pin);
        *qspi_packet_type = qspi_packet_header.packet_type;

        if (qspi_packet_header.start_symbol != QSPI_START_SYMBOL) {
            PR_ERROR("Invalid QSPI start byte 0x%08hhX", qspi_packet_header.start_symbol);
            return QSPI_STATUS_ERROR_RX;
        }

        while(!qspi_audio_int_flag);
        qspi_audio_int_flag = 0;

        switch(qspi_packet_header.packet_type) {
        case QSPI_PACKET_TYPE_AUDIO_CLASSIFICATION_RES:
            if (qspi_packet_header.packet_size != sizeof(classification_result_t)) {
                PR_ERROR("Invalid QSPI data len %u", qspi_packet_header.packet_size);
                return QSPI_STATUS_ERROR_RX;
            }

            GPIO_CLR(audio_cs_pin);
            spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, NULL, (uint8_t *) &device_status.classification_audio, qspi_packet_header.packet_size, MAX32666_QSPI_DMA_REQSEL_SPIRX, NULL);
            spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
            GPIO_SET(audio_cs_pin);

            PR_INFO("audio result %s %d %0.1f", device_status.classification_audio.result, device_status.classification_audio.classification, (double)device_status.classification_audio.probabily);

            return QSPI_STATUS_SUCCESS_RX;
            break;
        case QSPI_PACKET_TYPE_AUDIO_STATISTICS_RES:
            if (qspi_packet_header.packet_size != sizeof(max78000_statistics_t)) {
                PR_ERROR("Invalid QSPI data len %u", qspi_packet_header.packet_size);
                return QSPI_STATUS_ERROR_RX;
            }

            GPIO_CLR(audio_cs_pin);
            spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, NULL, (uint8_t *) &device_status.statistics.max78000_audio, qspi_packet_header.packet_size, MAX32666_QSPI_DMA_REQSEL_SPIRX, NULL);
            spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
            GPIO_SET(audio_cs_pin);

            PR_DEBUG("audio cnn: %lu", device_status.statistics.max78000_audio.cnn_duration_us);

            return QSPI_STATUS_SUCCESS_RX;
            break;
        case QSPI_PACKET_TYPE_AUDIO_VERSION_RES:
            if (qspi_packet_header.packet_size != sizeof(version_t)) {
                PR_ERROR("Invalid QSPI data len %u", qspi_packet_header.packet_size);
                return QSPI_STATUS_ERROR_RX;
            }

            GPIO_CLR(audio_cs_pin);
            spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, NULL, (uint8_t *) &device_info.device_version.max78000_audio, qspi_packet_header.packet_size, MAX32666_QSPI_DMA_REQSEL_SPIRX, NULL);
            spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
            GPIO_SET(audio_cs_pin);

            PR_INFO("MAX78000 Audio v%d.%d.%d", device_info.device_version.max78000_audio.major, device_info.device_version.max78000_audio.minor, device_info.device_version.max78000_audio.build);

            return QSPI_STATUS_SUCCESS_RX;
            break;
        case QSPI_PACKET_TYPE_AUDIO_SERIAL_RES:
            if (qspi_packet_header.packet_size != sizeof(serial_num_t)) {
                PR_ERROR("Invalid QSPI data len %u", qspi_packet_header.packet_size);
                return QSPI_STATUS_ERROR_RX;
            }

            GPIO_CLR(audio_cs_pin);
            spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, NULL, (uint8_t *) &device_info.device_serial_num.max78000_audio, qspi_packet_header.packet_size, MAX32666_QSPI_DMA_REQSEL_SPIRX, NULL);
            spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
            GPIO_SET(audio_cs_pin);

            PR_INFO("MAX78000 Audio serial number: ");
            for (int i = 0; i < sizeof(device_info.device_serial_num.max78000_audio); i++) {
                PR("%02X", device_info.device_serial_num.max78000_audio[i]);
            }
            PR("\n");

            return QSPI_STATUS_SUCCESS_RX;
            break;
        default:
            PR_ERROR("Unknown qspi audio packet");
            return QSPI_STATUS_ERROR_RX;
            break;
        }
    }

    return QSPI_STATUS_IDLE;
}
