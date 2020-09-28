/*******************************************************************************
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
 * $Date: 2020-09-01 17:50:12 +0300 (Tue, 01 Sep 2020) $
 * $Revision: 55090 $
 *
 ******************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "hci_drv.h"
#include "hci_tr.h"
#include "sema_regs.h"
#include "ipc_defs.h"
#include "max32665.h"
#include "mcr_regs.h"
#include "simo_regs.h"
#include "gcr_regs.h"
#include "sdma_regs.h"
#include "hci_drv_sdma.h"
#include "wut_regs.h"
#include "bb_api.h"
#include "ll_api.h"

#ifdef _RTE_
#include "RTE_Components.h"             // Component selection
#endif

#if defined(SDMA_ADV)
#include "sdma_adv_map.inc"
#elif defined(SDMA_SCN)
#include "sdma_scn_map.inc"
#else
#include "sdma_map.inc"
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* OTP Access Control register */
#define MXC_R_OTP_ACNTL         *((volatile uint32_t*)(0x40029040))
#define OTP_OFFSET              0x10800400 
#define OTP_LEN                 0x40
#define MXC_R_SIR_SHR21_OTP     *((volatile uint32_t*)(0x108001D4))
#define MXC_R_SIR_SHR23_OTP     *((volatile uint32_t*)(0x108001DC))

/* Position within SDMA code for the restart flag */
#define WAKEUP_POSITION         12
#define WAKEUP_FLAG_POSITION    8

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

#ifdef __ICCARM__
_Pragma("location=\".shared\"") shared_mem_t arm_shared_mem;
#else
shared_mem_t __attribute__ ((section (".arm_shared"))) arm_shared_mem;
#endif

#ifdef __ICCARM__
_Pragma("location=\".shared\"") shared_mem_t sdma_shared_mem;
#else
shared_mem_t __attribute__ ((section (".sdma_shared"))) sdma_shared_mem;
#endif

