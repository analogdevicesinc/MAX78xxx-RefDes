/**
 * @file  spixf.h
 * @brief SPI-XIP function declarations.
 */
 
/* *****************************************************************************
 * Copyright (C) 2015 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2018-10-31 18:32:51 +0300 (Wed, 31 Oct 2018) $
 * $Revision: 38826 $
 *
 **************************************************************************** */


#include "mxc_config.h"
#include "mxc_sys.h"
#include "spixf_regs.h"

#ifndef _SPIXF_H_
#define _SPIXF_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup spixf SPI External Flash (SPIXF)
 * @ingroup periphlibs
 * @{
 */

/***** Definitions *****/

/**
 * @brief SPIXF_CFG
 */
 
  /// @brief SPI Mode 0 & 3 selected
typedef enum {
	SPI_MODE_0 = MXC_S_SPIXF_CFG_MODE_SCLK_HI_SAMPLE_RISING,
	SPI_MODE_3 = MXC_S_SPIXF_CFG_MODE_SCLK_LO_SAMPLE_FAILLING,
} spixf_cfg_mode_t;

  /// @brief Polarity of slave select
typedef enum {
	SSPOL_ACTIVE_HI = 0,
	SSPOL_ACTIVE_LO = MXC_F_SPIXF_CFG_SSPOL,
} spixf_cfg_sspol_t;

  /// @brief Slave select active timing
typedef enum {
	SYS_CLOCKS_0 = MXC_S_SPIXF_CFG_SSACT_OFF,
	SYS_CLOCKS_2 = MXC_S_SPIXF_CFG_SSACT_FOR_2_MOD_CLK,
	SYS_CLOCKS_4 = MXC_S_SPIXF_CFG_SSACT_FOR_4_MOD_CLK,
	SYS_CLOCKS_8 = MXC_S_SPIXF_CFG_SSACT_FOR_8_MOD_CLK,
} spixf_cfg_ssact_t;

  /// @brief Slave select Inactive timing
typedef enum {
	SYS_CLOCKS_1 = MXC_S_SPIXF_CFG_SSIACT_FOR_1_MOD_CLK,  /**< 1 system clocks */
	SYS_CLOCKS_3 = MXC_S_SPIXF_CFG_SSIACT_FOR_3_MOD_CLK,  /**< 3 system clocks */ 
	SYS_CLOCKS_5 = MXC_S_SPIXF_CFG_SSIACT_FOR_5_MOD_CLK,  /**< 5 system clocks */
	SYS_CLOCKS_9 = MXC_S_SPIXF_CFG_SSIACT_FOR_9_MOD_CLK,  /**< 9 system clocks */
} spixf_cfg_ssiact_t;

/**
 * SPIXF_FETCH_CTRL
 */
 
  /// @brief Command Width, # of data I/O used to send commands
typedef enum { 
	SINGLE_SDIO_CMD = MXC_S_SPIXF_FETCH_CTRL_CMD_WIDTH_SINGLE,
	DUAL_SDIO_CMD   = MXC_S_SPIXF_FETCH_CTRL_CMD_WIDTH_DUAL_IO,
	QUAD_SDIO_CMD   = MXC_S_SPIXF_FETCH_CTRL_CMD_WIDTH_QUAD_IO,
	INVALID_CMD     = MXC_S_SPIXF_FETCH_CTRL_CMD_WIDTH_INVALID,
} spixf_fctl_cmdwth_t;

  /// @brief Addr Width, # of data I/O used to send addr and mode/dummy clks
typedef enum {
	SINGLE_SDIO_ADDR = MXC_S_SPIXF_FETCH_CTRL_ADDR_WIDTH_SINGLE,
	DUAL_SDIO_ADDR   = MXC_S_SPIXF_FETCH_CTRL_ADDR_WIDTH_DUAL_IO,
	QUAD_SDIO_ADDR   = MXC_S_SPIXF_FETCH_CTRL_ADDR_WIDTH_QUAD_IO,
	INVALID_ADDR     = MXC_S_SPIXF_FETCH_CTRL_ADDR_WIDTH_INVALID,
} spixf_fctl_adrwth_t;

  /// @brief Data Width, # of data I/O used to rcv data
typedef enum {
	SINGLE_SDIO_DATA = MXC_S_SPIXF_FETCH_CTRL_DATA_WIDTH_SINGLE,
	DUAL_SDIO_DATA   = MXC_S_SPIXF_FETCH_CTRL_DATA_WIDTH_DUAL_IO,
	QUAD_SDIO_DATA   = MXC_S_SPIXF_FETCH_CTRL_DATA_WIDTH_QUAD_IO,
	INVALID_DATA     = MXC_S_SPIXF_FETCH_CTRL_DATA_WIDTH_INVALID,	
} spixf_fctl_datwth_t;

  /// @brief 3 or 4 byte address mode flash
typedef enum {
	THREE_BYTE_ADDR_MODE = 0,
	FOUR_BYTE_ADDR_MODE  = MXC_F_SPIXF_FETCH_CTRL_FOUR_BYTE_ADDR,
} spixf_fctl_addr4_t;

/**
 * SPIXF_MODE_CTRL
 */
 
  /// @brief No command mode 
typedef enum {
	SND_RDCMD_EVRYTIME = 0,
	SND_RDCMD_ONLYONCE = MXC_F_SPIXF_MODE_CTRL_NO_CMD,
} spixf_mctl_nocmd_t;

/**
 * SPIXF_FB_CTRL
 */

  /// @brief SCLK Feedback enable/disable 
typedef enum {
	DISABLE_SCLK_FB = 0,
	ENABLE_SCLK_FB = MXC_F_SPIXF_SCLK_FB_CTRL_FB_EN,
} spixf_fctrl_fbmd_t;

  /// @brief Feedback clock invert
typedef enum {
	DISABLE_FBCLK_INV = 0,
	ENABLE_FBCLK_INV = MXC_F_SPIXF_SCLK_FB_CTRL_INVERT_EN,
} spixf_fctrl_finv_t;

/**
 * SPIXF_IOCTRL
 */

  /// @brief SCLK Drive Strength
typedef enum {
	LO_DRIVE_STRENGTH_SCLK = 0,
	HI_DRIVE_STRENGTH_SCLK = MXC_F_SPIXF_IO_CTRL_SCLK_DS,
} spixf_ioctl_sclk_t;

  /// @brief Slave Sel. Drive Strength
typedef enum {
	LO_DRIVE_STRENGTH_SS = 0,
	HI_DRIVE_STRENGTH_SS = MXC_F_SPIXF_IO_CTRL_SS_DS,
} spixf_ioctl_ss_t;

  /// @brief SDIO Drive Strength
typedef enum {
	LO_DRIVE_STRENGTH_SDIO = 0,
	HI_DRIVE_STRENGTH_SDIO = MXC_F_SPIXF_IO_CTRL_SDIO_DS,
} spixf_ioctl_sdio_t;

  /// @brief IO pullup/pulldown Control
typedef enum {
	TRISTATE  = MXC_S_SPIXF_IO_CTRL_PU_PD_CTRL_TRI_STATE,
	PULL_UP   = MXC_S_SPIXF_IO_CTRL_PU_PD_CTRL_PULL_UP,
	PULL_DOWN = MXC_S_SPIXF_IO_CTRL_PU_PD_CTRL_PULL_DOWN,
} spixf_ioctl_pup_t;

  
  /// @brief SPIXF Transaction request
typedef struct {  
    spixf_cfg_mode_t   mode;    //cfg
    spixf_cfg_sspol_t  sspol;
	uint32_t          baud;
    spixf_cfg_ssact_t  ssact;
	spixf_cfg_ssiact_t ssiact;
	
	uint8_t            cmdval;  //fetch_cntl
	spixf_fctl_cmdwth_t cmdwth;
	spixf_fctl_adrwth_t adrwth;
	spixf_fctl_datwth_t datwth;
	spixf_fctl_addr4_t  addr4;
	
	uint8_t           mdclk;   //mode_ctrl
	spixf_mctl_nocmd_t nocmd;
	
	uint16_t          mddata;  //mod_data   
	uint16_t          mdoe;
	
	spixf_fctrl_fbmd_t fbmd;    //fb_ctrl
	spixf_fctrl_finv_t finv;   
	
	uint32_t          busidle;
} spixf_req_t;



/***** Globals *****/

/***** Function Prototypes *****/

/**
 * @brief   Setup SPIXF for Execute in Place
 * @param   spixf    Pointer to SPIXF regs.
 * @param   req     Request for a SPIXF transaction.
 */
 
void SPIXF_setup(mxc_spixf_regs_t *spixf, spixf_req_t *req);


/**
 * @brief   Setup Drive Strength on the I/O pins
 * @param   sclk_ds  SCLK drive strength
 * @param   ss_ds    Slave Select Drive Strength
 * @param   sdio_ds  SDIO Drive Strength
 * @param   pupdctrl IO Pullup/Pulldown Control
 */
void SPIXF_ioctrl(spixf_ioctl_sclk_t sclk_ds, spixf_ioctl_ss_t ss_ds, 
                 spixf_ioctl_sdio_t sdio_ds, spixf_ioctl_pup_t pupdctrl);

#ifdef __cplusplus
}
#endif
/**@} end of group spixf */

#endif /* _SPIXF_H_ */
