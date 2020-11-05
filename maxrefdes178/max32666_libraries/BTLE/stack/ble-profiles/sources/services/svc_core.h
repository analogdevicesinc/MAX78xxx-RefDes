/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example GATT and GAP service implementations.
 *
 *  Copyright (c) 2009-2018 Arm Ltd. All Rights Reserved.
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

#ifndef SVC_CORE_H
#define SVC_CORE_H


#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup GATT_AND_GAP_SERVICE
 *  \{ */


/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/
/** \name GAP Service Handles
 * \note GAP -- RPAO characterstic added only when DM Privacy enabled
 */
/**@{*/
#define GAP_START_HDL               0x01               /*!< \brief GAP start handle */
#define GAP_END_HDL                 (GAP_MAX_HDL - 3)  /*!< \brief GAP end handle */
/**@}*/

/** \name GATT Service Handles
 *
 */
/**@{*/
#define GATT_START_HDL              0x10                /*!< \brief GATT start handle */
#define GATT_END_HDL                (GATT_MAX_HDL - 1)  /*!< \brief GATT end handle */
/**@}*/

/**************************************************************************************************
 Handles
**************************************************************************************************/

/** \name GAP Service Handles
 *
 */
/**@{*/
/*! \brief GAP service handle */
enum
{
  GAP_SVC_HDL = GAP_START_HDL,      /*!< \brief GAP service declaration */
  GAP_DN_CH_HDL,                    /*!< \brief Device name characteristic */
  GAP_DN_HDL,                       /*!< \brief Device name */
  GAP_AP_CH_HDL,                    /*!< \brief Appearance characteristic */
  GAP_AP_HDL,                       /*!< \brief Appearance */
  GAP_CAR_CH_HDL,                   /*!< \brief Central address resolution characteristic */
  GAP_CAR_HDL,                      /*!< \brief Central address resolution */
  GAP_RPAO_CH_HDL,                  /*!< \brief Resolvable private address only characteristic */
  GAP_RPAO_HDL,                     /*!< \brief Resolvable private address only */
  GAP_MAX_HDL                       /*!< \brief GAP maximum handle */
};
/**@}*/

/** \name GATT Service Handles
 *
 */
/**@{*/
/*! \brief GATT service handles */
enum
{
  GATT_SVC_HDL = GATT_START_HDL,    /*!< \brief GATT service declaration */
  GATT_SC_CH_HDL,                   /*!< \brief Service changed characteristic */
  GATT_SC_HDL,                      /*!< \brief Service changed */
  GATT_SC_CH_CCC_HDL,               /*!< \brief Service changed client characteristic configuration descriptor */
  GATT_MAX_HDL                      /*!< \brief GATT maximum handle */
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
void SvcCoreAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCoreRemoveGroup(void);

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
void SvcCoreGattCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

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
void SvcCoreGapCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

/*************************************************************************************************/
/*!
 *  \brief  Update the central address resolution attribute value.
 *
 *  \param  value   New value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCoreGapCentAddrResUpdate(bool_t value);

/*************************************************************************************************/
/*!
 *  \brief  Add the Resolvable Private Address Only (RPAO) characteristic to the GAP service.
 *          The RPAO characteristic should be added only when DM Privacy is enabled.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCoreGapAddRpaoCh(void);

/*! \} */    /* GATT_AND_GAP_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_CORE_H */

