/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Find Me profile, locator role.
 *
 *  Copyright (c) 2011-2018 Arm Ltd. All Rights Reserved.
 *  Arm Ltd. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Arm Ltd. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/
#ifndef FMPL_API_H
#define FMPL_API_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup FIND_ME_PROFILE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief Enumeration of handle indexes of characteristics to be discovered for immediate alert service */
enum
{
  FMPL_IAS_AL_HDL_IDX,          /*!< \brief Alert level */
  FMPL_IAS_HDL_LIST_LEN         /*!< \brief Handle list length */
};

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for Immediate Alert service.  Note
 *          that pHdlList must point to an array of handles of length \ref FMPL_IAS_HDL_LIST_LEN.
 *          If discovery is successful the handles of discovered characteristics and
 *          descriptors will be set in pHdlList.
 *
 *  \param  connId    Connection identifier.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void FmplIasDiscover(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Send an immediate alert to the peer device.
 *
 *  \param  connId DM connection ID.
 *  \param  handle Attribute handle.
 *  \param  alert  Alert value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void FmplSendAlert(dmConnId_t connId, uint16_t handle, uint8_t alert);

/*! \} */    /* FIND_ME_PROFILE */

#ifdef __cplusplus
};
#endif

#endif /* FMPL_API_H */
