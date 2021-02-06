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
#include <nvic_table.h>
#include <tmr.h>

#include "max32666_debug.h"
#include "max32666_data.h"
#include "max32666_pmic.h"
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
static const mxc_gpio_cfg_t button1_int_pin = MAX32666_BUTTON1_INT_PIN;
volatile uint32_t timer_ms_tick = 0;


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
void button1_int(void *cbdata)
{
    device_settings.enable_ble ^= 1;
    if (device_settings.enable_ble) {
        device_status.ble_running_status_changed = 1;
//        printf("BLE enabled\n");
    } else {
//        printf("BLE disabled\n");
    }
}

void ms_timer(void)
{
    // Clear interrupt
    MXC_TMR_ClearFlags(MAX32666_TIMER_MS);

    timer_ms_tick += 1;
}

int timer_led_button_init(void)
{
    // Init ms timer
    mxc_tmr_cfg_t tmr;
    MXC_TMR_Shutdown(MAX32666_TIMER_MS);
    tmr.pres = TMR_PRES_1;
    tmr.mode = TMR_MODE_CONTINUOUS;
    tmr.cmp_cnt = PeripheralClock / 1000;
    tmr.pol = 0;
    NVIC_SetVector(MXC_TMR_GET_IRQ(MXC_TMR_GET_IDX(MAX32666_TIMER_MS)), ms_timer);
    NVIC_EnableIRQ(MXC_TMR_GET_IRQ(MXC_TMR_GET_IDX(MAX32666_TIMER_MS)));
    MXC_TMR_Init(MAX32666_TIMER_MS, &tmr);
    MXC_TMR_Start(MAX32666_TIMER_MS);

    // Init button1 interrupt
    MXC_GPIO_Config(&button1_int_pin);
    MXC_GPIO_RegisterCallback(&button1_int_pin, button1_int, NULL);
    MXC_GPIO_IntConfig(&button1_int_pin, MAX32666_BUTTON1_INT_MODE);
    MXC_GPIO_EnableInt(button1_int_pin.port, button1_int_pin.mask);
    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(MXC_GPIO_GET_IDX(button1_int_pin.port)));

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
