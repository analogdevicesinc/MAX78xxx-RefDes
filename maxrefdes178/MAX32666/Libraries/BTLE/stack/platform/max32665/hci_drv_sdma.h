/*******************************************************************************
 * Copyright (C) 2019 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2020-07-28 19:51:51 +0300 (Tue, 28 Jul 2020) $
 * $Revision: 54416 $
 *
 ******************************************************************************/

#include "bb_api.h"
#include "ll_api.h"

/*************************************************************************************************/
/*!
 *  \brief HCI Interface for the SDMA.
 */
/*************************************************************************************************/

/**************************************************************************************************
  Macros
**************************************************************************************************/

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \fn         hciDrvInit
 *
 *  \brief      Initialize the HCI interface.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void hciDrvInit(void);

/*************************************************************************************************/
/*!
 *  \fn         SDMAGetWakeupTime
 *
 *  \brief      Get the SDMA's desired wakeup timer compare value. 
 *
 *  \return     WUT CMP value to wakeup SDMA.
 */
/*************************************************************************************************/
uint32_t SDMAGetWakeupTime(void);

/*************************************************************************************************/
/*!
 *  \fn         SDMASetWakeupFlag
 *
 *  \brief      Set the wakeup flag in the byte offset pos. 
 *
 *  \return     None.
 */
/*************************************************************************************************/
void SDMASetARMFlag(uint8_t flag);

/*************************************************************************************************/
/*!
 *  \fn         SDMAGetWakeupFlag
 *
 *  \brief      Get the wakeup flag at byte offset pos. 
 *
 *  \return     None.
 */
/*************************************************************************************************/
uint8_t SDMAGetSDMAFlag(void);

/*************************************************************************************************/
/*!
 *  \fn         SDMASetBBCfg
 *
 *  \brief      Overwrite the built in sdma BB config.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void SDMASetBBCfg(const BbRtCfg_t * p_bb_cfg);

/*************************************************************************************************/
/*!
 *  \fn         SDMASetLLCfg
 *
 *  \brief      Overwrite the built in sdma LL config.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void SDMASetLLCfg(const LlRtCfg_t * p_ll_cfg);

/*************************************************************************************************/
/*!
 *  \fn         SDMARestart
 *
 *  \brief      Restart the SDMA after a sleep event. 
 *
 *  \return     None.
 */
/*************************************************************************************************/
void SDMARestart(void);

