/**
 * @file       sdhc.c
 * @brief      This file contains the function implementations for the
 *             Secure Digital High Capacity (SDHC) peripheral module.
 */

/* *****************************************************************************
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
 * $Date: 2017-03-01 09:46:57 -0600 (Wed, 01 Mar 2017) $
 * $Revision: 26777 $
 *
 **************************************************************************** */

/* **** Includes **** */
#include "mxc_config.h"
#include "mxc_assert.h"
#include "mxc_sys.h"
#include "tmr_utils.h"
#include "sdhc.h"
#include "sdhc_lib.h"
#include "sdhc_resp_regs.h"

/**
 * @ingroup sdhc
 * @{
 */

/* **** Definitions **** */

/* SDHC commands and associated cmd register bits which inform hardware to wait for response, etc. */
#define SDHC_LIB_CMD0       0x0000
#define SDHC_LIB_CMD1       0x0102
#define SDHC_LIB_CMD2       0x0209
#define SDHC_LIB_CMD3       0x031A
#define SDHC_LIB_CMD4       0x0400
#define SDHC_LIB_CMD5       0x051A
#define SDHC_LIB_CMD6       0x060A
#define SDHC_LIB_CMD7       0x071B
#define SDHC_LIB_CMD8       0x081A
#define SDHC_LIB_CMD9       0x0901
#define SDHC_LIB_CMD10      0x0A01
#define SDHC_LIB_CMD11      0x0B1A
#define SDHC_LIB_CMD12      0x0C1B
#define SDHC_LIB_CMD13      0x0D1A
#define SDHC_LIB_CMD16      0x101A
#define SDHC_LIB_CMD17      0x113A
#define SDHC_LIB_CMD18      0x123A
#define SDHC_LIB_CMD23      0x171A
#define SDHC_LIB_CMD24      0x183E
#define SDHC_LIB_CMD25      0x193E
#define SDHC_LIB_CMD55      0x371A
/* Application commands (SD Card) which are prefixed by CMD55 */
#define SDHC_LIB_ACMD6      0x061B
#define SDHC_LIB_ACMD41     0x2902

#define SDHC_LIB_BLOCK_SIZE  512

#define SDHC_LIB_CURRENT_STATE_POS 9
#define SDHC_LIB_CURRENT_STATE ((uint32_t)(0xFUL << SDHC_LIB_CURRENT_STATE_POS)) 

#define SDHC_LIB_CMD8_ARG1 0x157
#define SDHC_LIB_DSR 0x04040000

#define SDHC_LIB_DEFAULT_MAX_BLOCK_SIZE 512
#define SDHC_LIB_DEFAULT_MAX_BLOCK_REG_VALUE 9
//ACMD41 Command arg
#define SDHC_LIB_HOST_CAPACITY_SUPPORT_POS 30
#define SDHC_LIB_HOST_CAPACITY_SUPPORT ((uint32_t)(0x1UL << SDHC_LIB_HOST_CAPACITY_SUPPORT_POS)) 
#define SDHC_LIB_SDXC_POWER_POS 28
#define SDHC_LIB_SDXC_POWER ((uint32_t)(0x1UL << SDHC_LIB_SDXC_POWER_POS)) 

//ACMD41 Response (response reg 1)
#define SDHC_LIB_ACMD41_BUSY_POS 31
#define SDHC_LIB_ACMD41_BUSY ((uint32_t)(0x1UL << SDHC_LIB_ACMD41_BUSY_POS)) 

/* **** Globals **** */
int card_rca;
sdhc_lib_card_type card_type = CARD_NONE;

/* **** Functions **** */

sdhc_lib_card_type SDHC_Lib_Get_Card_Type(void)
{
  return card_type;
}

/* ************************************************************************** */
int SDHC_Lib_SetRCA()
{
    sdhc_cmd_cfg_t cmd_cfg;
    int result;

    if (card_type == CARD_MMC) {
        /* We assign an address to MMC cards */
        card_rca = 0x80000000;
    }
    
    cmd_cfg.direction = SDHC_DIRECTION_CFG;
    cmd_cfg.arg_1 = card_rca;
    cmd_cfg.host_control_1 = SDHC_Get_Host_Cn_1();
    cmd_cfg.command = SDHC_LIB_CMD3;
    cmd_cfg.callback = NULL;
    result = SDHC_SendCommand(&cmd_cfg);

    if ((result == E_NO_ERROR) && (card_type == CARD_SDHC)) {
        /* SD Cards tell us their address */
        card_rca = SDHC_Get_Response32() & 0xFFFF0000;
    }

    return result;
}

