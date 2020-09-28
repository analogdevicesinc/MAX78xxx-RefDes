/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Alert-related service implementation.
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

#ifndef SVC_ALERT_H
#define SVC_ALERT_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup ALERT-RELATED_SERVICE
 *  \{ */

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Alert Handles
 *
 */
/**@{*/
#define ALERT_START_HDL               0xA0                /*!< \brief Start handle. */
#define ALERT_END_HDL                 (ALERT_MAX_HDL - 1) /*!< \brief End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Alert-Related Service Handles */
enum
{
  ALERT_ANS_SVC_HDL = ALERT_START_HDL,    /*!< \brief Alert notification service declaration */
  ALERT_ANS_SNA_CH_HDL,                   /*!< \brief Supported new alert category characteristic */
  ALERT_ANS_SNA_HDL,                      /*!< \brief Supported new alert category */
  ALERT_ANS_NEW_CH_HDL,                   /*!< \brief New alert characteristic */
  ALERT_ANS_NEW_HDL,                      /*!< \brief New alert */
  ALERT_ANS_NEW_CH_CCC_HDL,               /*!< \brief New alert client characteristic configuration */
  ALERT_ANS_UNR_CH_HDL,                   /*!< \brief Supported unread alert category characteristic */
  ALERT_ANS_UNR_HDL,                      /*!< \brief Supported unread alert category */
  ALERT_ANS_UAS_CH_HDL,                   /*!< \brief Unread alert status characteristic */
  ALERT_ANS_UAS_HDL,                      /*!< \brief Unread alert status */
  ALERT_ANS_UAS_CH_CCC_HDL,               /*!< \brief Unread alert status client characteristic configuration */
  ALERT_ANS_CP_CH_HDL,                    /*!< \brief Alert notification control point characteristic */
  ALERT_ANS_CP_HDL,                       /*!< \brief Alert notification control point */

  ALERT_PASS_SVC_HDL,                     /*!< \brief Phone alert status service declaration */
  ALERT_PASS_AS_CH_HDL,                   /*!< \brief Alert status characteristic */
  ALERT_PASS_AS_HDL,                      /*!< \brief Alert status */
  ALERT_PASS_AS_CCC_HDL,                  /*!< \brief Alert status client characteristic configuration */
  ALERT_PASS_RS_CH_HDL,                   /*!< \brief Ringer setting characteristic */
  ALERT_PASS_RS_HDL,                      /*!< \brief Ringer setting */
  ALERT_PASS_RS_CCC_HDL,                  /*!< \brief Ringer settting client characteristic configuration */
  ALERT_PASS_RCP_CH_HDL,                  /*!< \brief Ringer control point characteristic */
  ALERT_PASS_RCP_HDL,                     /*!< \brief Ringer control point */

  ALERT_NWS_SVC_HDL,                      /*!< \brief Network availability service declaration */
  ALERT_NWS_NWA_CH_HDL,                   /*!< \brief Network availability characteristic */
  ALERT_NWS_NWA_HDL,                      /*!< \brief Network availability */
  ALERT_NWS_NWA_CH_CCC_HDL,               /*!< \brief Network availability client characteristic configuration */
  ALERT_MAX_HDL                           /*!< \brief Maximum handle. */
};

/**@}*/

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Add the services to the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcAlertAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcAlertRemoveGroup(void);

/*! \} */    /* ALERT-RELATED_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_ALERT_H */

