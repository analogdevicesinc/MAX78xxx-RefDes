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
#include <core1.h>
#include <i2c.h>
#include <max20303.h>
#include <stdio.h>
#include <string.h>
#include <tmr_utils.h>

#include "ble.h"
#include "lcd.h"
#include "lcd_data.h"
#include "max32666_debug.h"
#include "maxrefdes178_definitions.h"
#include "qspi.h"
#include "sdcard.h"
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
    int ret = 0;
    uint16_t resultColor = 0;
    uint16_t frameColor = 0;
    uint8_t runFaceId = 0;
    char version[10] = {0};
    uint8_t cmdData[1] = {0};

    sys_cfg_sema_t sema_cfg = NULL;
    SEMA_Init(sema_cfg);

    snprintf(version, sizeof(version) - 1, "v%d.%d.%d", S_VERSION_MAJOR, S_VERSION_MINOR, S_VERSION_BUILD);
    PR_INFO("maxrefdes178_max32666 core0 %s [%s]", version, S_BUILD_TIMESTAMP);

    ret = MAX20303_initialize(1);
    if (ret != E_NO_ERROR) {
        PR_ERROR("MAX20303_initialize failed %d", ret);
        while(1);
    }

    /* Switch USB-TYpe-C Debug Connection to MAX78000-Video */
    cmdData[0] = 0xff;
    ret = I2C_MasterWrite(PMIC_I2C, 0xd8, cmdData, 1, 0);
    if (ret < E_NO_ERROR) {
        PR_ERROR("MAX78000 select failed %d", ret);
//        MAX20303_led_red(MAX20303_LED_OUTPUT_ON);
//        while(1);
    }

    // Set PORT1 and PORT2 rail to VDDIO
    MXC_GPIO0->vssel =  0x00;
    MXC_GPIO1->vssel =  0x00;

    // Initialize DMA peripheral
    ret = DMA_Init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("DMA_Init failed %d", ret);
        while(1);
    }

    ret = qspi_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("qspi_init failed %d", ret);
        MAX20303_led_red(MAX20303_LED_OUTPUT_ON);
        while(1);
    }

    ret = lcd_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("lcd_init failed %d", ret);
        MAX20303_led_red(MAX20303_LED_OUTPUT_ON);
        while(1);
    }

//    ret = Ble_Init();
//    if (ret != E_NO_ERROR) {
//        PR_ERROR("Ble_Init failed %d", ret);
//        MAX20303_led_red(MAX20303_LED_OUTPUT_ON);
//        while(1);
//    }

    ret = sdcard_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("sdhc_init failed %d", ret);
//        MAX20303_led_red(MAX20303_LED_OUTPUT_ON);
//        while(1);
    }

