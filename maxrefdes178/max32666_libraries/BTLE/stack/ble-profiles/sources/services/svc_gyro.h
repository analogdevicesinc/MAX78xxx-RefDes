/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example gyroscope sensor service implementation.
*
 *  Copyright (c) 2015-2018 Arm Ltd. All Rights Reserved.
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

#ifndef SVC_GYRO_H
#define SVC_GYRO_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup GYROSCOPE_SENSOR_SERVICE
 *  \{ */

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Gyroscope Sensor Service Handles
 *
 */
/**@{*/
#define GYRO_HANDLE_START  0x50                          /*!< \brief Start handle. */
#define GYRO_HANDLE_END   (GYRO_HANDLE_END_PLUS_ONE - 1) /*!< \brief End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Gyroscope service handles. */
enum
{
  GYRO_HANDLE_SVC = GYRO_HANDLE_START,    /*!< \brief Service declaration. */

  GYRO_HANDLE_DATA_CHR,                   /*!< \brief Data characteristic declaration. */
  GYRO_HANDLE_DATA,                       /*!< \brief Data characteristic value. */
  GYRO_HANDLE_DATA_CLIENT_CHR_CONFIG,     /*!< \brief Data characteristic CCCD. */
  GYRO_HANDLE_DATA_CHR_USR_DESCR,         /*!< \brief Data characteristic user description. */

  GYRO_HANDLE_TEMPDATA_CHR,               /*!< \brief Temporary data characteristic declaration. */
  GYRO_HANDLE_TEMPDATA,                   /*!< \brief Temporary data characteristic value. */
  GYRO_HANDLE_TEMPDATA_CLIENT_CHR_CONFIG, /*!< \brief Temporary data characteristic CCCD. */
  GYRO_HANDLE_TEMPDATA_CHR_USR_DESCR,     /*!< \brief Temporary data characteristic user description. */

  GYRO_HANDLE_CONFIG_CHR,                 /*!< \brief Configuration characteristic delcaration. */
  GYRO_HANDLE_CONFIG,                     /*!< \brief Configuration characteristic value. */
  GYRO_HANDLE_CONFIG_CHR_USR_DESCR,       /*!< \brief Configuration characteristic user description. */

  GYRO_HANDLE_PERIOD_CHR,                 /*!< \brief Period characteristic declaration. */
  GYRO_HANDLE_PERIOD,                     /*!< \brief Period characteristic value. */
  GYRO_HANDLE_PERIOD_CHR_USR_DESCR,       /*!< \brief Period characteristic user description. */

  GYRO_HANDLE_END_PLUS_ONE                /*!< \brief Maximum handle. */
};
/**@}*/

/**************************************************************************************************
 Macros
**************************************************************************************************/

/** \name Values for Config Attributes
 *
 */
/**@{*/
#define GYRO_ATT_CONFIG_DISABLE  0x00u /*!< \brief Disable */
#define GYRO_ATT_CONFIG_ENABLE   0x01u /*!< \brief Enable */
/**@}*/

/** \name Values for Period Attributes.
 *
 */
/**@{*/
#define GYRO_ATT_PERIOD_MAX      250u /*!< \brief Maximum period */
#define GYRO_ATT_PERIOD_DEFAULT  100u /*!< \brief Default period */
#define GYRO_ATT_PERIOD_MIN       10u /*!< \brief Minimum period */
/**@}*/

/*! \brief Sizes of attributes. */
#define GYRO_SIZE_CONFIG_ATT       1u /*!< \brief Configuration attribute size */
#define GYRO_SIZE_PERIOD_ATT       1u /*!< \brief Period attribute size */
#define GYRO_SIZE_DATA_ATT         6u /*!< \brief Data attribute size */
#define GYRO_SIZE_TEMPDATA_ATT     2u /*!< \brief Temp data attribute size */
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
void SvcGyroAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcGyroRemoveGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Register callbacks for the service.
 *
 *  \param  writeCback  Write callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcGyroCbackRegister(attsWriteCback_t writeCback);

/*! \} */    /* GYROSCOPE_SENSOR_SERVICE */

#ifdef __cplusplus
}
#endif

#endif /* SVC_GYRO_H */
