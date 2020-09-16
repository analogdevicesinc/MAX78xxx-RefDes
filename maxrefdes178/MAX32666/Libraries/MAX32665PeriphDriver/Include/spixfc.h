/**
 * @file    spixfc.h
 * @brief   SPI Master driver header file.
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
 * $Date: 2019-10-25 00:10:14 +0300 (Fri, 25 Oct 2019) $
 * $Revision: 48075 $
 *
 **************************************************************************** */

/* Define to prevent redundant inclusion */ 
#ifndef _SPIXFC_H_
#define _SPIXFC_H_ 
 
/* **** Includes **** */ 
#include "mxc_config.h"
#include "mxc_sys.h"
#include "spixfc_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup spixfc SPI External Flash Controller (SPIXFC)
 * @ingroup periphlibs
 * @{
 */

/* **** Definitions **** */

/**
 * @brief Active levels for slave select lines.
 */
typedef enum {
    SPIXFC_SSEL0_HIGH  =    (0x1 << 0),
    SPIXFC_SSEL0_LOW   =    0,
    SPIXFC_SSEL1_HIGH  =    (0x1 << 1),
    SPIXFC_SSEL1_LOW   =    0,
    SPIXFC_SSEL2_HIGH  =    (0x1 << 2),
    SPIXFC_SSEL2_LOW   =    0,
    SPIXFC_SSEL3_HIGH  =    (0x1 << 3),
    SPIXFC_SSEL3_LOW   =    0
}
spixfc_ssel_t;

/**
 * @brief Number of data lines to use.
 */
typedef enum {
    SPIXFC_WIDTH_1 = 0,  /**< 1 Data Line.                        */
    SPIXFC_WIDTH_2 = 1,  /**< 2 Data Lines (x2).                  */
    SPIXFC_WIDTH_4 = 2   /**< 4 Data Lines (x4).                  */
} spixfc_width_t;

/**
 * @brief SPIXFC configuration type.
 */
typedef struct {
    uint8_t     mode;       /**< SPIXFC mode to use, 0-3.                                            */
    uint32_t    ssel_pol;   /**< Mask of active levels for slave select signals, use spixfc_ssel_t.  */
    uint32_t    baud;       /**< Baud rate in Hz.                                                   */
} spixfc_cfg_t;

/**
 * @brief SPIXFC Transaction request.
 */
typedef struct spixfc_req spixfc_req_t;

struct spixfc_req {
    uint8_t             ssel;       /**< Slave select number.                                       */
    uint8_t             deass;      /**< De-assert slave select at the end of the transaction.      */
    uint8_t             wait_tx;    /**< Wait for the TX FIFO to be empty before returning.         */
    const uint8_t       *tx_data;   /**< TX buffer.                                                 */
    uint8_t             *rx_data;   /**< RX buffer.                                                 */
    spixfc_width_t        width;     /**< Number of data lines to use                                */
    unsigned            len;        /**< Number of bytes to send.                                   */
    unsigned            read_num;   /**< Number of bytes read.                                      */
    unsigned            write_num;  /**< Number of bytes written.                                   */
    /**
     * @brief   Callback for asynchronous request.
     * @param   spixfc_req_t*  Pointer to the transaction request.
     * @param   int         Error code.
     */
    void (*callback)(spixfc_req_t*, int);
};

/**
 * @brief  Defining all the bit positions for the 16-BIT Header.
 */
#define SPIXFC_HEADER_DIRECTION_POS  0
#define SPIXFC_HEADER_UNITS_POS      2
#define SPIXFC_HEADER_SIZE_POS       4
#define SPIXFC_HEADER_WIDTH_POS      9
#define SPIXFC_HEADER_DEASS_SS_POS   13


/**
 * @brief Page Size and their positions.
 */

#define MXC_V_SPIXFC_CONFIG_PAGE_SIZE_4B      ((uint32_t)0x00000000UL)
#define MXC_V_SPIXFC_CONFIG_PAGE_SIZE_8B      ((uint32_t)0x00000001UL)
#define MXC_V_SPIXFC_CONFIG_PAGE_SIZE_16B     ((uint32_t)0x00000002UL)
#define MXC_V_SPIXFC_CONFIG_PAGE_SIZE_32B     ((uint32_t)0x00000003UL)

#define MXC_S_SPIXFC_CONFIG_PAGE_4B           (MXC_V_SPIXFC_CONFIG_PAGE_SIZE_4B  << MXC_F_SPIXFC_CONFIG_PAGE_SIZE_POS)
#define MXC_S_SPIXFC_CONFIG_PAGE_8B           (MXC_V_SPIXFC_CONFIG_PAGE_SIZE_8B  << MXC_F_SPIXFC_CONFIG_PAGE_SIZE_POS)
#define MXC_S_SPIXFC_CONFIG_PAGE_16B          (MXC_V_SPIXFC_CONFIG_PAGE_SIZE_16B << MXC_F_SPIXFC_CONFIG_PAGE_SIZE_POS)
#define MXC_S_SPIXFC_CONFIG_PAGE_32B          (MXC_V_SPIXFC_CONFIG_PAGE_SIZE_32B << MXC_F_SPIXFC_CONFIG_PAGE_SIZE_POS)
 


typedef enum {
	SPIXFC_HEADER_DIR_NONE,
	SPIXFC_HEADER_DIR_TX,
	SPIXFC_HEADER_DIR_RX,
	SPIXFC_HEADER_DIR_BOTH 
} spixfc_hdr_direction_t;

typedef enum {
	SPIXFC_HEADER_UNITS_BITS,
	SPIXFC_HEADER_UNITS_BYTES,
	SPIXFC_HEADER_UNITS_PAGES 
} spixfc_hdr_units_t;

typedef enum {
    SPIXFC_HEADER_WIDTH_SINGLE,
	SPIXFC_HEADER_WIDTH_DUAL,
	SPIXFC_HEADER_WIDTH_QUAD,
	SPIXFC_HEADER_WIDTH_INVALID
} spixfc_hdr_width_t;

#define SPIXFC_HEADER_DEASS_SS  1
#define MAX_SCLK               0x10
#define SPIXFC_HEADER_TX_DIR    1
#define NOT_HEADER_DATA        0xF000  // 0xF makes sure it is not a header   

/***** Globals *****/

/***** Function Prototypes *****/

/**
 * @brief   Initialize SPIXFC module and setup the GPIO pins
 * @param   spixfc       Pointer to SPIXFC regs.
 * @param   cfg         Pointer to SPIXFC configuration.
 * @param   sys_cfg     Pointer to system configuration object
 * @returns #E_NO_ERROR If everything is successful.
 */
int SPIXFC_Init(mxc_spixfc_regs_t *spixfc, const spixfc_cfg_t *cfg, const sys_cfg_spixfc_t* sys_cfg);

/**
 * @brief   Shutdown SPIXFC module.
 * @param   spixfc    Pointer to SPIXFC regs.
 */
void SPIXFC_Shutdown(mxc_spixfc_regs_t *spixfc);

/**
 * @brief   Send Clock cycles on SCK without reading or writing.
 * @param   spixfc    Pointer to SPIXFC regs.
 * @param   len     Number of clock cycles to send.
 * @param   ssel    Slave select number.
 * @param   deass   De-assert slave select at the end of the transaction.
 * @returns Cycles transacted if everything is successful, error if unsuccessful.
 */
int SPIXFC_Clocks(mxc_spixfc_regs_t *spixfc, uint32_t len, uint8_t ssel, uint8_t deass);

/**
 * @brief   Read/write SPIXFC data. Will block until transaction is complete.
 * @param   spixfc    Pointer to SPIXFC regs.
 * @param   req     Request for a SPIXFC transaction.
 * @note    Callback is ignored.
 * @returns Bytes transacted if everything is successful, error if unsuccessful.
 */
int SPIXFC_Trans(mxc_spixfc_regs_t *spixfc, spixfc_req_t *req);

/**
 * @brief   Asynchronously read/write SPIXFC data.
 * @param   spixfc    Pointer to SPIXFC regs.
 * @param   req     Request for a SPIXFC transaction.
 * @note    Request struct must remain allocated until callback.
 * @return     #E_NO_ERROR If function is successful.
 * @return     @ref MXC_Error_Codes "Error Code"  Non-zero error code if unsuccessful.
 */
int SPIXFC_TransAsync(mxc_spixfc_regs_t *spixfc, spixfc_req_t *req);

/**
 * @brief   Abort asynchronous request.
 * @param   spixfc    Pointer to SPIXFC regs.
 * @param   req     Pointer to request for a SPIXFC transaction.
* @return     #E_NO_ERROR  Push buttons initialized successfully.
 * @return     @ref MXC_Error_Codes "Error Code"  Non-zero error code if unsuccessful.
 * @returns #E_NO_ERROR if request aborted, error if unsuccessful.
 */
int SPIXFC_AbortAsync(mxc_spixfc_regs_t *spixfc, spixfc_req_t *req);

/**
 * @brief   SPIXFC interrupt handler.
 * @details This function should be called by the application from the interrupt
 *          handler if SPIXFC interrupts are enabled. Alternately, this function
 *          can be periodically called by the application if SPIXFC interrupts are
 *          disabled.
 * @param   spixfc    Base address of the SPIXFC module.
 */
void SPIXFC_Handler(mxc_spixfc_regs_t *spixfc);

/**
 * @brief   Attempt to prepare the SPIXFC for sleep.
 * @param   spixfc    Pointer to SPIXFC regs.
 * @returns #E_NO_ERROR if ready to sleep, #E_BUSY if not ready for sleep.
 */
int SPIXFC_PrepForSleep(mxc_spixfc_regs_t *spixfc);

/**@} end of group spixfc */

#ifdef __cplusplus
}
#endif

#endif /* _SPIXFC_H_ */