/* ************************************************************************** */
int SDHC_Lib_GetCSD(mxc_sdhc_csd_regs_t *csd)
{
    sdhc_cmd_cfg_t cmd_cfg;
    int result;

    cmd_cfg.direction = SDHC_DIRECTION_CFG;
    cmd_cfg.host_control_1 = SDHC_Get_Host_Cn_1();
    cmd_cfg.callback = NULL;
    cmd_cfg.arg_1 = card_rca;
    cmd_cfg.command = SDHC_LIB_CMD9;
    
    if ((result = SDHC_SendCommand(&cmd_cfg)) == E_NO_ERROR) {
    SDHC_Get_Response128((unsigned char *)csd->array);
    }
    
    return result;
}

/* ************************************************************************** */
uint64_t SDHC_Lib_GetCapacity(mxc_sdhc_csd_regs_t* csd)
{
#ifdef __ICCARM__
  unsigned int temp_c_size_0;
  
  temp_c_size_0 = csd->csd_v1.c_size_0;
  
#endif  
    if(csd->csd_v1.csd_structure == 0)
    {
        uint64_t block_len = 1 << csd->csd_v1.read_bl_len;
        uint64_t mult = 1 << (csd->csd_v1.c_size_mult + 2);
        
#ifdef __ICCARM__   // This is being done to handle the IAR Warning Pa082
        uint64_t blocknr = (((csd->csd_v1.c_size_1 << 10) | temp_c_size_0) + 1) * mult; 
#else
        uint64_t blocknr = (((csd->csd_v1.c_size_1 << 10) | csd->csd_v1.c_size_0) + 1) * mult;
#endif
        return blocknr * block_len;
    }
    else
    {
        // Get size into 64-bit variable
        uint64_t size = csd->csd_v2.c_size;
        return (size + 1) * (512 * 1024);
    }
}

/* ************************************************************************** */
int SDHC_Lib_GetBlockSize(mxc_sdhc_csd_regs_t* csd)
{
    unsigned int size = csd->csd_v1.write_bl_len;
    
    if (size == SDHC_LIB_DEFAULT_MAX_BLOCK_REG_VALUE) {
        return SDHC_LIB_DEFAULT_MAX_BLOCK_SIZE;
    } else {
      return E_BAD_PARAM;
    }
}

/* ************************************************************************** */
int SDHC_Lib_GetTaskStatusRegister(uint32_t *tsr)
{
    sdhc_cmd_cfg_t cmd_cfg;
    int result;

    cmd_cfg.direction = SDHC_DIRECTION_CFG;
    cmd_cfg.host_control_1 = SDHC_Get_Host_Cn_1();
    cmd_cfg.arg_1 = card_rca;
    cmd_cfg.command = SDHC_LIB_CMD13;
    cmd_cfg.callback = NULL;
    result = SDHC_SendCommand(&cmd_cfg);

    *tsr = SDHC_Get_Response32();
    
    return result;
}

int SDHC_Lib_GetCurrentState(sdhc_state *state)
{
    int result;
    uint32_t tsr;

    result = SDHC_Lib_GetTaskStatusRegister(&tsr);
    *state = (sdhc_state)((tsr & SDHC_LIB_CURRENT_STATE) >> SDHC_LIB_CURRENT_STATE_POS);

    return result;
}

/* ************************************************************************** */
int SDHC_Lib_SetDsr()
{
    sdhc_cmd_cfg_t cmd_cfg;
    
    cmd_cfg.direction = SDHC_DIRECTION_CFG;
    cmd_cfg.arg_1 = 0;
    cmd_cfg.host_control_1 = SDHC_Get_Host_Cn_1();
    cmd_cfg.arg_1 = SDHC_LIB_DSR;
    cmd_cfg.command = SDHC_LIB_CMD4;
    cmd_cfg.callback = NULL;
    
    return SDHC_SendCommand(&cmd_cfg);
}

