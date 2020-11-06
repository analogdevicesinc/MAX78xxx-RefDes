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

#include <stddef.h>
#include "mxc_config.h"
#include "rtc_regs.h"
#include "rtc.h"
#include "tmr.h"
#include "tmr_utils.h"
#include "mxc_sys.h"
#include "mxc_delay.h"
#include "mxc_errors.h"

#define RTC_CTRL_RESET_DEFAULT  (0x0000UL)
#define RTC_IS_BUSY             (MXC_RTC->ctrl & MXC_F_RTC_CTRL_BUSY)
#define RTC_IS_ENABLED          (MXC_RTC->ctrl & MXC_F_RTC_CTRL_RTCE)

#define BUSY_TIMEOUT            1000   // Timeout counts for the Busy bit

#define SUBSECOND_MSEC_0        200
#define SEARCH_STEPS            7
#define CORRECTION_COUNTS       0xc30  /* Accounts for algo overhead, xtal offset, start uncertainty, and affects final accuracy */
#define SEARCH_TARGET           0x30d400  /* 1/2 of 32 MHz periods in 32.768 kHz */

#define RTCX1x_MASK             0x1F /* 5 bits */
#define RTCX2x_MASK             0x1F /* 5 bits */

#define MXC_BASE_BBSIR          ((uint32_t)0x40005400UL)

#define NOM_32K_FREQ            32768
#define TICKS_PER_RTC           122

/* Converts a time in milleseconds to the equivalent RSSA register value. */
#define MSEC_TO_RSSA(x)         (unsigned int)(0x100000000ULL    - ((x * 4096) / 1000))

// *****************************************************************************
int RTC_EnableTimeofdayInterrupt(mxc_rtc_regs_t *rtc)
{
    if (RTC_CheckBusy()) {
        return E_BUSY;
    }

    rtc->ctrl |= MXC_F_RTC_CTRL_ADE;   // Enable Time-of-day Interrupt
    return E_SUCCESS;
}

// *****************************************************************************
int RTC_DisableTimeofdayInterrupt(mxc_rtc_regs_t *rtc)
{

    if (RTC_CheckBusy()) {
        return E_BUSY;
    }

    rtc->ctrl &= ~MXC_F_RTC_CTRL_ADE;    // Disable Time-of-day Interrupt

    if (RTC_CheckBusy()) {
        return E_BUSY;
    }

    return E_SUCCESS;
}

// *****************************************************************************
int RTC_EnableSubsecondInterrupt(mxc_rtc_regs_t *rtc)
{
    if (RTC_CheckBusy()) {
        return E_BUSY;
    }

    rtc->ctrl |= MXC_F_RTC_CTRL_ASE;    // Enable Sub-Second Interrupt

    return E_SUCCESS;
}

// *****************************************************************************
int RTC_DisableSubsecondInterrupt(mxc_rtc_regs_t *rtc)
{

    if (RTC_CheckBusy()) {
        return E_BUSY;
    }

    rtc->ctrl &= ~MXC_F_RTC_CTRL_ASE;    // Alarm Sub-Second Interrupt disabled

    if (RTC_CheckBusy()) {
        return E_BUSY;
    }

    return E_SUCCESS;
}

// *****************************************************************************
int RTC_SetTimeofdayAlarm(mxc_rtc_regs_t *rtc, uint32_t ras)
{
    // ras can only be written if BUSY = 0 & (RTCE = 0 or ADE = 0);


    if(RTC_DisableTimeofdayInterrupt(rtc) == E_BUSY) {
        return E_BUSY;
    }

    if (RTC_CheckBusy()) {
        return E_BUSY;
    }

    rtc->toda = (ras << MXC_F_RTC_TODA_TOD_ALARM_POS) & MXC_F_RTC_TODA_TOD_ALARM;

    if(RTC_EnableTimeofdayInterrupt(rtc) == E_BUSY) {
        return E_BUSY;
    }

    return E_SUCCESS;
}

// *****************************************************************************
int RTC_SetSubsecondAlarm(mxc_rtc_regs_t *rtc, uint32_t rssa)
{
    // ras can only be written if BUSY = 0 & (RTCE = 0 or ASE = 0);

    if(RTC_DisableSubsecondInterrupt(rtc) == E_BUSY) {
        return E_BUSY;
    }

    if (RTC_CheckBusy()) {
        return E_BUSY;
    }

    rtc->sseca =  (rssa << MXC_F_RTC_SSECA_SSEC_ALARM_POS) & MXC_F_RTC_SSECA_SSEC_ALARM;

    if(RTC_EnableSubsecondInterrupt(rtc) == E_BUSY) {
        return E_BUSY;
    }

    return E_SUCCESS;
}


