/**
 * @file    crc.h
 * @brief   Crypto communications interface driver.
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
 * $Date: 2018-11-09 00:08:58 +0300 (Fri, 09 Nov 2018) $
 * $Revision: 39056 $
 *
 *************************************************************************** */

#ifndef _CRC_H_
#define _CRC_H_

#include <stdint.h>
#include "tpu_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup crc Cyclic Redundancy Check (CRC)
 * @ingroup tpu
 * @{
 */

/* ****** MACROS ****** */
// CRC Polynomials
#define TPU_CRC32_ETHERNET 0xEDB88320
#define TPU_CRC_CCITT      0x00008408
#define TPU_CRC16          0x0000A001
#define TPU_USBDATA        0x80050000
#define TPU_PARITY         0x00000001

/***************************************************************************************************************
                  DATA STRUCTURES FOR CRYPTO INITIALIZATION
***************************************************************************************************************/

/***************************************************************************************************************
                      DRIVER EXPOSED API's
***************************************************************************************************************/
/**
 * @brief      Reset the crypto accelerator
 */
void TPU_CRC_Reset(void);

/**
 * @brief      Shutdown crypto controller
 */
void TPU_CRC_Shutdown(void);

/**
 * @brief      Configure crypto CRC operation
 * @return     #E_SUCCESS    CRC algorithm configured successfully
 */
int TPU_CRC_Config(void);

/**
 * @brief      Test the CRC process
 * @param      src           Pointer to source message
 * @param      len           Specifies size of message in bytes
 * @param      poly          Selects the crc polynomial
 * @param      crc           Pointer to store crc value
 * @return     #E_NULL_PTR   Specified pointers \p src; points to null
 * @return     #E_SUCCESS    CRC process completed successfully
 */
int TPU_CRC(const uint8_t *src, uint32_t len, uint32_t poly, uint32_t *crc);

/**
 * @brief      Configure crypto HAM operation
 * @return     #E_SUCCESS    HAM algorithm configured successfully
 */
int TPU_Ham_Config(void);

/**
 * @brief      Test the CRC process
 * @param      src           Pointer to source message
 * @param      len           Specifies size of message in bytes
 * @param      ecc           Pointer to store ecc value
 * @return     #E_NULL_PTR   Specified pointers \p src; points to null
 * @return     #E_SUCCESS    CRC process completed successfully
 */
int TPU_Ham(const uint8_t *src, uint32_t len, uint32_t *ecc);

#ifdef __cplusplus
}
#endif
/**@} end of group crc */

#endif /* _CRC_H_ */
