/**
 * @file    spixr.h
 * @brief   Registers, Bit Masks and Bit Positions for the SPI RAM XIP Data module.
 */

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
 * $Date: 2018-09-11 00:20:17 +0300 (Tue, 11 Sep 2018) $
 * $Revision: 37771 $
 *
 *************************************************************************** */

/* Define to prevent redundant inclusion */
#ifndef _SPIXR_H_
#define _SPIXR_H_

/* **** Includes **** */
#include "spixr_regs.h"
#include "mxc_sys.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @defgroup spixr SPI External Ram (SPIXR)
 * @ingroup periphlibs
 * @{
 */

/* **** Definitions **** */
/**
 * @brief       Enum to define SPIXR data width.
 */
typedef enum {
    SPIXR_MONO = MXC_V_SPIXR_CTRL3_DATA_WIDTH_MONO,
    SPIXR_DUAL = MXC_V_SPIXR_CTRL3_DATA_WIDTH_DUAL,
    SPIXR_QUAD = MXC_V_SPIXR_CTRL3_DATA_WIDTH_QUAD
} spixr_width_t;

/**
 * @brief       Structure type for configuring a SPIXR port.
 */
typedef struct {
    uint32_t ssel;          /**< Slave Select, in Master mode selects which Slave devices are selected. More than one Slave device can be selected. */

    uint32_t numbits;       /**< Number of Bits per character. In slave mode 9-bit character length is not supported. */
    spixr_width_t data_width;    /**< SPI Data width */

    uint32_t ssel_act_1;    /**< Slave Select Action delay 1 */
    uint32_t ssel_act_2;    /**< Slave Select Action delay 2 */
    uint32_t ssel_inact;    /**< Slave Select Inactive delay */

    uint32_t baud_freq;     /**< Desired baud rate duty cycle control */
    uint32_t baud_scale;    /**< System Clock scale factor */
} spixr_cfg_t;


/* **** Function Prototypes **** */

/**
 * @brief       Initialize the SPI RAM XIP Data module.
 * @param       cfg   initialize SPIXR parameters
 * @return      #E_NO_ERROR if the SPIXR is initialized successfully,
 *              @ref MXC_Error_Codes "error" if unsuccessful.
 */
int SPIXR_Config(const spixr_cfg_t* cfg) ;

/**
 * @brief       Enable the SPI RAM XIP Data module.
 * @param       sys_cfg   System configuration object
 */
void SPIXR_Enable(const sys_cfg_spixr_t* sys_cfg);

/**
 * @brief       Send a SPI formatted instruction to external RAM
 * @param       cmd         Array of instructions to send
 * @param       length      number of bytes to send
 * @param       tx_num_char number of bytes to send
 */
void SPIXR_Send_Command(uint8_t* cmd, uint32_t length, uint32_t tx_num_char);

/**
 * @brief       Disable the SPI RAM XIP Data module.
 */
void SPIXR_Disable(void);

/**
 * @brief   SPI active status.
 *
 * In Master mode, set when transaction starts, cleared when last bit of last
 * character is acted upon and Slave Select de-assertion would occur.
 * @return  0 if inactive, 1 if active
 */
int SPIXR_Busy(void);

#ifdef __cplusplus
}
#endif
/**@} end of group spixr */

#endif /* _SPIXR_H_ */
