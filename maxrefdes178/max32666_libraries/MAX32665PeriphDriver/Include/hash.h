/**
 * @file    hash.h
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
 * $Date: 2018-09-11 00:20:17 +0300 (Tue, 11 Sep 2018) $
 * $Revision: 37771 $
 *
 *************************************************************************** */

#ifndef _CRYPTO_H_
#define _CRYPTO_H_

#include <stdint.h>
#include "tpu_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup hash HASH
 * @ingroup tpu
 * @{
 */
/* ****** MACROS ****** */

/**
  * Enumeration type for Hash function Select
  */
typedef enum {
  TPU_HASH_DIS             = MXC_V_TPU_HASH_CTRL_HASH_DIS,            // Disable
  TPU_HASH_SHA1            = MXC_V_TPU_HASH_CTRL_HASH_SHA1,           // Select SHA1
  TPU_HASH_SHA224          = MXC_V_TPU_HASH_CTRL_HASH_SHA224,         // Select SHA224
  TPU_HASH_SHA256          = MXC_V_TPU_HASH_CTRL_HASH_SHA256,         // Select SHA256
  TPU_HASH_SHA384          = MXC_V_TPU_HASH_CTRL_HASH_SHA384,         // Select SHA384
  TPU_HASH_SHA512          = MXC_V_TPU_HASH_CTRL_HASH_SHA512          // Select SHA384
} tpu_hashfunsel_t;

/**
 * @brief      Reset the crypto accelerator
 */
void TPU_Hash_Reset(void);

/**
 * @brief      Shutdown crypto controller
 */
void TPU_Hash_Shutdown(void);

/**
 * @brief      Configure     crypto hash operation for different hash functions
 * @param      func          Selects the hash function
 * @return     #E_SUCCESS    Hash algorithm configured successfully
 */
int TPU_Hash_Config(tpu_hashfunsel_t func);

/**
 * @brief      Test the SHA process
 * @param      fun           Selects the hash function
 * @param      msg           Pointer to source message
 * @param      byteLen       Specifies size of message in bytes
 * @param      digest        Digest buffer
 * @return     #E_NULL_PTR   Specified pointers \p msg; \p digest points to null
 * @return     #E_SUCCESS    SHA process completed successfully
 */
int TPU_SHA( const char *msg, tpu_hashfunsel_t fun, unsigned int byteLen, char *digest);

#ifdef __cplusplus
}
#endif
/**@} end of group hash */

#endif /* _CRYPTO_H_ */
