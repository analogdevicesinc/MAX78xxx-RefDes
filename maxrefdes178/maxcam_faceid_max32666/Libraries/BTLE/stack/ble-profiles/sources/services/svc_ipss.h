/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Internet Profile Support Service implementation.
 *
 *  Copyright (c) 2016-2018 Arm Ltd. All Rights Reserved.
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

#ifndef SVC_IPSS_H
#define SVC_IPSS_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup INTERNET_PROFILE_SUPPORT_SERVICE
 *  \{ */

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name IP Support Service Handles
 *
 */
/**@{*/
#define IPSS_START_HDL               0x0600             /*!< \brief Start handle. */
#define IPSS_END_HDL                 (IPSS_MAX_HDL - 1) /*!< \brief End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief IP Support Service Handles */
enum
{
  IPSS_SVC_HDL = IPSS_START_HDL,       /*!< \brief IP Support Server Service declaration */
  IPSS_MAX_HDL                         /*!< \brief Maximum handle. */
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
void SvcIpssAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcIpssRemoveGroup(void);

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
void SvcIpssCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

/*! \} */    /* INTERNET_PROFILE_SUPPORT_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_IPSS_H */
