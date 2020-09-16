/* *****************************************************************************
 * Copyright (C) 2018 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2018-10-16 00:49:29 +0300 (Tue, 16 Oct 2018) $
 * $Revision: 38520 $
 *
 **************************************************************************** */

/* **** Includes **** */
#include <stddef.h>
#include "pt.h"


/* ************************************************************************* */
void PT_Init(mxc_ptg_regs_t *ptg, const sys_cfg_ptg_t* sys_cfg)
{
    SYS_PT_Init(sys_cfg);

    //disable all pulse trains
    ptg->enable = 0;
    
    //clear all interrupts
    ptg->intfl = ptg->intfl;
}

/* ************************************************************************* */
int PT_PTConfig(mxc_pt_regs_t *pt, pt_pt_cfg_t *cfg, const sys_cfg_pt_t *sysCfg)
{
    int err;
    uint32_t ptClock;
    uint32_t rate;
    
    //check for valid base pointer
    MXC_ASSERT(MXC_PT_GET_IDX(pt) >= 0);
    
    if (cfg == NULL) {
        return E_NULL_PTR;
    }
    
    if (cfg->bps == 0) {
        return E_BAD_PARAM;
    }
    
    //disable pulse train
    PT_Stop(pt);
    
    //setup system GPIO configuration
    if ((err = SYS_PT_Config(pt, sysCfg)) != E_NO_ERROR) {
        return err;
    }
    
    //get PT clock frequency from SYS level
    ptClock = SYS_PT_GetFreq();
    
    if (ptClock == 0) {
        return E_UNINITIALIZED;
    }
    
    if (ptClock < (cfg->bps)) {
        return E_BAD_STATE;
    }
    
    rate = (ptClock / (cfg->bps));
    
    pt->rate_length = ((rate << MXC_F_PT_RATE_LENGTH_RATE_CONTROL_POS)
                       & MXC_F_PT_RATE_LENGTH_RATE_CONTROL) |
                      ((cfg->ptLength << MXC_F_PT_RATE_LENGTH_MODE_POS)
                       & MXC_F_PT_RATE_LENGTH_MODE);
                       
    pt->train = cfg->pattern;
    pt->loop = ((cfg->loop << MXC_F_PT_LOOP_COUNT_POS) & MXC_F_PT_LOOP_COUNT) |
               ((cfg->loopDelay << MXC_F_PT_LOOP_DELAY_POS) & MXC_F_PT_LOOP_DELAY);
               
    return E_NO_ERROR;
}

/* ************************************************************************* */
int PT_SqrWaveConfig(mxc_pt_regs_t *pt, uint32_t freq, const sys_cfg_pt_t *sysCfg)
{
    uint32_t rate;
    
    // This is not an exact match, but good for frequencies less than PClk/50
    rate = 2*freq;

    pt_pt_cfg_t sqwcfg;
    sqwcfg.bps = rate;
    sqwcfg.pattern = 0;
    sqwcfg.ptLength = MXC_V_PT_RATE_LENGTH_MODE_SQUARE_WAVE;
    sqwcfg.loop = 0;
    sqwcfg.loopDelay = 0;

    return PT_PTConfig(pt, &sqwcfg, sysCfg);
}

/* ************************************************************************* */
void PT_Start(mxc_pt_regs_t *pt)
{
    mxc_ptg_regs_t * ptg;
    int ptIndex = MXC_PT_GET_IDX(pt);
    
    MXC_ASSERT(ptIndex >= 0);
    
    ptg = MXC_PTG_GET_PTG(MXC_PT_GET_BASE(ptIndex));
    
    ptg->enable |= (1 << ptIndex);
    
    //wait for PT to start
    while ((ptg->enable & (1 << ptIndex)) == 0);
}

/* ************************************************************************* */
void PT_StartMulti(mxc_ptg_regs_t *ptg, uint32_t pts)
{
    MXC_ASSERT(ptg);
    
    ptg->enable |= pts;
    
    //wait for PTs to start
    while ((ptg->enable & pts) != pts);
}

/* ************************************************************************* */
void PT_Stop(mxc_pt_regs_t *pt)
{
    mxc_ptg_regs_t *ptg;
    int ptIndex = MXC_PT_GET_IDX(pt);

    MXC_ASSERT(ptIndex >= 0);

    ptg = MXC_PTG_GET_PTG(MXC_PT_GET_BASE(ptIndex));
    
    ptg->enable &= ~(1 << ptIndex);
}

/* ************************************************************************* */
void PT_StopMulti(mxc_ptg_regs_t *ptg, uint32_t pts)
{
    MXC_ASSERT(ptg);
    ptg->enable &= ~(pts);
}

