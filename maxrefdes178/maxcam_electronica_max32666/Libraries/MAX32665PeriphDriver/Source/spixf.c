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
 * $Date: 2020-05-17 01:50:01 +0300 (Sun, 17 May 2020) $
 * $Revision: 53531 $
 *
 **************************************************************************** */

/* **** Includes **** */
#include <string.h>
#include "mxc_config.h"
#include "mxc_assert.h"
#include "mxc_sys.h"
#include "spixf.h"
#include <stdio.h>

/* **** Functions **** */

/* ************************************************************************** */
void SPIXF_setup(mxc_spixf_regs_t *spixf, spixf_req_t *req)
{
    uint32_t spixf_clk, clocks;
    
    MXC_ASSERT(req != NULL);
    
    // Disable the SPIXFC before setting the SPIXF
    MXC_SPIXFC->gen_ctrl  &= ~(MXC_F_SPIXFC_GEN_CTRL_ENABLE);
    
    // Configure the mode and baud
    spixf_clk = SYS_SPIXFC_GetFreq(MXC_SPIXFC);  // Use the SPIXFC_GetFreq for SPIXF as well...
    MXC_ASSERT(spixf_clk > 0);
    
    // Make sure that we can generate this frequency
    clocks = (spixf_clk / (2 * req->baud));
    MXC_ASSERT((clocks > 0) && (clocks <= 0x10));
    
    // 0 => 16 in the 4 bit field for HI_CLK and LO_CLK
    if (clocks == 0x10) {
        clocks = 0;
    }
    
    spixf->cfg  = 0;
    
    spixf->cfg  = ((uint32_t) req->mode | (uint32_t) req->sspol |
                  (clocks << MXC_F_SPIXF_CFG_LO_CLK_POS)|
                  (clocks << MXC_F_SPIXF_CFG_HI_CLK_POS)|
                  req->ssact | req->ssiact);
                  
    spixf->fetch_ctrl =((req->cmdval << MXC_F_SPIXF_FETCH_CTRL_CMDVAL_POS) |
                       req->cmdwth | req->adrwth | req->datwth | req->addr4);
                       
    spixf->mode_ctrl  = (((req->mdclk << MXC_F_SPIXF_MODE_CTRL_MDCLK_POS) & MXC_F_SPIXF_MODE_CTRL_MDCLK) |
                       req->nocmd);
    
    spixf->mode_data = (req->mddata << MXC_F_SPIXF_MODE_DATA_DATA_POS |
                       req->mdoe << MXC_F_SPIXF_MODE_DATA_OUT_EN_POS);
                       
    //spixf->sclk_fb_ctrl = req->fbmd | req->finv;
    
    spixf->sclk_fb_ctrl = (uint32_t) req->fbmd | (uint32_t) req->finv;
    
}


/* ************************************************************************** */
void SPIXF_ioctrl(spixf_ioctl_sclk_t sclk_ds, spixf_ioctl_ss_t ss_ds,
                 spixf_ioctl_sdio_t sdio_ds, spixf_ioctl_pup_t pupdctrl)
{
    MXC_SPIXF->io_ctrl = 0;
    MXC_SPIXF->io_ctrl = ((uint32_t) sclk_ds | (uint32_t) ss_ds | (uint32_t) sdio_ds | (uint32_t) pupdctrl);
}
