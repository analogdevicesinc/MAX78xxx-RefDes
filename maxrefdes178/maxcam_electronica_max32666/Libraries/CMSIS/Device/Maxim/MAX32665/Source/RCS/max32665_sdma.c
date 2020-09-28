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
 * $Id: max32665_sdma.c 50207 2019-12-23 16:59:17Z kevin.gillespie $
 *
 *******************************************************************************
 */

#include "max32665.h"
#include "gcr_regs.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

uint32_t SystemCoreClock = 96000000;
uint32_t SystemCoreClockM = 96;
uint32_t TmrBdiv = 96000000 / 256 / 1000;

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

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
void NVIC_EnableIRQ(int irq)
{

}

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
void NVIC_DisableIRQ(int irq)
{

}

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
void NVIC_ClearPendingIRQ(int irq)
{

}

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
void NVIC_SetPriority(int irq, uint32_t priority)
{

}

/*************************************************************************************************/
/*!
 *  \fn     SystemCoreClockUpdate
 *
 *  \brief  Updates the SystemCoreClock variable.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SystemCoreClockUpdate(void)
{
    uint32_t base_freq, div, clk_src;
    int tmpx;

    // Determine the clock source and frequency
    clk_src = (MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CLKSEL);
    switch (clk_src)
    {
        case MXC_S_GCR_CLKCN_CLKSEL_HIRC:
            base_freq = HIRC_FREQ;
            break;
        case MXC_S_GCR_CLKCN_CLKSEL_XTAL32M:
            base_freq = XTAL32M_FREQ;
            break;
        case MXC_S_GCR_CLKCN_CLKSEL_LIRC8:
            base_freq = LIRC8_FREQ;
            break;
        case MXC_S_GCR_CLKCN_CLKSEL_HIRC96:
            base_freq = HIRC96_FREQ;
            break;
        case MXC_S_GCR_CLKCN_CLKSEL_HIRC8:
            base_freq = HIRC8_FREQ;
            break;
        case MXC_S_GCR_CLKCN_CLKSEL_XTAL32K:
            base_freq = XTAL32K_FREQ;
            break;
        default:
      // Values 001 and 111 are reserved, and should never be encountered.
      base_freq = HIRC_FREQ;
            break;
    }
    // Clock divider is retrieved to compute system clock
    div = (MXC_GCR->clkcn & MXC_F_GCR_CLKCN_PSC) >> MXC_F_GCR_CLKCN_PSC_POS;
	
    /*  avoid long div32() and mul32() functions
        use shifts and adds to do calculations
        note: use following approximations for division
            x/1000 ~ x/2^10 + 3x/2^17
            x/1000000 ~ x/2^20 + 3x/2^26
    */

    tmpx = base_freq >> div;
    SystemCoreClock = tmpx;
    SystemCoreClockM = ((((((tmpx*3 + 32)>>6) + tmpx))>>16) + 8)>>4;

    tmpx = tmpx /*(int)SystemCoreClock*/ >> 8;
    TmrBdiv = (((((tmpx*3 + 64)>>7) + tmpx )>>8) + 2)>>2;

}
