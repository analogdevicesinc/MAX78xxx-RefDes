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
#include <dma.h>
#include <stdint.h>
#include <string.h>
#include <rtc.h>

#include "max32666_debug.h"
#include "max32666_ble.h"
#include "max32666_expander.h"
#include "max32666_i2c.h"
#include "max32666_lcd.h"
#include "max32666_lcd_data.h"
#include "max32666_max20303.h"
#include "max32666_qspi.h"
#include "max32666_sdcard.h"
#include "max32666_utils.h"
#include "maxrefdes178_definitions.h"
#include "maxrefdes178_version.h"


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
    uint8_t run_faceid = 0;
    uint16_t result_color = 0;
    uint16_t frame_color = 0;
    uint32_t audio_result_time = 0;
    uint32_t lcd_draw_time = 0;
    int ret = 0;
    double fps = 0;
    char version[10] = {0};
    char fps_string[10] = {0};

    // Set PORT1 and PORT2 rail to VDDIO
    MXC_GPIO0->vssel =  0x00;
    MXC_GPIO1->vssel =  0x00;

    ret = MXC_SEMA_Init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("MXC_SEMA_Init failed %d", ret);
        while(1);
    }

    snprintf(version, sizeof(version) - 1, "v%d.%d.%d", S_VERSION_MAJOR, S_VERSION_MINOR, S_VERSION_BUILD);
    PR_INFO("maxrefdes178_max32666 core0 %s [%s]", version, S_BUILD_TIMESTAMP);

    ret = i2c_master_init(MAX32666_I2C, I2C_SPEED);
    if (ret != E_NO_ERROR) {
        PR_ERROR("i2c_init failed %d", ret);
        while(1);
    }

    ret = max20303_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("max20303_init failed %d", ret);
        while(1);
    }
    max20303_led_green(1);

    ret = expander_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("expander_init failed %d", ret);
        max20303_led_red(1);
        while(1);
    }

    /* Select USB-TYpe-C Debug Connection to MAX78000-Video on IO expander */
    ret = expander_debug_select(eExpanderDebugVideo);
    if (ret != E_NO_ERROR) {
        PR_ERROR("expander_debug_select failed %d", ret);
        max20303_led_red(1);
        while(1);
    }

    // Initialize DMA peripheral
    ret = MXC_DMA_Init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("MXC_DMA_Init failed %d", ret);
        max20303_led_red(1);
        while(1);
    }

    ret = lcd_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("lcd_init failed %d", ret);
        max20303_led_red(1);
        while(1);
    }

    ret = qspi_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("qspi_init failed %d", ret);
        max20303_led_red(1);
        while(1);
    }

    ret = sdcard_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("sdcard_init failed %d", ret);
        max20303_led_red(1);
    }

    ret = MXC_RTC_Init(0, 0);
    if (ret != E_NO_ERROR) {
        PR_ERROR("MXC_RTC_Init failed %d", ret);
        max20303_led_red(1);
    }

    ret = MXC_RTC_Start();
    if (ret != E_NO_ERROR) {
        PR_ERROR("MXC_RTC_Start failed %d", ret);
        max20303_led_red(1);
    }