/* ************************************************************************** */
int SDHC_Lib_SetBusWidth(sdhc_data_width bus_width)
{
    sdhc_cmd_cfg_t cmd_cfg;
    uint32_t card_status;
    int result;

    cmd_cfg.direction = SDHC_DIRECTION_CFG;
    cmd_cfg.callback = NULL;

    if (card_type == CARD_SDHC) {
    cmd_cfg.host_control_1 = SDHC_Get_Host_Cn_1();
    cmd_cfg.arg_1 = card_rca;
    cmd_cfg.command = SDHC_LIB_CMD55;
    result = SDHC_SendCommand(&cmd_cfg);
    
    if (result == E_NO_ERROR) {
        cmd_cfg.host_control_1 |= bus_width;
        cmd_cfg.arg_1 = bus_width;
        cmd_cfg.command = SDHC_LIB_ACMD6;
        result = SDHC_SendCommand(&cmd_cfg);
    }
    } else if (card_type == CARD_MMC) {
      cmd_cfg.host_control_1 = SDHC_Get_Host_Cn_1();
      if (bus_width == SDHC_LIB_SINGLE_DATA) {
    /* 1-bit bus */
    cmd_cfg.arg_1 = 0x03B70000;
      } else {
    /* 4-bit bus */
    cmd_cfg.arg_1 = 0x03B70100;
    cmd_cfg.host_control_1 |= MXC_F_SDHC_HOST_CN_1_DATA_TRANSFER_WIDTH;
      }
      cmd_cfg.command = SDHC_LIB_CMD6;
      result = SDHC_SendCommand(&cmd_cfg);
      /* Wait until card busy (D0 low) disappears */
      while (SDHC_Card_Busy());
      card_status = SDHC_Get_Response32();
      if ((result == E_NO_ERROR) && (card_status & 0x80)) {
    /* SWITCH_ERROR */
    result = E_BAD_STATE;
      }
    } else {
    result = E_BAD_STATE;
    }

    return result;
}

/* ************************************************************************** */
int SDHC_Lib_InitCard(int retries)
{
    sdhc_cmd_cfg_t cmd_cfg;
    int ocr = 0;
    int R3 = 0;
    int emmc = 0;
    int err;
    int cmd0 = 1;
    uint32_t response;

    card_type = CARD_NONE;

    /* SD Cards will reply to CMD8 when in Idle, while (e)MMC will not. Use that
     * to determine if we're dealing with one or the other.
     */
    SDHC_PowerUp();
    SDHC_Reset_CMD_DAT();
    
    while (retries--) {
        if (cmd0) {
            cmd_cfg.direction = SDHC_DIRECTION_CFG;
            cmd_cfg.arg_1 = 0;
            cmd_cfg.host_control_1 = SDHC_Get_Host_Cn_1();
            cmd_cfg.command = SDHC_LIB_CMD0;
            cmd_cfg.callback = NULL;
            if (SDHC_SendCommand(&cmd_cfg) != E_NO_ERROR) {
                return E_NO_DEVICE;
            }
        }
        
        if (emmc) {
            /* Try to identify an eMMC card with CMD1 */
            cmd_cfg.arg_1 = 0x00ff8000;
            cmd_cfg.command = SDHC_LIB_CMD1;
            err = SDHC_SendCommand(&cmd_cfg);
            if (err == E_NO_ERROR) {
                ocr = SDHC_Get_Response32();
                if ((ocr & 0xff8000) == 0xff8000) {
                    /* Card is present, but is it busy? */
                    if (ocr & (1UL << 31)) {
                        /* Ready! */
                        card_type = CARD_MMC;
                        break;
                    } else {
                        cmd0 = 0;
                        /* Busy, keep trying until it replies or retries are exhausted */
                    }
                } else {
                    /* Next time around, try SD card */
                    emmc = 0;
                }
            } else {
                /* Reset hardware for next probe */
                SDHC_Reset_CMD_DAT();
                /* Next time around, try SD card */
                emmc = 0;
            }
        } else {
            /* Try to identify a SD card with CMD8 */
            cmd_cfg.arg_1 = SDHC_LIB_CMD8_ARG1;
            cmd_cfg.command = SDHC_LIB_CMD8;
            err = SDHC_SendCommand(&cmd_cfg);
            if (err == E_NO_ERROR) {
                response = SDHC_Get_Response32();
                if (response == SDHC_LIB_CMD8_ARG1) {
                    card_type = CARD_SDHC;
                    break;
                } 
            } else {
                /* Reset hardware for next probe */
                SDHC_Reset_CMD_DAT();
            }
            /* Next time around, try eMMC */
            emmc = 1;
        }
    }

    if (card_type == CARD_NONE) {
        /* Nothing found */
        return E_NO_DEVICE;
    }

    if (card_type == CARD_SDHC) {
        /* SD Card */
        cmd_cfg.arg_1 = 0x0;
        cmd_cfg.command = SDHC_LIB_CMD55;
        err = SDHC_SendCommand(&cmd_cfg);
        if (err != E_NO_ERROR) {
            card_type = CARD_NONE;
            return err;
        }
        
        cmd_cfg.arg_1 = SDHC_LIB_SDXC_POWER | SDHC_LIB_HOST_CAPACITY_SUPPORT;
        cmd_cfg.command = SDHC_LIB_ACMD41;
        err= SDHC_SendCommand(&cmd_cfg);
        if (err != E_NO_ERROR) {
            card_type = CARD_NONE;
            return err;
        }
        
        ocr = SDHC_Get_Response32();
        R3 = 0;
        while (R3 == 0) {
            cmd_cfg.arg_1 = 0x0;
            cmd_cfg.command = SDHC_LIB_CMD55;
            err = SDHC_SendCommand(&cmd_cfg);
            if (err != E_NO_ERROR) {
                card_type = CARD_NONE;
                return err;
            }
            
            cmd_cfg.arg_1 = SDHC_LIB_SDXC_POWER | SDHC_LIB_HOST_CAPACITY_SUPPORT | ocr;
            cmd_cfg.command = SDHC_LIB_ACMD41;
            err = SDHC_SendCommand(&cmd_cfg);
            if (err != E_NO_ERROR) {
                card_type = CARD_NONE;
                return err;
            }
            response = SDHC_Get_Response32();
            R3 = response & SDHC_LIB_ACMD41_BUSY;
        }
    }

    /* CMD2 to get CID and CMD3 to set card address */
    cmd_cfg.arg_1 = 0x0;
    cmd_cfg.command = SDHC_LIB_CMD2;
    err = SDHC_SendCommand(&cmd_cfg);
    if (err != E_NO_ERROR) {
        return err;
    }
    ocr = SDHC_Get_Response32();
    err = SDHC_Lib_SetRCA();
        if (err != E_NO_ERROR) {
        return err;
    }

    return E_NO_ERROR;
}

