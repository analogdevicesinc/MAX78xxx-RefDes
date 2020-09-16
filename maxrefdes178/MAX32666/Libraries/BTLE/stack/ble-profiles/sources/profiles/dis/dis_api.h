/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Device information service client.
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
#ifndef DIS_API_H
#define DIS_API_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup DEVICE_INFORMATION_PROFILE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief Enumeration of handle indexes of characteristics to be discovered */
enum
{
  DIS_MFNS_HDL_IDX,         /*!< \brief Manufacturer name string */
  DIS_MNS_HDL_IDX,          /*!< \brief Model number string */
  DIS_SNS_HDL_IDX,          /*!< \brief Serial number string */
  DIS_HRS_HDL_IDX,          /*!< \brief Hardware revision string */
  DIS_FRS_HDL_IDX,          /*!< \brief Firmware revision string */
  DIS_SRS_HDL_IDX,          /*!< \brief Software revision string */
  DIS_SID_HDL_IDX,          /*!< \brief System ID */
  DIS_HDL_LIST_LEN          /*!< \brief Handle list length */
};

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for DIS service.  Note that pHdlList
 *          must point to an array of handles of length \ref DIS_HDL_LIST_LEN.  If discovery is
 *          successful the handles of discovered characteristics and descriptors will be set
 *          in pHdlList.
 *
 *  \param  connId    Connection identifier.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DisDiscover(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Process a value received in an ATT read response, notification, or indication
 *          message.  Parameter pHdlList must point to an array of length \ref DIS_HDL_LIST_LEN.
 *          If the attribute handle of the message matches a handle in the handle list the value
 *          is processed, otherwise it is ignored.
 *
 *  \param  pHdlList  Characteristic handle list.
 *  \param  pMsg      ATT callback message.
 *
 *  \return \ref ATT_SUCCESS if handle is found, \ref ATT_ERR_NOT_FOUND otherwise.
 */
/*************************************************************************************************/
uint8_t DisValueUpdate(uint16_t *pHdlList, attEvt_t *pMsg);

/*! \} */    /* DEVICE_INFORMATION_PROFILE */

#ifdef __cplusplus
};
#endif

#endif /* DIS_API_H */