//    ret = ble_init();
//    if (ret != E_NO_ERROR) {
//        PR_ERROR("ble_init failed %d", ret);
//        max20303_led_red(1);
//    }

    // Enable Core1
    Core1_Start();

    PR_INFO("init completed");

    // Print logo and version
    fonts_putSubtitle(LCD_WIDTH, LCD_HEIGHT, version, Font_16x26, RED, image_data_rsz_maxim_logo);
    lcd_drawImage(0, 0, LCD_WIDTH, LCD_HEIGHT, image_data_rsz_maxim_logo);

    while (1) {
//        ble_worker();

        ret = qspi_worker();
        if (ret > QSPI_TYPE_NO_DATA) {
            switch(ret)
            {
            case QSPI_TYPE_RESPONSE_VIDEO_DATA:
                if (run_faceid) {
                    if (strcmp(lcd_subtitle, "nothing")) {
                        fonts_putSubtitle(LCD_WIDTH, LCD_HEIGHT, lcd_subtitle, Font_16x26, result_color, lcd_data);
                    } else {
                        frame_color = WHITE;
                    }

                    fonts_drawRectangle(LCD_WIDTH, LCD_HEIGHT, FACEID_RECTANGLE_X1 - 0, FACEID_RECTANGLE_Y1 - 0,
                            FACEID_RECTANGLE_X2 + 0, FACEID_RECTANGLE_Y2 + 0, frame_color, lcd_data);
                    fonts_drawRectangle(LCD_WIDTH, LCD_HEIGHT, FACEID_RECTANGLE_X1 - 1, FACEID_RECTANGLE_Y1 - 1,
                            FACEID_RECTANGLE_X2 + 1, FACEID_RECTANGLE_Y2 + 1, frame_color, lcd_data);
                    fonts_drawRectangle(LCD_WIDTH, LCD_HEIGHT, FACEID_RECTANGLE_X1 - 2, FACEID_RECTANGLE_Y1 - 2,
                            FACEID_RECTANGLE_X2 + 2, FACEID_RECTANGLE_Y2 + 2, BLACK, lcd_data);
                    fonts_drawRectangle(LCD_WIDTH, LCD_HEIGHT, FACEID_RECTANGLE_X1 - 3, FACEID_RECTANGLE_Y1 - 3,
                            FACEID_RECTANGLE_X2 + 3, FACEID_RECTANGLE_Y2 + 3, BLACK, lcd_data);
                }

                fps = (double) 1000.0 / (double)(utils_get_time_ms() - lcd_draw_time);
                lcd_draw_time = utils_get_time_ms();
                if (LCD_FPS_ENABLE) {
                    snprintf(fps_string, sizeof(fps_string) - 1, "%5.2f", fps);
                    fonts_putString(LCD_WIDTH, LCD_HEIGHT, LCD_WIDTH - 37, 3, fps_string, Font_7x10, MAGENTA, 0, 0, lcd_data);
                }

                if (lcd_draw_time - audio_result_time < KWS_PRINT_DURATION) {
                    fonts_putToptitle(LCD_WIDTH, LCD_HEIGHT, lcd_toptitle, Font_16x26, YELLOW, lcd_data);
                }

                lcd_drawImage(0, 0, LCD_WIDTH, LCD_HEIGHT, lcd_data);
                break;
            case QSPI_TYPE_RESPONSE_VIDEO_RESULT:
                if (strcmp(lcd_subtitle, "Unknown") == 0) {
                    result_color = RED;
                    frame_color = RED;
                } else if(strcmp(lcd_subtitle, "Adjust Face") == 0) {
                    result_color = YELLOW;
                    frame_color = YELLOW;
                } else {
                    result_color = GREEN;
                    frame_color = GREEN;
                }
                break;
            case QSPI_TYPE_RESPONSE_AUDIO_RESULT:
                audio_result_time = utils_get_time_ms();

                if (strcmp(lcd_toptitle, "OFF") == 0) {
                    lcd_backlight(0);
                } else if(strcmp(lcd_toptitle, "ON") == 0) {
                    lcd_backlight(1);
                } else if (strcmp(lcd_toptitle, "GO") == 0) {
                    run_faceid = 1;
                } else if(strcmp(lcd_toptitle, "STOP") == 0) {
                    run_faceid = 0;
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

    }

    /* Quiet GCC warnings */
    return -1;
}

void HardFault_Handler(void)
{
    unsigned int cnt = 0;
    while(1) {
        if (cnt % 100000000 == 0) {
            PR("\n\n\n\n!!!!!\n Core0 FaultISR: CFSR %08X, BFAR %08x, MMFAR %08x, HFSR %08x\n!!!!!\n\n\n",
                    (unsigned int)SCB->CFSR, (unsigned int)SCB->BFAR, (unsigned int)SCB->MMFAR, (unsigned int)SCB->HFSR);
            cnt = 1;
        }
        cnt++;
    }
}
