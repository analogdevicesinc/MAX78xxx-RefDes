/**
 * @file trng.c
 * @brief      This file contains the function implementations for the
 *             True Random Number Generator(TRNG) peripheral module.
 */

/* *********************************************************************************************************
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
 * $Date: 2018-08-29 01:03:02 +0300 (Wed, 29 Aug 2018) $
 * $Revision: 37424 $
 *
 ********************************************************************************************************** */

/* **** Includes **** */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "mxc_config.h"
#include "mxc_errors.h"
#include "mxc_sys.h"
#include "trng_regs.h"
#include "trng.h"

/* **** MACROS **** */
#define WORD 3
#define BYTE 4

/* **** Functions **** */
static int TRNG_Read_Status(mxc_trng_regs_t *trng);

//***************************************************************
int TRNG_Init(const sys_cfg_t* sys_cfg)
{
    return SYS_TRNG_Init(sys_cfg);
}

//***************************************************************
int TRNG_Shutdown()
{
    return SYS_TRNG_Shutdown();
}

//***************************************************************
static int TRNG_Read_Status(mxc_trng_regs_t *trng)
{
    //  Check if a random number is ready to be read
    if (trng->st & MXC_F_TRNG_ST_RND_RDY) {
        return E_NO_ERROR;
    } else {
        //  Return bad state if TRNG is disabled
        return E_BAD_STATE;
    }
}

//***************************************************************
uint8_t TRNG_Read8BIT(mxc_trng_regs_t *trng)
{
    //  Wait for TRNG to be ready
    while (TRNG_Read_Status(trng) != E_NO_ERROR);
    
    return ((uint8_t)(trng->data & 0xFF));
}

//**************************************************************
uint16_t TRNG_Read16BIT(mxc_trng_regs_t *trng)
{
    //  Wait for TRNG to be ready
    while (TRNG_Read_Status(trng) != E_NO_ERROR);
    
    return ((uint16_t)(trng->data & 0xFFFF));
}

//**************************************************************
uint32_t TRNG_Read32BIT(mxc_trng_regs_t *trng)
{
    //   Wait for TRNG to be ready
    while (TRNG_Read_Status(trng) != E_NO_ERROR);
    
    return ((uint32_t)trng->data);
}

//**************************************************************
void TRNG_Read(mxc_trng_regs_t *trng, uint8_t *data, int len)
{
    int i = 0;
    uint32_t temp;
    
    while (len >= 4) {
        temp = TRNG_Read32BIT(trng);
        memcpy(&data[i], &temp, 4);
        i += 4;
        len -= 4;
    }
    while (len) {
        data[i++] = TRNG_Read8BIT(trng);
        len--;
    }
}

//**************************************************************
void TRNG_Generate_AES(mxc_trng_regs_t *trng)
{
    // Start key generation
    trng->cn |= MXC_F_TRNG_CN_AESKG_MEU;
    // Field will be auto-set to 0 while key generation is in progress.  Wait for it to complete.
    while ((trng->st & MXC_F_TRNG_ST_AESKGD_MEU_S) == 0);
}

