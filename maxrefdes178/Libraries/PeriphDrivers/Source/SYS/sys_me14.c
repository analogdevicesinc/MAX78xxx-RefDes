/* ****************************************************************************
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
 * $Date: 2020-01-17 08:24:56 -0600 (Fri, 17 Jan 2020) $
 * $Revision: 50770 $
 *
 *************************************************************************** */

/**
 * @file mxc_sys.c
 * @brief      System layer driver.
 * @details    This driver is used to control the system layer of the device.
 */

/* **** Includes **** */
#include <stddef.h>
#include "mxc_device.h"
#include "mxc_assert.h"
#include "mxc_sys.h"
#include "flc.h"
#include "mxc_delay.h"
#include "gcr_regs.h"
#include "fcr_regs.h"
#include "mcr_regs.h"

/**
 * @ingroup mxc_sys
 * @{
 */

/* **** Definitions **** */
#define MXC_SYS_CLOCK_TIMEOUT       MXC_DELAY_MSEC(1)
#define MXC_I2C0  MXC_I2C0_BUS0
#define MXC_I2C1  MXC_I2C1_BUS0
#define MXC_I2C2  MXC_I2C2_BUS0

/* **** Globals **** */

/* **** Functions **** */

/* ************************************************************************** */
int MXC_SYS_GetUSN (uint8_t* serialNumber, int len)
{
    if (len != MXC_SYS_USN_LEN) {
        return E_BAD_PARAM;
    }

    uint32_t infoblock[6];

    MXC_FLC_UnlockInfoBlock (0x0000);
    infoblock[0] = * (uint32_t *) MXC_INFO_MEM_BASE;
    infoblock[1] = * (uint32_t *) (MXC_INFO_MEM_BASE+4);
    infoblock[2] = * (uint32_t *) (MXC_INFO_MEM_BASE+8);
    infoblock[3] = * (uint32_t *) (MXC_INFO_MEM_BASE+12);
    infoblock[4] = * (uint32_t *) (MXC_INFO_MEM_BASE+16);
    infoblock[5] = * (uint32_t *) (MXC_INFO_MEM_BASE+20);
    MXC_FLC_LockInfoBlock (0x0000);

    serialNumber[0]  = (infoblock[0] & 0x007F8000) >> 15;
    serialNumber[1]  = (infoblock[0] & 0x7F800000) >> 23;
    serialNumber[2]  = (infoblock[0] & 0x80000000) >> 31;
    serialNumber[2] |= (infoblock[1] & 0x0000007F) << 1;
    serialNumber[3]  = (infoblock[1] & 0x00007F80) >> 7;
    serialNumber[4]  = (infoblock[1] & 0x007F8000) >> 15;
    serialNumber[5]  = (infoblock[1] & 0x7F800000) >> 23;
    serialNumber[6]  = (infoblock[2] & 0x007F8000) >> 15;
    serialNumber[7]  = (infoblock[2] & 0x7F800000) >> 23;
    serialNumber[8]  = (infoblock[2] & 0x80000000) >> 31;
    serialNumber[8] |= (infoblock[3] & 0x0000007F) << 1;
    serialNumber[9]  = (infoblock[3] & 0x00007F80) >> 7;
    serialNumber[10] = (infoblock[3] & 0x007F8000) >> 15;
    serialNumber[11] = (infoblock[3] & 0x7F800000) >> 23;
    serialNumber[12] = (infoblock[4] & 0x007F8000) >> 15;

    return E_NO_ERROR;
}

/* ************************************************************************** */
int MXC_SYS_IsClockEnabled (mxc_sys_periph_clock_t clock)
{
    /* The mxc_sys_periph_clock_t enum uses enum values that are the offset by 32 for the perckcn1 register. */
    if (clock > 31) {
        clock -= 32;
        return ! (MXC_GCR->pclk_dis1& (0x1 << clock));
    }
    else {
        return ! (MXC_GCR->pclk_dis0 & (0x1 << clock));
    }
}

