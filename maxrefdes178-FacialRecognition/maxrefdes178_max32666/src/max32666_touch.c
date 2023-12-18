/*******************************************************************************
 * Copyright (C) 2020 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
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
 *
 ******************************************************************************/
// https://github.com/adafruit/Adafruit_FT6206_Library

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <mxc_errors.h>
#include <stdio.h>

#include "max32666_touch.h"
#include "max32666_data.h"
#include "max32666_debug.h"
#include "max32666_i2c.h"
#include "max32666_timer_led_button.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "touch"

// FT6236U register map
#define FT6236U_DEV_MODE_ADDR      0x00  //!< Device mode, either WORKING or FACTORY
#define FT6236U_GEST_ID            0x01  //!< Device mode, either WORKING or FACTORY
#define FT6236U_TD_STATUS_ADDR     0x02  //!< Number of touch points
#define FT6236U_P1_XH_ADDR         0x03  //!< Touch1 X
#define FT6236U_P1_XL_ADDR         0x04  //!< Touch1 X
#define FT6236U_P1_YH_ADDR         0x05  //!< Touch1 Y
#define FT6236U_P1_YL_ADDR         0x06  //!< Touch1 Y
#define FT6236U_P1_WEIGHT_ADDR     0x07  //!< Touch1 weight
#define FT6236U_P1_MISC_ADDR       0x08  //!< Touch1 area
#define FT6236U_P2_XH_ADDR         0x09  //!< Touch2 X
#define FT6236U_P2_XL_ADDR         0x0A  //!< Touch2 X
#define FT6236U_P2_YH_ADDR         0x0B  //!< Touch2 Y
#define FT6236U_P2_YL_ADDR         0x0C  //!< Touch2 Y
#define FT6236U_P2_WEIGHT_ADDR     0x0D  //!< Touch2 weight
#define FT6236U_P2_MISC_ADDR       0x0E  //!< Touch2 area
#define FT6236U_FACTORYMODE_ADDR   0x40  //!< Factory mode
#define FT6236U_TH_GROUP_ADDR      0x80  //!< Threshold for touch detection
#define FT6236U_PERIODACTIVE_ADDR  0x88  //!< Active mode report rate
#define FT6236U_PERIODMONITOR_ADDR 0x89  //!< Monitor mode report rate
#define FT6236U_CHIPER_ADDR        0xA3  //!< Chip selecting
#define FT6236U_G_MODE_ADDR        0xA4  //!< Interrupt mode
#define FT6236U_PWR_MODE_ADDR      0xA5  //!< Power mode
#define FT6236U_FIRMID_ADDR        0xA6  //!< Firmware version
#define FT6236U_FOCALTECH_ID_ADDR  0xA8  //!< FocalTech's panel ID

// FT6236U register fields
#define FT6236U_G_MODE_POLLING     0x00  //!< Interrupt polling mode
#define FT6236U_G_MODE_TRIGGER     0x01  //!< Interrupt trigger mode

// FT6236U expected fields
#define FT6236U_FOCALTECH_ID       0x11  //!< FocalTech's panel ID
#define FT6236U_CHIPER             0x64  //!< Chip selecting

// FT6236U settings
#define FT6236U_TH_GROUP           50    //!< Default threshold for touch detection
#define FT6236U_PERIODMONITOR      20    //!< Default mMonitor mode report rate


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
static const mxc_gpio_cfg_t touch_int_pin = MAX32666_LCD_TOUCH_INT_PIN;
static TS_Buttons_t ts_buttons[TS_MAX_BUTTONS];
static int pressed_key = 0;
static volatile int touch_int_flag = 0;


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static int touch_point(uint16_t *x1, uint16_t *y1);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
void touch_int(void *cbdata)
{
    touch_int_flag = 1;
}

