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

#include "max32666_touch.h"
#include "max32666_debug.h"
#include "max32666_i2c.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "touch"

#define FT6236U_DEV_ID            0x11

#define FT6236U_DEV_MODE_ADDR     0x00
#define FT6236U_DEV_ID_ADDR       0xA8


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
int touch_init(void)
{
    int err;
    uint8_t dev_id;

    if ((err = i2c_master_reg_read(I2C_ADDR_FT6236U, FT6236U_DEV_ID_ADDR, &dev_id)) != E_NO_ERROR) {
        PR_ERROR("i2c_reg_read failed %d", err);
        return err;
    }
    if (dev_id != FT6236U_DEV_ID) {
        PR_ERROR("invalid device id 0x%x", dev_id);
        return E_NOT_SUPPORTED;
    }

    return E_NO_ERROR;
}
