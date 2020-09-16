/**
 * @file
 * @brief      This file contains the function definitions for the
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
#include <string.h>
#include "mxc_config.h"
#include "mxc_assert.h"
#include "mxc_sys.h"
#include "sdhc.h"
#include "tmr_utils.h"
#include <stdio.h>
#include "sdhc_resp_regs.h"

/**
 * @ingroup sdhc
 * @{
 */

/* **** Definitions **** */
typedef enum {
    SDHC_LIB_SINGLE_DATA = 1,
    SDHC_LIB_QUAD_DATA,
} sdhc_data_width;

typedef enum {
    SDHC_LIB_IDLE_STATE = 0,
    SDHC_LIB_READY_STATE = 1,
    SDHC_LIB_IDENT_STATE = 2,
    SDHC_LIB_STBY_STATE = 3,
    SDHC_LIB_TRAN_STATE = 4,
    SDHC_LIB_DATA_STATE = 5,
    SDHC_LIB_RCV_STATE = 6,
    SDHC_LIB_PRG_STATE = 7,
    SDHC_LIB_DIS_STATE = 8,
} sdhc_state;

typedef enum {
    CARD_NONE = 0,
    CARD_SDHC,
    CARD_MMC
} sdhc_lib_card_type;


sdhc_lib_card_type SDHC_Lib_Get_Card_Type(void);

/* ************************************************************************** */
int SDHC_Lib_SetRCA(void);

/* ************************************************************************** */
int SDHC_Lib_GetCSD(mxc_sdhc_csd_regs_t *csd);

/* ************************************************************************** */
uint64_t SDHC_Lib_GetCapacity(mxc_sdhc_csd_regs_t* csd);

/* ************************************************************************** */
unsigned int SDHC_Lib_GetSectors(mxc_sdhc_csd_regs_t* csd);

/* ************************************************************************** */
int SDHC_Lib_GetBlockSize(mxc_sdhc_csd_regs_t* csd);

/* ************************************************************************** */
int SDHC_Lib_GetCurrentState(sdhc_state *state);

/* ************************************************************************** */
int SDHC_Lib_SetDsr(void);

/* ************************************************************************** */
int SDHC_Lib_SetBusWidth(sdhc_data_width bus_width);

/* ************************************************************************** */
int SDHC_Lib_InitCard(int retries);

/* ************************************************************************** */
void SDHC_Lib_Async_Handler(void);

/* ************************************************************************** */
int SDHC_Lib_Prepare_Trans(sdhc_data_width width);

/* ************************************************************************** */
int SDHC_Lib_Write(unsigned int dst_addr, void *src_addr, unsigned int cnt, sdhc_data_width width);

/* ************************************************************************** */
int SDHC_Lib_Read(void *dst_addr, unsigned int src_addr, unsigned int cnt, sdhc_data_width width);

/* ************************************************************************** */
int SDHC_Lib_WriteAsync(unsigned int dst_addr, void *src_addr, unsigned int cnt, sdhc_data_width width, sdhc_callback_fn callback);

/* ************************************************************************** */
int SDHC_Lib_ReadAsync(void *dst_addr, unsigned int src_addr, unsigned int cnt, sdhc_data_width width, sdhc_callback_fn callback);


/**@} end of group sdhc */
