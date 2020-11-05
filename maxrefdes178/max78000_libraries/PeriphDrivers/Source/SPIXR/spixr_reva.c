/* ****************************************************************************
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
 *************************************************************************** */

/****** Includes *******/
#include <stddef.h>
#include <stdint.h>
#include "mxc_device.h"
#include "mxc_assert.h"
#include "mxc_lock.h"
#include "mxc_sys.h"
#include "spixr.h"
#include "spixr_regs.h"
#include "spixr_reva.h"

/****** Functions ******/

int MXC_SPIXR_RevA_ReadRXFIFO(uint8_t* buf, int len)
{
    int i;
    
    if (buf == NULL) {
        return E_NULL_PTR;
    }
    
    for (i = 0; i < len; i++) {
        buf[i] = MXC_SPIXR->data8[0];
    }
    
    return E_NO_ERROR;
}

int MXC_SPIXR_RevA_WriteTXFIFO(uint8_t* buf, int len)
{
    int i;
    
    if (buf == NULL) {
        return E_NULL_PTR;
    }
    
    for (i = 0; i < len; i++) {
        MXC_SPIXR->data8[0] = buf[i];
    }
    
    return E_NO_ERROR;
}

void MXC_SPIXR_RevA_SetSS(int ssIdx)
{
    MXC_SPIXR->ctrl1 |=
        (ssIdx << MXC_F_SPIXR_CTRL1_SS_POS);
}

int MXC_SPIXR_RevA_GetSS(void)
{
    return (MXC_SPIXR->ctrl1 & MXC_F_SPIXR_CTRL1_SS) >> MXC_F_SPIXR_CTRL1_SS_POS;
}

void MXC_SPIXR_RevA_SetSSCtrl(int stayActive)
{
    MXC_SPIXR->ctrl1 |=
        (stayActive << MXC_F_SPIXR_CTRL1_SS_CTRL_POS);
}

int MXC_SPIXR_RevA_GetSSCtrl(void)
{
    return (MXC_SPIXR->ctrl1 & MXC_F_SPIXR_CTRL1_SS_CTRL) >> MXC_F_SPIXR_CTRL1_SS_CTRL_POS;
}

void MXC_SPIXR_RevA_Enable(void)
{
    MXC_SPIXR->ctrl1 |= MXC_F_SPIXR_CTRL1_SPIEN;
    MXC_SPIXR->ctrl1 |= MXC_F_SPIXR_CTRL1_MMEN;
}

void MXC_SPIXR_RevA_Disable(void)
{
    while (MXC_SPIXR_RevA_Busy());
    
    MXC_SPIXR->ctrl1 &= ~MXC_F_SPIXR_CTRL1_SPIEN;
    MXC_SPIXR->ctrl1 &= ~(MXC_F_SPIXR_CTRL1_MMEN);
}

int MXC_SPIXR_RevA_IsEnabled(void)
{
    return !!(MXC_SPIXR->ctrl1 & MXC_F_SPIXR_CTRL1_SPIEN);
}

void MXC_SPIXR_RevA_ThreeWireModeEnable(void)
{
    MXC_SPIXR->ctrl3 |= MXC_F_SPIXR_CTRL3_THREE_WIRE;
}

void MXC_SPIXR_RevA_ThreeWireModeDisable(void)
{
    while (MXC_SPIXR_RevA_Busy());
    
    MXC_SPIXR->ctrl3 &= ~MXC_F_SPIXR_CTRL3_THREE_WIRE;
}

int MXC_SPIXR_RevA_ThreeWireModeIsEnabled(void)
{
    return !!(MXC_SPIXR->ctrl3 & MXC_F_SPIXR_CTRL3_THREE_WIRE);
}

int MXC_SPIXR_RevA_GetTXFIFOCount(void)
{
    return (MXC_SPIXR->dma & MXC_F_SPIXR_DMA_TX_FIFO_CNT) >> MXC_F_SPIXR_DMA_TX_FIFO_CNT_POS;
}

int MXC_SPIXR_RevA_GetRXFIFOCount(void)
{
    return (MXC_SPIXR->dma & MXC_F_SPIXR_DMA_RX_FIFO_CNT) >> MXC_F_SPIXR_DMA_RX_FIFO_CNT_POS;
}

