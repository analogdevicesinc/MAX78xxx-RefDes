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
#include <mxc_errors.h>

#include "max32666_debug.h"
#include "max32666_expander.h"
#include "max32666_i2c.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "exp"


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
int expander_init(void)
{
    int err;
    uint8_t regval;

    if ((err = i2c_master_byte_read(MAX32666_I2C, I2C_ADDR_MAX7325_OUTPUTS, &regval)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_byte_read failed %d", err);
        return err;
    }

    if ((err = i2c_master_byte_read(MAX32666_I2C, I2C_ADDR_MAX7325_PORTS, &regval)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_byte_read failed %d", err);
        return err;
    }

    // Set IO ports to open drain
    // logic-high for an open-drain output is high impedance
    // Any open-drain port can be used as an input by setting the open-drain output to logic-high
    regval = 0xff;
    if ((err = i2c_master_byte_write(MAX32666_I2C, I2C_ADDR_MAX7325_PORTS, regval)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_byte_write failed %d", err);
        return err;
    }

    if ((err = i2c_master_byte_read(MAX32666_I2C, I2C_ADDR_MAX7325_PORTS, &regval)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_byte_read failed %d", err);
        return err;
    }

    /* Select USB-Type-C Debug Connection to MAX78000-Video on IO expander */
    if ((err = expander_select_debugger(DEBUGGER_SELECT_MAX78000_VIDEO)) != E_NO_ERROR) {
        PR_ERROR("expander_debug_select failed %d", err);
        return err;
    }

    return E_NO_ERROR;
}

int expander_select_debugger(debugger_select_e debugger_select)
{
    int err;
    uint8_t regval;
    // MAX14689, Drive CB low to connect COM_ to NC_. Drive CB high to connect COM_ to NO_.

    if ((err = i2c_master_byte_read(MAX32666_I2C, I2C_ADDR_MAX7325_OUTPUTS, &regval)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_byte_read failed %d", err);
        return err;
    }

    switch (debugger_select) {
    case DEBUGGER_SELECT_MAX32666_CORE1:
        // set HDK1_TARGET_SEL to high
        regval |= EXPANDER_OUTPUT_HDK1_TARGET_SEL;
        break;
    case DEBUGGER_SELECT_MAX78000_VIDEO:
        // set UART_TARGET_SEL low
        // set HDK1_TARGET_SEL to low
        // set SLAVE_DEBUG_SEL to low
        regval &= ~(EXPANDER_OUTPUT_UART_TARGET_SEL | EXPANDER_OUTPUT_HDK1_TARGET_SEL | EXPANDER_OUTPUT_SLAVE_DEBUG_SEL);
        break;
    case DEBUGGER_SELECT_MAX78000_AUDIO:
        // set UART_TARGET_SEL low
        // set HDK1_TARGET_SEL to low
        // set SLAVE_DEBUG_SEL to high
        regval &= ~(EXPANDER_OUTPUT_UART_TARGET_SEL | EXPANDER_OUTPUT_HDK1_TARGET_SEL);
        regval |= EXPANDER_OUTPUT_SLAVE_DEBUG_SEL;
        break;
    default:
        PR_ERROR("invalid selection");
        return E_BAD_PARAM;
        break;
    }

    if ((err = i2c_master_byte_write(MAX32666_I2C, I2C_ADDR_MAX7325_OUTPUTS, regval)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_byte_write failed %d", err);
        return err;
    }

    return E_NO_ERROR;
}

int expander_set_output(uint8_t mask)
{
    int err;
    uint8_t regval;

    if ((err = i2c_master_byte_read(MAX32666_I2C, I2C_ADDR_MAX7325_OUTPUTS, &regval)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_byte_read failed %d", err);
        return err;
    }

    regval |= mask;

    if ((err = i2c_master_byte_write(MAX32666_I2C, I2C_ADDR_MAX7325_OUTPUTS, regval)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_byte_write failed %d", err);
        return err;
    }

    return E_NO_ERROR;
}

int expander_clear_output(uint8_t mask)
{
    int err;
    uint8_t regval;

    if ((err = i2c_master_byte_read(MAX32666_I2C, I2C_ADDR_MAX7325_OUTPUTS, &regval)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_byte_read failed %d", err);
        return err;
    }

    regval &= ~mask;

    if ((err = i2c_master_byte_write(MAX32666_I2C, I2C_ADDR_MAX7325_OUTPUTS, regval)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_byte_write failed %d", err);
        return err;
    }

    return E_NO_ERROR;
}
