/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Battery service implementation.
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

#ifndef SVC_BATT_H
#define SVC_BATT_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup BATTERY_SERVICE
 *  \{ */

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Battery Service Handles
 *
 */
/**@{*/
#define BATT_START_HDL                    0x60                /*!< \brief Service start handle. */
#define BATT_END_HDL                      (BATT_MAX_HDL - 1)  /*!< \brief Service end handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Battery Service Handles */
enum
{
  BATT_SVC_HDL = BATT_START_HDL,        /*!< \brief Battery service declaration */
  BATT_LVL_CH_HDL,                      /*!< \brief Battery level characteristic */
  BATT_LVL_HDL,                         /*!< \brief Battery level */
  BATT_LVL_CH_CCC_HDL,                  /*!< \brief Battery level CCCD */
  BATT_MAX_HDL                          /*!< \brief Maximum handle. */
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
void SvcBattAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcBattRemoveGroup(void);

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
void SvcBattCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

/*************************************************************************************************/
/*!
 *  \brief  Add the battery service using the dynamic attribute subsystem.
 *
 *  \return None.
 */
/*************************************************************************************************/
void *SvcBattAddGroupDyn(void);

/*! \} */    /* BATTERY_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_BATT_H */

