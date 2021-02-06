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
// https://github.com/BoschSensortec/BMI160_driver

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <mxc_delay.h>
#include <mxc_errors.h>
#include <stdio.h>

#include "max32666_accel.h"
#include "max32666_debug.h"
#include "max32666_i2c.h"

#undef LITTLE_ENDIAN
#include "bmi160.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "accel"


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static void delay_ms(uint32_t msec);
static int8_t i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);
static int8_t i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
int accel_init(void)
{
    int8_t rslt;
    struct bmi160_dev sensor;
    struct bmi160_int_settg int_config;

    sensor.id = I2C_ADDR_BMI160;
    sensor.interface = BMI160_I2C_INTF;
    sensor.read = i2c_read;
    sensor.write = i2c_write;
    sensor.delay_ms = delay_ms;
    if ((rslt = bmi160_init(&sensor)) != BMI160_OK) {
        PR_ERROR("bmi160_init failed %d", rslt);
        return E_NO_DEVICE;
    }

    /* Select the Output data rate, range of accelerometer sensor */
    sensor.accel_cfg.odr = BMI160_ACCEL_ODR_25HZ;
    sensor.accel_cfg.range = BMI160_ACCEL_RANGE_2G;
    sensor.accel_cfg.bw = BMI160_ACCEL_BW_OSR4_AVG1;

    /* Select the power mode of accelerometer sensor */
    sensor.accel_cfg.power = BMI160_ACCEL_LOWPOWER_MODE;

    /* Select the Output data rate, range of Gyroscope sensor */
    sensor.gyro_cfg.odr = BMI160_GYRO_ODR_100HZ;
    sensor.gyro_cfg.range = BMI160_GYRO_RANGE_2000_DPS;
    sensor.gyro_cfg.bw = BMI160_GYRO_BW_NORMAL_MODE;

    /* Select the power mode of Gyroscope sensor */
    sensor.gyro_cfg.power = BMI160_GYRO_SUSPEND_MODE;

    /* Set the sensor configuration */
    if ((rslt = bmi160_set_sens_conf(&sensor)) != BMI160_OK) {
        PR_ERROR("bmi160_set_sens_conf failed %d", rslt);
        return E_UNKNOWN;
    }

    /* Select the Interrupt channel/pin */
    int_config.int_channel = BMI160_INT_CHANNEL_1;// Interrupt channel/pin 1

    /* Select the Interrupt type */
    int_config.int_type = BMI160_ACC_ANY_MOTION_INT;// Choosing Any motion interrupt
    /* Select the interrupt channel/pin settings */
    int_config.int_pin_settg.output_en = BMI160_ENABLE;// Enabling interrupt pins to act as output pin
    int_config.int_pin_settg.output_mode = BMI160_DISABLE;// Choosing push-pull mode for interrupt pin
    int_config.int_pin_settg.output_type = BMI160_DISABLE;// Choosing active low output
    int_config.int_pin_settg.edge_ctrl = BMI160_ENABLE;// Choosing edge triggered output
    int_config.int_pin_settg.input_en = BMI160_DISABLE;// Disabling interrupt pin to act as input
    int_config.int_pin_settg.latch_dur = BMI160_LATCH_DUR_1_25_MILLI_SEC;// non-latched output

    /* Select the Any-motion interrupt parameters */
    int_config.int_type_cfg.acc_any_motion_int.anymotion_en = BMI160_ENABLE;// 1- Enable the any-motion, 0- disable any-motion
    int_config.int_type_cfg.acc_any_motion_int.anymotion_x = BMI160_ENABLE;// Enabling x-axis for any motion interrupt
    int_config.int_type_cfg.acc_any_motion_int.anymotion_y = BMI160_ENABLE;// Enabling y-axis for any motion interrupt
    int_config.int_type_cfg.acc_any_motion_int.anymotion_z = BMI160_ENABLE;// Enabling z-axis for any motion interrupt
    int_config.int_type_cfg.acc_any_motion_int.anymotion_dur = 0;// any-motion duration
    int_config.int_type_cfg.acc_any_motion_int.anymotion_thr = 20;// (2-g range) -> (slope_thr) * 3.91 mg, (4-g range) -> (slope_thr) * 7.81 mg, (8-g range) ->(slope_thr) * 15.63 mg, (16-g range) -> (slope_thr) * 31.25 mg

    /* Set the Any-motion interrupt */
    if ((rslt = bmi160_set_int_config(&int_config, &sensor)) != BMI160_OK) {
        PR_ERROR("bmi160_set_int_config failed %d", rslt);
        return E_UNKNOWN;
    }

    return E_NO_ERROR;
}

static void delay_ms(uint32_t msec)
{
    MXC_Delay(MXC_DELAY_MSEC(msec));
}

static int8_t i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    int err;

    if ((err = i2c_master_reg_read_buf(dev_addr, reg_addr, data, len)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_read_buf failed %d", err);
        return BMI160_E_COM_FAIL;
    }

    return BMI160_OK;
}

static int8_t i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    int err;

    if ((err = i2c_master_reg_write_buf(dev_addr, reg_addr, data, len)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_read_buf failed %d", err);
        return BMI160_E_COM_FAIL;
    }

    return BMI160_OK;
}
