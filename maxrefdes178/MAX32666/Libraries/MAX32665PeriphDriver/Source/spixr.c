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
 * $Date: 2018-08-30 19:01:36 +0300 (Thu, 30 Aug 2018) $
 * $Revision: 37545 $
 *
 **************************************************************************** */

/* **** Includes **** */
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "mxc_sys.h"
#include "mxc_assert.h"
#include "spixr.h"

/* **** Definitions **** */

/* **** Globals **** */

/* **** Local Function Prototypes **** */

/* ************************************************************************* */
int SPIXR_Config(const spixr_cfg_t* cfg)
{
    MXC_ASSERT(cfg);
    
    MXC_SPIXR->ctrl1 |=
        ((cfg->ssel) << MXC_F_SPIXR_CTRL1_SS_POS);
        
    MXC_SPIXR->ctrl3 |=
        ((cfg->numbits) << MXC_F_SPIXR_CTRL3_NUMBITS_POS) |
        ((cfg->data_width) << MXC_F_SPIXR_CTRL3_DATA_WIDTH_POS);
        
    MXC_SPIXR->ctrl4 =
        ((cfg->ssel_act_1) << MXC_F_SPIXR_CTRL4_SSACT1_POS) |
        ((cfg->ssel_act_2) << MXC_F_SPIXR_CTRL4_SSACT2_POS) |
        ((cfg->ssel_inact) << MXC_F_SPIXR_CTRL4_SSINACT_POS);
        
    MXC_SPIXR->brg_ctrl =
        ((cfg->baud_freq) << MXC_F_SPIXR_BRG_CTRL_LOW_POS) |
        ((cfg->baud_freq) << MXC_F_SPIXR_BRG_CTRL_HI_POS) |
        ((cfg->baud_scale) << MXC_F_SPIXR_BRG_CTRL_SCALE_POS);
        
    return E_NO_ERROR;
}

/* ************************************************************************* */
void SPIXR_Enable(const sys_cfg_spixr_t* sys_cfg)
{
    SYS_SPIXR_Init(sys_cfg);
    
    MXC_SPIXR->ctrl1 |= MXC_F_SPIXR_CTRL1_SPIEN;
    MXC_SPIXR->ctrl1 |= MXC_F_SPIXR_CTRL1_MMEN;
}

/* ************************************************************************* */
void SPIXR_Send_Command(uint8_t* cmd, uint32_t length, uint32_t tx_num_char)
{
    uint32_t i;
    
    MXC_SPIXR->ctrl2 =
        ((tx_num_char) << MXC_F_SPIXR_CTRL2_TX_NUM_CHAR_POS) | (MXC_SPIXR->ctrl2 &
                MXC_F_SPIXR_CTRL2_RX_NUM_CHAR);
                
    while (SPIXR_Busy());
    
    for (i=0; i<length; i++) {
        MXC_SPIXR->data8[0] = cmd[i];
    }
    
    MXC_SPIXR->ctrl1 |= MXC_F_SPIXR_CTRL1_TX_START; /* Send command to RAM */
    
    while (!(MXC_SPIXR->dma & MXC_F_SPIXR_DMA_TX_FIFO_CNT)); /* Wait for TXFIFO cnt to reach 0*/
    
}

/* ************************************************************************* */
void SPIXR_Disable(void)
{
    while (SPIXR_Busy());
    MXC_SPIXR->ctrl1 &= ~MXC_F_SPIXR_CTRL1_SPIEN;
}

/* ************************************************************************* */
int SPIXR_Busy(void)
{
    return (MXC_SPIXR->stat == MXC_F_SPIXR_STAT_BUSY);
}
