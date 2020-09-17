/**
 * @file    sdhc.h
 * @brief   Secure Digital High Capacity (SDHC) function prototypes and data types.
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
 * $Date: 2019-02-28 22:54:56 +0300 (Thu, 28 Feb 2019) $
 * $Revision: 41324 $
 *
 *************************************************************************** */

/* Define to prevent redundant inclusion */
#ifndef _SDHC_H_
#define _SDHC_H_

/* **** Includes **** */
#include "mxc_config.h"
#include "sdhc_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup sdhc Secure Digital High Capacity (SDHC)
 * @ingroup periphlibs
 * @{
 */

/* **** Definitions **** */
#define SDHC_Bus_Voltage_1_8       5
#define SDHC_Bus_Voltage_3_0       6
#define SDHC_Bus_Voltage_3_3       7

#define SDHC_DIRECTION_CFG         0
#define SDHC_DIRECTION_READ        1
#define SDHC_DIRECTION_WRITE       2

typedef struct {
    unsigned int bus_voltage;   /**< Use constants above for 1.8V, 3.0V, 3.3V. */
    unsigned int block_gap;     /**< Set block gap register default is 0       */
    unsigned int clk_div;       /**< Divider for SD clock                      */
} sdhc_cfg_t ;

/**
 * @brief Callback function type used in asynchronous SDHC communications requests.
 * @details The function declaration for the SDHC callback is:
 * @code 
 * void callback(int error_code);
 * @endcode
 * |        |                                            |
 * | -----: | :----------------------------------------- |
 * | \p error_code | An error code if the active transaction had a failure or   #E_NO_ERROR if successful. |
 * @addtogroup sdhc_async
 */
typedef void (*sdhc_callback_fn)( int error_code);

typedef struct {
    uint32_t            sdma;           /**< SDMA register for read or write transaction                                                            */
    uint32_t            block_size;     /**< Size of transfer block in bytes                                                                        */
    uint32_t            block_count;    /**< Number of blocks to transfer                                                                           */
    uint32_t            arg_1;          /**< Argument 1 holds the arguments for the commands sent to the card                                       */
    unsigned int        dma;            /**< DMA enable bit                                                                                         */
    unsigned int        direction;      /**< Direction of transfer                                                                                  */
    uint32_t            command;        /**< Command to be issued on bus (CMD0, CMD1, ...)                                                          */
    uint32_t            host_control_1; /**< Host control register 1 to be assigned before command is issued                                        */
    sdhc_callback_fn    callback;       /**< Function pointer to completion callback function, NULL if not desired                                  */
} sdhc_cmd_cfg_t;

/* **** Function Prototypes **** */

/**
 * @brief   Initialize and enable SDHC module.
 * @param   cfg         Pointer to SDHC configuration.
 * @param   sys_cfg     Pointer to system configuration object
 * 
 * @return     #E_NO_ERROR If everything is successful. 
 * @return     @ref MXC_Error_Codes If function is unsuccessful.
 */
int SDHC_Init(const sdhc_cfg_t *cfg, const sys_cfg_sdhc_t *sys_cfg);

/**
 * @brief   Shutdown SDHC module.
 * @return     #E_NO_ERROR If everything is successful. 
 * @return     @ref MXC_Error_Codes If function is unsuccessful.
 */
int SDHC_Shutdown(void);

/**
 * @brief      Enable SDHC Bus Power
 */
void SDHC_PowerUp(void);

/**
 * @brief      Disable SDHC Bus Power 
 */
void SDHC_PowerDown(void);

/**
 * @brief   Set clock divider
 * @param   clk_div    Divider setting
 * s
 * @return     #E_NO_ERROR If everything is successful. 
 * @return     @ref MXC_Error_Codes If function is unsuccessful.
 */
void SDHC_Set_Clock_Config(unsigned int clk_div);

/**
 * @brief   Get clock divider
 * @return  Clock divider setting
 * s
 * @return     #E_NO_ERROR If everything is successful. 
 * @return     @ref MXC_Error_Codes If function is unsuccessful.
 */
unsigned int SDHC_Get_Clock_Config(void);
  
/**
 * @brief      Send Command, <em>blocking</em>.
 *
 * @param      sd_cmd_cfg  The sd command configuration
 *
 * @return     #E_NO_ERROR If everything is successful. 
 * @return     @ref MXC_Error_Codes If function is unsuccessful.
 */
int SDHC_SendCommand(sdhc_cmd_cfg_t* sd_cmd_cfg);
/**
 * @brief      Send SDHC command <em>Async</em>.
 *
 * @param      sd_cmd_cfg  The sd command configuration
 *
 * @return     #E_NO_ERROR If everything is successful. 
 * @return     @ref MXC_Error_Codes If function is unsuccessful.
 */
int SDHC_SendCommandAsync(sdhc_cmd_cfg_t* sd_cmd_cfg);

/**
 * @brief      SDHC interrupt handler.
 * @details    This function should be called by the application from the
 *             interrupt handler if SDHC interrupts are enabled. Alternately,
 *             this function can be periodically called by the application if
 *             SDHC interrupts are disabled. Only necessary to call this when
 *             using asynchronous functions.
 *
 * @addtogroup sdhc_async
 */
void SDHC_Handler(void);

/**
 * @brief      See if card is inserted
 *
 * @return     1 is card inserted, 0 is card not inserted
 */
int SDHC_Card_Inserted(void);

/**
 * @brief      Clear interrupt flags.
 *
 * @param      mask  Mask of the SDHC interrupts to clear, see    SDHC_INTFL Register.
 */
void SDHC_ClearFlags(uint32_t mask);

/**
 * @brief      Get interrupt flags.
 *
 *
 * @return     Mask of active flags.
 */
unsigned SDHC_GetFlags(void);

/**
 * @brief      Resets SDHC back to default settings
 *
 */
void SDHC_Reset(void);

/**
 * @brief      Abort any command or data transaction in progress on controller
 *
 */
void SDHC_Reset_CMD_DAT(void);

/**
 * @brief      Check if Card is busy
 *
 * @return     1 card busy , 0 card is not busy
 */
int SDHC_Card_Busy(void);

/**
 * @brief      Retrieve host control 1 register
 *
 * @return     host control register
 */
unsigned int SDHC_Get_Host_Cn_1(void);
  
/**
 * @brief      Read a 32-bit command response
 * @details    This function may be used to read response 
 *             types R1, R1a, R3, R4, R5, R5b, and R6
 * @return     Command response
 */
uint32_t SDHC_Get_Response32(void);

/**
 * @brief      Read a 32-bit command response for Auto commands
 * @details    This function may be used to read response 
 *             types R1b and R1 after an Auto CMD12 or Auto CMD23
 * @return     Command response
 */
uint32_t SDHC_Get_Response32_Auto(void);

/**
 * @brief      Read a 128-bit command response
 * @param      response  Pointer to storage for response (16 bytes)
 * @details    This function may be used to read response 
 *             type R2 (CID or CSD)
 */
void SDHC_Get_Response128(unsigned char *response);

/**@} end of group sdhc */

#ifdef __cplusplus
}
#endif

#endif /* _SDHC_H_ */
