/* *****************************************************************************
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
 * $Date: 2019-08-02 18:28:21 +0300 (Fri, 02 Aug 2019) $
 * $Revision: 45187 $
 *
 **************************************************************************** */

/* **** Includes **** */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mxc_config.h"
#include "mxc_errors.h"
#include "mxc_sys.h"
#include "tpu_regs.h"
#include "cipher.h"
#include "mem_utils.h"

/* **** Global Data **** */

/* **** Functions **** */
static unsigned int Cipher_GetLength(tpu_ciphersel_t cipher, unsigned int data_size);
static void clear_done_flags(void);
static void Cipher_EncDecSelect(int enc);
static int TPU_DoOperation(const char *src, const char *iv, const char *key,
                             tpu_ciphersel_t cipher, tpu_modesel_t mode, unsigned int data_size, char *outptr);
static unsigned int get_key_size(tpu_ciphersel_t cipher);
static unsigned int get_block_size(tpu_ciphersel_t cipher);


static unsigned int get_key_size(tpu_ciphersel_t cipher){
    // Key size indexed by 'opsel'
    switch(cipher)
    {
        case TPU_CIPHER_DIS:
            return 0;
        case TPU_CIPHER_AES128:
            return 16;
        case TPU_CIPHER_AES192:
            return 24;
        case TPU_CIPHER_AES256:
            return 32;
        case TPU_CIPHER_DES:
            return 8;
        case TPU_CIPHER_TDES:
            return 24;
    }
    // if returns this bad param was passed in or disable.
    return 0;
}

static unsigned int get_block_size(tpu_ciphersel_t cipher)
{
    switch(cipher)
    {
        case TPU_CIPHER_DIS:
            return 0;
        case TPU_CIPHER_AES128:
            return AES_DATA_LEN;
        case TPU_CIPHER_AES192:
            return AES_DATA_LEN;
        case TPU_CIPHER_AES256:
            return AES_DATA_LEN;
        case TPU_CIPHER_DES:
            return DES_DATA_LEN;
        case TPU_CIPHER_TDES:
            return DES_DATA_LEN;
    }
    // if returns this bad param was passed in or disable.
    return 0;
}

// ************************************* Function to Get Number of Blocks **************************************
static unsigned int Cipher_GetLength(tpu_ciphersel_t cipher, unsigned int data_size)
{
    unsigned int numBlocks, block_size;
    block_size = get_block_size(cipher);
    numBlocks = data_size / block_size;
    if ((data_size % block_size) > 0) {
        numBlocks++;
    }
            
    return numBlocks;
}

// *********************************** Function to Clear all Done Flags ***************************************
static void clear_done_flags(void)
{
    // Clear all done flags
    MXC_TPU->ctrl |= MXC_F_TPU_CTRL_DMA_DONE | MXC_F_TPU_CTRL_GLS_DONE |
                        MXC_F_TPU_CTRL_HSH_DONE | MXC_F_TPU_CTRL_CPH_DONE | 
                        MXC_F_TPU_CTRL_MAA_DONE;
}

// ************************************** Function to Clear Crypto Register ************************************
void TPU_Cipher_Reset(void)
{

    SYS_TPU_Init(NULL);
    
    // Reset Crypto Accelerator
    MXC_TPU->ctrl = MXC_F_TPU_CTRL_RST;

    memset32((uint32_t *)MXC_TPU, 0, sizeof(mxc_tpu_regs_t));

    // Set the legacy bit so done bits are W1C.
    MXC_TPU->ctrl |= MXC_F_TPU_CTRL_FLAG_MODE;

    clear_done_flags();
}

// ************************************** Function to Shutdown Crypto Controller ************************************
void TPU_Shutdown(void)
{
    SYS_TPU_Shutdown();
}


// ************************ Function to Enable Encrypt/Decrypt Cipher Operation *******************************
static void Cipher_EncDecSelect(int enc)
{
    // Enable Encryption/Decryption Operation
    if (enc) {
        MXC_TPU->cipher_ctrl &= ~MXC_F_TPU_CIPHER_CTRL_ENC;
    } else {
        MXC_TPU->cipher_ctrl |= MXC_F_TPU_CIPHER_CTRL_ENC;
    }
}

// ******************************* Function to Configure Cipher Operation *************************************
int TPU_Cipher_Config(tpu_modesel_t mode, tpu_ciphersel_t cipher)
{
    // Reset crypto block
    TPU_Cipher_Reset();
    
    
    // Select cipher mode ECB/CBC/CFB/CTR
    // Select cipher operation AES_128/192/256/DES/TDES
    // Select user cipher key
    MXC_TPU->cipher_ctrl = (mode << MXC_F_TPU_CIPHER_CTRL_MODE_POS) | (cipher <<
                              MXC_F_TPU_CIPHER_CTRL_CIPHER_POS) | MXC_S_TPU_CIPHER_CTRL_SRC_CIPHERKEY;
                              
    return E_SUCCESS;
}

