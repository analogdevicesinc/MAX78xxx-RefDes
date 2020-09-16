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
 * $Date: 2019-10-25 17:20:09 -0500 (Fri, 25 Oct 2019) $
 * $Revision: 48104 $
 *
 **************************************************************************** */

/* **** Includes **** */
#include <string.h>
#include "mxc_device.h"
#include "mxc_assert.h"
#include "mxc_sys.h"
#include "sdhc.h"

/* **** Definitions **** */

/* **** Globals **** */
mxc_sdhc_callback_fn  sdhc_callback = NULL;

/* **** Functions **** */
static void MXC_SDHC_FreeCallback(int error);
static int MXC_SDHC_TransSetup(mxc_sdhc_cmd_cfg_t* sd_cmd_cfg);

/* ************************************************************************** */
void MXC_SDHC_RevA_Set_Clock_Config(unsigned int clk_div)
{
    MXC_SDHC->clk_cn = 0;
    /* clk_div is split across two fields in the register.  Break it up accordingly */
    MXC_SDHC->clk_cn = (clk_div & 0xff) << MXC_F_SDHC_CLK_CN_SDCLK_FREQ_SEL_POS;
    MXC_SDHC->clk_cn |= ((clk_div & 0x300) >> 8) << MXC_F_SDHC_CLK_CN_UPPER_SDCLK_FREQ_SEL_POS;
    MXC_SDHC->clk_cn |= MXC_F_SDHC_CLK_CN_INTERNAL_CLK_EN;
    
    while (!(MXC_SDHC->clk_cn & MXC_F_SDHC_CLK_CN_INTERNAL_CLK_STABLE));
    
    MXC_SDHC->clk_cn |= MXC_F_SDHC_CLK_CN_SD_CLK_EN;
}

/* ************************************************************************** */
unsigned int MXC_SDHC_RevA_Get_Clock_Config(void)
{
    /* clk_div is split across two fields in the register.  Build it up accordingly */
    return ((((MXC_SDHC->clk_cn >> MXC_F_SDHC_CLK_CN_UPPER_SDCLK_FREQ_SEL_POS) << 8) & 0x300) |
            ((MXC_SDHC->clk_cn >> MXC_F_SDHC_CLK_CN_SDCLK_FREQ_SEL_POS) & 0xff));
}

/* ************************************************************************** */
int MXC_SDHC_RevA_Init(const mxc_sdhc_cfg_t* cfg)
{
    MXC_ASSERT(cfg);
    
    if (cfg->clk_div > 0x3FF) {
        return E_BAD_PARAM;
    }
    
    MXC_SDHC_Reset();
    
    /* Turn on bus supply and enable clock */
    MXC_SDHC->pwr = (cfg->bus_voltage << MXC_F_SDHC_PWR_BUS_VOLT_SEL_POS) & MXC_F_SDHC_PWR_BUS_VOLT_SEL;
    
    MXC_SDHC->blk_gap = cfg->block_gap;
    
    MXC_SDHC->host_cn_1 = 0x00;
    
    MXC_SDHC_Set_Clock_Config(cfg->clk_div);
    
    /* Set TO to max until we know better */
    MXC_SDHC->to = MXC_F_SDHC_TO_DATA_COUNT_VALUE;
    
    /* Note: This only enables bits to show up in the int_stat register */
    /* The int_signal register is really what you want to generate interrupts out of the IP block */
    MXC_SDHC->int_en = 0xffff;
    MXC_SDHC->er_int_en = 0xffff;
    
    return E_NO_ERROR;
}

/* ************************************************************************** */
void MXC_SDHC_RevA_PowerUp(void)
{
    MXC_SDHC->pwr |= MXC_F_SDHC_PWR_BUS_POWER;
}

/* ************************************************************************** */
void MXC_SDHC_RevA_PowerDown(void)
{
    MXC_SDHC->pwr &= ~MXC_F_SDHC_PWR_BUS_POWER;
}

/* ************************************************************************** */
int MXC_SDHC_RevA_Shutdown(void)
{
    /* Disable and clear interrupts */
    MXC_SDHC->int_en = 0;
    MXC_SDHC->er_int_en = 0;
    MXC_SDHC->int_stat = MXC_SDHC->int_stat;
    MXC_SDHC->er_int_stat = MXC_SDHC->er_int_stat;
    
    if (sdhc_callback != NULL) {
        MXC_SDHC_FreeCallback(E_SHUTDOWN);
    }
    
    return E_NO_ERROR;
}