int MXC_SPIXR_RevA_SetWidth(mxc_spixr_width_t width)
{
    if (width == MXC_SPIXR_INVALID) {
        return E_BAD_PARAM;
    }
    
    MXC_SETFIELD(MXC_SPIXR->ctrl3, MXC_F_SPIXR_CTRL3_DATA_WIDTH, (width << MXC_F_SPIXR_CTRL3_DATA_WIDTH_POS));
    
    return E_NO_ERROR;
}

int MXC_SPIXR_RevA_SetSPIMode(mxc_spixr_mode_t mode)
{
    if (mode <= MXC_SPIXR_MODE_3) {
        return E_BAD_PARAM;
    }
    
    if (mode == MXC_SPIXR_MODE_0 || mode == MXC_SPIXR_MODE_1) {
        MXC_SETFIELD(MXC_SPIXR->ctrl3, MXC_F_SPIXR_CTRL3_CPOL, (0 << MXC_F_SPIXR_CTRL3_CPOL_POS));
        
    }
    else {
        MXC_SETFIELD(MXC_SPIXR->ctrl3, MXC_F_SPIXR_CTRL3_CPOL, (1 << MXC_F_SPIXR_CTRL3_CPOL_POS));
        
    }
    
    if (mode == MXC_SPIXR_MODE_0 || mode == MXC_SPIXR_MODE_2) {
        MXC_SETFIELD(MXC_SPIXR->ctrl3, MXC_F_SPIXR_CTRL3_CPHA, (0 << MXC_F_SPIXR_CTRL3_CPHA_POS));
        
    }
    else {
    
        MXC_SETFIELD(MXC_SPIXR->ctrl3, MXC_F_SPIXR_CTRL3_CPHA, (1 << MXC_F_SPIXR_CTRL3_CPHA_POS));
    }
    
    return E_NO_ERROR;
}

int MXC_SPIXR_RevA_SetSSPolarity(int active)
{
    MXC_SETFIELD(MXC_SPIXR->ctrl3, MXC_F_SPIXR_CTRL3_SSPOL, (active << MXC_F_SPIXR_CTRL3_SSPOL_POS));
    return E_NO_ERROR;
}

void MXC_SPIXR_RevA_SetSSTiming(unsigned int ssIActDelay, unsigned int postActive, unsigned int preActive)
{
    MXC_ASSERT(ssIActDelay > 0xFF || postActive > 0xFF || preActive > 0xFF);
    MXC_SETFIELD(MXC_SPIXR->ss_time, MXC_F_SPIXR_SS_TIME_SSACT1, (preActive << MXC_F_SPIXR_SS_TIME_SSACT1_POS));
    MXC_SETFIELD(MXC_SPIXR->ss_time, MXC_F_SPIXR_SS_TIME_SSACT2, (postActive << MXC_F_SPIXR_SS_TIME_SSACT2_POS));
    MXC_SETFIELD(MXC_SPIXR->ss_time, MXC_F_SPIXR_SS_TIME_SSINACT, (ssIActDelay << MXC_F_SPIXR_SS_TIME_SSINACT_POS));
}

int MXC_SPIXR_RevA_SetFrequency(int hz)
{
    int freq_div, hi_clk, lo_clk, scale;
    
    // Check if frequency is too high
    if (hz > PeripheralClock) {
        return E_BAD_PARAM;
    }
    
    // Set the clock high and low
    freq_div = PeripheralClock / (hz);
    hi_clk = freq_div / 2;
    lo_clk = freq_div / 2;
    scale = 0;
    
    if (freq_div % 2) {
        hi_clk += 1;
    }
    
    while (hi_clk >= 16 && scale < 8) {
        hi_clk /= 2;
        lo_clk /= 2;
        scale ++;
    }
    
    MXC_SPIXR->brg_ctrl =
        (lo_clk << MXC_F_SPIXR_BRG_CTRL_LOW_POS) |
        (hi_clk << MXC_F_SPIXR_BRG_CTRL_HI_POS) |
        (scale << MXC_F_SPIXR_BRG_CTRL_SCALE_POS);
        
    return MXC_SPIXR_GetFrequency();
}

