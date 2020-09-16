/* *********************************************************************************************************
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
 * $Date: 2019-07-01 17:37:33 +0300 (Mon, 01 Jul 2019) $
 * $Revision: 44366 $
 *
 ********************************************************************************************************** */

/* **** Includes **** */
#include <stdint.h>
#include <stdio.h>
#include "mxc_config.h"
#include "mxc_errors.h"
#include "mxc_sys.h"
#include "adc_regs.h"
#include "adc.h"

/* **** MACROS **** */
#define CLEAR   0x00
// Mask for all Interrupt Flag Fields
#define ADC_IF_MASK (MXC_F_ADC_INTR_DONE_IF     | MXC_F_ADC_INTR_REF_READY_IF | \
                     MXC_F_ADC_INTR_HI_LIMIT_IF | MXC_F_ADC_INTR_LO_LIMIT_IF  | \
                     MXC_F_ADC_INTR_OVERFLOW_IF )

// Mask for all Interrupt Enable Fields
#define ADC_IE_MASK (MXC_F_ADC_INTR_DONE_IE     | MXC_F_ADC_INTR_REF_READY_IE | \
                     MXC_F_ADC_INTR_HI_LIMIT_IE | MXC_F_ADC_INTR_LO_LIMIT_IE  | \
                     MXC_F_ADC_INTR_OVERFLOW_IE )

/* **** Functions **** */
// ************************************ Function to Read Interrupt Flag **************************************
uint32_t ADC_GetFlags(void)
{
    return (MXC_ADC->intr & ADC_IF_MASK);
}

// ************************************ Function to Clear Interrupt ******************************************
void ADC_ClearFlags(uint32_t mask)
{
    MXC_ADC->intr |= (mask & ADC_IF_MASK);
}

// ************************************ Function to Read ADC Status ******************************************
uint32_t ADC_GetStatus(void)
{
    return (MXC_ADC->status);
}

// ************************************ Function to Enable Interrupt *****************************************
void ADC_EnableINT(uint32_t mask)
{
    MXC_ADC->intr = ((MXC_ADC->intr | mask) & ADC_IE_MASK);
}

// ************************************ Function to Disable Interrupt ****************************************
void ADC_DisableINT(uint32_t mask)
{
    MXC_ADC->intr = ((MXC_ADC->intr & ~mask) & ADC_IE_MASK);
}

// ************************************ Function to Configure ADC ********************************************
int ADC_Init(unsigned divisor, const sys_cfg_adc_t* sys_cfg)
{
    int err;
    int clk;
    if (divisor > 0xf || divisor < 2) {
        return E_BAD_PARAM;
    } else {
        clk = PeripheralClock/divisor;
        if (clk > MXC_ADC_MAX_CLOCK) {
            return E_BAD_PARAM;
        }
    }
    if ((err = SYS_ADC_Init(sys_cfg)) != E_NO_ERROR) {
        return err;
    }
    
    // Wipe previous configuration
    MXC_ADC->ctrl = 0;
    //enable clock
    MXC_ADC->ctrl |= MXC_F_ADC_CTRL_CLK_EN;
    //clear interrupts
    MXC_ADC->intr = MXC_ADC->intr;
    //clear clock divisor
    MXC_GCR->pckdiv &= (~MXC_F_GCR_PCKDIV_ADCFRQ);
    //set clock divisor
    MXC_GCR->pckdiv |= (divisor << MXC_F_GCR_PCKDIV_ADCFRQ_POS);
    // Enable done interrupt
    MXC_ADC->intr = MXC_F_ADC_INTR_DONE_IE;
    // Power up the ADC
    MXC_ADC->ctrl |= MXC_F_ADC_CTRL_PWR;
    MXC_ADC->ctrl |= MXC_F_ADC_CTRL_REFBUF_PWR;
    
    while ((MXC_ADC->intr & MXC_F_ADC_INTR_REF_READY_IF)>>MXC_F_ADC_INTR_REF_READY_IF_POS);
    ADC_ClearFlags(MXC_F_ADC_INTR_REF_READY_IF);
    
    return E_NO_ERROR;
}