/* ************************************************************************** */
static int  MXC_SDHC_TransSetup(mxc_sdhc_cmd_cfg_t* sd_cmd_cfg)
{
    if (!MXC_SDHC_Card_Inserted()) {
        return E_NO_DEVICE;
    }
    
    if (MXC_SDHC->present & MXC_F_SDHC_PRESENT_CMD) {
        /* Command already in progress */
        return E_BAD_STATE;
    }
    
    MXC_SDHC->clk_cn |= MXC_F_SDHC_CLK_CN_SD_CLK_EN;
    
    MXC_SDHC->arg_1 = sd_cmd_cfg->arg_1;
    
    uint32_t hc1 = sd_cmd_cfg->host_control_1;
    
    if (sd_cmd_cfg->direction == MXC_SDHC_DIRECTION_WRITE || sd_cmd_cfg->direction == MXC_SDHC_DIRECTION_READ) {
        hc1 &= ~(MXC_F_SDHC_HOST_CN_1_DMA_SELECT |
                 MXC_F_SDHC_HOST_CN_1_CARD_DETECT_SIGNAL);
    }
    
    MXC_SDHC->host_cn_1 = hc1;
    
    /* Clear all flags */
    MXC_SDHC->int_stat = MXC_SDHC->int_stat;
    MXC_SDHC->er_int_stat = MXC_SDHC->er_int_stat;
    
    /* Set up Transfer registers */
    if (sd_cmd_cfg->direction != MXC_SDHC_DIRECTION_CFG) {
        MXC_SDHC->trans = 0;
        MXC_SDHC->sdma = sd_cmd_cfg->sdma;
        
        if (sd_cmd_cfg->dma) {
            MXC_SDHC->trans |= MXC_F_SDHC_TRANS_DMA_EN;
        }
        
        if (sd_cmd_cfg->direction == MXC_SDHC_DIRECTION_WRITE) {
            MXC_SDHC->trans &= ~(MXC_F_SDHC_TRANS_READ_WRITE);
        }
        else {
            MXC_SDHC->trans |= MXC_F_SDHC_TRANS_READ_WRITE;
        }
        
        MXC_SDHC->blk_size =  MXC_F_SDHC_BLK_SIZE_HOST_BUFF | ((sd_cmd_cfg->block_size << MXC_F_SDHC_BLK_SIZE_TRANS_POS) & MXC_F_SDHC_BLK_SIZE_TRANS);
        
        /* Determine transfer size and options */
        if (sd_cmd_cfg->block_count > 1) {
            /* Enable multi-block transfers, enable block count register, and automatically issue CMD12 to stop transfer */
            MXC_SDHC->trans |= (MXC_F_SDHC_TRANS_MULTI |
                                MXC_F_SDHC_TRANS_BLK_CNT_EN |
                                MXC_S_SDHC_TRANS_AUTO_CMD_EN_CMD12);
            MXC_SDHC->blk_cnt = sd_cmd_cfg->block_count;
        }
        
    }
    else {
        MXC_SDHC->trans = 0;
        MXC_SDHC->sdma = 0;
    }
    
    return E_NO_ERROR;
}

/* ************************************************************************** */
int MXC_SDHC_RevA_SendCommand(mxc_sdhc_cmd_cfg_t* sd_cmd_cfg)
{
    int err;
    
    if ((err = MXC_SDHC_TransSetup(sd_cmd_cfg)) != E_NO_ERROR) {
        return err;
    }
    
    /* Start transfer */
    MXC_SDHC->cmd = sd_cmd_cfg->command;
    
    /* Block on completion */
    if (sd_cmd_cfg->direction == MXC_SDHC_DIRECTION_CFG) {
        /* No data transfer, just command */
        while (!(MXC_SDHC->int_stat & MXC_F_SDHC_INT_STAT_CMD_COMP) &&
                !(MXC_SDHC->int_stat & MXC_F_SDHC_INT_STAT_ERR_INTR));
    }
    else {
        while (!(MXC_SDHC->int_stat & MXC_F_SDHC_INT_STAT_TRANS_COMP) &&
                !(MXC_SDHC->int_stat & MXC_F_SDHC_INT_STAT_ERR_INTR));
    }
    
    /* Determine if transfer was successful or not */
    if (MXC_SDHC->int_stat & MXC_F_SDHC_INT_STAT_ERR_INTR) {
        if (MXC_SDHC->er_int_stat & (MXC_F_SDHC_ER_INT_STAT_CMD_TO |
                                     MXC_F_SDHC_ER_INT_STAT_DATA_TO)) {
            return E_TIME_OUT;
        }
        else {
            return E_COMM_ERR;
        }
    }
    else {
        return E_NO_ERROR;
    }
}

/* ************************************************************************** */
int MXC_SDHC_RevA_SendCommandAsync(mxc_sdhc_cmd_cfg_t* sd_cmd_cfg)
{
    int err;
    
    if ((err = MXC_SDHC_TransSetup(sd_cmd_cfg)) != E_NO_ERROR) {
        return err;
    }
    
    sdhc_callback = sd_cmd_cfg->callback;
    
    if (sd_cmd_cfg->direction == MXC_SDHC_DIRECTION_CFG) {
        MXC_SDHC->int_signal = MXC_F_SDHC_INT_SIGNAL_CMD_COMP;
    }
    else {
        MXC_SDHC->int_signal = MXC_F_SDHC_INT_SIGNAL_TRANS_COMP;
    }
    
    /* Start transfer */
    MXC_SDHC->cmd = sd_cmd_cfg->command;
    
    return E_NO_ERROR;
}

