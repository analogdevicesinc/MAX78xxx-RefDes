/**
 * @file    wut.h
 * @brief   Wakeup Timer (WUT) function prototypes and data types.
 */

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
 * $Date: 2020-08-05 23:33:15 +0300 (Wed, 05 Aug 2020) $
 * $Revision: 54718 $
 *
 *************************************************************************** */

/* Define to prevent redundant inclusion */
#ifndef _WUT_H_
#define _WUT_H_

/* **** Includes **** */
#include "mxc_config.h"
#include "wut_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup wut Wakeup Timer (WUT)
 * @ingroup periphlibs
 * @{
 */

/**
 * @brief Wakeup Timer prescaler values
 */
typedef enum {
    WUT_PRES_1      = MXC_S_WUT_CN_PRES_DIV1,           /// Divide input clock by 1
    WUT_PRES_2      = MXC_S_WUT_CN_PRES_DIV2,           /// Divide input clock by 2
    WUT_PRES_4      = MXC_S_WUT_CN_PRES_DIV4,           /// Divide input clock by 4
    WUT_PRES_8      = MXC_S_WUT_CN_PRES_DIV8,           /// Divide input clock by 8
    WUT_PRES_16     = MXC_S_WUT_CN_PRES_DIV16,          /// Divide input clock by 16
    WUT_PRES_32     = MXC_S_WUT_CN_PRES_DIV32,          /// Divide input clock by 32
    WUT_PRES_64     = MXC_S_WUT_CN_PRES_DIV64,          /// Divide input clock by 64
    WUT_PRES_128    = MXC_S_WUT_CN_PRES_DIV128,         /// Divide input clock by 128
    WUT_PRES_256    = MXC_F_WUT_CN_PRES3 | MXC_S_WUT_CN_PRES_DIV1,  /// Divide input clock by 256
    WUT_PRES_512    = MXC_F_WUT_CN_PRES3 | MXC_S_WUT_CN_PRES_DIV2,  /// Divide input clock by 512
    WUT_PRES_1024   = MXC_F_WUT_CN_PRES3 | MXC_S_WUT_CN_PRES_DIV4,  /// Divide input clock by 1024
    WUT_PRES_2048   = MXC_F_WUT_CN_PRES3 | MXC_S_WUT_CN_PRES_DIV8,  /// Divide input clock by 2048
    WUT_PRES_4096   = MXC_F_WUT_CN_PRES3 | MXC_S_WUT_CN_PRES_DIV16  /// Divide input clock by 4096
} wut_pres_t;

/**
 * @brief Wakeup Timer modes
 */
typedef enum {
    WUT_MODE_ONESHOT          = MXC_V_WUT_CN_TMODE_ONESHOT,       /// Wakeup Timer Mode ONESHOT
    WUT_MODE_CONTINUOUS       = MXC_V_WUT_CN_TMODE_CONTINUOUS,    /// Wakeup Timer Mode CONTINUOUS
    WUT_MODE_COUNTER          = MXC_V_WUT_CN_TMODE_COUNTER,       /// Wakeup Timer Mode COUNTER
    WUT_MODE_PWM              = MXC_V_WUT_CN_TMODE_PWM,           /// Wakeup Timer Mode PWM
    WUT_MODE_CAPTURE          = MXC_V_WUT_CN_TMODE_CAPTURE,       /// Wakeup Timer Mode CAPTURE
    WUT_MODE_COMPARE          = MXC_V_WUT_CN_TMODE_COMPARE,       /// Wakeup Timer Mode COMPARE
    WUT_MODE_GATED            = MXC_V_WUT_CN_TMODE_GATED,         /// Wakeup Timer Mode GATED
    WUT_MODE_CAPTURE_COMPARE  = MXC_V_WUT_CN_TMODE_CAPTURECOMPARE /// Wakeup Timer Mode CAPTURECOMPARE
} wut_mode_t;

/**
 * @brief Wakeup Timer units of time enumeration
 */
typedef enum {
    WUT_UNIT_NANOSEC = 0,       /**< Nanosecond Unit Indicator. */
    WUT_UNIT_MICROSEC,          /**< Microsecond Unit Indicator. */
    WUT_UNIT_MILLISEC,          /**< Millisecond Unit Indicator. */
    WUT_UNIT_SEC                /**< Second Unit Indicator. */
} wut_unit_t;

/**
 * @brief Wakeup Timer Configuration
 */
typedef struct {
    wut_mode_t mode;    /// Desired timer mode
    uint32_t cmp_cnt;   /// Compare register value in timer ticks
} wut_cfg_t;

/* **** Definitions **** */

/* **** Function Prototypes **** */

/**
 * @brief      Initialize timer module clock.
 * @param      wut        Pointer to timer module to initialize.
 * @param      pres       Prescaler value.
 * @param      sys_cfg    System configuration object
 * @return     #E_NO_ERROR if successful. 
 */
void WUT_Init(wut_pres_t pres);

/**
 * @brief      Shutdown timer module clock.
 * @param      wut  Pointer to timer module to initialize.
 * @return     #E_NO_ERROR if successful. 
 */
void WUT_Shutdown(void);

/**
 * @brief      Enable the timer.
 * @param      wut  Pointer to timer module to initialize.
 * @return     #E_NO_ERROR if successful. 
 */
void WUT_Enable(void);

/**
 * @brief      Disable the timer.
 * @param      wut  Pointer to timer module to initialize.
 */
void WUT_Disable(void);

/**
 * @brief      Configure the timer.
 * @param      wut  Pointer to timer module to initialize.
 * @param      cfg  Pointer to timer configuration struct.
 * @return     #E_NO_ERROR if successful. 
 */
void WUT_Config(const wut_cfg_t *cfg);

/**
 * @brief      Get the timer prescaler.
 * @return     The current configured timer prescaler.
 */
wut_pres_t WUT_GetPres(void);

/**
 * @brief      Get the wakeup timer mode.
 * @return     The current configured wakeup timer mode.
 */
wut_mode_t WUT_GetMode(void);

/**
 * @brief   Get the timer compare count.
 * @param   wut     Pointer to timer module to initialize.
 * @return  Returns the current compare count.
 */
uint32_t WUT_GetCompare(void);

/**
 * @brief   Get the timer capture count.
 * @param   wut     Pointer to timer module to initialize.
 * @return  Returns the most recent capture count.
 */
uint32_t WUT_GetCapture(void);

/**
 * @brief   Get the timer count.
 * @param   wut     Pointer to timer module to initialize.
 * @return  Returns the current count.
 */
uint32_t WUT_GetCount(void);

/**
 * @brief   Clear the timer interrupt.
 * @param   wut     Pointer to timer module to initialize.
 */
void WUT_IntClear(void);

/**
 * @brief   Get the timer interrupt status.
 * @param   wut     Pointer to timer module to initialize.
 * @return  Returns the interrupt status. 1 if interrupt has occurred.
 */
uint32_t WUT_IntStatus(void);

/**
 * @brief   Set the timer prescaler.
 * @param   mode The wakeup timer prescale configuration. */
void WUT_SetPres(wut_pres_t pres);

/**
 * @brief   Set the wakeup timer mode.
 * @param   mode The wakeup timer mode. */
void WUT_SetMode(wut_mode_t mode);

/**
 * @brief   Set the timer compare count.
 * @param   wut     Pointer to timer module to initialize.
 * @param   cmp_cnt New compare count.
 * @note    This function does not protect against output glitches in PWM mode.
 *          Use WUT_PWMSetPeriod when in PWM mode.
 */
void WUT_SetCompare(uint32_t cmp_cnt);

/**
 * @brief   Set the timer count.
 * @param   wut     Pointer to timer module to initialize.
 * @param   cnt     New count.
 */
void WUT_SetCount(uint32_t cnt);

/**
 * @brief   Convert real time to timer ticks.
 * @param   wut     Pointer to timer module to initialize.
 * @param   time    Number of units of time.
 * @param   units   Which units of time you want to convert.
 * @param   ticks   Pointer to store the number of ticks calculated.
 * @return     #E_NO_ERROR If everything is successful. 
 * @return     @ref MXC_Error_Codes If function is unsuccessful.
 */
int WUT_GetTicks(uint32_t time, wut_unit_t units, uint32_t *ticks);

/**
 * @brief   Convert timer ticks to real time.
 * @param   wut     Pointer to timer module to initialize.
 * @param   ticks   Number of ticks.
 * @param   time    Pointer to store number of units of time.
 * @param   units   Pointer to store the units that time represents.
 * @return     #E_NO_ERROR If everything is successful. 
 * @return     @ref MXC_Error_Codes If function is unsuccessful.
 */
int WUT_GetTime(uint32_t ticks, uint32_t *time, wut_unit_t *units);

/**
 * @brief   Wait for an edge of the WUT count register.
 */
void WUT_Edge(void);

/**
 * @brief   Store the count and snapshot values.
 */
void WUT_Store(void);

/**
 * @brief   Restore the DBB clock with the stored count and snapshot values.
 * @param   dbbFreq  Frequency of DBB clock.
 */
void WUT_RestoreBBClock(uint32_t dbbFreq);

/**
 * @brief   Get the difference between the stored counter value 
 *          and the current counter value.
 * @param   dbbFreq  Frequency of DBB clock.
 * @return  Returns the current counter value - stored counter value.
 */
uint32_t WUT_GetSleepTicks(void);

/**
 * @brief   Delays for the given number of milliseconds.
 * @param   waitMs  Number of milliseconds to wait.
 */
void WUT_Delay_MS(uint32_t waitMs);

/**@} end of group wut */

#ifdef __cplusplus
}
#endif

#endif /* _WUT_H_ */
