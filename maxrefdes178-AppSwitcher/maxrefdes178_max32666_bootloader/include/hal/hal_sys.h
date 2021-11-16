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

#ifndef HAL_SYS_H
#define HAL_SYS_H

#include <stdint.h>

typedef enum {
    HAL_SYS_I2C0,
    HAL_SYS_I2C1
} hal_sys_dev_t;

void hal_sys_init(void);

void hal_sys_delay(unsigned long us);

void hal_sys_jump_fw(void *fw_entry);

void hal_sys_reset(void);

void hal_sys_lock_swd(void);

void hal_sys_reset_dev(hal_sys_dev_t dev);

uint32_t hal_sys_start_systick(void);

uint32_t hal_sys_get_systick_freq(void);

void hal_sys_disable_systick(void);

void hal_sys_icc_enable(void);

void hal_sys_icc_disable(void);

#endif /* HAL_SYS_H */