int touch_init(void)
{
    int err;
    uint8_t regval;

    // Init IO Expander interrupt
    MXC_GPIO_Config(&touch_int_pin);
    MXC_GPIO_RegisterCallback(&touch_int_pin, touch_int, NULL);
    MXC_GPIO_IntConfig(&touch_int_pin, MAX32666_LCD_TOUCH_INT_MODE);
    MXC_GPIO_EnableInt(touch_int_pin.port, touch_int_pin.mask);
    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(MXC_GPIO_GET_IDX(touch_int_pin.port)));

    if ((err = i2c_master_reg_read(I2C_ADDR_FT6236U, FT6236U_FOCALTECH_ID_ADDR, &regval)) != E_NO_ERROR) {
        PR_ERROR("i2c_reg_read failed %d", err);
        return err;
    }
    if (regval != FT6236U_FOCALTECH_ID) {
        PR_ERROR("invalid vendor id 0x%x", regval);
        return E_NOT_SUPPORTED;
    }

    if ((err = i2c_master_reg_read(I2C_ADDR_FT6236U, FT6236U_CHIPER_ADDR, &regval)) != E_NO_ERROR) {
        PR_ERROR("i2c_reg_read failed %d", err);
        return err;
    }
    if (regval != FT6236U_CHIPER) {
        PR_ERROR("invalid chip id 0x%x", regval);
        return E_NOT_SUPPORTED;
    }

    regval = FT6236U_TH_GROUP;
    if ((err = i2c_master_reg_write(I2C_ADDR_FT6236U, FT6236U_TH_GROUP_ADDR, regval)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }

    regval = FT6236U_PERIODMONITOR;
    if ((err = i2c_master_reg_write(I2C_ADDR_FT6236U, FT6236U_PERIODMONITOR_ADDR, regval)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }

    // Remove this for continuous read
    regval = FT6236U_G_MODE_POLLING;
    if ((err = i2c_master_reg_write(I2C_ADDR_FT6236U, FT6236U_G_MODE_ADDR, regval)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }

    return E_NO_ERROR;
}

int touch_worker(uint16_t *x1, uint16_t *y1)
{
    int err;

    if (!touch_int_flag) {
        return E_NONE_AVAIL;
    }
    touch_int_flag = 0;

    if ((err = touch_point(x1, y1)) != E_NO_ERROR) {
        PR_ERROR("touch_point failed %d", err);
        return err;
    }

    return E_NO_ERROR;
}

static int touch_point(uint16_t *x1, uint16_t *y1)
{
    int err;
    uint8_t buff[4];

    if ((err = i2c_master_reg_read_buf(I2C_ADDR_FT6236U, FT6236U_P1_XH_ADDR, buff, sizeof(buff))) != E_NO_ERROR) {
        PR_ERROR("i2c_reg_read failed %d", err);
        return err;
    }

    *x1 = ((buff[0] & 0x0F) << 8) | (buff[1]);
    *y1 = ((buff[2] & 0x0F) << 8) | (buff[3]);

    // 320 -> 240 map
    *x1 *= 3 / 4.0;
    *y1 *= 3 / 4.0;

    return E_NO_ERROR;
}

int MXC_TS_AddButton(int x0, int y0, int x1, int y1, int on_press_expected_code)
{
    int index;

    for (index = 0; index < 28; index++) {
        if (ts_buttons[index].key_code == -1) {
            ts_buttons[index].x0 = x0;
            ts_buttons[index].y0 = y0;
            ts_buttons[index].x1 = x1;
            ts_buttons[index].y1 = y1;
            ts_buttons[index].key_code = on_press_expected_code;
           // printf("WIll it come here?\n");
            break;
        }
    }

    return index;
}

int is_inBox(int x, int y, int x0, int y0, int x1, int y1)
{
    if ((x >= x0) && (x <= x1) && (y >= y0) && (y <= y1)) {
        return 1;
    }

    return 0;
}

int MXC_TS_GetKey(void)
{
    uint16_t touch_x1 = 0, touch_y1 = 0;
    touch_worker(&touch_x1, &touch_y1);
    int key;
    if(touch_x1){
            PR_INFO("touch inside getkey %d %d", touch_x1, touch_y1);
    }
    if (pressed_key == 0) { // wait until prev key process
        for (int i = 0; i < 28; i++) {
            if (ts_buttons[i].key_code != -1) {
                if (is_inBox(touch_x1, touch_y1, ts_buttons[i].x0, ts_buttons[i].y0, ts_buttons[i].x1,
                             ts_buttons[i].y1)) {
                    // pressed key
                    //printf("x1:%d, y1:%d\n",touch_x1,touch_y1);
                    pressed_key = ts_buttons[i].key_code;

                    touch_x1 = 0; 
                    touch_y1 = 0;
                    break;
                }   
            }
        }
    }
    key = pressed_key;
    pressed_key = 0;

    return key;
}

void MXC_TS_RemoveAllButton(void)
{
    for (int i = 0; i < TS_MAX_BUTTONS; i++) ts_buttons[i].key_code = -1;
}