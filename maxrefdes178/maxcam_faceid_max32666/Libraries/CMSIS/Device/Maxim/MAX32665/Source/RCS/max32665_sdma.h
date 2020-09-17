/*******************************************************************************
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
 * $Id: max32665_sdma.h 50208 2019-12-23 17:00:17Z kevin.gillespie $
 *
 *******************************************************************************
 */

#ifndef _MAX32665_SDMA_H_
#define _MAX32665_SDMA_H_

#include <inmaxq.h>

/**************************************************************************************************
  Macros
**************************************************************************************************/
extern uint32_t SystemCoreClock;
extern uint32_t TmrBdiv;


/* Definitions of MAXQ30 registers and related macros */
sfrw  r_dp0 = 0x3f;
sfrw  r_dp1 = 0xdf;
sfrw  r_bp = 0x0e;
sfrw  r_offs = 0x3e;
sfrw  at_dp0_LPP = 0x6f;
sfrw  at_dp0_BPP = 0x1f;
sfrw  r_lc0 = 0x6d;
sfrw  r_lc1 = 0x7d;
sfrw  r_djnz_lc0 = 0x4d;
sfrw  r_a7 = 0x79;
sfrw  r_a6 = 0x69;
sfrw  r_a5 = 0x59;
sfrw  r_a4 = 0x49;
sfrw  r_a3 = 0x39;
sfrw  r_a2 = 0x29;
sfrw  r_ap = 0x08;
sfrw  r_sp = 0x1d;
sfrw  r_jump = 0x0c;
sfrw  r_jump_z = 0x1c;
sfrw  r_jump_c = 0x2c;
sfrw  r_jump_e = 0x3c;
sfrw  r_jump_s = 0x4c;
sfrw  r_jump_nz = 0x5c;
sfrw  r_jump_nc = 0x6c;
sfrw  r_jump_ne = 0x7c;
sfrw  r_push = 0x0d;
sfrw  r_tbv = 0x30;
sfrw  r_tbcn = 0x20;
sfrw  r_ic = 0x58;
sfrw  r_intout = 0x60;

/*************************************************************************************************/
/*!
 *  \fn     __enable_irq
 *
 *  \brief  Globally enables all interrupts.
 *
 *  \param  None.
 *
 *  \return None.
 */
/*************************************************************************************************/
#define   __enable_irq()   {r_ic |= (uint32_t)1;}

/*************************************************************************************************/
/*!
 *  \fn     __disable_irq
 *
 *  \brief  Globally enables all interrupts.
 *
 *  \param  None.
 *
 *  \return None.
 */
/*************************************************************************************************/
#define   __disable_irq()   {r_ic &= (uint32_t)~1;}


/*************************************************************************************************/
/*!
 *  \fn     interrupt_arm
 *
 *  \brief  Interrupt ARM core.
 *
 *  \param  None.
 *
 *  \return None.
 */
/*************************************************************************************************/
#define   interrupt_ARM()  {r_intout = 1;}

/*************************************************************************************************/
/*!
 *  \fn     NVIC_EnableIRQ
 *
 *  \brief  Enables given interrupt.
 *
 *  \param  irq Interrupt number. 
 *
 *  \return None.
 */
/*************************************************************************************************/
void NVIC_EnableIRQ(int irq);

/*************************************************************************************************/
/*!
 *  \fn     NVIC_DisableIRQ
 *
 *  \brief  Disables given interrupt.
 *
 *  \param  irq Interrupt number. 
 *
 *  \return None.
 */
/*************************************************************************************************/
void NVIC_DisableIRQ(int irq);

/*************************************************************************************************/
/*!
 *  \fn     NVIC_ClearPendingIRQ
 *
 *  \brief  Clears given interrupt.
 *
 *  \param  irq Interrupt number. 
 *
 *  \return None.
 */
/*************************************************************************************************/
void NVIC_ClearPendingIRQ(int irq);

/*************************************************************************************************/
/*!
 *  \fn     NVIC_SetPriority
 *
 *  \brief  Sets the priority of the given interrupt.
 *
 *  \param  irq Interrupt number. 
 *  \param  prioirty Priority number of the given interrupt. 
 *
 *  \return None.
 */
/*************************************************************************************************/
void NVIC_SetPriority(int irq, uint32_t priority);

/*************************************************************************************************/
/*!
 *  \fn     SystemCoreClockUpdate
 *
 *  \brief  Updates the SystemCoreClock variable.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SystemCoreClockUpdate(void);

/*************************************************************************************************/
/*!
 *  \fn     TMRB_Disable
 *
 *  \brief  Disable TMRB.
 *
 *  \return None.
 */
/*************************************************************************************************/
#define   TMRB_Disable()   {r_tbcn = 0;}

#endif /* _MAX32665_SDMA_H_ */