/* ************************************************************************** */
void MXC_SYS_ClockDisable (mxc_sys_periph_clock_t clock)
{
    /* The mxc_sys_periph_clock_t enum uses enum values that are the offset by 32 for the perckcn1 register. */
    if (clock > 31) {
        clock -= 32;
        MXC_GCR->pclk_dis1 |= (0x1 << clock);
    }
    else {
        MXC_GCR->pclk_dis0 |= (0x1 << clock);
    }
}

/* ************************************************************************** */
void MXC_SYS_ClockEnable (mxc_sys_periph_clock_t clock)
{
    /* The mxc_sys_periph_clock_t enum uses enum values that are the offset by 32 for the perckcn1 register. */
    if (clock > 31) {
        clock -= 32;
        MXC_GCR->pclk_dis1 &= ~ (0x1 << clock);
    }
    else {
        MXC_GCR->pclk_dis0 &= ~ (0x1 << clock);
    }
}
/* ************************************************************************** */
void MXC_SYS_RTCClockEnable()
{
    MXC_GCR->clk_ctrl |= MXC_F_GCR_CLK_CTRL_X32K_EN;
}

/* ************************************************************************** */
int MXC_SYS_RTCClockDisable (void)
{
    /* Check that the RTC is not the system clock source */
    if ((MXC_GCR->clk_ctrl & MXC_F_GCR_CLK_CTRL_SYSOSC_SEL) != MXC_S_GCR_CLK_CTRL_SYSOSC_SEL_XTAL32K) {
        MXC_GCR->clk_ctrl &= ~MXC_F_GCR_CLK_CTRL_X32K_EN;
        return E_NO_ERROR;
    } else {
        return E_BAD_STATE;
    }
}

/******************************************************************************/
int MXC_SYS_ClockSourceEnable (mxc_sys_system_clock_t clock)
{
    switch(clock) {
        case MXC_SYS_CLOCK_HIRC96:
            MXC_GCR->clk_ctrl |= MXC_F_GCR_CLK_CTRL_HIRCMM_EN;
            return MXC_SYS_Clock_Timeout(MXC_F_GCR_CLK_CTRL_HIRCMM_RDY);
            break;
        case MXC_SYS_CLOCK_HIRC8:
            MXC_GCR->clk_ctrl |= MXC_F_GCR_CLK_CTRL_HIRC8M_EN;
            return MXC_SYS_Clock_Timeout(MXC_F_GCR_CLK_CTRL_HIRC8M_RDY);
            break;
        case MXC_SYS_CLOCK_HIRC:
            MXC_GCR->clk_ctrl |= MXC_F_GCR_CLK_CTRL_HIRC60M_EN;
            return MXC_SYS_Clock_Timeout(MXC_F_GCR_CLK_CTRL_HIRC60M_RDY);
            break;
        case MXC_SYS_CLOCK_LIRC8K:
            // The 8k clock is always enabled
            return MXC_SYS_Clock_Timeout(MXC_F_GCR_CLK_CTRL_LIRC8K_RDY);
            break;
        case MXC_SYS_CLOCK_XTAL32M:
            MXC_GCR->clk_ctrl |= MXC_F_GCR_CLK_CTRL_X32M_EN;
            return MXC_SYS_Clock_Timeout(MXC_F_GCR_CLK_CTRL_X32M_RDY);
            break;
        case MXC_SYS_CLOCK_XTAL32K:
            MXC_GCR->clk_ctrl |= MXC_F_GCR_CLK_CTRL_X32K_EN;
            return MXC_SYS_Clock_Timeout(MXC_F_GCR_CLK_CTRL_X32K_RDY);
            break;
        default:
            return E_BAD_PARAM;
            break;
    }
}

