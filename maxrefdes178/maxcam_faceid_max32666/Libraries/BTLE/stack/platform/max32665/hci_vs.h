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
 * $Date: 2019-06-19 18:12:00 +0300 (Wed, 19 Jun 2019) $
 * $Revision: 44073 $
 *
 ******************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief Vendor specific HCI Interface.
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
 *  \fn         HciVsSetAddr
 *
 *  \brief      Vendor-specific HCI command to set Bluetooth device address to be used by LL.
 *
 *  \param      pAddr       Bluetooth device address.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciVsSetBdAddr(uint8_t *pAddr);

/*************************************************************************************************/
/*!
 *  \fn         HciVsSetDefaultTxPower
 *
 *  \brief      Vendor-specific HCI command to defautlt transmit power.
 *
 *  \param      power 		Default power setting.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciVsSetDefaultTxPower(int8_t power);

/*************************************************************************************************/
/*!
 *  \fn         HciVsCalibrate
 *
 *  \brief      Vendor-specific HCI command to re-calibrate the BLE AFE.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciVsCalibrate(void);

/*************************************************************************************************/
/*!
 *  \fn         HciVsSetLocalFeatures
 *
 *  \brief      Vendor-specific HCI command to set controller features.
 *
 *  \param      pFeatures   Supported features bitmask.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciVsSetLocalFeatures(uint8_t *pFeatures);

/*************************************************************************************************/
/*!
 *  \fn         HciVsSetConnTx Power
 *
 *  \brief      Vendor-specific HCI command to the TX power for a specified connection. 
 *
 *  \param      handle 		Connection handle.
 *  \param      power 		Desired power setting.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciVsSetConnTxPower(uint16_t handle, int8_t power);

/*************************************************************************************************/
/*!
 *  \fn         HciVsGetAdvStats
 *
 *  \brief      Vendor-specific HCI command to get the advertising statistics. 
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciVsGetAdvStats(void);

/*************************************************************************************************/
/*!
 *  \fn         HciVsGetScanStats
 *
 *  \brief      Vendor-specific HCI command to get the scanning statistics. 
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciVsGetScanStats(void);

/*************************************************************************************************/
/*!
 *  \fn         HciVsGetConnStats
 *
 *  \brief      Vendor-specific HCI command to get the connection statistics. 
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciVsGetConnStats(void);

/*************************************************************************************************/
/*!
 *  \fn         HciVsGetTestStats
 *
 *  \brief      Vendor-specific HCI command to get the test statistics. 
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciVsGetTestStats(void);
