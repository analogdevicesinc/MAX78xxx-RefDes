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
*
**************************************************************************** */

#include "mxc_config.h"
#include "htmr.h"
#include "mxc_sys.h"
#include "mxc_delay.h"
#include "gpio_regs.h"
#include "mxc_errors.h"

#define HTMR_CTRL_RESET_DEFAULT (0x8000UL)
#define BUSY_TIMEOUT 10000   // Timeout counts for the Busy bit
#define ASYNC_MODE (htmr->ctrl & MXC_F_HTMR_CTRL_ACRE)

//******************************************************************************
int HTMR_EnableLongIntInterrupt(mxc_htmr_regs_t *htmr)
{

    if (HTMR_CheckBusy(htmr)) {
        return E_BUSY;
    }

    htmr->ctrl |= MXC_F_HTMR_CTRL_ADE;   // Enable Long Interval Interrupt

    return E_SUCCESS;
}

//******************************************************************************
int HTMR_DisableLongIntInterrupt(mxc_htmr_regs_t *htmr)
{

    if (HTMR_CheckBusy(htmr)) {
        return E_BUSY;
    }

    htmr->ctrl &= ~MXC_F_HTMR_CTRL_ADE;    // Disable Long Interval Interrupt
    if (HTMR_CheckBusy(htmr)) {
        return E_BUSY;
    }

    return E_SUCCESS;
}

//******************************************************************************
int HTMR_EnableShortIntInterrupt(mxc_htmr_regs_t *htmr)
{

    if (HTMR_CheckBusy(htmr)) {
        return E_BUSY;
    }
    
    htmr->ctrl |= MXC_F_HTMR_CTRL_ASE;    // Enable Short Interval Interrupt

    return E_SUCCESS;
}

//******************************************************************************
int HTMR_DisableShortIntInterrupt(mxc_htmr_regs_t *htmr)
{

    if (HTMR_CheckBusy(htmr)) {
        return E_BUSY;
    }

    htmr->ctrl &= ~MXC_F_HTMR_CTRL_ASE;    // Disable Short Interval Interrupt
    if (HTMR_CheckBusy(htmr)) {
        return E_BUSY;
    }

    return E_SUCCESS;
}

//******************************************************************************
int HTMR_SetLongIntAlarm(mxc_htmr_regs_t *htmr, uint32_t ras)
{
    if(ras > 0xFFFFF) {
        return E_BAD_PARAM;
    }

    if(HTMR_DisableLongIntInterrupt(htmr) == E_BUSY) {
        return E_BUSY;
    }

    if (HTMR_CheckBusy(htmr)) {
        return E_BUSY;
    }

    htmr->ras = (ras << MXC_F_HTMR_RAS_RAS_POS) & MXC_F_HTMR_RAS_RAS;
    
    if(HTMR_EnableLongIntInterrupt(htmr) == E_BUSY) {
        return E_BUSY;
    }

    return E_SUCCESS;
}

//******************************************************************************
int HTMR_SetShortIntAlarm(mxc_htmr_regs_t *htmr, uint32_t rssa)
{

    if(HTMR_DisableShortIntInterrupt(htmr) == E_BUSY) {
        return E_BUSY;
    }

    if (HTMR_CheckBusy(htmr)) {
        return E_BUSY;
    }

    htmr->rssa =  (rssa << MXC_F_HTMR_RSSA_RSSA_POS) & MXC_F_HTMR_RSSA_RSSA;

    if(HTMR_EnableShortIntInterrupt(htmr) == E_BUSY) {
        return E_BUSY;
    }

    return E_SUCCESS;
}


//******************************************************************************
int HTMR_Enable(mxc_htmr_regs_t *htmr)
{
    if (HTMR_CheckBusy(htmr)) {
        return E_BUSY;
    }

    htmr->ctrl |= MXC_F_HTMR_CTRL_WE;       // Allow writing to registers
    if (HTMR_CheckBusy(htmr)) {
        return E_BUSY;
    }
    // Can only write if WE=1 and BUSY=0
    htmr->ctrl |= MXC_F_HTMR_CTRL_HTEN;    // setting RTCE = 1
    if (HTMR_CheckBusy(htmr)) {
        return E_BUSY;
    }

    htmr->ctrl &= ~MXC_F_HTMR_CTRL_WE;       // Prevent Writing...

    return E_SUCCESS;
}

