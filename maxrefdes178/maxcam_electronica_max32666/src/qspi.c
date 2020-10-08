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

#define S_MODULE_NAME   "qspi"


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include "spi.h"
#include "tmr_utils.h"

#include "qspi.h"
#include "faceid_definitions.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define QSPI              SPI0
#define QSPI_REGS         MXC_SPI17Y0
#define QSPI_IRQ          SPI0_IRQn


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
uint8_t qspi_image_buff[IMAGE_SIZE];
char video_result_string[RESULT_MAX_SIZE];
char audio_result_string[RESULT_MAX_SIZE];
static volatile int qspi_video_int_flag;
static volatile int qspi_audio_int_flag;
static const gpio_cfg_t ai85_video_int = {PORT_0, PIN_30, GPIO_FUNC_IN, GPIO_PAD_PULL_UP};
static const gpio_cfg_t ai85_video_cs = {PORT_0, PIN_8, GPIO_FUNC_OUT, GPIO_PAD_NONE};
static const gpio_cfg_t ai85_audio_cs = {PORT_0, PIN_14, GPIO_FUNC_OUT, GPIO_PAD_NONE};
static const gpio_cfg_t ai85_audio_int = {PORT_1, PIN_13, GPIO_FUNC_IN, GPIO_PAD_PULL_UP};


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
void SPI0_IRQHandler(void)
{
    SPI_Handler(QSPI);
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

    GPIO_OutSet(&ai85_video_cs);
    GPIO_Config(&ai85_video_cs);

    GPIO_OutSet(&ai85_audio_cs);
    GPIO_Config(&ai85_audio_cs);

    qspi_master_cfg.map = MAP_B;
    qspi_master_cfg.ss0 = Disable;
    qspi_master_cfg.ss1 = Disable;
    qspi_master_cfg.ss2 = Disable;
    qspi_master_cfg.num_io = 4;

    SPI_Shutdown(QSPI);

    // Configure the peripheral
    if (SPI_Init(QSPI, 0, QSPI_SPEED, qspi_master_cfg) != 0) {

        printf("Error configuring QSPI\n");
    }

    qspi_video_int_flag = 0;
    GPIO_Config(&ai85_video_int);
    GPIO_RegisterCallback(&ai85_video_int, qspi_video_int, NULL);
    GPIO_IntConfig(&ai85_video_int, GPIO_INT_EDGE, GPIO_INT_FALLING);
    GPIO_IntEnable(&ai85_video_int);
    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(ai85_video_int.port));

    qspi_audio_int_flag = 0;
    GPIO_Config(&ai85_audio_int);
    GPIO_RegisterCallback(&ai85_audio_int, qspi_audio_int, NULL);
    GPIO_IntConfig(&ai85_audio_int, GPIO_INT_EDGE, GPIO_INT_FALLING);
    GPIO_IntEnable(&ai85_audio_int);
    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(ai85_audio_int.port));

    return E_NO_ERROR;
}

