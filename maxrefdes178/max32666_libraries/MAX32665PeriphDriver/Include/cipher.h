/**
 * @file    cipher.h
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

#ifndef _CIPHER_H_
#define _CIPHER_H_

#include <stdint.h>
#include "tpu_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup tpu Trust Protection Unit
 * @ingroup periphlibs
 */
  
/**
 *
 * @defgroup cipher AES and DES
 * @ingroup tpu
 * @{
 */

/* ****** MACROS ****** */
#define DES_DATA_LEN  8     // The byte length for DES data block
#define AES_DATA_LEN  16      // The byte length for AES data block
#define MAX_KEY_SIZE    32      // Defines maximum key length
#define MXC_AES_DATA_LEN  (128 / 8) /**< Number of bytes in an AES plaintext or ciphertext block, which are always 128-bits long. */
#define MXC_AES_KEY_128_LEN   (128 / 8) /**< Number of bytes in a AES-128 key. */
#define MXC_AES_KEY_192_LEN   (192 / 8) /**< Number of bytes in a AES-192 key. */
#define MXC_AES_KEY_256_LEN   (256 / 8) /**< Number of bytes in a AES-256 key. */

/***************************************************************************************************************
                  DATA STRUCTURES FOR CRYPTO INITIALIZATION
***************************************************************************************************************/
/**
  * Enumeration type for the Crypto Cipher Operation(128/192/256-bit key)
  */
typedef enum {
  TPU_CIPHER_DIS           = MXC_V_TPU_CIPHER_CTRL_CIPHER_DIS,       // Disable
  TPU_CIPHER_AES128        = MXC_V_TPU_CIPHER_CTRL_CIPHER_AES128,    // Select AES-128
  TPU_CIPHER_AES192        = MXC_V_TPU_CIPHER_CTRL_CIPHER_AES192,    // Select AES-192
  TPU_CIPHER_AES256        = MXC_V_TPU_CIPHER_CTRL_CIPHER_AES256,    // Select AES-256
  TPU_CIPHER_DES           = MXC_V_TPU_CIPHER_CTRL_CIPHER_DES,       // Select DES
  TPU_CIPHER_TDES          = MXC_V_TPU_CIPHER_CTRL_CIPHER_TDES       // Select TDEA
} tpu_ciphersel_t;

/**
  * Enumeration type for the Crypto Mode Select
  */
typedef enum {
  TPU_MODE_ECB             = MXC_V_TPU_CIPHER_CTRL_MODE_ECB,         // Select ECB
  TPU_MODE_CBC             = MXC_V_TPU_CIPHER_CTRL_MODE_CBC,         // Select CBC
  TPU_MODE_CFB             = MXC_V_TPU_CIPHER_CTRL_MODE_CFB,         // Select CFB
  TPU_MODE_CTR             = MXC_V_TPU_CIPHER_CTRL_MODE_CTR          // Select CTR
} tpu_modesel_t;

/***************************************************************************************************************
                      DRIVER EXPOSED API's
***************************************************************************************************************/
/**
 * @brief      Reset the crypto accelerator
 */
void TPU_Cipher_Reset(void);

/**
 * @brief      Shutdown crypto controller
 */
void TPU_Shutdown(void);

/**
 * @brief      Configure crypto cipher operation for different modes
 * @param      mode      Selects the Crypto operation mode
 * @param      cipher    Selects the Cipher Operation mode
 * @return     #E_SUCCESS     Cipher algorithm configured successfully
 */
int TPU_Cipher_Config(tpu_modesel_t mode, tpu_ciphersel_t cipher);

/**
 * @brief      The DES encryption process
 * @param      plaintext     Pointer to the plaintext data
 * @param      iv            Pointer to the initial vector data
 * @param      key           Pointer to the crypto key
 * @param      mode          Selects the Crypto operation mode
 * @param      data_size     Specifies length of data in bytes
 * @param      outptr        Output buffer
 * @return     #E_NULL_PTR   Specified pointers \p plaintext; \p iv; \p key points to null
 * @return     #E_INVALID    DES Encryption process failed
 * @return     #E_SUCCESS    DES Encryption process completed successfully
 */
int TPU_DES_Encrypt(const char *plaintext, const char *iv, const char *key, tpu_modesel_t mode, unsigned int data_size, char *outptr);

