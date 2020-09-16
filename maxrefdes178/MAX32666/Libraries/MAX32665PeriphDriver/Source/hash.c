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
 * $Date: 2019-09-11 21:22:10 +0300 (Wed, 11 Sep 2019) $
 * $Revision: 46044 $
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
#include "hash.h"
#include "mem_utils.h"

/* **** Global Data **** */

/* **** Functions **** */
static void clear_done_flags(void);
static unsigned int get_block_size_sha(tpu_hashfunsel_t func);
static unsigned int get_dgst_size(tpu_hashfunsel_t func);
static unsigned int get_block_size_sha(tpu_hashfunsel_t func)
{
    // Block size in bytes indexed by hash function
    switch(func)
    {
        case TPU_HASH_DIS:
            return 0;
        case TPU_HASH_SHA1:
            return 64;
        case TPU_HASH_SHA224:
            return 64;
        case TPU_HASH_SHA256:
            return 64;
        case TPU_HASH_SHA384 :
            return 128;
        case TPU_HASH_SHA512:
            return 128;
    }
    // if returns this bad param was passed in or disable.
    return 0;
}
static unsigned int get_dgst_size(tpu_hashfunsel_t func)
{
    // Digest length in bytes indexed by hash function
    switch(func)
    {
        case TPU_HASH_DIS:
            return 0;
        case TPU_HASH_SHA1:
            return 20;
        case TPU_HASH_SHA224:
            return 28;
        case TPU_HASH_SHA256:
            return 32;
        case TPU_HASH_SHA384 :
            return 48;
        case TPU_HASH_SHA512:
            return 64;
    }
    // if returns this bad param was passed in or disable.
    return 0;
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
void TPU_Hash_Reset(void)
{

    SYS_TPU_Init(NULL);
    
    // Reset Crypto Accelerator
    MXC_TPU->ctrl = MXC_F_TPU_CTRL_RST;

    memset(MXC_TPU, 0, sizeof(mxc_tpu_regs_t));

    // Set the legacy bit so done bits are W1C.
    MXC_TPU->ctrl |= MXC_F_TPU_CTRL_FLAG_MODE;

    clear_done_flags();
}

// ************************************** Function to Shutdown Crypto Controller ************************************
void TPU_Hash_Shutdown(void)
{
    SYS_TPU_Shutdown();
}

static void sha_size(unsigned int *blocks, unsigned int *length, unsigned int *lbyte,
                     tpu_hashfunsel_t fun)
{
    unsigned int block_size_sha = get_block_size_sha(fun);

    if (*blocks == 0) {
        // Special case for 0 length message
        *blocks = 1;
        *lbyte = 1;
    } else {
        // Get size of last block of data
        *lbyte = ((*length) - (((*blocks) - 1) * block_size_sha));
    }
}

// ********************************** Function to Configure SHA Algorithm *************************************
int TPU_Hash_Config(tpu_hashfunsel_t func)
{
    if((func < MXC_V_TPU_HASH_CTRL_HASH_DIS) || (func > MXC_V_TPU_HASH_CTRL_HASH_SHA512))
    {
        return E_BAD_PARAM;
    }
    
    // Reset Crypto Block
    TPU_Hash_Reset();
    
    // Select the Hash Function
    MXC_TPU->hash_ctrl = func << MXC_F_TPU_HASH_CTRL_HASH_POS;
    
    // Initialize the hash values
    MXC_TPU->hash_ctrl |= MXC_F_TPU_HASH_CTRL_INIT;
    
    // Wait until operation is complete
    while (MXC_TPU->hash_ctrl & MXC_F_TPU_HASH_CTRL_INIT) {
    }
    
    return E_SUCCESS;
}

// ************************************ Function to test SHA Algorithm ****************************************
int TPU_SHA(const char *msg, tpu_hashfunsel_t fun, unsigned int byteLen, char *digest)
{
    unsigned int last_byte = 0;
    unsigned int block, word, numBlocks, block_size_sha, dgst_size;
    int i;
    int err;
    
    // Check if msg, digest msg is a null pointer
    if (msg == NULL || digest == NULL) {
        return E_NULL_PTR;
    }
    
    if (byteLen == 0) {
        return E_INVALID;
    }
    
    err = TPU_Hash_Config(fun);
    if(err != E_SUCCESS) {
        return err;
    }
    block_size_sha = get_block_size_sha(fun);
    dgst_size = get_dgst_size(fun);
    // Write message size
    MXC_TPU->hash_msg_sz[0] = byteLen;

    // Determine the size of message data without padding
    numBlocks = (byteLen + (block_size_sha - 1)) / block_size_sha;
    sha_size(&numBlocks, &byteLen, &last_byte, fun);
    
    for (block = 0; block < numBlocks; block++) {
        // Clear done flags
        clear_done_flags();
        
        // Send data to the crypto data register 32-bits at a time
        if (block != numBlocks - 1) {
            for (i = block*block_size_sha, word = 0; word < block_size_sha; word += 4) {
                // Wait until ready for data
                while (!(MXC_TPU->ctrl & MXC_F_TPU_CTRL_RDY));
                
                MXC_TPU->din[0] = (uint32_t)(msg[word     + i])        | 
                                    ((uint32_t)(msg[word + 1 + i]) << 8)  | 
                                    ((uint32_t)(msg[word + 2 + i]) << 16) | 
                                    ((uint32_t)(msg[word + 3 + i]) << 24);
            }
        } else {
            // Determine the current block
            i = block*block_size_sha;
            
            // Set the last msg bit for auto padding the msg
            MXC_TPU->hash_ctrl |= MXC_F_TPU_HASH_CTRL_LAST;
            
            // Process the last block
            for (word = 0; word < last_byte; word += 4) {
                // Wait until ready for data
                while (!(MXC_TPU->ctrl & MXC_F_TPU_CTRL_RDY));
                
                // Send data to the crypto data register
                if (last_byte >= (word + 4)) {
                
                    MXC_TPU->din[0] = (uint32_t)(msg[word + i]) + 
                                        ((uint32_t)(msg[word + 1 + i]) << 8) + 
                                        ((uint32_t)(msg[word + 2 + i]) << 16) + 
                                        ((uint32_t)(msg[word + 3 + i]) << 24);
                } else if ((last_byte & 3) == 1) {
                    MXC_TPU->din[0] = msg[word + i];
                } else if ((last_byte & 3) == 2) {
                    MXC_TPU->din[0] = (uint32_t)msg[word + i] + 
                                        ((uint32_t)(msg[word + 1 + i]) << 8);
                } else if ((last_byte & 3) == 3) {
                    MXC_TPU->din[0] = (uint32_t)msg[word + i] + 
                                       ((uint32_t)(msg[word + 1 + i]) << 8) + 
                                       ((uint32_t)(msg[word + 2 + i]) << 16);
                }
                
            }
        }
        
        // Wait until operation is complete
        while (!(MXC_TPU->ctrl & MXC_F_TPU_CTRL_HSH_DONE)) {
        }
    }
    
    // Clear the done flags
    clear_done_flags();
    
    // Get the msg digest
    memcpy32((uint32_t *)digest, (uint32_t *)&MXC_TPU->hash_digest[0], dgst_size);
    
    return E_SUCCESS;
}
