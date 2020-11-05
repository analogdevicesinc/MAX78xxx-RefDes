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
#include "mxc_device.h"
#include "smon_reva.h"
#include "mxc_pins.h"
#include "mxc_lock.h"
#include "mxc_delay.h"


int MXC_SMON_RevA_ExtSensorEnable(mxc_smon_ext_cfg_t* cfg, uint32_t delay)
{
    int err;
    
    if (cfg == NULL) {
        return E_NULL_PTR;
    }
    
    if ((err = MXC_SMON_SetSensorFrequency(cfg)) != E_NO_ERROR) {
        return err;
    }
    
    if ((err = MXC_SMON_SetErrorCount(cfg->errorCount)) != E_NO_ERROR) {
        return err;
    }
    
    //Enable external sensor
    MXC_SMON->extscn |= cfg->sensorNumber;
    
    if ((err = MXC_SMON_isBusy(SMON_EXTSENSOR, delay)) != E_NO_ERROR) {
        return err;
    }
    
    return err;
}

int MXC_SMON_RevA_SetSensorFrequency(mxc_smon_ext_cfg_t* cfg)
{
    int err;
    
    if (cfg == NULL) {
        return E_NULL_PTR;
    }
    
    MXC_SMON->extscn |= (cfg->clockDivide | cfg->freqDivide);
    
    if ((err = MXC_SMON_isBusy(SMON_EXTSENSOR, 0)) != E_NO_ERROR) {
        return err;
    }
    
    return err;
}

int MXC_SMON_RevA_SetErrorCount(uint8_t errorCount)
{
    int err;
    
    if (errorCount > 31) {
        return E_BAD_PARAM;
    }
    
    MXC_SMON->extscn &= ~MXC_F_SMON_EXTSCN_EXTCNT;
    MXC_SMON->extscn |= errorCount << MXC_F_SMON_EXTSCN_EXTCNT_POS;
    
    if ((err = MXC_SMON_isBusy(SMON_EXTSENSOR, 0)) != E_NO_ERROR) {
        return err;
    }
    
    return err;
}

int MXC_SMON_RevA_TempSensorEnable(mxc_smon_temp_t threshold, uint32_t delay)
{
    int err;
    
    if ((err = MXC_SMON_SetTempThreshold(threshold)) != E_NO_ERROR) {
        return err;
    }
    
    MXC_SMON->intscn |= MXC_F_SMON_INTSCN_TEMP_EN;                  //Enable Sensor
    
    if ((err = MXC_SMON_isBusy(SMON_INTSENSOR, delay)) != E_NO_ERROR) {
        return err;
    }
    
    return err;
}

int MXC_SMON_RevA_SetTempThreshold(mxc_smon_temp_t threshold)
{
    int err;
    
    if (threshold == SMON_TEMP_THRESHOLD_NEG_50) {
        MXC_SMON->intscn &= ~MXC_F_SMON_INTSCN_LOTEMP_SEL;
    }
    else if (threshold == SMON_TEMP_THRESHOLD_NEG_30) {
        MXC_SMON->intscn |= MXC_F_SMON_INTSCN_LOTEMP_SEL;
    }
    else {
        return E_BAD_PARAM;
    }
    
    if ((err = MXC_SMON_isBusy(SMON_INTSENSOR, 0)) != E_NO_ERROR) {
        return err;
    }
    
    return err;
}

int MXC_SMON_RevA_VoltageMonitorEnable(mxc_smon_vtm_t threshold, uint32_t delay)
{
    int err;
    
    if ((err = MXC_SMON_SetVTMThreshold(threshold)) != E_NO_ERROR) {
        return err;
    }
    
    MXC_SMON->intscn |= MXC_F_SMON_INTSCN_VBAT_EN;                  //Enable Sensor
    
    if ((err = MXC_SMON_isBusy(SMON_INTSENSOR, delay)) != E_NO_ERROR) {
        return err;
    }
    
    return err;
}

int MXC_SMON_RevA_SetVTMThreshold(mxc_smon_vtm_t threshold)
{
    int err;
    
    if (threshold == SMON_VTM_THRESHOLD_1_6) {
        MXC_SMON->intscn &= ~(MXC_F_SMON_INTSCN_VCORELOEN | MXC_F_SMON_INTSCN_VCOREHIEN);
    }
    else if (threshold == SMON_VTM_THRESHOLD_2_2) {
        MXC_SMON->intscn &= ~MXC_F_SMON_INTSCN_VCOREHIEN;
        MXC_SMON->intscn |= MXC_F_SMON_INTSCN_VCORELOEN;
    }
    else if (threshold == SMON_VTM_THRESHOLD_2_8) {
        MXC_SMON->intscn |= (MXC_F_SMON_INTSCN_VCORELOEN | MXC_F_SMON_INTSCN_VCOREHIEN);
    }
    else {
        return E_BAD_PARAM;
    }
    
    if ((err = MXC_SMON_isBusy(SMON_INTSENSOR, 0)) != E_NO_ERROR) {
        return err;
    }
    
    return err;
}