/* ************************************************************************** */
void MXC_SDHC_RevA_Handler(void)
{
    int signal = MXC_SDHC->int_signal;
    int flag = MXC_SDHC_GetFlags() & signal;
    
    // Need to check if there is anything to do in case this function is called
    //  in a polling fashion instead of from the interrupt handler.
    if (!flag) {
        return;
    }
    
    // Command complete interrupt
    if ((signal & MXC_F_SDHC_INT_SIGNAL_CMD_COMP) && (flag & MXC_F_SDHC_INT_STAT_CMD_COMP)) {
        MXC_SDHC_ClearFlags(MXC_F_SDHC_INT_STAT_CMD_COMP);
        MXC_SDHC->int_signal &= ~MXC_F_SDHC_INT_SIGNAL_CMD_COMP;
        MXC_SDHC_FreeCallback(E_NO_ERROR);
        return;
    }
    
    // Transfer complete interrupt
    if ((signal & MXC_F_SDHC_INT_SIGNAL_TRANS_COMP) && (flag & MXC_F_SDHC_INT_STAT_TRANS_COMP)) {
        MXC_SDHC_ClearFlags(MXC_F_SDHC_INT_STAT_TRANS_COMP);
        MXC_SDHC->int_signal &= ~MXC_F_SDHC_INT_SIGNAL_TRANS_COMP;
        MXC_SDHC_FreeCallback(E_NO_ERROR);
        return;
    }
    
    MXC_SDHC_ClearFlags(flag);
    MXC_SDHC->int_signal = 0;
    MXC_SDHC_FreeCallback(E_UNKNOWN);
}

/* ************************************************************************** */
void MXC_SDHC_RevA_ClearFlags(uint32_t mask)
{
    MXC_SDHC->int_stat = mask;
}

/* ************************************************************************** */
unsigned MXC_SDHC_RevA_GetFlags(void)
{
    return MXC_SDHC->int_stat;
}

/* ************************************************************************** */
int MXC_SDHC_RevA_Card_Inserted(void)
{
    unsigned int detect, inserted, stable;
    
    detect = !!(MXC_SDHC->present & MXC_F_SDHC_PRESENT_CARD_DETECT);
    inserted = !!(MXC_SDHC->present & MXC_F_SDHC_PRESENT_CARD_INSERTED);
    stable = !!(MXC_SDHC->present & MXC_F_SDHC_PRESENT_CARD_STATE);
    
    return (detect & inserted & stable);
}

/* ************************************************************************** */
void MXC_SDHC_RevA_Reset(void)
{
    MXC_SDHC->sw_reset = MXC_F_SDHC_SW_RESET_RESET_ALL;
    
    /* Reset takes non-zero time, so wait for completion */
    while (MXC_SDHC->sw_reset & MXC_F_SDHC_SW_RESET_RESET_ALL);
}

/* ************************************************************************** */
void MXC_SDHC_RevA_Reset_CMD_DAT(void)
{
    MXC_SDHC->sw_reset = MXC_F_SDHC_SW_RESET_RESET_CMD | MXC_F_SDHC_SW_RESET_RESET_DAT;
    
    /* Reset takes non-zero time, so wait for completion */
    while (MXC_SDHC->sw_reset & (MXC_F_SDHC_SW_RESET_RESET_CMD | MXC_F_SDHC_SW_RESET_RESET_DAT));
}

/* ************************************************************************** */
int MXC_SDHC_RevA_Card_Busy(void)
{
    /* Response type 1b uses the DAT[0] line low to indicate busy */
    return (!((MXC_SDHC->present >> MXC_F_SDHC_PRESENT_DAT_SIGNAL_LEVEL_POS) & 1));
}

/* ************************************************************************** */
unsigned int MXC_SDHC_RevA_Get_Host_Cn_1(void)
{
    return MXC_SDHC->host_cn_1;
}

/* ************************************************************************** */
uint32_t MXC_SDHC_RevA_Get_Response32(void)
{
    return MXC_SDHC->resp[0];
}

/* ************************************************************************** */
uint32_t MXC_SDHC_RevA_Get_Response32_Auto(void)
{
    /* The response for auto commands get set at idx 3 */
    return MXC_SDHC->resp[3];
}

/* ************************************************************************** */
void MXC_SDHC_RevA_Get_Response128(unsigned char* response)
{
    uint32_t* p = (uint32_t*) response;
    
    *p++ = MXC_SDHC->resp[0];
    *p++ = MXC_SDHC->resp[1];
    *p++ = MXC_SDHC->resp[2];
    *p = MXC_SDHC->resp[3];
}


/* ************************************************************************** */
static void MXC_SDHC_FreeCallback(int error)
{
    /* Save the request so the callback can be NULLed out and still be called. */
    mxc_sdhc_callback_fn temp_callback = sdhc_callback;
    
    sdhc_callback = NULL;
    
    temp_callback(error);
    
}