int MXC_SPIXR_RevA_GetFrequency(void)
{
    int spixr_periph_clock, scale, hi, lo;
    
    scale = ((MXC_SPIXR->brg_ctrl & MXC_F_SPIXR_BRG_CTRL_SCALE) >> MXC_F_SPIXR_BRG_CTRL_SCALE_POS);
    hi    = ((MXC_SPIXR->brg_ctrl & MXC_F_SPIXR_BRG_CTRL_HI)  >> MXC_F_SPIXR_BRG_CTRL_HI_POS);
    lo    = ((MXC_SPIXR->brg_ctrl & MXC_F_SPIXR_BRG_CTRL_LOW) >> MXC_F_SPIXR_BRG_CTRL_LOW_POS);
    
    spixr_periph_clock = PeripheralClock / (1 << scale);
    
    if (hi == 0) {
        hi = 1;
    }
    
    if (lo == 0) {
        lo = 1;
    }
    
    return spixr_periph_clock / (hi + lo);
}

int MXC_SPIXR_RevA_GetIntFlags(void)
{
    return MXC_SPIXR->int_fl;
}

void MXC_SPIXR_RevA_EnableInt(int flags)
{
    MXC_SETFIELD(MXC_SPIXR->int_en, flags, 1);
}

void MXC_SPIXR_RevA_DisableInt(int flags)
{
    MXC_SETFIELD(MXC_SPIXR->int_en, flags, 0);
}

int MXC_SPIXR_RevA_GetWakeUpFlags(void)
{
    return MXC_SPIXR->wake_fl;
}

void MXC_SPIXR_RevA_EnableWakeUp(int flags)
{
    MXC_SETFIELD(MXC_SPIXR->wake_en, flags, 1);
}

void MXC_SPIXR_RevA_DisableWakeUp(int flags)
{
    MXC_SETFIELD(MXC_SPIXR->wake_en, flags, 0);
}

void MXC_SPIXR_RevA_ExMemEnable(void)
{
    MXC_SETFIELD(MXC_SPIXR->xmem_ctrl, MXC_F_SPIXR_XMEM_CTRL_XMEM_EN, (1 << MXC_F_SPIXR_XMEM_CTRL_XMEM_EN_POS));
}

void MXC_SPIXR_RevA_ExMemDisable(void)
{
    MXC_SETFIELD(MXC_SPIXR->xmem_ctrl, MXC_F_SPIXR_XMEM_CTRL_XMEM_EN, (0 << MXC_F_SPIXR_XMEM_CTRL_XMEM_EN_POS));
}

void MXC_SPIXR_RevA_ExMemUseDummy(int delay255)
{
    MXC_SETFIELD(MXC_SPIXR->xmem_ctrl, MXC_F_SPIXR_XMEM_CTRL_DUMMY_CLK, (delay255 << MXC_F_SPIXR_XMEM_CTRL_DUMMY_CLK_POS));
}

void MXC_SPIXR_RevA_ExMemSetWriteCommand(uint8_t command)
{
    MXC_SETFIELD(MXC_SPIXR->xmem_ctrl, MXC_F_SPIXR_XMEM_CTRL_WR_CMD, (command << MXC_F_SPIXR_XMEM_CTRL_WR_CMD_POS));
}

uint8_t MXC_SPIXR_RevA_ExMemGetWriteCommand(void)
{
    return ((MXC_SPIXR->xmem_ctrl & MXC_F_SPIXR_XMEM_CTRL_WR_CMD) >> MXC_F_SPIXR_XMEM_CTRL_WR_CMD_POS);
}

void MXC_SPIXR_RevA_ExMemSetReadCommand(uint8_t command)
{
    MXC_SETFIELD(MXC_SPIXR->xmem_ctrl, MXC_F_SPIXR_XMEM_CTRL_RD_CMD, (command << MXC_F_SPIXR_XMEM_CTRL_RD_CMD_POS));
}

uint8_t MXC_SPIXR_RevA_ExMemGetReadCommand(void)
{
    return ((MXC_SPIXR->xmem_ctrl & MXC_F_SPIXR_XMEM_CTRL_RD_CMD) >> MXC_F_SPIXR_XMEM_CTRL_RD_CMD_POS);
}

int MXC_SPIXR_RevA_Busy(void)
{
    return (MXC_SPIXR->stat == MXC_F_SPIXR_STAT_BUSY);
}

