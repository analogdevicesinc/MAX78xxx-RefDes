/* ****************************************************************************
 * Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
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
 *************************************************************************** */

/* **** Includes **** */
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include "mxc_device.h"
#include "mxc_assert.h"
#include "mxc_lock.h"
#include "mxc_sys.h"
#include "mxc_delay.h"
#include "dma.h"
#include "i2s_regs.h"
#include "i2s.h"

int MXC_I2S_RevA_Init(mxc_i2s_req_t *req);

int MXC_I2S_RevA_Shutdown(void);

int MXC_I2S_RevA_ConfigData(mxc_i2s_req_t *req);

void MXC_I2S_RevA_TXEnable(void);

void MXC_I2S_RevA_TXDisable(void);

void MXC_I2S_RevA_RXEnable(void);

void MXC_I2S_RevA_RXDisable(void);

int MXC_I2S_RevA_SetRXThreshold(uint8_t threshold);

int MXC_I2S_RevA_SetFrequency(mxc_i2s_ch_mode_t mode, uint16_t clkdiv);

void MXC_I2S_RevA_Flush(void);

void MXC_I2S_RevA_EnableInt(uint32_t flags);

void MXC_I2S_RevA_DisableInt(uint32_t flags);

int MXC_I2S_RevA_GetFlags(void);

void MXC_I2S_RevA_ClearFlags(uint32_t flags);

void MXC_I2S_RevA_TXDMAConfig(void *src_addr, int len);

void MXC_I2S_RevA_RXDMAConfig(void *dest_addr, int len);