/******************************************************************************/
int MXC_SYS_ClockSourceDisable (mxc_sys_system_clock_t clock)
{
    uint32_t current_clock;

    current_clock = MXC_GCR->clk_ctrl & MXC_F_GCR_CLK_CTRL_SYSOSC_SEL;

    // Don't turn off the clock we're running on
    if(clock == current_clock) {
        return E_BAD_PARAM;
    }

    switch(clock) {
        case MXC_SYS_CLOCK_HIRC96:
            MXC_GCR->clk_ctrl &= ~MXC_F_GCR_CLK_CTRL_HIRCMM_EN;
            break;
        case MXC_SYS_CLOCK_HIRC8:
            MXC_GCR->clk_ctrl &= ~MXC_F_GCR_CLK_CTRL_HIRC8M_EN;
            break;
        case MXC_SYS_CLOCK_HIRC:
            MXC_GCR->clk_ctrl &= ~MXC_F_GCR_CLK_CTRL_HIRC60M_EN;
            break;
        case MXC_SYS_CLOCK_LIRC8K:
            // The 8k clock is always enabled
            break;
        case MXC_SYS_CLOCK_XTAL32M:
            MXC_GCR->clk_ctrl &= ~MXC_F_GCR_CLK_CTRL_X32M_EN;
            break;
        case MXC_SYS_CLOCK_XTAL32K:
            MXC_GCR->clk_ctrl &= ~MXC_F_GCR_CLK_CTRL_X32K_EN;
            break;
        default:
            return E_BAD_PARAM;
    }

    return E_NO_ERROR;
}

/* ************************************************************************** */
int MXC_SYS_Clock_Timeout(uint32_t ready)
{
    // Start timeout, wait for ready
    MXC_DelayAsync(MXC_SYS_CLOCK_TIMEOUT, NULL);

    do {
        if (MXC_GCR->clk_ctrl & ready) {
            MXC_DelayAbort();
            return E_NO_ERROR;
        }
    }
    while (MXC_DelayCheck() == E_BUSY);



    return E_TIME_OUT;
}

