/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  WP service implementation. Arm Ltd. proprietary servicde.
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

#ifndef SVC_WP_H
#define SVC_WP_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup WP_SERVICE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Arm Ltd. Proprietary Service Handles
 *
 */
/**@{*/
/*! \brief Proprietary Service */
#define WP_START_HDL               0x200            /*!< \brief Start handle. */
#define WP_END_HDL                 (WP_MAX_HDL - 1) /*!< \brief End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Proprietary Service Handles */
enum
{
  WP_SVC_HDL = WP_START_HDL,       /*!< \brief Proprietary service declaration */
  WP_DAT_CH_HDL,                   /*!< \brief Proprietary data characteristic */
  WP_DAT_HDL,                      /*!< \brief Proprietary data */
  WP_DAT_CH_CCC_HDL,               /*!< \brief Proprietary data client characteristic configuration */
  WP_MAX_HDL                       /*!< \brief Maximum handle. */
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
void SvcWpAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcWpRemoveGroup(void);

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
void SvcWpCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

/*! \} */    /* WP_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_WP_H */