//******************************************************************************
int HTMR_Disable(mxc_htmr_regs_t *htmr)
{
    if (HTMR_CheckBusy(htmr)) {
        return E_BUSY;
    }

    htmr->ctrl |= MXC_F_HTMR_CTRL_WE;      // Allow writing to registers
    if (HTMR_CheckBusy(htmr)) {
        return E_BUSY;
    }
    // Can only write if WE=1 and BUSY=0
    htmr->ctrl &= ~MXC_F_HTMR_CTRL_HTEN;  // setting RTCE = 0
    if (HTMR_CheckBusy(htmr)) {
        return E_BUSY;
    }
    
    htmr->ctrl &= ~MXC_F_HTMR_CTRL_WE;       // Prevent Writing...
    
    return E_SUCCESS;
}


//******************************************************************************
int HTMR_Init(mxc_htmr_regs_t *htmr, uint32_t sec, uint8_t ssec, sys_cfg_htmr_t *sys_cfg)
{

    SYS_HTMR_Init(htmr);

    if (HTMR_CheckBusy(htmr)) {
        return E_BUSY;
    }

    htmr->ctrl = MXC_F_HTMR_CTRL_WE;        //  Allow Writes
    if (HTMR_CheckBusy(htmr)) {
        return E_BUSY;
    }

    htmr->ctrl = HTMR_CTRL_RESET_DEFAULT;  // Start with a Clean Register
    if (HTMR_CheckBusy(htmr)) {
        return E_BUSY;
    }

    htmr->ctrl |= MXC_F_HTMR_CTRL_WE;      // Set Write Enable, allow writing to reg.
    if (HTMR_CheckBusy(htmr)) {
        return E_BUSY;
    }

    htmr->ssec = ssec; 
    if (HTMR_CheckBusy(htmr)) {
        return E_BUSY;
    }

    htmr->sec = sec;
    if (HTMR_CheckBusy(htmr)) {
        return E_BUSY;
    }

    /*
     * If the peripheral clock is twice the frequency of the clock driving HTMR
     * we can enable Asynchronous read mode for htmr->sec and htmr->ssec registers
     */
    if(PeripheralClock > 2*HIRC8_FREQ) {
        htmr->ctrl |= MXC_F_HTMR_CTRL_ACRE;
    }

    htmr->ctrl &= ~MXC_F_HTMR_CTRL_WE;       // Prevent Writing...

    return E_SUCCESS;
}

//******************************************************************************
int HTMR_CheckBusy(mxc_htmr_regs_t *htmr)
{

    mxc_delay_start(MXC_DELAY_USEC(BUSY_TIMEOUT));
    while (htmr->ctrl & MXC_F_HTMR_CTRL_BUSY) {
        if (mxc_delay_check() != E_BUSY) {
            return E_BUSY;
        }
    }
    mxc_delay_stop();

    return E_SUCCESS;
}

//******************************************************************************
int HTMR_GetFlags(mxc_htmr_regs_t *htmr)
{
    return htmr->ctrl;
}

//******************************************************************************
int HTMR_ClearFlags(mxc_htmr_regs_t *htmr, int flags)
{
    if (HTMR_CheckBusy(htmr)) {
        return E_BUSY;
    }
    htmr->ctrl &= ~(flags);

    return E_SUCCESS;
}

//******************************************************************************
int HTMR_GetShortCount(mxc_htmr_regs_t *htmr)
{
    // Don't bother calling CheckBusy() if we're in async mode
    if(!ASYNC_MODE) {
        if (HTMR_CheckBusy(htmr)) {
            return E_BUSY;
        }        
    }

    return htmr->ssec; 
}

//******************************************************************************
int HTMR_GetLongCount(mxc_htmr_regs_t *htmr)
{
    // Don't bother calling CheckBusy() if we're in async mode
    if(!ASYNC_MODE) {
        if (HTMR_CheckBusy(htmr)) {
            return E_BUSY;
        }        
    }

    return htmr->sec; 
}