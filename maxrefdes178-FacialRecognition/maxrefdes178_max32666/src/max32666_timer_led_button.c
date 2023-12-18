/*******************************************************************************
 * Copyright (C) 2020-2021 Maxim Integrated Products, Inc., All rights Reserved.
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
#include <nvic_table.h>
#include <tmr.h>

#include "max32666_debug.h"
#include "max32666_data.h"
#include "max32666_expander.h"
#include "max32666_fonts.h"
#include "max32666_lcd.h"
#include "max32666_pmic.h"
#include "max32666_qspi_master.h"
#include "max32666_timer_led_button.h"
#include "maxrefdes178_definitions.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "led"


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
static const mxc_gpio_cfg_t button_x_int_pin = MAX32666_BUTTON_X_INT_PIN;
static const mxc_gpio_cfg_t button_power_int_pin = MAX32666_BUTTON_POWER_INT_PIN;
volatile uint32_t timer_ms_tick = 0;
volatile int button_x_int = 0;
volatile int button_y_int = 0;
volatile int button_power_int = 0;
int record = 0;
extern int block_button_x;

//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
void ms_timer(void);
void power_off_timer(void);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
void button_x_int_handler(void *cbdata)
{   
    if (!block_button_x){
        button_x_int = 1;
    }
}

void button_power_int_handler(void *cbdata)
{
    if (MXC_GPIO_InGet(button_power_int_pin.port, button_power_int_pin.mask)) {
//        printf("button power released\n");
        MXC_TMR_Stop(MAX32666_TIMER_BUTTON_POWER);
    } else {
//        printf("button power pressed\n");
        MXC_TMR_Stop(MAX32666_TIMER_BUTTON_POWER);
        MXC_TMR_Start(MAX32666_TIMER_BUTTON_POWER);
        if (!record){
            PR_INFO("button power pressed and record is 0");
            button_power_int = 1;
        }
        
    }
}

void ms_timer(void)
{
    // Clear interrupt
    MXC_TMR_ClearFlags(MAX32666_TIMER_MS);

    timer_ms_tick += 1;
}

void power_off_timer(void)
{
    // Clear interrupt
    MXC_TMR_ClearFlags(MAX32666_TIMER_BUTTON_POWER);

    if (MXC_GPIO_InGet(button_power_int_pin.port, button_power_int_pin.mask)) {
        return;
    }

    printf("POWER OFF\n\n");
    fflush(stdout);

    lcd_backlight(0, 0);

    pmic_led_blue(0);
    pmic_led_green(0);
    pmic_led_red(1);

    device_settings.enable_ble = 0;
    for (int i = 0; (i < 10000000) && !device_status.ble_running_status_changed; i++) {}
    Core1_Stop();

    MXC_Delay(MXC_DELAY_MSEC(10));

    // wait until power button release
    while(!MXC_GPIO_InGet(button_power_int_pin.port, button_power_int_pin.mask));

    pmic_power_off();
}

int timer_led_button_init(void)
{
    mxc_tmr_cfg_t tmr;

    // Init ms timer
    MXC_TMR_Shutdown(MAX32666_TIMER_MS);
    tmr.pres = TMR_PRES_1;
    tmr.mode = TMR_MODE_CONTINUOUS;
    tmr.cmp_cnt = PeripheralClock / 1000;
    tmr.pol = 0;
    MXC_NVIC_SetVector(MXC_TMR_GET_IRQ(MXC_TMR_GET_IDX(MAX32666_TIMER_MS)), ms_timer);
    NVIC_EnableIRQ(MXC_TMR_GET_IRQ(MXC_TMR_GET_IDX(MAX32666_TIMER_MS)));
    MXC_TMR_Init(MAX32666_TIMER_MS, &tmr);
    MXC_TMR_Start(MAX32666_TIMER_MS);

    // Init power button timer
    MXC_TMR_Shutdown(MAX32666_TIMER_BUTTON_POWER);
    tmr.pres = TMR_PRES_128;
    tmr.mode = TMR_MODE_ONESHOT;
    tmr.cmp_cnt = MAX32666_BUTTON_POWER_OFF_INTERVAL * PeripheralClock / 128;
    tmr.pol = 0;
    MXC_NVIC_SetVector(MXC_TMR_GET_IRQ(MXC_TMR_GET_IDX(MAX32666_TIMER_BUTTON_POWER)), power_off_timer);
    NVIC_EnableIRQ(MXC_TMR_GET_IRQ(MXC_TMR_GET_IDX(MAX32666_TIMER_BUTTON_POWER)));
    MXC_TMR_Init(MAX32666_TIMER_BUTTON_POWER, &tmr);

    // Init button X interrupt
    MXC_GPIO_Config(&button_x_int_pin);
    MXC_GPIO_RegisterCallback(&button_x_int_pin, button_x_int_handler, NULL);
    MXC_GPIO_IntConfig(&button_x_int_pin, MAX32666_BUTTON_X_INT_MODE);
    MXC_GPIO_EnableInt(button_x_int_pin.port, button_x_int_pin.mask);
    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(MXC_GPIO_GET_IDX(button_x_int_pin.port)));

    // Init button_power interrupt
    MXC_GPIO_Config(&button_power_int_pin);
    MXC_GPIO_RegisterCallback(&button_power_int_pin, button_power_int_handler, NULL);
    MXC_GPIO_IntConfig(&button_power_int_pin, MAX32666_BUTTON_POWER_INT_MODE);
    MXC_GPIO_EnableInt(button_power_int_pin.port, button_power_int_pin.mask);
    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(MXC_GPIO_GET_IDX(button_power_int_pin.port)));

    return E_NO_ERROR;
}

int led_worker(void)
{
    static uint8_t led_toogle = 0;

    led_toogle ^= 1;

    // If BLE is enabled, blink blue
    // If BLE is connected, solid blue
    // If BLE is disabled, blink green

    if (device_settings.enable_ble) {
        pmic_led_green(0);
        if (device_status.ble_connected) {
            pmic_led_blue(1);
        } else if (led_toogle) {
            pmic_led_blue(1);
        } else {
            pmic_led_blue(0);
        }
    } else {
        pmic_led_blue(0);
        if (led_toogle) {
            pmic_led_green(1);
        } else {
            pmic_led_green(0);
        }
    }

    return E_NO_ERROR;
}

int button_worker(void)
{   
    if (!device_settings.enable_max78000_video) {
        button_x_int = 0; // if video is disabled, ignore button X
    }
    if (button_x_int) {
        button_x_int = 0;
        PR_INFO("button X pressed");
        MXC_Delay(20000);
        if (!record) {
            for (int try = 0; try < 3; try++) {
                qspi_master_send_video(NULL, 0, QSPI_PACKET_TYPE_VIDEO_RECORD_EN);
                qspi_master_wait_video_int();
            }
            record = 1;
            block_button_x = 1; // Until capture screen is displayed, block button X
        } else {
            for (int try = 0; try < 3; try++) {
                qspi_master_send_video(NULL, 0, QSPI_PACKET_TYPE_VIDEO_RECORD_DSB);
                qspi_master_wait_video_int();
            }
            record = 0;
        }
        timestamps.activity_detected = timer_ms_tick;

/*
        device_settings.enable_lcd_statistics = !device_settings.enable_lcd_statistics;

        if (device_settings.enable_lcd_statistics) {
            lcd_notification(MAGENTA, "LCD statistics enabled");
        } else {
            lcd_notification(MAGENTA, "LCD statistics disabled");
        }

        timestamps.faceid_subject_names_received = timer_ms_tick;
        */
    }

    if (button_power_int && !record) {
        button_power_int = 0;

        PR_INFO("button power pressed");

        device_settings.enable_max78000_video = 0;
        for (int try = 0; try < 3; try++) {
                qspi_master_send_video(NULL, 0, QSPI_PACKET_TYPE_VIDEO_DISABLE_CMD);
                qspi_master_wait_video_int();
            }
        lcd_notification(MAGENTA, "Video disabled");
    }

    return E_NO_ERROR;
}

void button_y_int_handler(int state)
{
    if (state) {
        PR_DEBUG("button Y released");
    } else {
        PR_INFO("button Y pressed");
        static debugger_select_e debugger_select = DEBUGGER_SELECT_MAX32666_CORE1;
        debugger_select++;
        debugger_select = debugger_select % DEBUGGER_SELECT_LAST;
        expander_select_debugger(debugger_select);
        timestamps.activity_detected = timer_ms_tick;
    }
}
