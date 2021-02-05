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


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <mxc_delay.h>
#include <mxc_errors.h>
#include <stdio.h>

#include "max32666_debug.h"
#include "max32666_i2c.h"
#include "max32666_powmon.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "powmon"

#define MAX34417_DEVICE_REV_ID  0x3A

#define MAX34417_UPDATE_CMD     0x00
#define MAX34417_CONTROL_CMD    0x01
#define MAX34417_ACC_COUNT_CMD  0x02
#define MAX34417_PWR_ACC_1_CMD  0x03
#define MAX34417_PWR_ACC_2_CMD  0x04
#define MAX34417_PWR_ACC_3_CMD  0x05
#define MAX34417_PWR_ACC_4_CMD  0x06
#define MAX34417_BULK_PWR_CMD   0x10
#define MAX34417_BULK_VOLT_CMD  0x11
#define MAX34417_DEV_REV_ID_CMD 0x0F

#define MAX34417_CNTRL_MODE     0x80
#define MAX34417_CNTRL_CAM      0x40
#define MAX34417_CNTRL_SMM      0x20
#define MAX34417_CNTRL_PARK_EN  0x10
#define MAX34417_CNTRL_PARK1    0x08
#define MAX34417_CNTRL_PARK0    0x04
#define MAX34417_CNTRL_SLOW     0x02
#define MAX34417_CNTRL_OVF      0x01


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static uint32_t convert_14to16(const uint8_t * p14);
static uint32_t convert_24to32(const uint8_t * p24);
static uint64_t convert_56to64(const uint8_t * p56);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
static uint32_t convert_14to16(const uint8_t * p14)
{
    uint32_t v16;
    uint8_t *p16 = (uint8_t*) &v16;
    for (uint8_t i = 0; i < 2; i++) {
        p16[1-i] = p14[i];
    }
    return v16 >> 2;
}

static uint32_t convert_24to32(const uint8_t * p24)
{
    uint32_t v32;
    uint8_t *p32 = (uint8_t*)&v32;
    for (uint8_t i = 0; i < 3; i++) {
        p32[2-i] = p24[i];
    }
    p32[2] = 0;
    return v32;
}

static uint64_t convert_56to64(const uint8_t * p56)
{
    uint64_t v64;
    uint8_t *p64 = (uint8_t*) &v64;
    for (uint8_t i = 0; i < 7; i++) {
        p64[6-i] = p56[i];
    }
    p64[6] = 0;
    return v64;
}

int powmon_init(void)
{
    int err;
    uint8_t dev_rev_id;

//    uint8_t buf[2];
//    buf[0] = MAX34417_DEV_REV_ID_CMD;
//    mxc_i2c_req_t reqMaster;
//    reqMaster.i2c = MAX32666_I2C;
//    reqMaster.addr = I2C_ADDR_MAX34417;
//    reqMaster.tx_buf = &buf[0];
//    reqMaster.tx_len = 1;
//    reqMaster.rx_buf = &buf[1];
//    reqMaster.rx_len = 1;
//    reqMaster.restart = 0;
//    reqMaster.callback = NULL;
//    if((err = MXC_I2C_MasterTransaction(&reqMaster)) != E_NO_ERROR) {
//        PR_ERROR("i2c_reg_read failed %d", err);
//        return err;
//    }

    if ((err = i2c_master_reg_read(I2C_ADDR_MAX34417, MAX34417_DEV_REV_ID_CMD, &dev_rev_id)) != E_NO_ERROR) {
        PR_ERROR("i2c_reg_read failed %d", err);
        return err;
    }
    if (dev_rev_id != MAX34417_DEVICE_REV_ID) {
        PR_ERROR("invalid device id 0x%x", dev_rev_id);
        return E_NOT_SUPPORTED;
    }

    if ((err = i2c_master_reg_write(I2C_ADDR_MAX34417, MAX34417_CONTROL_CMD, MAX34417_CNTRL_MODE)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }

    powmon_update();
    MXC_Delay(MXC_DELAY_MSEC(1));

    if ((err = i2c_master_reg_write(I2C_ADDR_MAX34417, MAX34417_CONTROL_CMD, MAX34417_CNTRL_CAM | MAX34417_CNTRL_MODE)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }

    powmon_update();
    MXC_Delay(MXC_DELAY_MSEC(1));

    return E_NO_ERROR;
}

int powmon_update(void)
{
    int err;

    if ((err = i2c_master_byte_write(I2C_ADDR_MAX34417, MAX34417_UPDATE_CMD)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_byte_write failed %d", err);
        return err;
    }

    return E_NO_ERROR;
}

uint32_t powmon_acc_count(void)
{
    int err;
    static uint8_t buf[4];

    if ((err = i2c_master_reg_read_buf(I2C_ADDR_MAX34417, MAX34417_ACC_COUNT_CMD, buf, sizeof(buf))) != E_NO_ERROR) {
        PR_ERROR("i2c_reg_read failed %d", err);
        return err;
    }

    return convert_24to32(&buf[1]);
}

int powmon_bulk_voltage(double voltage[4])
{
    int err;
    uint16_t adc;
    static uint8_t buf[9];
    static const double adc2v = 24.0 / (16384.0 - 1.0);

    if ((err = i2c_master_reg_read_buf(I2C_ADDR_MAX34417, MAX34417_BULK_VOLT_CMD, buf, sizeof(buf))) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_read_buf failed %d", err);
        return err;
    }

    for (uint8_t i = 0; i < 4; i++) {
        adc = convert_14to16(&buf[1 + i*2]);
        voltage[i] = (double)adc * adc2v;
        PR_DEBUG("%g", voltage[i] );
    }
    return E_NO_ERROR;
}

int powmon_bulk_energy(double energy_raw[4])
{
    int err;
    static uint8_t buf[29];

    if ((err = i2c_master_reg_read_buf(I2C_ADDR_MAX34417, MAX34417_BULK_PWR_CMD, buf, sizeof(buf))) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_read_buf failed %d", err);
        return err;
    }

    for (uint8_t i = 0; i < 4; i++) {
        energy_raw[i] = convert_56to64(&buf[1+i*7]);
        PR_DEBUG("%g", energy_raw[i] );
    }
    return E_NO_ERROR;
}

int powmon_bulk_power(double power_raw[4])
{
    int err;
    uint32_t acc_count = powmon_acc_count();

    if ((err = powmon_bulk_energy(power_raw)) != E_NO_ERROR) {
        PR_ERROR("max34417_bulk_energy failed %d", err);
        return err;
    }

    for (uint8_t i = 0; i < 4; i++)
    {
        power_raw[i] = power_raw[i] / (double)acc_count;
        PR_DEBUG("%g", power_raw[i] );
    }
    return E_NO_ERROR;
}
