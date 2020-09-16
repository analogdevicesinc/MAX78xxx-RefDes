/**
 * @file   htmr.h
 * @brief  High speed timer (HTMR) functions and prototypes.
 */
 
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
 * $Date: 2018-11-09 00:08:58 +0300 (Fri, 09 Nov 2018) $
 * $Revision: 39056 $
 *************************************************************************** */

/* Define to prevent redundant inclusion */
#ifndef _HTMR_H_
#define _HTMR_H_

/* **** Includes **** */
#include <stdint.h>
#include "mxc_config.h"
#include "htmr_regs.h"
#include "mxc_sys.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup htmr HTMR
 * @ingroup periphlibs
 * @{
 */

/* **** Definitions **** */

/**
 *@brief     Enables Long Interval Counter's Alarm Interrupt
 *@param     htmr    pointer to the htmr register structure
 *@return    #E_SUCCESS If function is successful.  
 *@return    #E_BUSY  If function is unsuccessful.
 */
int HTMR_EnableLongIntInterrupt(mxc_htmr_regs_t *htmr);

/**
 *@brief     Disable Long Interval Counter's Alarm Interrupt
 *@param     htmr    pointer to the htmr register structure
 *@return    #E_SUCCESS If function is successful.  
 *@return    #E_BUSY  If function is unsuccessful.
 */
int HTMR_DisableLongIntInterrupt(mxc_htmr_regs_t *htmr);

/**
 *@brief     Enables Short Interval Counter's Alarm Interrupt
 *@param     htmr    pointer to the htmr register structure
 *@return    #E_SUCCESS If function is successful.  
 *@return    #E_BUSY  If function is unsuccessful.
 */
int HTMR_EnableShortIntInterrupt(mxc_htmr_regs_t *htmr);

/**
 *@brief     Disable Short Interval's Alarm Interrupt
 *@param     htmr    pointer to the htmr register structure
 *@return    #E_SUCCESS If function is successful.  
 *@return    #E_BUSY  If function is unsuccessful.
 */
int HTMR_DisableShortIntInterrupt(mxc_htmr_regs_t *htmr);

/**
 *@brief     Set Long Interval alarm value and enable Interrupt
 *@param     htmr    pointer to the htmr register structure
 *@param     ras    20-bit value 0-0xFFFFF
 *@return    #E_SUCCESS If function is successful.
 *@return    #E_BAD_PARAM If there is an invalid parameter.
 *@return    #E_BUSY  If function is unsuccessful.
 */
int HTMR_SetLongIntAlarm(mxc_htmr_regs_t *htmr, uint32_t ras);

/**
 *@brief     Set Short Interval alarm value and enable interrupt,
 *@param     htmr    pointer to the htmr register structure
 *@param     rssa   32-bit value 0-0xFFFFFFFF
 *@return    #E_SUCCESS If function is successful.  
 *@return    #E_BUSY  If function is unsuccessful.
 */
int HTMR_SetShortIntAlarm(mxc_htmr_regs_t *htmr, uint32_t rssa);

/**
 *@brief     Enable/Start the High Speed Timer
 *@param     htmr    pointer to the htmr register structure
 *@return    #E_SUCCESS If function is successful.  
 *@return    #E_BUSY  If function is unsuccessful.
 */
int HTMR_Enable(mxc_htmr_regs_t *htmr);

/**
 *@brief     Disable/Stop the High Speed Timer
 *@param     htmr    pointer to the htmr register structure
 *@return    #E_SUCCESS If function is successful.  
 *@return    #E_BUSY  If function is unsuccessful.
 */
int HTMR_Disable(mxc_htmr_regs_t *htmr);

/**
 *@brief     Initialize the sec and ssec registers and enable HTMR
 *@param     htmr   pointer to the htmr register structure
 *@param     sec    set the HTMR long counter (32-bit)
 *@param     ssec   set the HTMR short counter (8-bit)
 *@param     sys_cfg System configuration object
 *@return    #E_SUCCESS If function is successful.  
 *@return    #E_BAD_STATE If HTMR is not in a valid state.
 */
int HTMR_Init(mxc_htmr_regs_t *htmr, uint32_t sec, uint8_t ssec, sys_cfg_htmr_t *sys_cfg);

/**
 *@brief     Check if BUSY bit is 0.
 *@param     htmr   pointer to the htmr register structure
 *@return    #E_SUCCESS If function is successful. 
 *@return    #E_BUSY  If function is unsuccessful.
 */
int HTMR_CheckBusy(mxc_htmr_regs_t *htmr);

/**
 *@brief     Gets Interrupt flags.
 *@param     htmr   pointer to the htmr register structure
 *@return    Interrupts flags that have not been cleared
 */

int HTMR_GetFlags(mxc_htmr_regs_t *htmr);
/**
 *@brief     Clear Interrupt flag.
 *@param     htmr   pointer to the htmr register structure
 *@param     flags the flags that need to be cleared
 */
int HTMR_ClearFlags(mxc_htmr_regs_t *htmr, int flags);

/**
 *@brief     Gets value in short interval register
 *@param     htmr   pointer to the htmr register structure
 *@return    Returns short count value
 */
int HTMR_GetShortCount(mxc_htmr_regs_t *htmr);

/**
 *@brief     Gets value in long interval register
 *@param     htmr   pointer to the htmr register structure
 *@return    returns long count value
 */
int HTMR_GetLongCount(mxc_htmr_regs_t *htmr);

#ifdef __cplusplus
}
#endif
/**@} end of group htmr */
#endif /* _HTMR_H_ */
