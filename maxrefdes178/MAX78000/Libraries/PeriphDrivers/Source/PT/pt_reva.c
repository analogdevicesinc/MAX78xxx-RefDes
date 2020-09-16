/* ****************************************************************************
 * Copyright (C) 2019 Maxim Integrated Products, Inc., All Rights Reserved.
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

#include "pt.h"
#include "adc.h"
#include "gcr_regs.h"
#include "pt_regs.h"
#include "adc_regs.h"
#include "ptg_regs.h"
#include "pt_reva.h"


void MXC_PT_RevA_Init(mxc_clk_scale_t clk_scale)
{
    //disable all pulse trains
    MXC_PTG->enable = 0;
    
    //clear all interrupts
    MXC_PTG->intfl = MXC_PTG->intfl;
}

int  MXC_PT_RevA_Shutdown(uint32_t pts)
{
    MXC_PT_Stop(pts);
    return (!(MXC_PT_IsActive(MXC_PTG->enable)));
}

int  MXC_PT_RevA_Config(mxc_pt_cfg_t* cfg)
{
    uint32_t ptClock, rate;
    
    if (cfg == NULL || cfg->bps == 0) {
        return E_BAD_PARAM;
    }
    
    ptClock = PeripheralClock;
    
    if (ptClock == 0) {
        return E_UNINITIALIZED;
    }
    
    if (ptClock < (cfg->bps)) {
        return E_BAD_STATE;
    }
    
    //disable pulse train
    MXC_PT_Stop(1 << cfg->channel);
    
    rate = (ptClock / (cfg->bps));
    
    if (cfg->bps > 1000000) {           //To lessen the delta between generated and expected clock
        rate += 2;
    }
    
    mxc_pt_regs_t* temp = MXC_PT_GET_PT(cfg->channel);
    
    MXC_ASSERT(temp != NULL);
    
    
    temp->rate_length = ((rate << MXC_F_PT_RATE_LENGTH_RATE_CONTROL_POS)
                         & MXC_F_PT_RATE_LENGTH_RATE_CONTROL) |
                        ((cfg->ptLength << MXC_F_PT_RATE_LENGTH_MODE_POS)
                         & MXC_F_PT_RATE_LENGTH_MODE);
                         
    temp->train = cfg->pattern;
    temp->loop = ((cfg->loop << MXC_F_PT_LOOP_COUNT_POS) & MXC_F_PT_LOOP_COUNT) |
                 ((cfg->loopDelay << MXC_F_PT_LOOP_DELAY_POS) & MXC_F_PT_LOOP_DELAY);
                 
    return E_NO_ERROR;
}

int  MXC_PT_RevA_SqrWaveConfig(unsigned channel, uint32_t freq)
{
    uint32_t rate;
    
    rate = 2 * freq;
    mxc_pt_cfg_t sqwcfg;
    sqwcfg.channel = channel;
    sqwcfg.bps = rate;
    sqwcfg.pattern = 0;
    sqwcfg.ptLength = MXC_V_PT_RATE_LENGTH_MODE_SQUARE_WAVE;
    sqwcfg.loop = 0;
    sqwcfg.loopDelay = 0;
    
    return MXC_PT_Config(&sqwcfg);
    
}

void MXC_PT_RevA_Start(unsigned pts)
{

    MXC_ASSERT(pts >= 0)
    MXC_PTG->enable |= pts;
    
    //wait for PT to start
    while ((MXC_PTG->enable & (pts)) != pts);
}

void MXC_PT_RevA_Stop(unsigned pts)
{
    MXC_PTG->enable &= ~pts;
}

uint32_t MXC_PT_RevA_IsActive(uint32_t pts)
{
    return !!(MXC_PTG->enable & pts);
}

void  MXC_PT_RevA_SetPattern(unsigned pts, uint32_t pattern)
{
    mxc_pt_regs_t* temp;
    uint32_t pt;
    
    for (uint8_t i = 0 ; i < 32 && (pts >> i) > 0 ; i++) {
        pt = 1 << i;
        
        if (pts & pt) {
            temp = MXC_PT_GET_PT(pt);
            MXC_ASSERT(temp);
            temp->train = pattern;
        }
    }
}

void  MXC_PT_RevA_EnableInt(uint32_t pts)
{
    MXC_PTG->inten |= pts;
}

void  MXC_PT_RevA_DisableInt(uint32_t pts)
{
    MXC_PTG->inten &= ~pts;
}

uint32_t  MXC_PT_RevA_GetFlags(void)
{
    return MXC_PTG->intfl;
}

void  MXC_PT_RevA_ClearFlags(uint32_t flags)
{
    MXC_PTG->intfl = flags;
}

void  MXC_PT_RevA_EnableRestart(unsigned start, unsigned stop, uint8_t restartIndex)
{
    mxc_pt_regs_t* temp = MXC_PT_GET_PT(start);
    MXC_ASSERT(temp);
    
    if (restartIndex) {
        temp->restart |= (stop << MXC_F_PT_RESTART_PT_Y_SELECT_POS) |
                         MXC_F_PT_RESTART_ON_PT_Y_LOOP_EXIT;
    }
    else {
        temp->restart |= (stop << MXC_F_PT_RESTART_PT_X_SELECT_POS) |
                         MXC_F_PT_RESTART_ON_PT_X_LOOP_EXIT;
    }
    
}

void  MXC_PT_RevA_DisableRestart(unsigned channel, uint8_t restartIndex)
{
    mxc_pt_regs_t* temp = MXC_PT_GET_PT(channel);
    MXC_ASSERT(temp);
    
    if (restartIndex) {
        temp->restart &= ~MXC_F_PT_RESTART_ON_PT_Y_LOOP_EXIT;
    }
    else {
        temp->restart &= ~MXC_F_PT_RESTART_ON_PT_X_LOOP_EXIT;
    }
}

void  MXC_PT_RevA_Resync(uint32_t pts)
{
    // Set the PT to be synced
    MXC_PTG->resync |= pts;
    
    // Wait for them to sync
    while (MXC_PTG->resync & pts);
}
