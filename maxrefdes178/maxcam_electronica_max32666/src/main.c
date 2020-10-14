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
#include "tmr_utils.h"
#include "i2c.h"

#include "max20303.h"
#include "board.h"
#include "config.h"
#include "qspi.h"
#include "lcd.h"
#include "lcd_data.h"
#include "faceid_definitions.h"
#include "maxcam_debug.h"
#include "version.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "main"


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
int main(void)
{
    int qspi_return = 0;
    uint16_t resultColor = 0;
    uint16_t frameColor = 0;
    uint8_t runFaceId = 0;
    char version[10] = {0};
    uint8_t cmdData[1] = {0};

    snprintf(version, sizeof(version) - 1, "v%d.%d.%d", S_VERSION_MAJOR, S_VERSION_MINOR, S_VERSION_BUILD);
    PR_INFO("maxcam_electronica_max32666 %s", version);

    if (MAX20303_initialize(1) != E_NO_ERROR) {
        PR_ERROR("pmic init failed");
        while(1);
    }

    /* Switch USB-TYpe-C Debug Connection to MAX78000-Video */
    cmdData[0] = 0xff;
    if (I2C_MasterWrite(PMIC_I2C, 0xd8, cmdData, 1, 0) ) {
        PR_ERROR("MAX78000 select failed");
    }

    // Set PORT1 and PORT2 rail to VDDIO
    MXC_GPIO0->vssel =  0x00;
    MXC_GPIO1->vssel =  0x00;

    // Initialize DMA peripheral
    DMA_Init();

    if (qspi_init(1) != E_NO_ERROR) {
        PR_ERROR("qspi init failed");
        while(1);
    }

    if (lcd_init() != E_NO_ERROR) {
        PR_ERROR("lcd init failed");
        while(1);
    }

    PR_INFO("init completed");

    MAX20303_led_green(MAX20303_LED_OUTPUT_ON);

    // Print logo and version
    lcd_drawImage(0, 0, LCD_WIDTH, LCD_HEIGHT, image_data_rsz_maxim_logo);
    lcd_writeStringWithBG(60, 210, version, Font_16x26, RED, WHITE);

    int audio_result_print_cnt = 0;
    while (1) {
        qspi_return = qspi_worker();
        if (qspi_return > QSPI_TYPE_NO_DATA) {
            switch(qspi_return)
            {
                case QSPI_TYPE_RESPONSE_VIDEO_DATA:
                    if (runFaceId) {
                        if (strcmp(video_result_string, "nothing")) {
                            fonts_putSubtitle(LCD_WIDTH, LCD_HEIGHT, video_result_string, Font_16x26, resultColor, qspi_image_buff);
                        } else {
                            frameColor = WHITE;
                        }

                        fonts_drawRectangle(LCD_WIDTH, LCD_HEIGHT, 60, 40, 180, 200, frameColor, qspi_image_buff);
                        fonts_drawRectangle(LCD_WIDTH, LCD_HEIGHT, 59, 39, 181, 201, frameColor, qspi_image_buff);
                        fonts_drawRectangle(LCD_WIDTH, LCD_HEIGHT, 58, 38, 182, 202, BLACK, qspi_image_buff);
                        fonts_drawRectangle(LCD_WIDTH, LCD_HEIGHT, 57, 37, 183, 203, BLACK, qspi_image_buff);
                    }

                    if (audio_result_print_cnt) {
                        fonts_putToptitle(LCD_WIDTH, LCD_HEIGHT, audio_result_string, Font_16x26, YELLOW, qspi_image_buff);
                        audio_result_print_cnt--;
                    }

                    lcd_drawImage(0, 0, LCD_WIDTH, LCD_HEIGHT, qspi_image_buff);
                    break;
                case QSPI_TYPE_RESPONSE_VIDEO_RESULT:
                    if (strcmp(video_result_string, "Unknown") == 0) {
                        resultColor = RED;
                        frameColor = RED;
                    } else if(strcmp(video_result_string, "Adjust Face") == 0) {
                        resultColor = YELLOW;
                        frameColor = YELLOW;
                    } else {
                        resultColor = GREEN;
                        frameColor = GREEN;
                    }
                    break;
                case QSPI_TYPE_RESPONSE_AUDIO_RESULT:
                    audio_result_print_cnt = 10;

                    if (strcmp(audio_result_string, "OFF") == 0) {
                        lcd_backlight(0);
                        audio_result_print_cnt = 0;
                    } else if(strcmp(audio_result_string, "ON") == 0) {
                        lcd_backlight(1);
                        audio_result_print_cnt = 0;
                    } else if (strcmp(audio_result_string, "GO") == 0) {
                        runFaceId = 1;
                    } else if(strcmp(audio_result_string, "STOP") == 0) {
                        runFaceId = 0;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}