//    // To debug Core1 set alternate function 3
//    const gpio_cfg_t gpio_cfg_swdio2 =   { PORT_0, (PIN_6 | PIN_7), GPIO_FUNC_ALT3, GPIO_PAD_NONE };
//    GPIO_Config(&gpio_cfg_swdio2);

    // Enable Core1 and start it
    Core1_Start();
    PR_INFO("Core1 started by Core0");

    PR_INFO("init completed");

    MAX20303_led_green(MAX20303_LED_OUTPUT_ON);

    // Print logo and version
    fonts_putSubtitle(LCD_WIDTH, LCD_HEIGHT, version, Font_16x26, RED, image_data_rsz_maxim_logo);
    lcd_drawImage(0, 0, LCD_WIDTH, LCD_HEIGHT, image_data_rsz_maxim_logo);

    int audio_result_print_cnt = 0;
    while (1) {
//        Ble_Worker();

        ret = qspi_worker();
        if (ret > QSPI_TYPE_NO_DATA) {
            switch(ret)
            {
                case QSPI_TYPE_RESPONSE_VIDEO_DATA:
                    if (runFaceId) {
                        if (strcmp(lcd_subtitle, "nothing")) {
                            fonts_putSubtitle(LCD_WIDTH, LCD_HEIGHT, lcd_subtitle, Font_16x26, resultColor, lcd_data);
                        } else {
                            frameColor = WHITE;
                        }

                        fonts_drawRectangle(LCD_WIDTH, LCD_HEIGHT, FACEID_RECTANGLE_X1 - 0, FACEID_RECTANGLE_Y1 - 0,
                                FACEID_RECTANGLE_X2 + 0, FACEID_RECTANGLE_Y2 + 0, frameColor, lcd_data);
                        fonts_drawRectangle(LCD_WIDTH, LCD_HEIGHT, FACEID_RECTANGLE_X1 - 1, FACEID_RECTANGLE_Y1 - 1,
                                FACEID_RECTANGLE_X2 + 1, FACEID_RECTANGLE_Y2 + 1, frameColor, lcd_data);
                        fonts_drawRectangle(LCD_WIDTH, LCD_HEIGHT, FACEID_RECTANGLE_X1 - 2, FACEID_RECTANGLE_Y1 - 2,
                                FACEID_RECTANGLE_X2 + 2, FACEID_RECTANGLE_Y2 + 2, BLACK, lcd_data);
                        fonts_drawRectangle(LCD_WIDTH, LCD_HEIGHT, FACEID_RECTANGLE_X1 - 3, FACEID_RECTANGLE_Y1 - 3,
                                FACEID_RECTANGLE_X2 + 3, FACEID_RECTANGLE_Y2 + 3, BLACK, lcd_data);
                    }

                    if (audio_result_print_cnt) {
                        fonts_putToptitle(LCD_WIDTH, LCD_HEIGHT, lcd_toptitle, Font_16x26, YELLOW, lcd_data);
                        audio_result_print_cnt--;
                    }

                    lcd_drawImage(0, 0, LCD_WIDTH, LCD_HEIGHT, lcd_data);
                    break;
                case QSPI_TYPE_RESPONSE_VIDEO_RESULT:
                    if (strcmp(lcd_subtitle, "Unknown") == 0) {
                        resultColor = RED;
                        frameColor = RED;
                    } else if(strcmp(lcd_subtitle, "Adjust Face") == 0) {
                        resultColor = YELLOW;
                        frameColor = YELLOW;
                    } else {
                        resultColor = GREEN;
                        frameColor = GREEN;
                    }
                    break;
                case QSPI_TYPE_RESPONSE_AUDIO_RESULT:
                    audio_result_print_cnt = 10;

                    if (strcmp(lcd_toptitle, "OFF") == 0) {
                        lcd_backlight(0);
                        audio_result_print_cnt = 0;
                    } else if(strcmp(lcd_toptitle, "ON") == 0) {
                        lcd_backlight(1);
                        audio_result_print_cnt = 0;
                    } else if (strcmp(lcd_toptitle, "GO") == 0) {
                        runFaceId = 1;
                    } else if(strcmp(lcd_toptitle, "STOP") == 0) {
                        runFaceId = 0;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

// Similar to Core 0, the entry point for Core 1
// is Core1Main()
// Execution begins when the CPU1 Clock is enabled
int Core1_Main(void) {

    PR_INFO("maxrefdes178_max32666 core1");

    //  __asm__("BKPT");

    while (1) {
//        Ble_Worker();
    }

    /* Quiet GCC warnings */
    return -1;
}

void HardFault_Handler(void)
{
    unsigned int cnt = 0;
    while(1) {
        if (cnt % 100000000 == 0) {
            PR_ERROR("\n\n\n\n!!!!!\n Core0 FaultISR: CFSR %08X, BFAR %08x, MMFAR %08x, HFSR %08x\n!!!!!\n\n\n",
                    (unsigned int)SCB->CFSR, (unsigned int)SCB->BFAR, (unsigned int)SCB->MMFAR, (unsigned int)SCB->HFSR);
            cnt = 1;
        }
        cnt++;
    }
}

void HardFault_Handler_Core1(void)
{
    unsigned int cnt = 0;
    while(1) {
        if (cnt % 100000000 == 0) {
            PR_ERROR("\n\n\n\n!!!!!\n Core1 FaultISR: CFSR %08X, BFAR %08x, MMFAR %08x, HFSR %08x\n!!!!!\n\n\n",
                    (unsigned int)SCB->CFSR, (unsigned int)SCB->BFAR, (unsigned int)SCB->MMFAR, (unsigned int)SCB->HFSR);
            cnt = 1;
        }
        cnt++;
    }
}
