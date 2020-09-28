/**
 * @file    trng.h
 * @brief   True Random Number Generator(TRNG) function prototypes and data types.
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
 * $Date: 2018-10-16 00:49:29 +0300 (Tue, 16 Oct 2018) $
 * $Revision: 38520 $
 *
 *************************************************************************** */

/* Define to prevent redundant inclusion */
#ifndef _TRNG_H_
#define _TRNG_H_

/* **** Includes **** */
#include <stdint.h>
#include "mxc_config.h"
#include "mxc_sys.h"
#include "trng_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

 /**
  * @brief      TRNG initialize
  * @param      System configuration object
  * @return     #E_NO_ERROR if successful
  */
 int TRNG_Init(const sys_cfg_t* sys_cfg);

 /**
  * @brief      TRNG Shutdown
  *
  * @return     #E_NO_ERROR if successful
  */
 int TRNG_Shutdown(void);

/**
  *@brief	Reads 8-bit value stored in the data register.
  *@param	trng	Pointer to the trng register structure.
  *@return	8-bit data register value.
  */
uint8_t TRNG_Read8BIT(mxc_trng_regs_t *trng);

/**
  *@brief	Reads 16-bit value stored in the data register.
  *@param	trng	Pointer to the trng register structure.
  *@return	16-bit data register value.
  */
uint16_t TRNG_Read16BIT(mxc_trng_regs_t *trng);

/**
  *@brief	Reads 32-bit value stored in the data register.
  *@param	trng	Pointer to the trng register structure.
  *@return	32-bit data register value.
  */
uint32_t TRNG_Read32BIT(mxc_trng_regs_t *trng);

/**
  *@brief	Generates Random Number of variable length.
  *@param	trng	Pointer to the trng register structure.
  *@param	data	Pointer to the Data Buffer.
  *@param	len	Defines length of data(bytes).
  */
void TRNG_Read(mxc_trng_regs_t *trng, uint8_t *data, int len);

/**
  *@brief	Generates 256-bits random number automatically.
  *@param	trng	Pointer to the trng register structure.
  */
void TRNG_Generate_AES(mxc_trng_regs_t *trng);
/**@} end of group trng */

#ifdef __cplusplus
}
#endif

#endif /* _TRNG_H_ */