/* ************************************************************************* */
uint32_t PT_IsActive(mxc_pt_regs_t *pt)
{
    mxc_ptg_regs_t *ptg;
    int ptIndex = MXC_PT_GET_IDX(pt);
    MXC_ASSERT(ptIndex >= 0);
    
    ptg = MXC_PTG_GET_PTG(MXC_PT_GET_BASE(ptIndex));
    
    return (!!(ptg->enable & (1 << ptIndex)));
}

/* ************************************************************************* */
uint32_t PT_IsActiveMulti(mxc_ptg_regs_t *ptg, uint32_t pts)
{
    MXC_ASSERT(ptg);
    return (ptg->enable & pts);
}

/* ************************************************************************* */
void PT_SetPattern(mxc_pt_regs_t *pt, uint32_t pattern)
{
    MXC_ASSERT(pt);
    pt->train = pattern;
}

/* ************************************************************************* */
float PT_GetBPS(mxc_pt_regs_t *pt)
{
    uint32_t rate = pt->rate_length & MXC_F_PT_RATE_LENGTH_RATE_CONTROL >> MXC_F_PT_RATE_LENGTH_RATE_CONTROL_POS;
    uint32_t ptClock = SYS_PT_GetFreq();
    
    return ((float)ptClock)/((float)rate);
}

/* ************************************************************************* */
void PT_EnableINT(mxc_pt_regs_t *pt)
{
    mxc_ptg_regs_t * ptg;
    int ptIndex = MXC_PT_GET_IDX(pt);
    MXC_ASSERT(ptIndex >= 0);
    
    ptg = MXC_PTG_GET_PTG(MXC_PT_GET_BASE(ptIndex));
    
    ptg->inten |= (1 << ptIndex);
}

/* ************************************************************************* */
void PT_EnableINTMulti(mxc_ptg_regs_t *ptg, uint32_t pts)
{
    MXC_ASSERT(ptg);
    ptg->inten |= pts;
}

/* ************************************************************************* */
void PT_DisableINT(mxc_pt_regs_t *pt)
{
    mxc_ptg_regs_t * ptg;
    int ptIndex = MXC_PT_GET_IDX(pt);
    MXC_ASSERT(ptIndex >= 0);
    
    ptg = MXC_PTG_GET_PTG(MXC_PT_GET_BASE(ptIndex));
    
    ptg->inten &= ~(1 << ptIndex);
}

/* ************************************************************************* */
void PT_DisableINTMulti(mxc_ptg_regs_t *ptg, uint32_t pts)
{
    MXC_ASSERT(ptg);
    ptg->inten &= ~pts;
}

/* ************************************************************************* */
uint32_t PT_GetFlags(mxc_ptg_regs_t *ptg)
{
    MXC_ASSERT(ptg);
    return ptg->intfl;
}

/* ************************************************************************* */
void PT_ClearFlags(mxc_ptg_regs_t *ptg, uint32_t mask)
{
    MXC_ASSERT(ptg);
    ptg->intfl = mask;
}

/* ************************************************************************* */
void PT_SetRestart(mxc_pt_regs_t *ptToRestart, mxc_pt_regs_t *ptStop, uint8_t restartIndex)
{
    int ptStopIndex = MXC_PT_GET_IDX(ptStop);
    
    MXC_ASSERT(ptStopIndex >= 0);
    
    if (restartIndex) {
        ptToRestart->restart |= (ptStopIndex << MXC_F_PT_RESTART_PT_Y_SELECT_POS) |
                                MXC_F_PT_RESTART_ON_PT_Y_LOOP_EXIT;
    } else {
        ptToRestart->restart |= (ptStopIndex << MXC_F_PT_RESTART_PT_X_SELECT_POS) |
                                MXC_F_PT_RESTART_ON_PT_X_LOOP_EXIT;
    }
}

/* ************************************************************************* */
void PT_RestartDisable(mxc_pt_regs_t *ptToRestart, uint8_t restartIndex)
{
    MXC_ASSERT(ptToRestart);
    if (restartIndex) {
        ptToRestart->restart &= ~MXC_F_PT_RESTART_ON_PT_Y_LOOP_EXIT;
    } else {
        ptToRestart->restart &= ~MXC_F_PT_RESTART_ON_PT_X_LOOP_EXIT;
    }
}

/* ************************************************************************* */
void PT_Resync(mxc_ptg_regs_t *ptg, uint32_t resyncPts)
{
    MXC_ASSERT(ptg);
    // Set the PT to be synced
    ptg->resync |= resyncPts;
    
    // Wait for them to sync
    while (ptg->resync & resyncPts);
}
