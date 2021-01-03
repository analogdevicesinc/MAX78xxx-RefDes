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
#include <i2c.h>
#include <mxc_errors.h>

#include "max32666_debug.h"
#include "max32666_i2c.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "i2c"

// Interrupt flags that signal an error
#define MXC_I2C_ERROR (MXC_F_I2C_INT_FL0_ARB_ER         | \
                       MXC_F_I2C_INT_FL0_TO_ER          | \
                       MXC_F_I2C_INT_FL0_ADDR_NACK_ER   | \
                       MXC_F_I2C_INT_FL0_DATA_ER        | \
                       MXC_F_I2C_INT_FL0_DO_NOT_RESP_ER | \
                       MXC_F_I2C_INT_FL0_START_ER       | \
                       MXC_F_I2C_INT_FL0_STOP_ER)


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static inline void i2c_flush(mxc_i2c_regs_t *i2c);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
/**
 * @brief   Flush I2C. The native driver seems paranoid calling this function
 * often. Follow suit until determined otherwise.
 * @returns    This function has no return value.
 */
static inline void i2c_flush(mxc_i2c_regs_t *i2c)
{
    i2c->int_fl0 = i2c->int_fl0;
    i2c->int_fl1 = i2c->int_fl1;
    i2c->tx_ctrl0 |= MXC_F_I2C_TX_CTRL0_TX_FLUSH; while (i2c->tx_ctrl0 & MXC_F_I2C_TX_CTRL0_TX_FLUSH);
    i2c->rx_ctrl0 |= MXC_F_I2C_RX_CTRL0_RX_FLUSH; while (i2c->rx_ctrl0 & MXC_F_I2C_RX_CTRL0_RX_FLUSH);
}

int i2c_master_byte_write(mxc_i2c_regs_t *i2c, uint8_t addr, uint8_t val)
{
    i2c_flush(i2c);

    i2c->fifo = addr;
    i2c->m |= MXC_F_I2C_M_START;
    i2c->fifo = val;
    i2c->m |= MXC_F_I2C_M_STOP;

    while (!(i2c->int_fl0 & MXC_F_I2C_INT_FL0_STOP));
    i2c->int_fl0 = MXC_F_I2C_INT_FL0_STOP;

    return (i2c->int_fl0 & MXC_I2C_ERROR) ? E_COMM_ERR : E_NO_ERROR;
}

int i2c_master_reg_write(mxc_i2c_regs_t *i2c, uint8_t addr, uint8_t reg, uint8_t val)
{
//    int err;
//    uint8_t buf[2] = {reg, val};
//    mxc_i2c_req_t reqMaster;
//    reqMaster.i2c = i2c;
//    reqMaster.addr = addr >> 1;
//    reqMaster.tx_buf = buf;
//    reqMaster.tx_len = 2;
//    reqMaster.rx_buf = NULL;
//    reqMaster.rx_len = 0;
//    reqMaster.restart = 0;
//    reqMaster.callback = NULL;
//    if((err = MXC_I2C_MasterTransaction(&reqMaster)) != E_NO_ERROR) {
//        PR_ERROR("i2c_reg_read failed %d", err);
//        return err;
//    }
//    return E_NO_ERROR;

    i2c_flush(i2c);

    i2c->fifo = addr;
    i2c->m |= MXC_F_I2C_M_START;
    i2c->fifo = reg;
    i2c->fifo = val;
    i2c->m |= MXC_F_I2C_M_STOP;

    while (!(i2c->int_fl0 & MXC_F_I2C_INT_FL0_STOP));
    i2c->int_fl0 = MXC_F_I2C_INT_FL0_STOP;

    return (i2c->int_fl0 & MXC_I2C_ERROR) ? E_COMM_ERR : E_NO_ERROR;
}