int MXC_SMON_RevA_ActiveDieShieldEnable(uint32_t delay)
{
    int err;
    
    MXC_SMON->intscn |= MXC_F_SMON_INTSCN_SHIELD_EN;                //Enable Sensor
    
    if ((err = MXC_SMON_isBusy(SMON_INTSENSOR, delay)) != E_NO_ERROR) {
        return err;
    }
    
    return err;
}

int MXC_SMON_RevA_SelfDestructByteEnable(mxc_smon_ext_cfg_t* cfg, uint32_t delay)
{
    int err;
    
    if (cfg == NULL) {
        return E_NULL_PTR;
    }
    
    MXC_SMON->sdbe &= ~MXC_F_SMON_SDBE_SBDEN;
    
    MXC_SMON->sdbe |= cfg->data << MXC_F_SMON_SDBE_DBYTE_POS;
    
    if ((err = MXC_SMON_ExtSensorEnable(cfg, delay)) != E_NO_ERROR) {
        return err;
    }
    
    if ((err = MXC_SMON_isBusy(SMON_INTSENSOR, delay)) != E_NO_ERROR) {
        return err;
    }
    
    MXC_SMON->sdbe |= MXC_F_SMON_SDBE_SBDEN;
    
    return err;
}

void MXC_SMON_RevA_EnablePUFTrimErase()
{
    MXC_SMON->intscn |= MXC_F_SMON_INTSCN_PUF_TRIM_ERASE;
    
    MXC_SMON_isBusy(SMON_INTSENSOR, 0);
}

void MXC_SMON_RevA_DisablePUFTrimErase()
{
    MXC_SMON->intscn &= ~MXC_F_SMON_INTSCN_PUF_TRIM_ERASE;
    
    MXC_SMON_isBusy(SMON_INTSENSOR, 0);
}

int MXC_SMON_RevA_DigitalFaultDetectorEnable(mxc_smon_interrupt_mode_t interruptMode, mxc_smon_lowpower_mode_t lowPowerMode, uint32_t delay)
{
    int err;
    
    if (interruptMode == SMON_DFD_INTERRUPT_NMI) {
        MXC_SMON->intscn &= ~MXC_F_SMON_INTSCN_DFD_STDBY;
    }
    else if (interruptMode == SMON_DFD_INTERRUPT_PFW) {
        MXC_SMON->intscn |= MXC_F_SMON_INTSCN_DFD_STDBY;
    }
    else {
        return E_BAD_PARAM;
    }
    
    if (lowPowerMode == SMON_DFD_LOWPOWER_ENABLE) {
        MXC_SMON->intscn &= ~MXC_F_SMON_INTSCN_DFD_NMI;
    }
    else if (lowPowerMode == SMON_DFD_LOWPOWER_DISABLE) {
        MXC_SMON->intscn |= MXC_F_SMON_INTSCN_DFD_NMI;
    }
    else {
        return E_BAD_PARAM;
    }
    
    MXC_SMON->intscn |= MXC_F_SMON_INTSCN_DFD_EN;                    //Enable DFD
    
    if ((err = MXC_SMON_isBusy(SMON_INTSENSOR, delay)) != E_NO_ERROR) {
        return err;
    }
    
    return err;
}

uint32_t MXC_SMON_RevA_GetFlags()
{
    return MXC_SMON->secalm;
}

void MXC_SMON_RevA_ClearFlags(uint32_t flags)
{
    MXC_SMON_RevA_isBusy(SMON_SECALARM, 0);
    MXC_SMON->secalm &= ~flags;
    MXC_SMON_RevA_isBusy(SMON_SECALARM, 0);
}

void MXC_SMON_RevA_ExtSensorLock()
{
    MXC_SMON->extscn |= MXC_F_SMON_EXTSCN_LOCK;
}

void MXC_SMON_RevA_IntSensorLock()
{
    MXC_SMON->intscn |= MXC_F_SMON_INTSCN_LOCK;
}

int MXC_SMON_RevA_isBusy(mxc_smon_busy_t reg, uint32_t delay)
{
    if (delay == 0) {
        while (MXC_SMON->secst & reg);
        
        return E_NO_ERROR;
    }
    
    MXC_Delay(delay);
    
    if (MXC_SMON->secst & reg) {
        return E_BUSY;
    }
    
    return E_NO_ERROR;
}
