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
 * $Date: 2018-10-16 00:49:29 +0300 (Tue, 16 Oct 2018) $
 * $Revision: 38520 $
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
#include "crc.h"
#include "mem_utils.h"

/* **** Global Data **** */

/* **** Functions **** */
static void clear_done_flags(void);

// *********************************** Function to Clear all Done Flags ***************************************
static void clear_done_flags(void)
{
    // Clear all done flags
    MXC_TPU->ctrl |= MXC_F_TPU_CTRL_DMA_DONE | MXC_F_TPU_CTRL_GLS_DONE |
                        MXC_F_TPU_CTRL_HSH_DONE | MXC_F_TPU_CTRL_CPH_DONE | 
                        MXC_F_TPU_CTRL_MAA_DONE;
}

// ************************************** Function to Clear Crypto Register ************************************
void TPU_CRC_Reset(void)
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
void TPU_CRC_Shutdown(void)
{
    SYS_TPU_Shutdown();
}

// ************************************ Function to Configure CRC Algorithm ****************************************
int TPU_CRC_Config(void)
{
    // Reset Crypto Block
    TPU_CRC_Reset();
    
    // Set input and output FIFO modes
    MXC_TPU->ctrl |= MXC_S_TPU_CTRL_RDSRC_DMAORAPB ;
    
    // Enable CRC
    MXC_TPU->crc_ctrl = MXC_F_TPU_CRC_CTRL_CRC;
    
    return E_SUCCESS;
}

// ************************************* Function to Generate CRC value *******************************************
int TPU_CRC(const uint8_t *src, uint32_t len, uint32_t poly, uint32_t *crc)
{
    int err;
    
    // Check if src is pointing to null
    if (src == NULL || crc == NULL) {
        return E_NULL_PTR;
    }
    
    // Check if data size is null;
    if (len == 0) {
        return E_INVALID;
    }
    
    err = TPU_CRC_Config();
    if(err != E_SUCCESS) {
        return err;
    }
    
    // Load CRC polynomial into crc polynomial register
    MXC_TPU->crc_poly = poly;
    
    // Setup DMA transfer
    MXC_TPU->dma_src = (uint32_t)src;
    MXC_TPU->dma_cnt = len;
    
    // Wait for dma done flag
    while (!(MXC_TPU->ctrl & MXC_F_TPU_CTRL_DMA_DONE)){
    }
    
    // Wait until operation is complete
    while (!(MXC_TPU->ctrl & MXC_F_TPU_CTRL_GLS_DONE)) {
    }
    
    // Clear gls done flag
    MXC_TPU->ctrl |= MXC_F_TPU_CTRL_GLS_DONE;
    
    // Store the crc value
    *crc = MXC_TPU->crc_val;
    
    return E_SUCCESS;
}

// ************************************ Function to Configure HAM Algorithm ****************************************
int TPU_Ham_Config(void)
{
    // Reset Crypto Block
    TPU_CRC_Reset();
    
    MXC_TPU->ctrl |= MXC_F_TPU_CTRL_DMADNEMSK;
    
    // Set input and output FIFO modes
    MXC_TPU->ctrl |= MXC_S_TPU_CTRL_RDSRC_DMAORAPB;
    
    // Reset hamming code
    MXC_TPU->crc_ctrl = MXC_F_TPU_CRC_CTRL_HRST;
    while (MXC_TPU->crc_ctrl & MXC_F_TPU_CRC_CTRL_HRST) {
    }
    
    // Enable Hamming code
    MXC_TPU->crc_ctrl |= MXC_F_TPU_CRC_CTRL_HAM;
    
    // Clear all done flags
    clear_done_flags();
    
    return E_SUCCESS;
}

// ************************************* Function to Generate ECC value *******************************************
int TPU_Ham(const uint8_t *src, uint32_t len, uint32_t *ecc)
{
    int err;
    
    // Check if data is pointing to null
    if (src == NULL || ecc == NULL) {
        return E_NULL_PTR;
    }
    
    // Check if data size is null;
    if (len == 0) {
        return E_INVALID;
    }

    err = TPU_Ham_Config();
    if(err != E_SUCCESS) {
        return err;
    }
    
    // Setup DMA transfer
    MXC_TPU->dma_src = (uint32_t)src;
    MXC_TPU->dma_cnt = len;
    
    // Wait until operation is complete
    while (!(MXC_TPU->ctrl & MXC_F_TPU_CTRL_GLS_DONE)) {
    }
    
    // Wait for dma done flag
    while (!(MXC_TPU->ctrl & MXC_F_TPU_CTRL_DMA_DONE)) {
    }
    
    // Clear gls done flag
    MXC_TPU->ctrl |= MXC_F_TPU_CTRL_GLS_DONE;
    
    // Store the ecc value
    *ecc = MXC_TPU->ham_ecc;
    
    return E_SUCCESS;
}