/**
 * @brief      The DES decryption process
 * @param      ciphertext    Pointer to the ciphertext data
 * @param      iv            Pointer to the initial vector data
 * @param      key           Pointer to the crypto key
 * @param      mode          Selects the Crypto operation mode
 * @param      data_size     Specifies length of data in bytes
 * @param      outptr        Output buffer
 * @return     #E_NULL_PTR   Specified pointers \p plaintext; \p iv; \p key points to null
 * @return     #E_INVALID    DES Decryption process failed
 * @return     #E_SUCCESS    DES Decryption process completed successfully
 */
int TPU_DES_Decrypt(const char *ciphertext, const char *iv, const char *key, tpu_modesel_t mode, unsigned int data_size, char *outptr);

/**
 * @brief      The TDES encryption process
 * @param      plaintext     Pointer to the plaintext data
 * @param      iv            Pointer to the initial vector data
 * @param      key           Pointer to the crypto key
 * @param      mode          Selects the Crypto operation mode
 * @param      data_size     Specifies length of data in bytes
 * @param      outptr        Output buffer
 * @return     #E_NULL_PTR   Specified pointers \p plaintext; \p iv; \p key points to null
 * @return     #E_INVALID    TDES Encryption process failed
 * @return     #E_SUCCESS    TDES Encryption process completed successfully
 */
int TPU_TDES_Encrypt(const char *plaintext, const char *iv, const char *key, tpu_modesel_t mode, unsigned int data_size, char *outptr);

/**
 * @brief      The TDES decryption process
 * @param      ciphertext    Pointer to the ciphertext data
 * @param      iv            Pointer to the initial vector data
 * @param      key           Pointer to the crypto key
 * @param      mode          Selects the Crypto operation mode
 * @param      data_size     Specifies length of data in bytes
 * @param      outptr        Output buffer
 * @return     #E_NULL_PTR   Specified pointers \p plaintext; \p iv; \p key points to null
 * @return     #E_INVALID   TDES Decryption process failed
 * @return     #E_SUCCESS    TDES Decryption process completed successfully
 */
int TPU_TDES_Decrypt(const char *ciphertext, const char *iv, const char *key, tpu_modesel_t mode, unsigned int data_size, char *outptr);

/**
 * @brief      The AES encryption process
 * @param      plaintext     Pointer to the plaintext data
 * @param      iv            Pointer to the initial vector data
 * @param      key           Pointer to the crypto key
 * @param      cipher        Selects the Cipher Operation mode
 * @param      mode          Selects the Crypto operation mode
 * @param      data_size     Specifies length of data in bytes
 * @param      outptr        Output buffer
 * @return     #E_NULL_PTR   Specified pointers \p plaintext; \p iv; \p key points to null
 * @return     #E_BAD_PARAM   Specified Cipher operation mode \p cipher is invalid, see #tpu_ciphersel_t
 * @return     #E_INVALID    AES Encryption process failed
 * @return     #E_SUCCESS     AES Encryption process completed successfully
 */
int TPU_AES_Encrypt(const char *plaintext, const char *iv, const char *key, tpu_ciphersel_t cipher, tpu_modesel_t mode, unsigned int data_size, char *outptr);

/**
 * @brief      The AES decryption process
 * @param      ciphertext    Pointer to the ciphertext data
 * @param      iv            Pointer to the initial vector data
 * @param      key           Pointer to the crypto key
 * @param      cipher        Selects the Cipher Operation mode
 * @param      mode          Selects the Crypto operation mode
 * @param      data_size     Specifies length of data in bytes
 * @param      outptr        Output buffer
 * @return     #E_NULL_PTR   Specified pointers \p plaintext; \p iv; \p key points to null
 * @return     #E_BAD_PARAM  Specified Cipher operation mode \p cipher is invalid, see #tpu_ciphersel_t
 * @return     #E_INVALID    AES Encryption process failed
 * @return     #E_SUCCESS    AES Encryption process completed successfully
 */
int TPU_AES_Decrypt(const char *ciphertext, const char *iv, const char *key, tpu_ciphersel_t cipher, tpu_modesel_t mode, unsigned int data_size, char *outptr);

#ifdef __cplusplus
}
#endif
/**@} end of group cipher */

#endif /* _CIPHER_H_ */