/* ************************************************************************** */
int MXC_SYS_Clock_Select (mxc_sys_system_clock_t clock)
{
    uint32_t current_clock;
    
    // Save the current system clock
    current_clock = MXC_GCR->clk_ctrl & MXC_F_GCR_CLK_CTRL_SYSOSC_SEL;
    
    switch (clock) {
        case MXC_SYS_CLOCK_HIRC96:
            // Enable HIRC96 clock
            if(!(MXC_GCR->clk_ctrl & MXC_F_GCR_CLK_CTRL_HIRCMM_EN)) {

                MXC_GCR->clk_ctrl |= MXC_F_GCR_CLK_CTRL_HIRCMM_EN;
                        
                // Check if HIRC96 clock is ready
                if (MXC_SYS_Clock_Timeout(MXC_F_GCR_CLK_CTRL_HIRCMM_RDY) != E_NO_ERROR) {
                    return E_TIME_OUT;
                }
            }
            
            // Set HIRC96 clock as System Clock
            MXC_SETFIELD(MXC_GCR->clk_ctrl, MXC_F_GCR_CLK_CTRL_SYSOSC_SEL, MXC_S_GCR_CLK_CTRL_SYSOSC_SEL_HIRC96);
            
            break;
        case MXC_SYS_CLOCK_HIRC8:
            // Enable HIRC8 clock
            if(!(MXC_GCR->clk_ctrl & MXC_F_GCR_CLK_CTRL_HIRC8M_EN)) {
                MXC_GCR->clk_ctrl |=MXC_F_GCR_CLK_CTRL_HIRC8M_EN;
                
                // Check if HIRC8 clock is ready
                if (MXC_SYS_Clock_Timeout(MXC_F_GCR_CLK_CTRL_HIRC8M_RDY) != E_NO_ERROR) {
                    return E_TIME_OUT;
                }
            }
            
            // Set HIRC8 clock as System Clock
            MXC_SETFIELD(MXC_GCR->clk_ctrl, MXC_F_GCR_CLK_CTRL_SYSOSC_SEL, MXC_S_GCR_CLK_CTRL_SYSOSC_SEL_HIRC8);
            
            break;
        case MXC_SYS_CLOCK_HIRC:
            // Enable HIRC clock
            if(!(MXC_GCR->clk_ctrl & MXC_F_GCR_CLK_CTRL_HIRC60M_EN)) {
                MXC_GCR->clk_ctrl |=MXC_F_GCR_CLK_CTRL_HIRC60M_EN;
                
                // Check if HIRC clock is ready
                if (MXC_SYS_Clock_Timeout(MXC_F_GCR_CLK_CTRL_HIRC60M_RDY) != E_NO_ERROR) {
                    return E_TIME_OUT;
                }
            }
            
            // Set HIRC clock as System Clock
            MXC_SETFIELD(MXC_GCR->clk_ctrl, MXC_F_GCR_CLK_CTRL_SYSOSC_SEL, MXC_S_GCR_CLK_CTRL_SYSOSC_SEL_HIRC);
            
            break;
        case MXC_SYS_CLOCK_XTAL32M:
            // Enable XTAL32M clock
            if(!(MXC_GCR->clk_ctrl & MXC_F_GCR_CLK_CTRL_X32M_EN)) {
                MXC_GCR->clk_ctrl |=MXC_F_GCR_CLK_CTRL_X32M_EN;
                
                // Check if XTAL32M clock is ready
                if (MXC_SYS_Clock_Timeout(MXC_F_GCR_CLK_CTRL_X32M_RDY) != E_NO_ERROR) {
                    return E_TIME_OUT;
                }
            }
            
            // Set XTAL32M clock as System Clock
            MXC_SETFIELD(MXC_GCR->clk_ctrl, MXC_F_GCR_CLK_CTRL_SYSOSC_SEL, MXC_S_GCR_CLK_CTRL_SYSOSC_SEL_XTAL32M);
            
            break;
        case MXC_SYS_CLOCK_LIRC8K:
            // Set LIRC8 clock as System Clock
            MXC_SETFIELD(MXC_GCR->clk_ctrl, MXC_F_GCR_CLK_CTRL_SYSOSC_SEL, MXC_S_GCR_CLK_CTRL_SYSOSC_SEL_LIRC8);
            
            break;
        case MXC_SYS_CLOCK_XTAL32K:
            // Enable XTAL32K clock
            if(!(MXC_GCR->clk_ctrl & MXC_F_GCR_CLK_CTRL_X32K_EN)) {
                MXC_GCR->clk_ctrl |=MXC_F_GCR_CLK_CTRL_X32K_EN;
                
                // Check if XTAL32K clock is ready
                if (MXC_SYS_Clock_Timeout(MXC_F_GCR_CLK_CTRL_X32K_RDY) != E_NO_ERROR) {
                    return E_TIME_OUT;
                }
            }
            
            // Set XTAL32K clock as System Clock
            MXC_SETFIELD(MXC_GCR->clk_ctrl, MXC_F_GCR_CLK_CTRL_SYSOSC_SEL, MXC_S_GCR_CLK_CTRL_SYSOSC_SEL_XTAL32K);
            
            break;
        default:
            return E_BAD_PARAM;
    }
    
    // Wait for system clock to be ready
    if (MXC_SYS_Clock_Timeout(MXC_F_GCR_CLK_CTRL_SYSOSC_RDY) != E_NO_ERROR) {
    
        // Restore the old system clock if timeout
        MXC_SETFIELD(MXC_GCR->clk_ctrl, MXC_F_GCR_CLK_CTRL_SYSOSC_SEL, current_clock);
        
        return E_TIME_OUT;
    }

    // Update the system core clock
    SystemCoreClockUpdate();
    
    return E_NO_ERROR;
}

/* ************************************************************************** */
void MXC_SYS_Clock_Div (mxc_sys_system_div_t div)
{
    MXC_SETFIELD(MXC_GCR->clk_ctrl, MXC_F_GCR_CLK_CTRL_SYSCLK_PRESCALE, div);

    // Update the system core clock
    SystemCoreClockUpdate();
}

/* ************************************************************************** */
void MXC_SYS_Reset_Periph (mxc_sys_reset_t reset)
{
    /* The mxc_sys_reset_t enum uses enum values that are the offset by 32 for the rstr1 register. */
    if (reset > 31) {
        reset -= 32;
        MXC_GCR->rst1 = (0x1 << reset);
    }
    else {
        MXC_GCR->rst0 = (0x1 << reset);
    }
}

/* ************************************************************************** */
uint8_t MXC_SYS_GetRev (void)
{

    uint8_t serialNumber[13];
    MXC_SYS_GetUSN (serialNumber, 13);
    
    
    if ( (serialNumber[0] < 0x9F) | ( (serialNumber[0] & 0x0F) > 0x09)) {
        // Fail back to the hardware register
        return MXC_GCR->revision & 0xFF;
    }
    
    return serialNumber[0];
}


/**@} end of mxc_sys */