/* ************************************************************************** */
void SDHC_Lib_Async_Handler()
{
    SDHC_Handler();
}

int SDHC_Lib_Prepare_Trans(sdhc_data_width width)
{
    sdhc_state state;
    sdhc_cmd_cfg_t cmd_cfg;
    int result;
    
    cmd_cfg.direction = SDHC_DIRECTION_CFG;
    cmd_cfg.arg_1 = 0;
    cmd_cfg.host_control_1 = SDHC_Get_Host_Cn_1();
    cmd_cfg.callback = NULL;

    do {
    if ((result = SDHC_Lib_GetCurrentState(&state)) != E_NO_ERROR) {
        return result;
    }

    /* We expect to be in STBY, TRANS, or PRG state, otherwise failure */
    switch (state) {
        case SDHC_LIB_STBY_STATE:
        if ((result = SDHC_Lib_SetDsr()) != E_NO_ERROR) {
            return result;
        }
        cmd_cfg.arg_1 = card_rca;
        cmd_cfg.command = SDHC_LIB_CMD7;
        if ((result = SDHC_SendCommand(&cmd_cfg)) != E_NO_ERROR) {
            return result;
        }
        break;
        case SDHC_LIB_TRAN_STATE:
        /* No action, ready to go */
        break;
        case SDHC_LIB_PRG_STATE:
        /* If card is currently in the programming state (writing data to card) with BUSY=1,
         * block until that is done. This could be optimized in the case of the Async
         * calls, but that is left as a future enhancement to the library.
         */
        if (!SDHC_Card_Busy()) {
            /* Bump us out of the loop */
            state = SDHC_LIB_TRAN_STATE;
        }
        break;
        default:
        /* Other states are errors at this point */
        return E_BAD_STATE;
    }
    } while (state != SDHC_LIB_TRAN_STATE);

    if ((result = SDHC_Lib_SetBusWidth(width)) != E_NO_ERROR) {
    return result;
    }

    return E_NO_ERROR;
}

