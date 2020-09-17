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
 * $Date: 2020-07-30 23:33:15 +0300 (Thu, 30 Jul 2020) $
 * $Revision: 54553 $
 *
 **************************************************************************** */

/* **** Includes **** */
#include "mxc_config.h"
#include "mxc_assert.h"
#include "mxc_sys.h"
#include "wut.h"
#include "gcr_regs.h"

/* **** Definitions **** */

/* **** Globals **** */

/* **** Local Variables **** */
static uint32_t wut_count;
static uint32_t wut_snapshot;

/* **** Functions **** */

/* ************************************************************************** */
void WUT_Init(wut_pres_t pres)
{   
    // Disable timer and clear settings
    MXC_WUT->cn = 0;
    
    // Clear interrupt flag
    MXC_WUT->intr = MXC_F_WUT_INTR_IRQ_CLR;

    // Set the prescaler
    MXC_WUT->cn = pres;

    // Initialize the compare register
    MXC_WUT->cmp = 0xFFFFFFFF;

    // Initialize the local variables
    wut_count = 0;
    wut_snapshot = 0;
}

void WUT_Shutdown(void)
{   
    // Disable timer and clear settings
    MXC_WUT->cn = 0;
}

/* ************************************************************************** */
void WUT_Enable(void)
{
    MXC_WUT->cn |= MXC_F_WUT_CN_TEN;
}

/* ************************************************************************** */
void WUT_Disable(void)
{
    MXC_WUT->cn &= ~(MXC_F_WUT_CN_TEN);
}

/* ************************************************************************** */
void WUT_Config(const wut_cfg_t *cfg)
{
    // Configure the timer
    MXC_WUT->cn = (MXC_WUT->cn & ~(MXC_F_WUT_CN_TMODE | MXC_F_WUT_CN_TPOL)) |
              ((cfg->mode << MXC_F_WUT_CN_TMODE_POS) & MXC_F_WUT_CN_TMODE);
              
    MXC_WUT->cnt = 0x1;

    MXC_WUT->cmp = cfg->cmp_cnt;
}

/* ************************************************************************* */
wut_pres_t WUT_GetPres()
{
    return((wut_pres_t)(MXC_WUT->cn & (MXC_F_WUT_CN_PRES | MXC_F_WUT_CN_PRES3)));
}

/* ************************************************************************* */
wut_mode_t WUT_GetMode()
{
    return ((wut_mode_t)((MXC_WUT->cn & MXC_F_WUT_CN_TMODE) >> MXC_F_WUT_CN_TMODE_POS));
}

/* ************************************************************************** */
uint32_t WUT_GetCompare(void)
{
    return MXC_WUT->cmp;
}

/* ************************************************************************** */
uint32_t WUT_GetCapture(void)
{
    return MXC_WUT->pwm;
}

/* ************************************************************************* */
uint32_t WUT_GetCount(void)
{
    return MXC_WUT->cnt;
}

/* ************************************************************************* */
void WUT_IntClear(void)
{
    MXC_WUT->intr = MXC_F_WUT_INTR_IRQ_CLR;
}

/* ************************************************************************* */
uint32_t WUT_IntStatus(void)
{
    return MXC_WUT->intr;
}

/* ************************************************************************* */
void WUT_SetPres(wut_pres_t pres)
{
    MXC_WUT->cn = (pres & (MXC_F_WUT_CN_PRES | MXC_F_WUT_CN_PRES3));
}

/* ************************************************************************* */
void WUT_SetMode(wut_mode_t mode)
{
    MXC_WUT->cn = (MXC_WUT->cn & ~(MXC_F_WUT_CN_TMODE | MXC_F_WUT_CN_TPOL)) |
              ((mode << MXC_F_WUT_CN_TMODE_POS) & MXC_F_WUT_CN_TMODE);
}

/* ************************************************************************* */
void WUT_SetCompare(uint32_t cmp_cnt)
{
    MXC_WUT->cmp = cmp_cnt;
}

/* ************************************************************************* */
void WUT_SetCount(uint32_t cnt)
{
    MXC_WUT->cnt = cnt;
}

