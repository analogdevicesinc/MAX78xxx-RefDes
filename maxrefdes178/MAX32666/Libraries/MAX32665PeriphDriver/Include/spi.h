
/**
 * @file    spi.h
 * @brief   Serial Peripheral Interface function prototypes and data types.
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
 * $Date: 2018-11-05 22:40:08 +0300 (Mon, 05 Nov 2018) $
 * $Revision: 38949 $
 *
 *************************************************************************** */
#ifndef _SPI_H_
#define _SPI_H_

/* **** Includes **** */
#include "spi17y_regs.h"
#include "mxc_sys.h"
#include "spi17y.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup spi SPI
 * @ingroup periphlibs
 * @{
 */

/* **** Definitions **** */

/**
 *   Assigning SPI0 instances
 */
typedef enum {
    SPI0,   // SPI17Y instance 0,
    SPI1,   // SPI17Y instance 1
    SPI2,   // SPI17Y instance 2
}spi_type;

#define MXC_SPI0 MXC_SPI17Y0   // SPI17Y instance 0,
#define MXC_SPI1 MXC_SPI17Y1   // SPI17Y instance 1
#define MXC_SPI2 MXC_SPI17Y2   // SPI17Y instance 2

/**
 *  Renaming Interrupt SPI Interrupt sources
 */
#define SPI0_IRQn  SPI17Y0_IRQn
#define SPI1_IRQn  SPI17Y1_IRQn
#define SPI2_IRQn  SPI17Y2_IRQn


/**
 * Renaming SPI Width
 */
#define SPI_WIDTH_1  SPI17Y_WIDTH_1  /**< 1 Data Line.                        */
#define SPI_WIDTH_2  SPI17Y_WIDTH_2  /**< 2 Data Lines (x2).                  */
#define SPI_WIDTH_4  SPI17Y_WIDTH_4  /**< 4 Data Lines (x4).                  */

/**
 * Renaming SPI Polarity
 */
#define SPI_POL_LOW   SPI17Y_POL_LOW    /**< Slave Select polarity Low.       */
#define SPI_POL_HIGH  SPI17Y_POL_HIGH   /**< Slave Select polarity High.      */

typedef sys_cfg_spi17y_t sys_cfg_spi_t; 


/**
 * Union consolidating SPI registers
 */
typedef union {
	mxc_spi17y_regs_t spi17y_regs;
}spi_regs_t;


/**
 * Structure type representing a SPI Master Transaction request.
 */
typedef struct spi_req spi_req_t;

/**
 * @brief Callback function type used in asynchronous SPI Master communication requests.
 * @details The function declaration for the SPI Master callback is:
 * @code
 * void callback(spi_req_t * req, int error_code);
 * @endcode
 * |        |                                            |
 * | -----: | :----------------------------------------- |
 * | \p req |  Pointer to a #spi_req object representing the active SPI Master active transaction. |
 * | \p error_code | An error code if the active transaction had a failure or #E_NO_ERROR if successful. |
 * @note Callback will execute in interrupt context
 * @addtogroup spi_async
 */
typedef void (*spi_callback_fn)(void * req, int error_code);

/**
 * @brief      Structure definition for an SPI Master Transaction request.
 * @note       When using this structure for an asynchronous operation, the
 *             structure must remain allocated until the callback is completed.
 * @addtogroup spi_async
 */
struct spi_req {
    uint8_t         ssel;       /**< Slave select line to use. (Master only) */
    uint8_t         deass;      /**< Non-zero to de-assert slave select after transaction. (Master only)*/
    spi17y_sspol_t  ssel_pol;   /**< Slave select line polarity. */
    const void      *tx_data;   /**< Pointer to a buffer to transmit data from. NULL if undesired. */
    void            *rx_data;   /**< Pointer to a buffer to store data received. NULL if undesired.*/
    spi17y_width_t  width;      /**< Number of data lines to use, see #spi17y_width_t. */
    unsigned        len;        /**< Number of transfer units to send from the \p tx_data buffer. */
    unsigned        bits;       /**< Number of bits in transfer unit (e.g. 8 for byte, 16 for short) */
    unsigned        rx_num;     /**< Number of bytes actually read into the \p rx_data buffer. */
    unsigned        tx_num;     /**< Number of bytes actually sent from the \p tx_data buffer */
    spi_callback_fn    callback;   /**< Callback function if desired, NULL otherwise */
};


/* **** Function Prototypes **** */

/**
 * @brief Initialize the spi.
 * @param      spi_name SPI instance being used
 * @param      mode SPI mode for clock phase and polarity.
 * @param      freq Desired clock frequency.
 * @param      spi17y_cfg The spi configuration
 * 
 * @return     #E_NO_ERROR If everything is successful. 
 * @return     @ref MXC_Error_Codes If function is unsuccessful.
 */
int SPI_Init(spi_type spi_name, unsigned int mode, unsigned int freq, sys_cfg_spi_t spi17y_cfg);

/**
 * @brief      Asynchronously read/write SPI Master data
 *
 * @param      spi_name  SPI instance being used
 * @param      req   Pointer to spi request
 *
 * @return     #E_NO_ERROR If everything is successful. 
 * @return     @ref MXC_Error_Codes If function is unsuccessful.
 */
int SPI_MasterTransAsync(spi_type spi_name,  spi_req_t *req);

/**
 * @brief     Execute a master transaction.
 * @param     spi_name  SPI instance being used
 * @param     req   Pointer to spi request
 *
 * @return     #E_NO_ERROR If everything is successful. 
 * @return     @ref MXC_Error_Codes If function is unsuccessful.
 */
int SPI_MasterTrans(spi_type spi_name, spi_req_t *req);

/**
 * @brief      Asynchronously read/write SPI Slave data
 * @param      spi_name  SPI instance being used
 * @param      req   Pointer to spi request
 *
 * @return     #E_NO_ERROR If everything is successful. 
 * @return     @ref MXC_Error_Codes If function is unsuccessful.
 */
int SPI_SlaveTransAsync(spi_type spi_name, spi_req_t *req);

/**
 * @brief     Execute a slave transaction.
 * @param     spi_name  SPI instance being used
 * @param     req   Pointer to spi request
 *
 * @return     #E_NO_ERROR If everything is successful. 
 * @return     @ref MXC_Error_Codes If function is unsuccessful.
 */
int SPI_SlaveTrans(spi_type spi_name, spi_req_t *req);

/**
 * @brief     Shutdown SPI module.
 * @param     spi_name  SPI instance being used
 *
 * @return     #E_NO_ERROR If everything is successful. 
 * @return     @ref MXC_Error_Codes If function is unsuccessful.
 */
int SPI_Shutdown(spi_type spi_name);


/**
 * @brief      Aborts an Asynchronous request
 * @param      spi_name  SPI instance being used
 * @param      req   Pointer to spi request
 *
 * @return     #E_NO_ERROR If everything is successful. 
 * @return     @ref MXC_Error_Codes If function is unsuccessful.
 */
int SPI_AbortAsync(spi_type spi_name, spi_req_t *req);

/**
 * @brief      Execute SPI transaction based on interrupt handler
 * @param      spi_name  SPI instance being used
 *
 * @return     #E_NO_ERROR If everything is successful. 
 * @return     @ref MXC_Error_Codes If function is unsuccessful.
 */
int SPI_Handler(spi_type spi_name);

/**
 * @brief     Enable SPI
 * @param     spi_name  Pointer to spi module.
 *
 * @return     #E_NO_ERROR If everything is successful. 
 * @return     @ref MXC_Error_Codes If function is unsuccessful.
 */
int SPI_Enable(spi_type spi_name);

/**
 * @brief     Disable SPI
 * @param     spi_name  Pointer to spi module.
 *
 * @return     #E_NO_ERROR If everything is successful. 
 * @return     @ref MXC_Error_Codes If function is unsuccessful.
 */
int SPI_Disable(spi_type spi_name);

/**
 * @brief     Clear the TX and RX FIFO
 * @param     spi_name  Pointer to spi module.
 *
 * @return     #E_NO_ERROR If everything is successful. 
 * @return     @ref MXC_Error_Codes If function is unsuccessful.
 */
int SPI_Clear_fifo(spi_type spi_name);

//-------------------------------------------------------------------------------------------
/**@} end of group spi */

#ifdef __cplusplus
}
#endif

#endif /* _SPI_H_ */



