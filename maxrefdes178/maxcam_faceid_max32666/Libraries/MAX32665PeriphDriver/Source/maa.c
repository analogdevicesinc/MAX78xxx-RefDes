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
 * $Date: 2019-08-02 18:45:17 +0300 (Fri, 02 Aug 2019) $
 * $Revision: 45192 $
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
#include "maa.h"
#include "mem_utils.h"

/* **** Definitions **** */
int exponent = 0;

// MAA crypto memory block address
#define MAA_A (MXC_BASE_TPU + 0x100)     // MAA_A memory offset
#define MAA_B (MXC_BASE_TPU + 0x200)     // MAA_B memory offset
#define MAA_R (MXC_BASE_TPU + 0x300)     // MAA_R memory offset
#define MAA_T (MXC_BASE_TPU + 0x400)     // MAA_T memory offset
#define MAA_E (MXC_BASE_TPU + 0x500)     // MAA_E memory offset
#define MAA_M (MXC_BASE_TPU + 0x600)     // MAA_M memory offset

// Define a pointer pointing to different MAA memory location
volatile unsigned int *maa_a = (volatile unsigned int *)MAA_A;
volatile unsigned int *maa_b = (volatile unsigned int *)MAA_B;
volatile unsigned int *maa_r = (volatile unsigned int *)MAA_R;
volatile unsigned int *maa_t = (volatile unsigned int *)MAA_T;
volatile unsigned int *maa_e = (volatile unsigned int *)MAA_E;
volatile unsigned int *maa_m = (volatile unsigned int *)MAA_M;

/* **** Functions **** */
// *****************************************************************************
static void clear_done_flags(void)
{
    // Clear all done flags
    MXC_TPU->ctrl |= MXC_F_TPU_CTRL_DMA_DONE | MXC_F_TPU_CTRL_GLS_DONE |
                        MXC_F_TPU_CTRL_HSH_DONE | MXC_F_TPU_CTRL_CPH_DONE | MXC_F_TPU_CTRL_MAA_DONE;
}

// *****************************************************************************
static void maa_mem_clear(void)
{
    // Initialize MAA memory
    memset32((uint32_t *)MAA_A, 0, MAA_MAX_SIZE);
    memset32((uint32_t *)MAA_B, 0, MAA_MAX_SIZE);
    memset32((uint32_t *)MAA_R, 0, MAA_MAX_SIZE);
    memset32((uint32_t *)MAA_T, 0, MAA_MAX_SIZE);
    memset32((uint32_t *)MAA_E, 0, MAA_MAX_SIZE);
    memset32((uint32_t  *)MAA_M, 0, MAA_MAX_SIZE);
}

// *****************************************************************************
void MAA_Reset(void)
{
    // Reset Crypto Accelerator
    MXC_TPU->ctrl = MXC_F_TPU_CTRL_RST;
    memset32((uint32_t *)MXC_TPU, 0, sizeof(mxc_tpu_regs_t));
    maa_mem_clear();
}

// *****************************************************************************
int MAA_Init(unsigned int size)
{
    int err;

    // Check if size is within the valid range
    if (size > MAA_MAX_WORD_SIZE) {
        return E_BAD_PARAM;
    }
    
    err = SYS_TPU_Init(NULL);
    if(err != E_NO_ERROR) {
        return err;
    }
    
    // Reset crypto block
    MAA_Reset();
    
    // Set the legacy bit
    MXC_TPU->ctrl = MXC_F_TPU_CTRL_FLAG_MODE;
    
    // Clear interrupt flags
    MXC_TPU->ctrl &= ~MXC_F_TPU_CTRL_INTR;
    
    // Set MAA word size
    MXC_TPU->maa_maws = (size << MXC_F_TPU_MAA_MAWS_MAWS_POS) & MXC_F_TPU_MAA_MAWS_MAWS;
    
    // Define memory locations for MAA
    MXC_TPU->maa_ctrl |= (0x6<<MXC_F_TPU_MAA_CTRL_TMA_POS) | (0x4<<MXC_F_TPU_MAA_CTRL_RMA_POS) | 
                            (0x2<<MXC_F_TPU_MAA_CTRL_BMA_POS) | (0X0<<MXC_F_TPU_MAA_CTRL_AMA_POS);
                            
    clear_done_flags();
    
    return E_SUCCESS;
}

// *****************************************************************************
int MAA_Shutdown(void)
{
    return SYS_TPU_Shutdown();
}

// *****************************************************************************
int MAA_Compute(tpu_maa_clcsel_t clc, char *multiplier, char *multiplicand, char *exp, char *mod,
                int *result, unsigned int len)
{
    unsigned i;
    
    // Check that we're performing a valid operation
    if (clc >= 0x6) {
        return E_INVALID;
    }

    // Check if exp pointing to NULL for exponent calculation
    if ((clc == TPU_MAA_EXP) && (exp == NULL)) {
        return E_NULL_PTR;
    }
    
    // Check if multiplier, multipilicand & mod operands pointing to null
    if (multiplier == NULL || multiplicand == NULL || mod == NULL || result == NULL) {
        return E_NULL_PTR;
    }
    
    if (len == 0) {
        return E_INVALID;
    }

    // Initialize MAA memory
    maa_mem_clear();
    
    // Copy operands into the memory
    if (clc == TPU_MAA_EXP) {
        memcpy32((void *)MAA_E, (uint32_t*)exp, len);
    }
    memcpy32((void *)MAA_A, (uint32_t*)multiplier, len);
    memcpy32((void *)MAA_B, (uint32_t*)multiplicand, len);
    memcpy32((void *)MAA_M, (uint32_t*)mod, len);
    
    // Start MAA
    MXC_SETFIELD(MXC_TPU->maa_ctrl, MXC_F_TPU_MAA_CTRL_CLC, clc);
    MXC_TPU->maa_ctrl |= MXC_F_TPU_MAA_CTRL_STC;
    
    // Check if MAA Error occurs
    if (MXC_TPU->maa_ctrl & MXC_F_TPU_MAA_CTRL_MAAER) {
        return E_BAD_STATE;
    }
    
    // Wait until operation is complete
    while (!(MXC_TPU->ctrl & MXC_F_TPU_CTRL_MAA_DONE));
    
    // load the output buffer
    for (i=0; i<(len/4); i++) {
        // Copy 4 bytes data
        *(result + i) = (unsigned int)(*(maa_r + i));
    }
    
    clear_done_flags();
    
    return E_SUCCESS;
}