// ********************************* Function to start ADC Conversion ****************************************
void ADC_StartConvert(mxc_adc_chsel_t channel, unsigned int adc_scale, unsigned int ref_scale)
{
    uint32_t ctrl_tmp;
    
    // Clear the ADC done flag
    ADC_ClearFlags(MXC_F_ADC_INTR_DONE_IF);
    
    // Clear and insert channel selection
    ctrl_tmp = MXC_ADC->ctrl;
    ctrl_tmp &= ~(MXC_F_ADC_CTRL_CH_SEL | MXC_F_ADC_CTRL_REF_SCALE | MXC_F_ADC_CTRL_SCALE);
    ctrl_tmp |= ((channel << MXC_F_ADC_CTRL_CH_SEL_POS) & MXC_F_ADC_CTRL_CH_SEL);
    
    // Finalize user-requested channel configuration
    if (adc_scale || (channel > ADC_CH_3)) {
        ctrl_tmp |= MXC_F_ADC_CTRL_SCALE;
    }
    
    // Write this configuration
    MXC_ADC->ctrl = ctrl_tmp;
    
    // Start conversion
    MXC_ADC->ctrl |= MXC_F_ADC_CTRL_START;
}

// ************************************* Function to Read ADC Data *******************************************
int ADC_GetData(uint16_t *outdata)
{
    // See if a conversion is in process
    if (MXC_ADC->status & MXC_F_ADC_STATUS_ACTIVE) {
        // Wait for conversion to complete
        while ((MXC_ADC->intr & MXC_F_ADC_INTR_DONE_IF) == 0);
    }
    
    // Read 32-bit value and truncate to 16-bit for output depending on data align bit
    if ((MXC_ADC->ctrl & MXC_F_ADC_CTRL_DATA_ALIGN) == 0) {
        *outdata = (uint16_t)(MXC_ADC->data);    /* LSB justified */
    } else {
        *outdata = (uint16_t)(MXC_ADC->data >> 6);    /* MSB justified */
    }
    
    // Check for overflow
    if (MXC_ADC->status & MXC_F_ADC_STATUS_OVERFLOW) {
        return E_OVERFLOW;
    }
    
    return E_NO_ERROR;
}

// ************************************* Function to Set ADC Limit *******************************************
int ADC_SetLimit(mxc_adc_limitsel_t unit, mxc_adc_chsel_t channel, unsigned int low_enable,
                 unsigned int low_limit, unsigned int high_enable, unsigned int high_limit)
{
    // Check args
    if (unit >= ADC_LIMIT_MAX) {
        return E_BAD_PARAM;
    }
    
    // set channel using the limit
    MXC_ADC->limit[unit] = ((channel << MXC_F_ADC_LIMIT_CH_SEL_POS) & MXC_F_ADC_LIMIT_CH_SEL);
    
    // enable/disable the limit
    if (low_enable) {
        MXC_ADC->limit[unit] |= MXC_F_ADC_LIMIT_CH_LO_LIMIT_EN |
                                ((low_limit << MXC_F_ADC_LIMIT_CH_LO_LIMIT_POS) & MXC_F_ADC_LIMIT_CH_LO_LIMIT);
    } else {
        MXC_ADC->limit[unit] &= ~MXC_F_ADC_LIMIT_CH_LO_LIMIT_EN;
    }
    
    if (high_enable) {
        MXC_ADC->limit[unit] |= MXC_F_ADC_LIMIT_CH_HI_LIMIT_EN |
                                ((high_limit << MXC_F_ADC_LIMIT_CH_HI_LIMIT_POS) & MXC_F_ADC_LIMIT_CH_HI_LIMIT);
    } else {
        MXC_ADC->limit[unit] &= ~MXC_F_ADC_LIMIT_CH_HI_LIMIT_EN;
    }
    
    return E_NO_ERROR;
}
