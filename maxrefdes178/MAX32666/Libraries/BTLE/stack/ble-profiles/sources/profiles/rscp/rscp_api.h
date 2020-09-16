/*************************************************************************************************/
/*!
*  \file
*
*  \brief  Running Speed and Cadence Profile API.
*
*  Copyright (c) 2016-2018 Arm Ltd. All Rights Reserved.
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

#ifndef RSCP_API_H
#define RSCP_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup RUNNING_SPEED_AND_CADENCE_PROFILE
 *  \{ */

/**************************************************************************************************
Macros
**************************************************************************************************/

/** \name Running Speed and Cadence Measurement Parameter Types
 *
 */
/**@{*/
#define RSCP_SM_PARAM_SPEED                       0     /*!< \brief Instantaneous Speed */
#define RSCP_SM_PARAM_CADENCE                     1     /*!< \brief Instantaneous Cadence */
#define RSCP_SM_PARAM_STRIDE_LENGTH               2     /*!< \brief Instantaneous Stride Length */
#define RSCP_SM_PARAM_TOTAL_DISTANCE              3     /*!< \brief Total Distance */
#define RSCP_SM_PARAM_STATUS                      4     /*!< \brief Walking or Running Status (0: walking, 1: running) */
/**@}*/

/** \name Running Speed and Cadence Running Status Values
 *
 */
/**@{*/
#define RSCP_SM_STATUS_WALKING                    0     /*!< \brief Walking */
#define RSCP_SM_STATUS_RUNNING                    1     /*!< \brief Running */
/**@}*/

/**************************************************************************************************
Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
*  \brief  Set a running speed measurement parameter.
*
*  \param  type   Parameter identifier
*  \param  value  Measurement value.
*
*  \return none
*/
/*************************************************************************************************/
void RscpsSetParameter(uint8_t type, uint32_t value);

/*************************************************************************************************/
/*!
*  \brief  Set the sensor location attribute.
*
*  \param  location   Sensor Location.
*
*  \return none
*/
/*************************************************************************************************/
void RscpsSetSensorLocation(uint8_t location);

/*************************************************************************************************/
/*!
*  \brief  Set the features attribute.
*
*  \param  features   Features bitmask.
*
*  \return none
*/
/*************************************************************************************************/
void RscpsSetFeatures(uint16_t features);

/*************************************************************************************************/
/*!
*  \brief  Notifies the collector of a Running Speed and Cadence Measurement.
*
*  \param  connId  Connection ID
*
*  \return none
*/
/*************************************************************************************************/
void RscpsSendSpeedMeasurement(dmConnId_t connId);

/*! \} */    /* RUNNING_SPEED_AND_CADENCE_PROFILE */

#ifdef __cplusplus
};
#endif

#endif /* RSCP_API_H */