/* ************************************************************************* */
int WUT_GetTicks(uint32_t time, wut_unit_t units, uint32_t *ticks)
{
    uint32_t unit_div0, unit_div1;
    uint32_t timerClock;
    uint32_t prescale;
    uint64_t temp_ticks;
    
#ifdef __ICCARM__
    uint32_t temp_cn =  MXC_WUT->cn;
#endif
    
    timerClock = SYS_WUT_GetFreq();
    
#ifdef __ICCARM__  // This is being done to handle the IAR Warning: Pa082
    prescale = ((temp_cn & MXC_F_WUT_CN_PRES) >> MXC_F_WUT_CN_PRES_POS) 
        | (((temp_cn & MXC_F_WUT_CN_PRES3) >> (MXC_F_WUT_CN_PRES3_POS))<<3);
#else
    prescale = ((MXC_WUT->cn & MXC_F_WUT_CN_PRES) >> MXC_F_WUT_CN_PRES_POS) 
        | (((MXC_WUT->cn & MXC_F_WUT_CN_PRES3) >> (MXC_F_WUT_CN_PRES3_POS))<<3);
#endif    
    
    switch (units) {
        case WUT_UNIT_NANOSEC:
            unit_div0 = 1000000;
            unit_div1 = 1000;
            break;
        case WUT_UNIT_MICROSEC:
            unit_div0 = 1000;
            unit_div1 = 1000;
            break;
        case WUT_UNIT_MILLISEC:
            unit_div0 = 1;
            unit_div1 = 1000;
            break;
        case WUT_UNIT_SEC:
            unit_div0 = 1;
            unit_div1 = 1;
            break;
        default:
            return E_BAD_PARAM;
    }
    
    temp_ticks = (uint64_t)time * (timerClock / unit_div0) / (unit_div1 * (1 << (prescale & 0xF)));
    
    //make sure ticks is within a 32 bit value
    if (!(temp_ticks & 0xffffffff00000000)  && (temp_ticks & 0xffffffff)) {
        *ticks = temp_ticks;
        return E_NO_ERROR;
    }
    
    return E_INVALID;
}

/* ************************************************************************* */
int WUT_GetTime(uint32_t ticks, uint32_t *time, wut_unit_t *units)
{
    uint64_t temp_time = 0;
    uint32_t timerClock = SYS_WUT_GetFreq();
    
#ifdef __ICCARM__
    uint32_t temp_cn =  MXC_WUT->cn;
    
    uint32_t prescale = ((temp_cn & MXC_F_WUT_CN_PRES) >> MXC_F_WUT_CN_PRES_POS) 
        | (((temp_cn & MXC_F_WUT_CN_PRES3) >> (MXC_F_WUT_CN_PRES3_POS))<<3);
#else
     uint32_t prescale = ((MXC_WUT->cn & MXC_F_WUT_CN_PRES) >> MXC_F_WUT_CN_PRES_POS) 
        | (((MXC_WUT->cn & MXC_F_WUT_CN_PRES3) >> (MXC_F_WUT_CN_PRES3_POS))<<3);   
#endif
    
    temp_time = (uint64_t)ticks * 1000 * (1 << (prescale & 0xF)) / (timerClock / 1000000);
    if (!(temp_time & 0xffffffff00000000)) {
        *time = temp_time;
        *units = WUT_UNIT_NANOSEC;
        return E_NO_ERROR;
    }
    
    temp_time = (uint64_t)ticks * 1000 * (1 << (prescale & 0xF)) / (timerClock / 1000);
    if (!(temp_time & 0xffffffff00000000)) {
        *time = temp_time;
        *units = WUT_UNIT_MICROSEC;
        return E_NO_ERROR;
    }
    
    temp_time = (uint64_t)ticks * 1000 * (1 << (prescale & 0xF)) / timerClock;
    if (!(temp_time & 0xffffffff00000000)) {
        *time = temp_time;
        *units = WUT_UNIT_MILLISEC;
        return E_NO_ERROR;
    }
    
    temp_time = (uint64_t)ticks * (1 << (prescale & 0xF)) / timerClock;
    if (!(temp_time & 0xffffffff00000000)) {
        *time = temp_time;
        *units = WUT_UNIT_SEC;
        return E_NO_ERROR;
    }
    
    return E_INVALID;
}

/* ************************************************************************** */
void WUT_Edge(void)
{
    // Wait for a WUT edge
    uint32_t tmp = MXC_WUT->cnt;
    while (tmp == MXC_WUT->cnt) {}
}

/* ************************************************************************** */
void WUT_Store(void)
{
    wut_count = MXC_WUT->cnt;
    wut_snapshot = MXC_WUT->snapshot;
}

/* ************************************************************************** */
void WUT_RestoreBBClock(uint32_t dbbFreq)
{
    /* restore DBB clock from WUT */
    WUT_Edge();
    MXC_WUT->preset = wut_snapshot + (uint64_t)(MXC_WUT->cnt - wut_count + 1)
                      * dbbFreq / SYS_WUT_GetFreq();
    MXC_WUT->reload = 1;  // arm DBB_CNT update on the next rising WUT clock
    WUT_Edge();
}

/* ************************************************************************** */
uint32_t WUT_GetSleepTicks(void)
{
    return (MXC_WUT->cnt - wut_count);
}

/* ************************************************************************** */
void WUT_Delay_MS(uint32_t waitMs)
{
    /* assume WUT is already running */
    uint32_t  tmp = MXC_WUT->cnt;

    tmp += (waitMs * (SYS_WUT_GetFreq() / 
        (0x1 << ((MXC_WUT->cn & MXC_F_WUT_CN_PRES) >> MXC_F_WUT_CN_PRES_POS)))
        + 500) / 1000 ;

    while(MXC_WUT->cnt < tmp){}
}
