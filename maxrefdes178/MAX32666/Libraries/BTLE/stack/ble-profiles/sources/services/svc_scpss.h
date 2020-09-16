/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Scan Parameter Service Server implementation.
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

#ifndef SVC_SCPSS_H
#define SVC_SCPSS_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup SCAN_PARAMETER_SERVICE
 *  \{ */

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Scan Parameter Service Handles
 *
 */
/**@{*/
#define SCPSS_START_HDL               0x0300              /*!< \brief Start handle. */
#define SCPSS_END_HDL                 (SCPSS_MAX_HDL - 1) /*!< \brief End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Scan Parameter Service Handles */
enum
{
  SCPSS_SVC_HDL = SCPSS_START_HDL,    /*!< \brief Scan Parameter Server Service declaration */
  SCPSS_SIW_CH_HDL,                   /*!< \brief Scan Interval Window characteristic */
  SCPSS_SIW_HDL,                      /*!< \brief Scan Interval Window */
  SCPSS_MAX_HDL                       /*!< \brief Maximum handle. */
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
void SvcScpssAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcScpssRemoveGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Register callbacks for the service.
 *
 *  \param  writeCback  Write callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcScpssCbackRegister(attsWriteCback_t writeCback);

/*! \} */    /* SCAN_PARAMETER_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_SCPSS_H */