int i2c_master_reg_write_buf(mxc_i2c_regs_t *i2c, uint8_t addr, uint8_t reg, uint8_t* buf, int len)
{
    /* Limit write length to one FIFOs worth of data.
       This module in its current state would not benefit from the
       additional complexity. */
    if (len > (MXC_I2C_FIFO_DEPTH - 2)) {
        return E_BAD_PARAM;
    }

    i2c_flush(i2c);

    i2c->fifo = addr;
    i2c->m |= MXC_F_I2C_M_START;
    i2c->fifo = reg;

    while (len--) {
        i2c->fifo = *buf++;
    }

    i2c->m |= MXC_F_I2C_M_STOP;

    while (!(i2c->int_fl0 & MXC_F_I2C_INT_FL0_STOP));
    i2c->int_fl0 = MXC_F_I2C_INT_FL0_STOP;

    return (i2c->int_fl0 & MXC_I2C_ERROR) ? E_COMM_ERR : E_NO_ERROR;
}

int i2c_master_reg_read(mxc_i2c_regs_t *i2c, uint8_t addr, uint8_t reg, uint8_t *buf)
{
    i2c_flush(i2c);

    i2c->fifo = addr;
    i2c->m = MXC_F_I2C_M_START;
    i2c->fifo = reg;

    i2c->rx_ctrl1 = 1;
    i2c->m = MXC_F_I2C_M_RESTART;
    while (i2c->m & MXC_F_I2C_M_RESTART);
    i2c->fifo = addr | 0x01;

    while (!(i2c->int_fl0 & MXC_F_I2C_INT_FL0_RX_THRESH));
    *buf = i2c->fifo;
    i2c->int_fl0 = MXC_F_I2C_INT_FL0_RX_THRESH;

    i2c->m = MXC_F_I2C_M_STOP;
    while (!(i2c->int_fl0 & MXC_F_I2C_INT_FL0_STOP));
    i2c->int_fl0 = MXC_F_I2C_INT_FL0_STOP;

    return (i2c->int_fl0 & MXC_I2C_ERROR) ? E_COMM_ERR : E_NO_ERROR;
}

int i2c_master_reg_read_buf(mxc_i2c_regs_t *i2c, uint8_t addr, uint8_t reg, uint8_t *buf, int len)
{
    i2c_flush(i2c);

    i2c->fifo = addr;
    i2c->m = MXC_F_I2C_M_START;
    i2c->fifo = reg;

    i2c->rx_ctrl1 = 1;
    i2c->m = MXC_F_I2C_M_RESTART;
    while (i2c->m & MXC_F_I2C_M_RESTART);
    i2c->fifo = addr | 0x01;

    while (!(i2c->int_fl0 & MXC_F_I2C_INT_FL0_RX_THRESH));

    while (len--) {
        *buf = i2c->fifo;
    }
    i2c->int_fl0 = MXC_F_I2C_INT_FL0_RX_THRESH;

    i2c->m = MXC_F_I2C_M_STOP;
    while (!(i2c->int_fl0 & MXC_F_I2C_INT_FL0_STOP));
    i2c->int_fl0 = MXC_F_I2C_INT_FL0_STOP;

    return (i2c->int_fl0 & MXC_I2C_ERROR) ? E_COMM_ERR : E_NO_ERROR;
}

void I2C0_IRQHandler(void)
{
    MXC_I2C_AsyncHandler(MXC_I2C0_BUS0);
    return;
}

int i2c_master_init(mxc_i2c_regs_t *i2c, unsigned int speed)
{
    int err;

    if ((err = MXC_I2C_Init(i2c, 1, 0)) != E_NO_ERROR) {
        PR_ERROR("MXC_I2C_Init failed %d", err);
        return err;
    }

    if ((err = MXC_I2C_SetFrequency(i2c, speed)) <= 0) {
        PR_ERROR("MXC_I2C_SetFrequency failed %d", err);
        return err;
    }

    if ((err = MXC_I2C_SetTXThreshold(i2c, 1)) != E_NO_ERROR) {
        PR_ERROR("MXC_I2C_SetTXThreshold failed %d", err);
        return err;
    }

    if ((err = MXC_I2C_SetRXThreshold(i2c, 1)) != E_NO_ERROR) {
        PR_ERROR("MXC_I2C_SetRXThreshold failed %d", err);
        return err;
    }

    MXC_GPIO0->ds0 |= 1 << 6;
    MXC_GPIO0->ds1 |= 1 << 7;

    return E_NO_ERROR;
}
