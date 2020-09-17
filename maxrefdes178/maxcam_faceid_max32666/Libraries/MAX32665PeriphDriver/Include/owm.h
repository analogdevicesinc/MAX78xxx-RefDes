/**
 * @file       owm.h
 * @brief      Registers, Bit Masks and Bit Positions for the 1-Wire Master
 *             peripheral module.
 */
 /* ****************************************************************************
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
 * $Date: 2018-11-09 00:08:58 +0300 (Fri, 09 Nov 2018) $
 * $Revision: 39056 $
 *
 **************************************************************************** */

/* Define to prevent redundant inclusion */
#ifndef _OWM_H_
#define _OWM_H_

/* **** Includes **** */
#include "mxc_config.h"
#include "mxc_sys.h"
#include "owm_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup periphlibs
 * @defgroup owm 1-Wire Master (OWM)
 * @{
 */

/* **** Definitions **** */

/**
 * Enumeration type for specifying options for 1-Wire external pullup mode.
 */
typedef enum {
  OWM_EXT_PU_ACT_HIGH = 0,  /**< Pullup pin is active high when enabled.        */
  OWM_EXT_PU_ACT_LOW = 1,   /**< Pullup pin is active low when enabled.         */
  OWM_EXT_PU_UNUSED = 2,    /**< Pullup pin is not used for an external pullup. */
} owm_ext_pu_t;

/**
 * Structure type for 1-Wire Master configuration.
 */
typedef struct {
    uint8_t int_pu_en;              /**< 1 = internal pullup on.   */
    owm_ext_pu_t ext_pu_mode;       /**< See #owm_ext_pu_t.   */
    uint8_t long_line_mode;         /**< 1 = long line mode enable.    */
    // owm_overdrive_t overdrive_spec; /**< 0 = timeslot is 12us, 1 = timeslot is 10us.   */
} owm_cfg_t;


#define READ_ROM_COMMAND        0x33      /**< Read ROM Command */
#define MATCH_ROM_COMMAND       0x55      /**< Match ROM Command */
#define SEARCH_ROM_COMMAND      0xF0      /**< Search ROM Command */
#define SKIP_ROM_COMMAND        0xCC      /**< Skip ROM Command */
#define OD_SKIP_ROM_COMMAND     0x3C      /**< Overdrive Skip ROM Command */
#define OD_MATCH_ROM_COMMAND    0x69      /**< Overdrive Match ROM Command */
#define RESUME_COMMAND          0xA5      /**< Resume Command */

/* **** Globals **** */

/* **** Function Prototypes **** */

/**
 * @brief   Initialize and enable OWM module.
 * @param   cfg         Pointer to OWM configuration.
 * @param   sys_cfg     System configuration object
 *
 * @return  #E_NO_ERROR If function is successful.
 * @return  #E_NULL_PTR If parameter is a null pointer.
 * @return  #E_BUSY If IOMAN was not configured correctly.
 * @return  #E_UNINITIALIZED If OWM CLK is disabled.
 * @return  #E_NOT_SUPPORTED If 1MHz CLK cannot be created with given system and owm CLK.
 * @return  #E_BAD_PARAM If bad cfg parameter passed in.
 */
int OWM_Init(const owm_cfg_t *cfg, const sys_cfg_owm_t* sys_cfg);

/**
 * @brief   Shutdown OWM module.
 */
void OWM_Shutdown(void);

/**
 * @brief   Send 1-Wire reset pulse. Will block until transaction is complete.
 */
int OWM_Reset(void);

/**
 * @brief   Send and receive one byte of data. Will block until transaction is complete.
 * @param   data        data to send
 * @return  Data read (1 byte)
 */
int OWM_TouchByte(uint8_t data);

/**
 * @brief   Write one byte of data. Will block until transaction is complete.
 * @param   data        data to send
 * @return  #E_NO_ERROR If everything is successful.
 * @return  #E_COMM_ERR If data written is not equal to data parameter.
 */
int OWM_WriteByte(uint8_t data);

/**
 * @brief   Read one byte of data. Will block until transaction is complete.
 * @return  Data read (1 byte)
 */
int OWM_ReadByte(void);

/**
 * @brief   Send and receive one bit of data. Will block until transaction is complete.
 * @param   bit         bit to send
 * @return  bit read
 */
int OWM_TouchBit(uint8_t bit);

/**
 * @brief   Write one bit of data. Will block until transaction is complete.
 * @param   bit         bit to send
 * @return  #E_NO_ERROR if everything is successful
 * @return  #E_COMM_ERR if bit written != bit parameter
 */
int OWM_WriteBit(uint8_t bit);

/**
 * @brief   Read one bit of data. Will block until transaction is complete.
 * @return  Bit read
 */
int OWM_ReadBit(void);

/**
 * @brief   Write multiple bytes of data. Will block until transaction is complete.
 * @param   data    Pointer to buffer for write data.
 * @param   len     Number of bytes to write.
 *
 * @return  Number of bytes written if successful.
 * @return  #E_COMM_ERR If line short detected before transaction.
 */
int OWM_Write(uint8_t* data, int len);

/**
 * @brief   Read multiple bytes of data. Will block until transaction is complete.
 * @param   data    Pointer to buffer for read data.
 * @param   len     Number of bytes to read.
 *
 * @return Number of bytes read if successful
 * @return #E_COMM_ERR If line short detected before transaction.
 */
int OWM_Read(uint8_t* data, int len);

/**
 * @brief   Starts 1-Wire communication with Read ROM command
 * @note    Only use the Read ROM command with one slave on the bus
 * @param   ROMCode     Pointer to buffer for ROM code read
 * @return  #E_NO_ERROR If everything is successful.
 * @return  #E_COMM_ERR If reset, read or write fails.
 */
int OWM_ReadROM(uint8_t* ROMCode);

/**
 * @brief   Starts 1-Wire communication with Match ROM command
 * @param   ROMCode     Pointer to buffer with ROM code to match
 * @return  #E_NO_ERROR If everything is successful.
 * @return  #E_COMM_ERR If reset or write fails.
 */
int OWM_MatchROM(uint8_t* ROMCode);

/**
 * @brief   Starts 1-Wire communication with Overdrive Match ROM command
 * @note    After Overdrive Match ROM command is sent, the OWM is set to
 *          overdrive speed. To set back to standard speed use OWM_SetOverdrive.
 * @param   ROMCode     Pointer to buffer with ROM code to match
 * @return  #E_NO_ERROR If everything is successful.
 * @return  #E_COMM_ERR If reset or write fails.
 */
int OWM_ODMatchROM(uint8_t* ROMCode);

/**
 * @brief   Starts 1-Wire communication with Skip ROM command
 * @return  #E_NO_ERROR If everything is successful.
 * @return  #E_COMM_ERR If reset or write fails.
 */
int OWM_SkipROM(void);

/**
 * @brief   Starts 1-Wire communication with Overdrive Skip ROM command
 * @note    After Overdrive Skip ROM command is sent, the OWM is set to
 *          overdrive speed. To set back to standard speed use OWM_SetOverdrive
 * @return  #E_NO_ERROR If everything is successful.
 * @return  #E_COMM_ERR If reset or write fails.
 */
int OWM_ODSkipROM(void);

/**
 * @brief   Starts 1-Wire communication with Resume command
 * @return  #E_NO_ERROR If everything is successful.
 * @return  #E_COMM_ERR If reset or write fails.
 */
int OWM_Resume(void);

/**
 * @brief   Starts 1-Wire communication with Search ROM command
 * @param   newSearch   (1) = start new search, (0) = continue search for next ROM
 * @param   ROMCode     Pointer to buffer with ROM code found
 * @return  (1) = ROM found, (0) = no new ROM found, end of search.
 */
int OWM_SearchROM(int newSearch, uint8_t* ROMCode);

/**
 * @brief   Clear interrupt flags.
 * @param   mask        Mask of interrupts to clear.
 */
void OWM_ClearFlags(uint32_t mask);

/**
 * @brief   Get interrupt flags.
 * @return  Mask of active flags.
 */
unsigned OWM_GetFlags(void);

/**
 * @brief   Enables/Disables the External pullup
 * @param   enable      (1) = enable, (0) = disable
 */
void OWM_SetExtPullup(int enable);

/**
 * @brief   Enables/Disables Overdrive speed
 * @param   enable      (1) = overdrive, (0) = standard
 */
void OWM_SetOverdrive(int enable);

/**@} end of group owm */
#ifdef __cplusplus
}
#endif

#endif /* _OWM_H_ */
