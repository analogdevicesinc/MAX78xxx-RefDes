/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Blood Pressure profile client.
 *
 *  Copyright (c) 2012-2018 Arm Ltd. All Rights Reserved.
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
#ifndef BLPC_API_H
#define BLPC_API_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup BLOOD_PRESSURE_PROFILE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief Blood Pressure service enumeration of handle indexes of characteristics to be discovered */
enum
{
  BLPC_BPS_BPM_HDL_IDX,           /*!< \brief Blood pressure measurement */
  BLPC_BPS_BPM_CCC_HDL_IDX,       /*!< \brief Blood pressure measurement CCC descriptor */
  BLPC_BPS_ICP_HDL_IDX,           /*!< \brief Intermediate cuff pressure */
  BLPC_BPS_ICP_CCC_HDL_IDX,       /*!< \brief Intermediate cuff pressure CCC descriptor */
  BLPC_BPS_BPF_HDL_IDX,           /*!< \brief Blood pressure feature */
  BLPC_BPS_HDL_LIST_LEN           /*!< \brief Handle list length */
};

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for Blood Pressure service.
 *          Parameter pHdlList must point to an array of length \ref BLPC_BPS_HDL_LIST_LEN.
 *          If discovery is successful the handles of discovered characteristics and
 *          descriptors will be set in pHdlList.
 *
 *  \param  connId    Connection identifier.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BlpcBpsDiscover(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Process a value received in an ATT read response, notification, or indication
 *          message.  Parameter pHdlList must point to an array of length \ref BLPC_BPS_HDL_LIST_LEN.
 *          If the ATT handle of the message matches a handle in the handle list the value
 *          is processed, otherwise it is ignored.
 *
 *  \param  pHdlList  Characteristic handle list.
 *  \param  pMsg      ATT callback message.
 *
 *  \return \ref ATT_SUCCESS if handle is found, \ref ATT_ERR_NOT_FOUND otherwise.
 */
/*************************************************************************************************/
uint8_t BlpcBpsValueUpdate(uint16_t *pHdlList, attEvt_t *pMsg);

/*! \} */    /* BLOOD_PRESSURE_PROFILE */

#ifdef __cplusplus
};
#endif

#endif /* BLPC_API_H */