int qspi_worker(void)
{
    if (qspi_video_int_flag) {
        qspi_video_int_flag = 0;
        spi_req_t qspi_req = {0};
        qspi_header_t qspi_header = {0};

        GPIO_OutClr(&ai85_video_cs);
        qspi_req.tx_data = NULL;
        qspi_req.rx_data = (uint8_t *) &qspi_header;
        qspi_req.len = sizeof(qspi_header_t);
        qspi_req.bits = 8;
        qspi_req.width = SPI17Y_WIDTH_4;
        qspi_req.ssel = 0;
        qspi_req.deass = 0;
        qspi_req.ssel_pol = SPI17Y_POL_LOW;
        qspi_req.tx_num = 0;
        qspi_req.rx_num = 0;
        qspi_req.callback = NULL;
        SPI_MasterTrans(QSPI, &qspi_req);
        GPIO_OutSet(&ai85_video_cs);

        if (qspi_header.start_symbol != QSPI_START_SYMBOL) {
            printf("Invalid QSPI start byte 0x%08hhX\n", qspi_header.start_symbol);
            return E_BAD_STATE;
        }

        TMR_Delay(MXC_TMR0, USEC(5), 0);

        if (qspi_header.data_type == QSPI_TYPE_RESPONSE_VIDEO_DATA) {
            if ((qspi_header.data_len == 0) || (qspi_header.data_len > IMAGE_SIZE)) {
                printf("Invalid QSPI data len %u\n", qspi_header.data_len);
                return E_BAD_PARAM;
            }

            GPIO_OutClr(&ai85_video_cs);
            qspi_req.tx_data = NULL;
            qspi_req.rx_data = (void *)qspi_image_buff;
            qspi_req.len = qspi_header.data_len / 2;
            qspi_req.bits = 8;
            qspi_req.width = SPI17Y_WIDTH_4;
            qspi_req.ssel = 0;
            qspi_req.deass = 0;
            qspi_req.ssel_pol = SPI17Y_POL_LOW;
            qspi_req.tx_num = 0;
            qspi_req.rx_num = 0;
            qspi_req.callback = NULL;
            SPI_MasterTrans(QSPI, &qspi_req);
            GPIO_OutSet(&ai85_video_cs);

            TMR_Delay(MXC_TMR0, USEC(5), 0);

            GPIO_OutClr(&ai85_video_cs);
            qspi_req.tx_data = NULL;
            qspi_req.rx_data = (void *)&(qspi_image_buff[qspi_header.data_len/2]);
            qspi_req.len = qspi_header.data_len / 2;
            qspi_req.bits = 8;
            qspi_req.width = SPI17Y_WIDTH_4;
            qspi_req.ssel = 0;
            qspi_req.deass = 0;
            qspi_req.ssel_pol = SPI17Y_POL_LOW;
            qspi_req.tx_num = 0;
            qspi_req.rx_num = 0;
            qspi_req.callback = NULL;
            SPI_MasterTrans(QSPI, &qspi_req);
            GPIO_OutSet(&ai85_video_cs);

            printf("video %u\n", qspi_req.rx_num * 2);

            return QSPI_TYPE_RESPONSE_VIDEO_DATA;
        } else if (qspi_header.data_type == QSPI_TYPE_RESPONSE_VIDEO_RESULT) {
            if ((qspi_header.data_len == 0) || (qspi_header.data_len > sizeof(video_result_string))) {
                printf("Invalid QSPI data len %u\n", qspi_header.data_len);
                return E_BAD_PARAM;
            }

            memset(video_result_string, 0, sizeof(video_result_string));

            GPIO_OutClr(&ai85_video_cs);
            qspi_req.tx_data = NULL;
            qspi_req.rx_data = (void *)video_result_string;
            qspi_req.len = qspi_header.data_len;
            qspi_req.bits = 8;
            qspi_req.width = SPI17Y_WIDTH_4;
            qspi_req.ssel = 0;
            qspi_req.deass = 0;
            qspi_req.ssel_pol = SPI17Y_POL_LOW;
            qspi_req.tx_num = 0;
            qspi_req.rx_num = 0;
            qspi_req.callback = NULL;
            SPI_MasterTrans(QSPI, &qspi_req);
            GPIO_OutSet(&ai85_video_cs);

            printf("video result %u %s\n", qspi_req.rx_num, video_result_string);

            return QSPI_TYPE_RESPONSE_VIDEO_RESULT;
        }
    }

    if (qspi_audio_int_flag) {
        qspi_audio_int_flag = 0;

        spi_req_t qspi_req = {0};
        qspi_header_t qspi_header = {0};

        GPIO_OutClr(&ai85_audio_cs);
        qspi_req.tx_data = NULL;
        qspi_req.rx_data = (uint8_t *) &qspi_header;
        qspi_req.len = sizeof(qspi_header_t);
        qspi_req.bits = 8;
        qspi_req.width = SPI17Y_WIDTH_4;
        qspi_req.ssel = 0;
        qspi_req.deass = 0;
        qspi_req.ssel_pol = SPI17Y_POL_LOW;
        qspi_req.tx_num = 0;
        qspi_req.rx_num = 0;
        qspi_req.callback = NULL;
        SPI_MasterTrans(QSPI, &qspi_req);
        GPIO_OutSet(&ai85_audio_cs);

        if (qspi_header.start_symbol != QSPI_START_SYMBOL) {
            printf("Invalid QSPI start byte 0x%08hhX\n", qspi_header.start_symbol);
            return E_BAD_STATE;
        }

        TMR_Delay(MXC_TMR0, USEC(5), 0);

        if (qspi_header.data_type == QSPI_TYPE_RESPONSE_AUDIO_RESULT) {
            memset(audio_result_string, 0, sizeof(audio_result_string));

            if ((qspi_header.data_len == 0) || (qspi_header.data_len > sizeof(audio_result_string))) {
                printf("Invalid QSPI data len %u\n", qspi_header.data_len);
                return E_BAD_PARAM;
            }

            GPIO_OutClr(&ai85_audio_cs);
            qspi_req.tx_data = NULL;
            qspi_req.rx_data = (void *)audio_result_string;
            qspi_req.len = qspi_header.data_len;
            qspi_req.bits = 8;
            qspi_req.width = SPI17Y_WIDTH_4;
            qspi_req.ssel = 0;
            qspi_req.deass = 0;
            qspi_req.ssel_pol = SPI17Y_POL_LOW;
            qspi_req.tx_num = 0;
            qspi_req.rx_num = 0;
            qspi_req.callback = NULL;
            SPI_MasterTrans(QSPI, &qspi_req);
            GPIO_OutSet(&ai85_audio_cs);

            printf("audio result %u %s\n", qspi_req.rx_num, audio_result_string);

            return QSPI_TYPE_RESPONSE_AUDIO_RESULT;
        }
    }

    return QSPI_TYPE_NO_DATA;
}