/* ************************************************************************** */
int SDHC_Lib_Write(unsigned int dst_addr, void *src_addr, unsigned int cnt, sdhc_data_width width)
{
    int result;
    sdhc_cmd_cfg_t cmd_cfg;

    result = SDHC_Lib_Prepare_Trans(width);
    if (result != E_NO_ERROR) {
    return result;
    }
    
    cmd_cfg.host_control_1 = SDHC_Get_Host_Cn_1();
    cmd_cfg.block_size = SDHC_LIB_BLOCK_SIZE;
    cmd_cfg.sdma = (uint32_t)src_addr;
    cmd_cfg.block_count = cnt;
    cmd_cfg.dma = 1;
    cmd_cfg.direction = SDHC_DIRECTION_WRITE;
    cmd_cfg.arg_1 = dst_addr;
    if (cnt > 1) {
      cmd_cfg.command = SDHC_LIB_CMD25;
    } else {
      cmd_cfg.command = SDHC_LIB_CMD24;
    }
    cmd_cfg.callback = NULL;

    return SDHC_SendCommand(&cmd_cfg);
}

/* ************************************************************************** */
int SDHC_Lib_Read(void *dst_addr, unsigned int src_addr, unsigned int cnt, sdhc_data_width width)
{
    int result;
    sdhc_cmd_cfg_t cmd_cfg;

    result = SDHC_Lib_Prepare_Trans(width);
    if (result != E_NO_ERROR) {
    return result;
    }

    cmd_cfg.host_control_1 = SDHC_Get_Host_Cn_1();
    cmd_cfg.sdma = (uint32_t)dst_addr;
    cmd_cfg.block_size = SDHC_LIB_BLOCK_SIZE;
    cmd_cfg.block_count = cnt;
    cmd_cfg.dma = 1;
    cmd_cfg.direction = SDHC_DIRECTION_READ;
    cmd_cfg.arg_1 = src_addr;
    if (cnt > 1) {
      cmd_cfg.command = SDHC_LIB_CMD18;
    } else {
      cmd_cfg.command = SDHC_LIB_CMD17;
    }
    cmd_cfg.callback = NULL;

    return SDHC_SendCommand(&cmd_cfg);
}

/* ************************************************************************** */
int SDHC_Lib_WriteAsync(unsigned int dst_addr, void *src_addr, unsigned int cnt, sdhc_data_width width, sdhc_callback_fn callback)
{
    int data;
    sdhc_cmd_cfg_t cmd_cfg;

    data = SDHC_Lib_Prepare_Trans(width);
    if (data == E_BUSY) {
    return E_BUSY;
    }
    if (data == E_BAD_STATE) {
        return E_BAD_STATE;
    }

    cmd_cfg.host_control_1 = SDHC_Get_Host_Cn_1();
    cmd_cfg.block_size = SDHC_LIB_BLOCK_SIZE;
    cmd_cfg.sdma = (uint32_t)src_addr;
    cmd_cfg.block_count = cnt;
    cmd_cfg.dma = 1;
    cmd_cfg.direction = SDHC_DIRECTION_WRITE;
    cmd_cfg.arg_1 = dst_addr;
    if (cnt > 1) {
      cmd_cfg.command = SDHC_LIB_CMD25;
    } else {
      cmd_cfg.command = SDHC_LIB_CMD24;
    }
    cmd_cfg.callback = callback;

    return SDHC_SendCommandAsync(&cmd_cfg);
}

/* ************************************************************************** */
int SDHC_Lib_ReadAsync(void *dst_addr, unsigned int src_addr, unsigned int cnt, sdhc_data_width width, sdhc_callback_fn callback)
{
    int data;
    sdhc_cmd_cfg_t cmd_cfg;

    data = SDHC_Lib_Prepare_Trans(width);
    if(data == E_BUSY){
        return E_BUSY;
    }
    if(data == E_BAD_STATE){
        return E_BAD_STATE;
    }

    cmd_cfg.host_control_1 = SDHC_Get_Host_Cn_1();
    cmd_cfg.sdma = (uint32_t)dst_addr;
    cmd_cfg.block_size = SDHC_LIB_BLOCK_SIZE;
    cmd_cfg.block_count = cnt;
    cmd_cfg.dma = 1;
    cmd_cfg.direction = SDHC_DIRECTION_READ;
    cmd_cfg.arg_1 = src_addr;
    if (cnt > 1) {
      cmd_cfg.command = SDHC_LIB_CMD18;
    } else {
      cmd_cfg.command = SDHC_LIB_CMD17;
    }
    cmd_cfg.callback = callback;

    return SDHC_SendCommandAsync(&cmd_cfg);
}
/**@} end of group sdhc */
