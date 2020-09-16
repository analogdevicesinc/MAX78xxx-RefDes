/**
 * @file    adc.h
 * @brief   Analog to Digital Converter(ADC) function prototypes and data types.
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
 * $Date: 2019-01-31 19:54:56 +0300 (Thu, 31 Jan 2019) $
 * $Revision: 40661 $
 *
 *************************************************************************** */

/* Define to prevent redundant inclusion */
#ifndef _ADC_H_
#define _ADC_H_

/* **** Includes **** */
#include <stdint.h>
#include "adc_regs.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @defgroup adc Analog To Digital Converter (ADC)
 * @ingroup periphlibs
 * @{
 */

/***************************************************************************************************************
                                    DATA STRUCTURES FOR ADC INITIALIZATION
***************************************************************************************************************/
/**
  * Enumeration type for the ADC Input Channels
  */
typedef enum {
    ADC_CH_0            = MXC_V_ADC_CTRL_CH_SEL_AIN0,      // Select Channel 0
    ADC_CH_1            = MXC_V_ADC_CTRL_CH_SEL_AIN1,      // Select Channel 1
    ADC_CH_2            = MXC_V_ADC_CTRL_CH_SEL_AIN2,      // Select Channel 2
    ADC_CH_3            = MXC_V_ADC_CTRL_CH_SEL_AIN3,      // Select Channel 3
    ADC_CH_4            = MXC_V_ADC_CTRL_CH_SEL_AIN4,      // Select Channel 4
    ADC_CH_5            = MXC_V_ADC_CTRL_CH_SEL_AIN5 ,     // Select Channel 5
    ADC_CH_6            = MXC_V_ADC_CTRL_CH_SEL_AIN6,      // Select Channel 6
    ADC_CH_7            = MXC_V_ADC_CTRL_CH_SEL_AIN7,      // Select Channel 7
    ADC_CH_VCOREA       = MXC_V_ADC_CTRL_CH_SEL_VCOREA,    // Vcorea
    ADC_CH_VCOREB       = MXC_V_ADC_CTRL_CH_SEL_VCOREB,    // Vcoreb
    ADC_CH_VRXOUT       = MXC_V_ADC_CTRL_CH_SEL_VRXOUT,    // Vrxout
    ADC_CH_VTXOUT       = MXC_V_ADC_CTRL_CH_SEL_VTXOUT,    // Vtxout
    ADC_CH_VDDA         = MXC_V_ADC_CTRL_CH_SEL_VDDA,      // Vdda
    ADC_CH_VDDB         = MXC_V_ADC_CTRL_CH_SEL_VDDB,      // Vddb divided by 4
    ADC_CH_VDDIO        = MXC_V_ADC_CTRL_CH_SEL_VDDIO,     // Vddio divided by 4
    ADC_CH_VDDIOH       = MXC_V_ADC_CTRL_CH_SEL_VDDIOH,    // Vddioh divided by 4
    ADC_CH_VREGI        = MXC_V_ADC_CTRL_CH_SEL_VREGI,     // VregI divided by 4
} mxc_adc_chsel_t;

/**
 * Enumeration type for the ADC limit register to set 
 */
typedef enum {
  ADC_LIMIT_0,      // ADC Limit Register 0
  ADC_LIMIT_1,      // ADC Limit Register 1 
  ADC_LIMIT_2,      // ADC Limit Register 2 
  ADC_LIMIT_3,      // ADC Limit Register 3 
  ADC_LIMIT_MAX     // Number of Limit registers 
} mxc_adc_limitsel_t;

/***************************************************************************************************************
                                            DRIVER EXPOSED API's
***************************************************************************************************************/
/**
 * @brief Initialize the ADC hardware
 * @param      divisor The divisor
 * @param      sys_cfg System Configuration Object
 * @return     #E_NO_ERROR If successful
 */
int ADC_Init(unsigned divisor, const sys_cfg_adc_t* sys_cfg);

/**
 * @brief Start ADC conversion on the selected channel
 * @param      channel Channel select from \c #mxc_adc_chsel_t
 * @param      adc_scale Enable the ADC input scaling mode if non-zero
 * @param      ref_scale The reference scale
 */
void ADC_StartConvert(mxc_adc_chsel_t channel, unsigned int adc_scale, unsigned int ref_scale);

/**
 * @brief Gets the result from the previous ADC conversion
 * @param      outdata Pointer to store the ADC data conversion result
 * @return     #E_OVERFLOW   ADC overflow error
 * @return     #E_NO_ERROR   Data returned in \p outdata parameter
 */
int ADC_GetData(uint16_t *outdata);

/**
 * @brief      Set the data limits for an ADC channel monitor
 * @param      unit         Which data limit unit to configure
 * @param      channel      Channel select from mxc_adc_chsel_t
 * @param      low_enable   Enable the lower limit on this monitor
 * @param      low_limit    Value for lower limit monitor
 * @param      high_enable  Enable the upper limit on this monitor
 * @param      high_limit   Value for upper limit monitor
 * @return     #E_BAD_PARAM  ADC limit or channel greater than supported
 * @return     #E_NO_ERROR   ADC limit set successfully.
 */
int ADC_SetLimit(mxc_adc_limitsel_t unit, mxc_adc_chsel_t channel, unsigned int low_enable, 
                 unsigned int low_limit, unsigned int high_enable, unsigned int high_limit);

/**
 * @brief      Get interrupt flags
 * @return     ADC Interrupt flags bit mask. See the @ref #ADC_INTR
 *             "ADC_INTR Register" for the interrupt flag masks.
 */
uint32_t ADC_GetFlags(void);

/**
 * @brief      Clear interrupt flag(s) using the mask parameter. All bits set in
 *             the parameter will be cleared
 * @param      mask  Interrupt flags to clear. See the @ref #ADC_INTR
 *                   "ADC_INTR Register" for the interrupt flag masks
 */
void ADC_ClearFlags(uint32_t mask);

/**
 * @brief      Get the Status of the ADC
 * @return     ADC status register. See @ref #ADC_STATUS "ADC_STATUS
 *             Register" for details.
 */
uint32_t ADC_GetStatus(void);

/**
 * @brief      Enables the ADC interrupts specified by the mask parameter
 * @param      mask  ADC interrupts to enable. See @ref #ADC_INTR
 *                   "ADC_INTR Register" for the interrupt enable bit masks.
 */
void ADC_EnableINT(uint32_t mask);

/**
 * @brief      Disable ADC interrupts based on mask
 * @param      mask  ADC interrupts to disable. See @ref #ADC_INTR
 *                   "ADC_INTR Register" for the interrupt enable bit masks.
 */
void ADC_DisableINT(uint32_t mask);

/**@} end of group adc */

#ifdef __cplusplus
}
#endif

#endif /* _ADC_H_ */
