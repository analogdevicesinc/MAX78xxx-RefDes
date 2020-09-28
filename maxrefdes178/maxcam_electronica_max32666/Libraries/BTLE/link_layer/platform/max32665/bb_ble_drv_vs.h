/* Copyright (c) 2009-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 *  \brief Vendor Specific Baseband driver interface file.
 */
/*************************************************************************************************/

#ifndef BB_BLE_DRV_VS_H
#define BB_BLE_DRV_VS_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/


/*************************************************************************************************/
/*!
 *  \brief      Calibrate the baseband layer for BLE operation.
 *
 *  \return     None.
 *
 *  This routine should block until the BB hardware is completely initialized.
 */
/*************************************************************************************************/
void BbBleDrvCalibrate(void);

/*************************************************************************************************/
/*!
 *  \brief      Set the TX power for BLE operations
 *
 *  \param      power   	Power setting in dBm. 
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbBleDrvSetTxPower(int8_t power);



#ifdef __cplusplus
};
#endif

#endif /* BB_BLE_DRV_VS_H */
