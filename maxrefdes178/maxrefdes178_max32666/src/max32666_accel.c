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
#include <mxc_errors.h>
#include <stdio.h>

#include "max32666_accel.h"
#include "max32666_debug.h"
#include "max32666_i2c.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "accel"

// https://github.com/BoschSensortec/BMI160_driver
/** BMI160 Register map */
#define BMI160_CHIP_ID_ADDR                  UINT8_C(0x00)
#define BMI160_ERROR_REG_ADDR                UINT8_C(0x02)
#define BMI160_PMU_STATUS_ADDR               UINT8_C(0x03)
#define BMI160_AUX_DATA_ADDR                 UINT8_C(0x04)
#define BMI160_GYRO_DATA_ADDR                UINT8_C(0x0C)
#define BMI160_ACCEL_DATA_ADDR               UINT8_C(0x12)
#define BMI160_STATUS_ADDR                   UINT8_C(0x1B)
#define BMI160_INT_STATUS_ADDR               UINT8_C(0x1C)
#define BMI160_FIFO_LENGTH_ADDR              UINT8_C(0x22)
#define BMI160_FIFO_DATA_ADDR                UINT8_C(0x24)
#define BMI160_ACCEL_CONFIG_ADDR             UINT8_C(0x40)
#define BMI160_ACCEL_RANGE_ADDR              UINT8_C(0x41)
#define BMI160_GYRO_CONFIG_ADDR              UINT8_C(0x42)
#define BMI160_GYRO_RANGE_ADDR               UINT8_C(0x43)
#define BMI160_AUX_ODR_ADDR                  UINT8_C(0x44)
#define BMI160_FIFO_DOWN_ADDR                UINT8_C(0x45)
#define BMI160_FIFO_CONFIG_0_ADDR            UINT8_C(0x46)
#define BMI160_FIFO_CONFIG_1_ADDR            UINT8_C(0x47)
#define BMI160_AUX_IF_0_ADDR                 UINT8_C(0x4B)
#define BMI160_AUX_IF_1_ADDR                 UINT8_C(0x4C)
#define BMI160_AUX_IF_2_ADDR                 UINT8_C(0x4D)
#define BMI160_AUX_IF_3_ADDR                 UINT8_C(0x4E)
#define BMI160_AUX_IF_4_ADDR                 UINT8_C(0x4F)
#define BMI160_INT_ENABLE_0_ADDR             UINT8_C(0x50)
#define BMI160_INT_ENABLE_1_ADDR             UINT8_C(0x51)
#define BMI160_INT_ENABLE_2_ADDR             UINT8_C(0x52)
#define BMI160_INT_OUT_CTRL_ADDR             UINT8_C(0x53)
#define BMI160_INT_LATCH_ADDR                UINT8_C(0x54)
#define BMI160_INT_MAP_0_ADDR                UINT8_C(0x55)
#define BMI160_INT_MAP_1_ADDR                UINT8_C(0x56)
#define BMI160_INT_MAP_2_ADDR                UINT8_C(0x57)
#define BMI160_INT_DATA_0_ADDR               UINT8_C(0x58)
#define BMI160_INT_DATA_1_ADDR               UINT8_C(0x59)
#define BMI160_INT_LOWHIGH_0_ADDR            UINT8_C(0x5A)
#define BMI160_INT_LOWHIGH_1_ADDR            UINT8_C(0x5B)
#define BMI160_INT_LOWHIGH_2_ADDR            UINT8_C(0x5C)
#define BMI160_INT_LOWHIGH_3_ADDR            UINT8_C(0x5D)
#define BMI160_INT_LOWHIGH_4_ADDR            UINT8_C(0x5E)
#define BMI160_INT_MOTION_0_ADDR             UINT8_C(0x5F)
#define BMI160_INT_MOTION_1_ADDR             UINT8_C(0x60)
#define BMI160_INT_MOTION_2_ADDR             UINT8_C(0x61)
#define BMI160_INT_MOTION_3_ADDR             UINT8_C(0x62)
#define BMI160_INT_TAP_0_ADDR                UINT8_C(0x63)
#define BMI160_INT_TAP_1_ADDR                UINT8_C(0x64)
#define BMI160_INT_ORIENT_0_ADDR             UINT8_C(0x65)
#define BMI160_INT_ORIENT_1_ADDR             UINT8_C(0x66)
#define BMI160_INT_FLAT_0_ADDR               UINT8_C(0x67)
#define BMI160_INT_FLAT_1_ADDR               UINT8_C(0x68)
#define BMI160_FOC_CONF_ADDR                 UINT8_C(0x69)
#define BMI160_CONF_ADDR                     UINT8_C(0x6A)

#define BMI160_IF_CONF_ADDR                  UINT8_C(0x6B)
#define BMI160_SELF_TEST_ADDR                UINT8_C(0x6D)
#define BMI160_OFFSET_ADDR                   UINT8_C(0x71)
#define BMI160_OFFSET_CONF_ADDR              UINT8_C(0x77)
#define BMI160_INT_STEP_CNT_0_ADDR           UINT8_C(0x78)
#define BMI160_INT_STEP_CONFIG_0_ADDR        UINT8_C(0x7A)
#define BMI160_INT_STEP_CONFIG_1_ADDR        UINT8_C(0x7B)
#define BMI160_COMMAND_REG_ADDR              UINT8_C(0x7E)
#define BMI160_SPI_COMM_TEST_ADDR            UINT8_C(0x7F)
#define BMI160_INTL_PULLUP_CONF_ADDR         UINT8_C(0x85)

/** BMI160 unique chip identifier */
#define BMI160_CHIP_ID                       UINT8_C(0xD1)


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
int accel_init(void)
{
    int err;
    uint8_t chip_id;

    if ((err = i2c_master_reg_read(I2C_ADDR_BMI160, BMI160_CHIP_ID_ADDR, &chip_id)) != E_NO_ERROR) {
        PR_ERROR("i2c_reg_read failed %d", err);
        return err;
    }
    if (chip_id != BMI160_CHIP_ID) {
        PR_ERROR("invalid chip id 0x%x", chip_id);
        return E_NOT_SUPPORTED;
    }

    return E_NO_ERROR;
}