int MXC_SPIXR_RevA_Init(mxc_spixr_cfg_t* cfg)
{
    MXC_ASSERT(cfg);
    MXC_SPIXR_Enable();
    MXC_SPIXR->ctrl1 |=
        (1 << MXC_F_SPIXR_CTRL1_SS_POS);
        
    MXC_SPIXR->ctrl3 |=
        ((cfg->numbits) << MXC_F_SPIXR_CTRL3_NUMBITS_POS) |
        ((cfg->data_width) << MXC_F_SPIXR_CTRL3_DATA_WIDTH_POS);
        
    MXC_SPIXR->ss_time =
        ((cfg->ssel_act_1) << MXC_F_SPIXR_SS_TIME_SSACT1_POS) |
        ((cfg->ssel_act_2) << MXC_F_SPIXR_SS_TIME_SSACT2_POS) |
        ((cfg->ssel_inact) << MXC_F_SPIXR_SS_TIME_SSINACT_POS);
        
    MXC_SPIXR_SetFrequency(cfg->baud_freq);
    
    return E_NO_ERROR;
}

int MXC_SPIXR_RevA_Shutdown(void)
{
    return E_NO_ERROR;
}

void MXC_SPIXR_RevA_SendCommand(uint8_t* cmd, uint32_t length, uint32_t tx_num_char)
{
    uint32_t i;
    
    MXC_SPIXR->ctrl2 =
        ((tx_num_char) << MXC_F_SPIXR_CTRL2_TX_NUM_CHAR_POS) | (MXC_SPIXR->ctrl2 &
                MXC_F_SPIXR_CTRL2_RX_NUM_CHAR);
                
    while (MXC_SPIXR_RevA_Busy());
    
    for (i = 0; i < length; i++) {
        MXC_SPIXR->data8[0] = cmd[i];
    }
    
    MXC_SPIXR->ctrl1 |= MXC_F_SPIXR_CTRL1_TX_START; /* Send command to RAM */
    
    while (!(MXC_SPIXR->dma & MXC_F_SPIXR_DMA_TX_FIFO_CNT));  /* Wait for TXFIFO cnt to reach 0*/
}

void MXC_SPIXR_RevA_TXFIFOEnable(void)
{
    MXC_SPIXR->dma |= MXC_F_SPIXR_DMA_TX_FIFO_EN;
}

void MXC_SPIXR_RevA_TXFIFODisable(void)
{
    MXC_SPIXR->dma &= ~(MXC_F_SPIXR_DMA_TX_FIFO_EN);
}

int MXC_SPIXR_RevA_TXFIFOIsEnabled(void)
{
    return !!(MXC_SPIXR->dma & MXC_F_SPIXR_DMA_TX_FIFO_EN);
}

void MXC_SPIXR_RevA_DmaTXFIFOEnable(void)
{
    MXC_SPIXR->dma |= MXC_F_SPIXR_DMA_TX_DMA_EN;
}

void MXC_SPIXR_RevA_DmaTXFIFODisable(void)
{
    MXC_SPIXR->dma &= ~(MXC_F_SPIXR_DMA_TX_DMA_EN);
}

int MXC_SPIXR_RevA_DmaTXFIFOIsEnabled(void)
{
    return !!(MXC_SPIXR->dma & MXC_F_SPIXR_DMA_TX_DMA_EN);
}

void MXC_SPIXR_RevA_RXFIFOEnable(void)
{
    MXC_SPIXR->dma |= MXC_F_SPIXR_DMA_RX_FIFO_EN;
}

void MXC_SPIXR_RevA_RXFIFODisable(void)
{
    MXC_SPIXR->dma &= ~(MXC_F_SPIXR_DMA_RX_FIFO_EN);
}

int MXC_SPIXR_RevA_RXFIFOIsEnabled(void)
{
    return !!(MXC_SPIXR->dma & MXC_F_SPIXR_DMA_RX_FIFO_EN);
}

void MXC_SPIXR_RevA_DmaRXFIFOEnable(void)
{
    MXC_SPIXR->dma |= MXC_F_SPIXR_DMA_RX_DMA_EN;
}

void MXC_SPIXR_RevA_DmaRXFIFODisable(void)
{
    MXC_SPIXR->dma &= ~(MXC_F_SPIXR_DMA_RX_DMA_EN);
}

int MXC_SPIXR_RevA_DmaRXFIFOIsEnabled(void)
{
    return !!(MXC_SPIXR->dma & MXC_F_SPIXR_DMA_RX_DMA_EN);
}

void MXC_SPIXR_RevA_TXFIFOClear(void)
{
    MXC_SPIXR->dma |= MXC_F_SPIXR_DMA_TX_FIFO_CLEAR;
}

void MXC_SPIXR_RevA_RXFIFOClear(void)
{
    MXC_SPIXR->dma |= MXC_F_SPIXR_DMA_RX_FIFO_CLEAR;
}