#ifdef __ICCARM__
_Pragma("location=\".bin_code\"") uint8_t sdma_code[] = {
#else
volatile uint8_t __attribute__ ((section (".sdma_code"))) sdma_code[] = {
#endif
#if defined(SDMA_ADV)
#include "sdma_adv_code.inc"
#elif defined(SDMA_SCN)
#include "sdma_scn_code.inc"
#else
#include "sdma_code.inc"
#endif
};

#ifdef __ICCARM__
_Pragma("location=\".bin_data\"") uint8_t sdma_data[] = {
#else
volatile uint8_t __attribute__ ((section (".sdma_data"))) sdma_data[] = {
#endif
#if defined(SDMA_ADV)
#include "sdma_adv_data.inc"
#elif defined(SDMA_SCN)
#include "sdma_scn_data.inc"
#else
#include "sdma_data.inc"
#endif
};


// SDMA can not read from OTP, have to copy it into an SRAM section
#ifdef __ICCARM__
_Pragma("location=\".otp\"") uint8_t otp[OTP_LEN];
#else
uint8_t __attribute__ ((section (".otp"))) otp[OTP_LEN];
#endif

extern void hciTrSerialRxIncoming(uint8_t *pBuf, uint8_t len);


/******************************************************************************/
uint32_t SDMAGetWakeupTime(void)
{

    bool_t retval = FALSE;
    uint32_t sema_val;
    uint32_t value = 0;

    while(!retval) {

        __disable_irq();

        // Attempt to lock the SDMA_SEMA
        // Reading the register does an atomic test and set, returns previous value
        sema_val = MXC_SEMA->semaphores[SDMA_SEMA];

        if(!sema_val) {
            value |= sdma_code[WAKEUP_POSITION];
            value |= sdma_code[WAKEUP_POSITION+1] << 8;
            value |= sdma_code[WAKEUP_POSITION+2] << 16;
            value |= sdma_code[WAKEUP_POSITION+3] << 24;
            retval = TRUE;
        }

        // Unlock the ARM_SEMA
        if(!sema_val) {
            MXC_SEMA->semaphores[SDMA_SEMA] = 0x0;
        }

        __enable_irq();
    }


    return value;
}


/******************************************************************************/
void SDMASetARMFlag(uint8_t flag)
{
    bool_t retval = FALSE;
    uint32_t sema_val;

    while(!retval) {

        __disable_irq();

        // Attempt to lock the ARM_SEMA
        // Reading the register does an atomic test and set, returns previous value
        sema_val = MXC_SEMA->semaphores[ARM_SEMA];

        if(!sema_val) {
            sdma_code[WAKEUP_FLAG_POSITION+ARM_FLAG_POS] = flag;
            retval = TRUE;
        }

        // Unlock the ARM_SEMA
        if(!sema_val) {
            MXC_SEMA->semaphores[ARM_SEMA] = 0x0;
        }

        __enable_irq();
    }
}

/******************************************************************************/
uint8_t SDMAGetSDMAFlag(void)
{
    bool_t retval = FALSE;
    uint32_t sema_val;
    uint8_t flag_val;

    while(!retval) {

        __disable_irq();

        // Attempt to lock the SDMA_SEMA
        // Reading the register does an atomic test and set, returns previous value
        sema_val = MXC_SEMA->semaphores[SDMA_SEMA];

        if(!sema_val) {
            flag_val = sdma_code[WAKEUP_FLAG_POSITION+SDMA_FLAG_POS];
            retval = TRUE;
        }

        // Unlock the ARM_SEMA
        if(!sema_val) {
            MXC_SEMA->semaphores[SDMA_SEMA] = 0x0;
        }

        __enable_irq();
    }

    return flag_val;
}

/******************************************************************************/
#ifdef SDMA_BB_CFG_OFFSET
void SDMASetBBCfg(const BbRtCfg_t * p_bb_cfg)
{
    *((volatile BbRtCfg_t *)&sdma_data[SDMA_BB_CFG_OFFSET]) = *p_bb_cfg;
}
#endif /* SDMA_BB_CFG_OFFSET */

/******************************************************************************/
#ifdef SDMA_LL_CFG_OFFSET
void SDMASetLLCfg(const LlRtCfg_t * p_ll_cfg)
{
    *((volatile LlRtCfg_t *)&sdma_data[SDMA_LL_CFG_OFFSET]) = *p_ll_cfg;
}
#endif /* SDMA_LL_CFG_OFFSET */

/******************************************************************************/
void sdmaSetIRQ(void)
{
    if(MXC_GCR->revision != 0xA1) {
        // Enable interrupt mux, not for A1
        MXC_SDMA->int_mux_ctrl0 =
            (BTLE_TX_DONE_IRQn << MXC_F_SDMA_INT_MUX_CTRL0_INTSEL16_POS) |
            (BTLE_RX_RCVD_IRQn << MXC_F_SDMA_INT_MUX_CTRL0_INTSEL17_POS) |
            (BTLE_SFD_DET_IRQn << MXC_F_SDMA_INT_MUX_CTRL0_INTSEL18_POS) |
            (BTLE_SFD_TO_IRQn << MXC_F_SDMA_INT_MUX_CTRL0_INTSEL19_POS);

        MXC_SDMA->int_mux_ctrl1 =
            (BTLE_GP_EVENT_IRQn << MXC_F_SDMA_INT_MUX_CTRL1_INTSEL20_POS);
    } else {

        // Disable interrupt mux and poll BLE interrupts with A1
        MXC_SDMA->int_mux_ctrl0 = 0;
        MXC_SDMA->int_mux_ctrl1 = 0;
    }

    MXC_SDMA->int_mux_ctrl2 = 0;
    MXC_SDMA->int_mux_ctrl3 = 0;
}

/******************************************************************************/
void hciDrvInit(void)
{
    /* Prevent optimization of sdma_data */
    sdma_data[0] = sdma_data[0];
    
    /* Disable SDMA, assert reset */
    MXC_SDMA->ctrl &= ~(MXC_F_SDMA_CTRL_EN);

    /* Enable SDMA clock */
    MXC_GCR->perckcn1 &= ~(MXC_F_GCR_PERCKCN1_SDMAD);

    /* Setup code address */
    MXC_SDMA->ip_addr = (uint32_t)&sdma_code;

#ifndef __CROSSWORKS
    /* Unlock OTP */
    MXC_R_OTP_ACNTL = 0x3a7f5ca3;
    MXC_R_OTP_ACNTL = 0xa1e34f20;
    MXC_R_OTP_ACNTL = 0x9608b2c1;

    /* Copy the OTP into the OTP memory section */
    memcpy((void*)otp, (void*)OTP_OFFSET, OTP_LEN-2);
    otp[OTP_LEN-2] = MXC_R_SIR_SHR21_OTP;
    otp[OTP_LEN-1] = MXC_R_SIR_SHR23_OTP;
#endif

    // Disable ARM BLE interrupts
    NVIC_DisableIRQ(BTLE_TX_DONE_IRQn);
    NVIC_DisableIRQ(BTLE_RX_RCVD_IRQn);
    NVIC_DisableIRQ(BTLE_RX_ENG_DET_IRQn);
    NVIC_DisableIRQ(BTLE_SFD_DET_IRQn);
    NVIC_DisableIRQ(BTLE_SFD_TO_IRQn);
    NVIC_DisableIRQ(BTLE_GP_EVENT_IRQn);
    NVIC_DisableIRQ(BTLE_CFO_IRQn);
    NVIC_DisableIRQ(BTLE_SIG_DET_IRQn);
    NVIC_DisableIRQ(BTLE_AGC_EVENT_IRQn);
    NVIC_DisableIRQ(BTLE_RFFE_SPIM_IRQn);
    NVIC_DisableIRQ(BTLE_TX_AES_IRQn);
    NVIC_DisableIRQ(BTLE_RX_AES_IRQn);
    NVIC_DisableIRQ(BTLE_INV_APB_ADDR_IRQn);
    NVIC_DisableIRQ(BTLE_IQ_DATA_VALID_IRQn);

    sdmaSetIRQ();

    // Initialize the SDMA shared memory segment
    memset((void*)arm_shared_mem.data, 0, SHARED_MEM_DATA_LEN);
    arm_shared_mem.dirty = 1;
    arm_shared_mem.len = 0;

    SDMASetARMFlag(ARM_FLAG_INIT);

    /* Enable SDMA */
    MXC_SDMA->ctrl |= MXC_F_SDMA_CTRL_EN;

    /* Wait for the SDMA to finish initialization */
    while(!MXC_SDMA->irq_flag) {}

    // Clear any pending SDMA interrupts
    MXC_SDMA->irq_flag = 0x1;
    NVIC_ClearPendingIRQ(SDMA_IRQn);

    // Enable interrupt from SDMA
    MXC_SDMA->irq_ie = 1;
    NVIC_EnableIRQ(SDMA_IRQn);
}

/******************************************************************************/
uint16_t hciDrvWrite(uint8_t type, uint16_t len, uint8_t *pData)
{
    uint16_t len_written = 0;
    uint32_t sema_val;

    // Add 1 byte for type
    len++;

    __disable_irq();

    if(SDMAGetSDMAFlag() == SDMA_FLAG_SLEEPING) {
        SDMARestart();
    }

    // Attempt to lock the SDMA_SEMA
    // Reading the register does an atomic test and set, returns previous value
    sema_val = MXC_SEMA->semaphores[SDMA_SEMA];

    // Write the data to the shared memory segment
    if(!sema_val && sdma_shared_mem.dirty) {


        if(len > SHARED_MEM_DATA_LEN) {
            len_written = SHARED_MEM_DATA_LEN;
        } else {
            len_written = len;
        }

        memcpy((void*)sdma_shared_mem.data,(void*)&type, 1);
        memcpy((void*)&sdma_shared_mem.data[1], (void*)pData, len_written-1);
        sdma_shared_mem.len = len_written;
        sdma_shared_mem.dirty = 0;
    }

    if(len_written) {
        // Adjust the len_written that we're returning for they type
        len_written--;

        // Interrupt the SDMA to signal that there is data to process
        MXC_SDMA->int_in_ctrl = 0x1;
    }

    // Unlock the SDMA_SEMA
    if(!sema_val) {
        MXC_SEMA->semaphores[SDMA_SEMA] = 0x0;
    }

    __enable_irq();

    // Wait for SDMA to process the data
    while(!sdma_shared_mem.dirty) {}

    return len_written;
}

/******************************************************************************/
void SDMA0_IRQHandler(void)
{
    uint32_t sema_val = 1;

    while(sema_val) {
        // Attempt to lock the ARM_SEMA
        // Reading the register does an atomic test and set, returns previous value
        sema_val = MXC_SEMA->semaphores[ARM_SEMA];
    }

    // Clear the SDMA interrupt
    MXC_SDMA->irq_flag = 0x1;

    if(!arm_shared_mem.dirty) {
        hciTrSerialRxIncoming((uint8_t*)arm_shared_mem.data, arm_shared_mem.len);
        arm_shared_mem.dirty = 1;
    }

    // Unlock the ARM_SEMA
    MXC_SEMA->semaphores[ARM_SEMA] = 0x0;
    
}

/******************************************************************************/
/* Restart SDMA after sleep */
void SDMARestart(void)
{
    /* Reset SDMA */
    MXC_SDMA->ctrl &= ~(MXC_F_SDMA_CTRL_EN);

    /* Enable SDMA clock */
    MXC_GCR->perckcn1 &= ~(MXC_F_GCR_PERCKCN1_SDMAD);

    sdmaSetIRQ();

    MXC_SDMA->irq_ie = 0;
    NVIC_DisableIRQ(SDMA_IRQn);

    /* restart SDMA core */
    SDMASetARMFlag(ARM_FLAG_SLEEP);  /* SDMA re-start */
    MXC_SDMA->ip_addr = (uint32_t)&sdma_code;
    MXC_SDMA->ctrl |= MXC_F_SDMA_CTRL_EN;

    /* Wait for the SDMA to finish re-initialization */
    while(!MXC_SDMA->irq_flag) {}

    // Clear any pending SDMA interrupts
    MXC_SDMA->irq_flag = 0x1;
    NVIC_ClearPendingIRQ(SDMA_IRQn);

    // Enable interrupt from SDMA
    MXC_SDMA->irq_ie = 1;
    NVIC_EnableIRQ(SDMA_IRQn);

    // Service any pending messages from the SDMA
    SDMA0_IRQHandler();
}