// ************************************ Function to Test Cipher Algorithm ***********************************
static int TPU_DoOperation(const char *src, const char *iv, const char *key,
                             tpu_ciphersel_t cipher, tpu_modesel_t mode, unsigned int data_size, char *outptr)
{
    unsigned int keyLength, dataLength, numBlocks, i;

    if (data_size == 0) {
        return E_INVALID;
    }
    
    // Check if src, key, iv is a null pointer
    if (src == NULL || (iv == NULL && mode != TPU_MODE_ECB) || key == NULL) {
        return E_NULL_PTR;
    }
    
    numBlocks = Cipher_GetLength(cipher, data_size);
    
    keyLength = get_key_size(cipher);
    dataLength = get_block_size(cipher);

    // Load key into cipher key register
    memcpy32((void *)&MXC_TPU->cipher_key[0], (void *)key, keyLength);
    
    // Load Initial Vector if necessary
    if (mode != TPU_MODE_ECB) {
        memcpy32((void *)&MXC_TPU->cipher_init[0], (void *)iv, dataLength);
    }
    
    for (i = 0; i < numBlocks; i++) {

        // Wait until ready for data
        while (!(MXC_TPU->ctrl & MXC_F_TPU_CTRL_RDY)) {
        }
        
        // Load plaintext into data in register to start the operation
        memcpy32((void *)&MXC_TPU->din[0], (void *)src, dataLength);
        
        // Wait until operation is complete
        while (!(MXC_TPU->ctrl & MXC_F_TPU_CTRL_CPH_DONE)) {
        }
        
        // Copy data out
        memcpy32((void *)outptr, (void *)&MXC_TPU->dout[0], dataLength);
        
        src += dataLength;
        outptr += dataLength;
        
        // Clear done flag so reading of the next block will be gated properly.
        MXC_TPU->ctrl |= MXC_F_TPU_CTRL_CPH_DONE;
    }
    
    return E_SUCCESS;
}

int TPU_DES_Encrypt(const char *plaintext, const char *iv, const char *key,
                       tpu_modesel_t mode, unsigned int data_size, char *outptr)
{    
    // Enable cipher encrypt/decrypt process
    Cipher_EncDecSelect(1);
    
    // Start the cipher operation
    return TPU_DoOperation(plaintext, iv, key, TPU_CIPHER_DES, mode, data_size, outptr);
}

int TPU_DES_Decrypt(const char *ciphertext, const char *iv, const char *key,
                       tpu_modesel_t mode, unsigned int data_size, char *outptr)
{    
    // Enable cipher encrypt/decrypt process
    Cipher_EncDecSelect(0);
    
    // Start the cipher operation
    return TPU_DoOperation(ciphertext, iv, key, TPU_CIPHER_DES, mode, data_size, outptr);
}

int TPU_TDES_Encrypt(const char *plaintext, const char *iv, const char *key,
                        tpu_modesel_t mode, unsigned int data_size, char *outptr)
{
    // Enable cipher encrypt/decrypt process
    Cipher_EncDecSelect(1);
    
    // Start the cipher operation
    return TPU_DoOperation(plaintext, iv, key, TPU_CIPHER_TDES, mode, data_size, outptr);
}

int TPU_TDES_Decrypt(const char *ciphertext, const char *iv, const char *key,
                        tpu_modesel_t mode, unsigned int data_size, char *outptr)
{
    // Enable cipher encrypt/decrypt process
    Cipher_EncDecSelect(0);
    
    // Start the cipher operation
    return TPU_DoOperation(ciphertext, iv, key, TPU_CIPHER_TDES, mode, data_size, outptr);
}

int TPU_AES_Encrypt(const char *plaintext, const char *iv, const char *key,
                       tpu_ciphersel_t cipher, tpu_modesel_t mode, unsigned int data_size, char *outptr)
{
    if ((cipher != TPU_CIPHER_AES128) &&
        (cipher != TPU_CIPHER_AES192) &&
        (cipher != TPU_CIPHER_AES256)) {
        return E_BAD_PARAM;
    }

    // Enable cipher encrypt/decrypt process
    Cipher_EncDecSelect(1);
    
    // Start the cipher operation
    return TPU_DoOperation(plaintext, iv, key, cipher, mode, data_size, outptr);
}

int TPU_AES_Decrypt(const char *ciphertext, const char *iv, const char *key,
                       tpu_ciphersel_t cipher, tpu_modesel_t mode, unsigned int data_size, char *outptr)
{
    if ((cipher != TPU_CIPHER_AES128) && 
        (cipher != TPU_CIPHER_AES192) && 
        (cipher != TPU_CIPHER_AES256)) {
        return E_BAD_PARAM;
    }

    // Enable cipher encrypt/decrypt process
    Cipher_EncDecSelect(0);
    
    // Start the cipher operation
    return TPU_DoOperation(ciphertext, iv, key, cipher, mode, data_size, outptr);
}
