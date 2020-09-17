/*************************************************************************************************/
/*!
*  \file
*
*  \brief  Scan Parameter Profile Server Application Interface.
*
*  Copyright (c) 2016-2018 Arm Ltd. All Rights Reserved.
*  ARM Ltd. confidential and proprietary.
*
*  IMPORTANT.  Your use of this file is governed by a Software License Agreement
*  ("Agreement") that must be accepted in order to download or otherwise receive a
*  copy of this file.  You may not use or copy this file for any purpose other than
*  as described in the Agreement.  If you do not agree to all of the terms of the
*  Agreement do not use this file and delete all copies in your possession or control;
*  if you do not have a copy of the Agreement, you must contact ARM Ltd. prior
*  to any use, copying or further distribution of this software.
*/
/*************************************************************************************************/

#ifndef SCPPS_API_H
#define SCPPS_API_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup SCAN_PARAMETER_PROFILE
 *  \{ */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*! \brief Application interval window callback */
typedef void ScppsAppCback_t(dmConnId_t connId, uint16_t interval, uint16_t window);

/*************************************************************************************************/
/*!
 *  \brief  Called to register an application scan interval window callback function
 *
 *  \param  cback    Application interval window callback
 *
 *  \return Status
 */
/*************************************************************************************************/
void ScppsRegisterCback(ScppsAppCback_t *cback);

/*************************************************************************************************/
/*!
 *  \brief  Called when the peer writes to SCPPS attributes
 *
 *  \param  connId      DM connection identifier.
 *  \param  handle      ATT handle.
 *  \param  operation   ATT operation.
 *  \param  offset      Write offset.
 *  \param  len         Write length.
 *  \param  pValue      Value to write.
 *  \param  pAttr       Attribute to write.
 *
 *  \return Status
 */
/*************************************************************************************************/
uint8_t ScppsAttsWriteCback(dmConnId_t connId, uint16_t handle, uint8_t operation,
                            uint16_t offset, uint16_t len, uint8_t *pValue, attsAttr_t *pAttr);

/*! \} */    /* SCAN_PARAMETER_PROFILE */

#ifdef __cplusplus
};
#endif

#endif /* SCPPS_API_H */
