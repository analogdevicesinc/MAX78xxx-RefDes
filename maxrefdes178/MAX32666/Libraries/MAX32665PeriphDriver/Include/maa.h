/**
 * @file    maa.h
 * @brief   MAA communications interface driver.
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
 * $Date: 2018-09-11 00:20:17 +0300 (Tue, 11 Sep 2018) $
 * $Revision: 37771 $
 *
 *************************************************************************** */

#ifndef _MAA_H_
#define _MAA_H_

#include <stdint.h>
#include "tpu_regs.h"
#include "mxc_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup maa Modular Arithmetic Accelerator (MAA)
 * @ingroup tpu
 * @{
 */

/* ****** MACROS ****** */
#define MAA_MAX_SIZE           256     // in bytes
#define MAA_MAX_WORD_SIZE      2048    // in bits

/***************************************************************************************************************
									DATA STRUCTURES FOR MAA INITIALIZATION
***************************************************************************************************************/
typedef enum {
    TPU_MAA_EXP   = MXC_V_TPU_MAA_CTRL_CLC_EXP,        // Select exponentiation operation
    TPU_MAA_SQ    = MXC_V_TPU_MAA_CTRL_CLC_SQ,         // Select square operation
    TPU_MAA_MUL   = MXC_V_TPU_MAA_CTRL_CLC_MUL,        // Select multiplication operation
    TPU_MAA_SQMUL = MXC_V_TPU_MAA_CTRL_CLC_SQMUL,      // Select square followed by multiplication operation
    TPU_MAA_ADD   = MXC_V_TPU_MAA_CTRL_CLC_ADD,        // Select add operation
    TPU_MAA_SUB   = MXC_V_TPU_MAA_CTRL_CLC_SUB         // Select subtract operation
} tpu_maa_clcsel_t;

/***************************************************************************************************************
											DRIVER EXPOSED API's
***************************************************************************************************************/
/**
 * @brief      Reset the TPU accelerator
 */
void MAA_Reset(void);

/**
 * @brief      Configure MAA operation with appropriate MAA word size
 * @param      size		 	 Defines the number of bits for modular operation 
 * @return     #E_BAD_PARAM  Specified size \p size, out of range		
 * @return     #E_SUCCESS    Cipher algorithm configured successfully 
 */
int MAA_Init(unsigned int size);

/**
 * @brief      Release MAA
 * @details    Shuts down the MAA engine and any associated clocks
 * @return     #E_BAD_PARAM if MAA cannot be stopped
 * @return     #E_NO_ERROR otherwise
 */  
int MAA_Shutdown(void);
    
/**
 * @brief      MAA operation
 * @param      clc       	 Selects the MAA calculation operation  		
 * @param      multiplier 	 Pointer to multiplier data
 * @param      multiplicand  Pointer to multiplicand data
 * @param      exp		 	 Pointer to exponent data
 * @param      mod           Pointer to modular data
 * @param      result        Output buffer
 * @param      len           Specifies length to the nearest 32-bit boundary
 * @return     #E_NULL_PTR   Specified pointers \p multiplier; \p multiplicand; \p exp; \p mod points to null
 * @return     #E_INVALID    Specified MAA calculation operation is invalid
 * @return     #E_BAD_STATE  MAA Error occurs
 * @return     #E_SUCCESS    MAA process completed successfully
 *
 * @note       \p multiplier; \p multiplicand; \p exp; \p mod, data must be loaded with zero pad to specified length \p len, or the "garbage bits" will case erroneous results
 */
int MAA_Compute(tpu_maa_clcsel_t clc, char *multiplier, char *multiplicand, char *exp, char *mod, int *result, unsigned int len);

#ifdef __cplusplus
}
#endif
/**@} end of group maa */

#endif /* _MAA_H_ */
