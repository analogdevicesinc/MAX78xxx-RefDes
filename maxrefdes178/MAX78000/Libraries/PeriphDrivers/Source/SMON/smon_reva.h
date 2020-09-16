/* *****************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
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
 **************************************************************************** */

#include <stddef.h>
#include "mxc_assert.h"
#include "mxc_pins.h"
#include "mxc_lock.h"
#include "mxc_delay.h"
#include "mxc_device.h"
#include "smon.h"

int MXC_SMON_RevA_ExtSensorEnable (mxc_smon_ext_cfg_t* cfg, uint32_t delay);

int MXC_SMON_RevA_SetSensorFrequency (mxc_smon_ext_cfg_t* cfg);

int MXC_SMON_RevA_SetErrorCount (uint8_t errorCount);

int MXC_SMON_RevA_TempSensorEnable (mxc_smon_temp_t threshold, uint32_t delay);

int MXC_SMON_RevA_SetTempThreshold (mxc_smon_temp_t threshold);

int MXC_SMON_RevA_VoltageMonitorEnable (mxc_smon_vtm_t threshold, uint32_t delay);

int MXC_SMON_RevA_SetVTMThreshold (mxc_smon_vtm_t threshold);

int MXC_SMON_RevA_ActiveDieShieldEnable (uint32_t delay);

int MXC_SMON_RevA_SelfDestructByteEnable (mxc_smon_ext_cfg_t* cfg, uint32_t delay);

void MXC_SMON_RevA_EnablePUFTrimErase();

void MXC_SMON_RevA_DisablePUFTrimErase();

int MXC_SMON_RevA_DigitalFaultDetectorEnable (mxc_smon_interrupt_mode_t interruptMode, mxc_smon_lowpower_mode_t lowPowerMode, uint32_t delay);

uint32_t MXC_SMON_RevA_GetFlags();

void MXC_SMON_RevA_ClearFlags (uint32_t flags);

void MXC_SMON_RevA_ExtSensorLock();

void MXC_SMON_RevA_IntSensorLock();

int MXC_SMON_RevA_isBusy (mxc_smon_busy_t reg, uint32_t delay);
