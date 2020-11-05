/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Weight Scale service implementation.
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

#ifndef SVC_WSS_H
#define SVC_WSS_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup WEIGHT_SCALE_SERVICE
 *  \{ */

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Weight Scale Service Handles
 *
 */
/**@{*/
#define WSS_START_HDL               0x0100            /*!< \brief Start handle. */
#define WSS_END_HDL                 (WSS_MAX_HDL - 1) /*!< \brief End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Weight Scale Service Handles */
enum
{
  WSS_SVC_HDL = WSS_START_HDL,      /*!< \brief Weight scale service declaration */
  WSS_WM_CH_HDL,                    /*!< \brief Weight measurement characteristic */
  WSS_WM_HDL,                       /*!< \brief Weight measurement */
  WSS_WM_CH_CCC_HDL,                /*!< \brief Weight measurement client characteristic configuration */
  WSS_WSF_CH_HDL,                   /*!< \brief Weight scale feature characteristic */
  WSS_WSF_HDL,                      /*!< \brief Weight scale feature */
  WSS_MAX_HDL                       /*!< \brief Maximum handle. */
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
void SvcWssAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcWssRemoveGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Register callbacks for the service.
 *
 *  \param  readCback   Read callback function.
 *  \param  writeCback  Write callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcWssCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

/*! \} */    /* WEIGHT_SCALE_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_WSS_H */