// *****************************************************************************
int RTC_EnableRTCE(mxc_rtc_regs_t *rtc)
{
    if (RTC_CheckBusy()) {
        return E_BUSY;
    }

    rtc->ctrl |= MXC_F_RTC_CTRL_WE;       // Allow writing to registers
    if (RTC_CheckBusy()) {
        return E_BUSY;
    }
    // Can only write if WE=1 and BUSY=0
    rtc->ctrl |= MXC_F_RTC_CTRL_RTCE;    // setting RTCE = 1
    if (RTC_CheckBusy()) {
        return E_BUSY;
    }

    rtc->ctrl &= ~MXC_F_RTC_CTRL_WE;       // Prevent Writing...

    return E_SUCCESS;
}

// *****************************************************************************
int RTC_DisableRTCE(mxc_rtc_regs_t *rtc)
{
    if (RTC_CheckBusy()) {
        return E_BUSY;
    }

    rtc->ctrl |= MXC_F_RTC_CTRL_WE;      // Allow writing to registers
    if (RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    // Can only write if WE=1 and BUSY=0
    rtc->ctrl &= ~MXC_F_RTC_CTRL_RTCE;  // setting RTCE = 0
    if (RTC_CheckBusy()) {
        return E_BUSY;
    }

    rtc->ctrl &= ~MXC_F_RTC_CTRL_WE;       // Prevent Writing...

    return E_SUCCESS;
}


// *****************************************************************************
int RTC_Init(mxc_rtc_regs_t *rtc, uint32_t sec, uint16_t ssec, sys_cfg_rtc_t *sys_cfg)
{
    SYS_RTCClockEnable(sys_cfg);

    if (RTC_CheckBusy()) {
        return E_BUSY;
    }

    rtc->ctrl = MXC_F_RTC_CTRL_WE;        //  Allow Writes
    if (RTC_CheckBusy()) {
        return E_BUSY;
    }

    rtc->ctrl = RTC_CTRL_RESET_DEFAULT;  // Start with a Clean Register
    if (RTC_CheckBusy()) {
        return E_BUSY;
    }

    rtc->ctrl |= MXC_F_RTC_CTRL_WE;      // Set Write Enable, allow writing to reg.
    if (RTC_CheckBusy()) {
        return E_BUSY;
    }

    rtc->ssec = ssec;
    if (RTC_CheckBusy()) {
        return E_BUSY;
    }

    rtc->sec = sec;
    if (RTC_CheckBusy()) {
        return E_BUSY;
    }

    rtc->ctrl &= ~MXC_F_RTC_CTRL_WE;       // Prevent Writing...

    return E_SUCCESS;
}

// *****************************************************************************
int RTC_SquareWave(mxc_rtc_regs_t *rtc, rtc_sqwave_en_t sqe, rtc_freq_sel_t ft,
                   const sys_cfg_rtc_t* sys_cfg)
{

    SYS_RTC_SqwavInit(sys_cfg);  // Set the Output pins for the squarewave.

    if (RTC_CheckBusy()) {
        return E_BUSY;
    }

    rtc->ctrl |= MXC_F_RTC_CTRL_WE;   // Allow writing to registers

    if (RTC_CheckBusy()) {
        return E_BUSY;
    }

    if (sqe == SQUARE_WAVE_ENABLED) {
        if (ft == F_32KHZ){        // if 32KHz output is selected...
            rtc->oscctrl |= MXC_F_RTC_OSCCTRL_OUT32K;   // Enable 32KHz wave
            if (RTC_CheckBusy()) {
                return E_BUSY;
            }
            rtc->ctrl |= MXC_F_RTC_CTRL_SQE;                  // Enable output on the pin
        } else {                  // if 1Hz, 512Hz, 4KHz output is selected

            rtc->oscctrl &= ~MXC_F_RTC_OSCCTRL_OUT32K;  // Must make sure that the 32KHz is disabled
            if (RTC_CheckBusy()) {
                return E_BUSY;
            }
            rtc->ctrl &= ~MXC_F_RTC_CTRL_FT;
            if (RTC_CheckBusy()) {
                return E_BUSY;
            }
            rtc->ctrl |= (MXC_F_RTC_CTRL_SQE | ft);  // Enable Sq. wave,
        }

        if (RTC_CheckBusy()) {
            return E_BUSY;
        }
        rtc->ctrl |= MXC_F_RTC_CTRL_RTCE;     // Enable Real Time Clock
    } else { // Turn off the square wave output on the pin

        rtc->oscctrl &= ~MXC_F_RTC_OSCCTRL_OUT32K;     // Must make sure that the 32KHz is disabled
        if (RTC_CheckBusy()) {
            return E_BUSY;
        }
        rtc->ctrl &= ~MXC_F_RTC_CTRL_SQE;  // No sq. wave output
    }

    if (RTC_CheckBusy()) {
        return E_BUSY;
    }
    rtc->ctrl &= ~MXC_F_RTC_CTRL_WE;       // Disable Writing to register

    return E_SUCCESS;
}

// *****************************************************************************
int RTC_Trim(mxc_rtc_regs_t *rtc, int8_t trim)
{

    if (RTC_CheckBusy()) {
        return E_BUSY;
    }

    rtc->ctrl |= MXC_F_RTC_CTRL_WE;

    if (RTC_CheckBusy()) {
        return E_BUSY;
    }

    MXC_SETFIELD(rtc->trim, MXC_F_RTC_TRIM_TRIM, trim << MXC_F_RTC_TRIM_TRIM_POS);

    if (RTC_CheckBusy()) {
        return E_BUSY;
    }
    rtc->ctrl &= ~MXC_F_RTC_CTRL_WE;       // Disable Writing to register
    
    return E_SUCCESS;
}

// *****************************************************************************
int RTC_CheckBusy(void)
{
    // Time-out transfer if it takes > BUSY_TIMEOUT microseconds
    mxc_delay_start(MXC_DELAY_USEC(BUSY_TIMEOUT));
    while (RTC_IS_BUSY) {
        if (mxc_delay_check() != E_BUSY){
            return E_BUSY;
        }
    }
    mxc_delay_stop();
    return E_SUCCESS;
}

// *****************************************************************************
int RTC_GetFlags(void)
{
    return MXC_RTC->ctrl & (MXC_F_RTC_CTRL_ALDF | MXC_F_RTC_CTRL_ALSF | MXC_F_RTC_CTRL_RDY);
}

// *****************************************************************************
int RTC_ClearFlags(int flags)
{
    if (RTC_CheckBusy()) {
        return E_BUSY;
    }
    MXC_RTC->ctrl &= ~(flags & (MXC_F_RTC_CTRL_ALDF | MXC_F_RTC_CTRL_ALSF | MXC_F_RTC_CTRL_RDY));

    return E_SUCCESS;
}

// *****************************************************************************
int RTC_GetSubSecond(void)
{
    return MXC_RTC->ssec;
}

// *****************************************************************************
int RTC_GetSecond(void)
{
    return MXC_RTC->sec;
}

// *****************************************************************************
int RTC_GetTime(uint32_t* sec, uint32_t* subsec) 
{
    uint32_t temp_sec;
    do {
        // Check if an update is about to happen.
        if(!(MXC_RTC->ctrl & MXC_F_RTC_CTRL_RDY)) {
            return E_BUSY;
        }
        
        // Read the seconds count.
        temp_sec = RTC_GetSecond();

        // Check if an update is about to happen.
        if(!(MXC_RTC->ctrl & MXC_F_RTC_CTRL_RDY)) {
            return E_BUSY;
        }

        // Read the sub-seconds count.
        *subsec = RTC_GetSubSecond();

        // Check if an update is about to happen.
        if(!(MXC_RTC->ctrl & MXC_F_RTC_CTRL_RDY)) {
            return E_BUSY;
        }

        // Read the seconds count.
        *sec = RTC_GetSecond();
        
        // Repeat until a steady state is reached.
    } while (temp_sec != *sec);
    
    return E_NO_ERROR;
}

// *****************************************************************************
static void write_bb_sir(unsigned int rtcx1_, unsigned int rtcx2_)
{
    uint32_t *bb_sir;
    uint32_t shift1, shift2;

    if(MXC_GCR->revision == 0xA1) {
        bb_sir = (uint32_t *)(MXC_BASE_BBSIR+0x00000018UL);
        shift1 = 4;
        shift2 = 9;
    } else {
        bb_sir = (uint32_t *)(MXC_BASE_BBSIR+0x00000008UL);
        shift1 = 16;
        shift2 = 21;
    }

    /* Clear old values */
    *bb_sir &= ~((RTCX1x_MASK << shift1) | (RTCX2x_MASK << shift2));

    /* Insert new values, DO NOT ALTER ANY OTHER BITS WITHIN BB-SIR6 */
    *bb_sir |= ((rtcx1_ & RTCX1x_MASK) << shift1) | ((rtcx2_ & RTCX2x_MASK) << shift2);
}

// *****************************************************************************
int RTC_LoadTrim(void) 
{
    unsigned int flags, search_step;
    unsigned int elapsed;
    tmr_cfg_t tmr_config = { .mode = TMR_MODE_CONTINUOUS, .cmp_cnt = 0xffffffff, .pol = 0 };
    unsigned int upper, lower, trim, oldtrim, bestTrim, bestElapsed, bestElapsedDiff;
    unsigned int freq = NOM_32K_FREQ;
    int retval;

    /* Determine starting point for internal load capacitors */
    upper = RTCX1x_MASK;
    lower = 0;
    trim = (upper + lower) / 2;

    /* Initialize best trim variables */
    bestTrim = trim;
    bestElapsed = bestElapsedDiff = SEARCH_TARGET;

    /* Init timer to count 32 MHz periods */
    TMR_Init(MXC_TMR3, TMR_PRES_1, NULL);
    TMR_Config(MXC_TMR3, &tmr_config);

    /* Trim loop */
    search_step = 0;
    while (search_step < SEARCH_STEPS) {
        /* Set new trim point */
        oldtrim = trim;
        trim = (lower + upper) / 2;
        if ((search_step > 0) && (trim == oldtrim)) {
            /* Found trim value */
            break;
        }
        write_bb_sir(trim, trim);

        /* Sleep to settle new caps */
        TMR_Delay(MXC_TMR0, MSEC(10), 0);

        /* Start 200 msec sampling window */
        TMR_Disable(MXC_TMR3);
        TMR_SetCount(MXC_TMR3, 0);
        retval = RTC_SetSubsecondAlarm(MXC_RTC, MSEC_TO_RSSA(SUBSECOND_MSEC_0));
        if (retval != E_NO_ERROR) {
            return retval;
        }

        retval = RTC_CheckBusy();
        if (retval != E_NO_ERROR) {
            return retval;
        }
        TMR_Enable(MXC_TMR3);
        RTC_ClearFlags(RTC_GetFlags());

        flags = RTC_GetFlags();
        while (!(flags & MXC_F_RTC_CTRL_ALSF)) {
            flags = RTC_GetFlags();
        }

        elapsed = TMR_GetCount(MXC_TMR3);
        TMR_Disable(MXC_TMR3);

        elapsed -= CORRECTION_COUNTS;
        /* Binary search for optimal trim value */
        if (elapsed > SEARCH_TARGET) {
            /* Too slow */
            upper = trim;

            /* Record best setting */
            if((elapsed - SEARCH_TARGET) <= bestElapsedDiff) {
                bestElapsedDiff = elapsed - SEARCH_TARGET;
                bestElapsed = elapsed;
                bestTrim = trim;
            }
        } else {
            /* Too fast */
            lower = trim;

            /* Record best setting */
            if((SEARCH_TARGET - elapsed) <= bestElapsedDiff) {
                bestElapsedDiff = SEARCH_TARGET - elapsed;
                bestElapsed = elapsed;
                bestTrim = trim;
            }
        }

        search_step++;
    }

    /* Apply the closest trim setting */
    write_bb_sir(bestTrim, bestTrim);

    /* Adjust 32K freq if we can't get close enough to 32768 Hz */
    if(bestElapsed >= SEARCH_TARGET) {
        freq -= (((bestElapsed - SEARCH_TARGET) + (TICKS_PER_RTC/2-1)) / TICKS_PER_RTC);
    } else {
        freq += (((SEARCH_TARGET - bestElapsed) + (TICKS_PER_RTC/2-1)) / TICKS_PER_RTC);
    }

	/* Clear hardware state */
    TMR_Disable(MXC_TMR3);
    TMR_Shutdown(MXC_TMR3);
    RTC_ClearFlags(RTC_GetFlags());

    return freq;
}
