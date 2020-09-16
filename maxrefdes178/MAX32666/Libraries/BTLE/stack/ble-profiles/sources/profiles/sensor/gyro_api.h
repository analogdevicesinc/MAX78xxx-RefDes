/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example gyroscope service profile.
*
 *  Copyright (c) 2015-2018 Arm Ltd. All Rights Reserved.
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

#ifndef GYRO_API_H
#define GYRO_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup GYROSCOPE_SERVICE_PROFILE
 *  \{ */

#include "wsf_types.h"
#include "wsf_os.h"

/*************************************************************************************************/
/*!
 *  \brief  Start service.
 *
 *  \param  handlerId       Handler ID.
 *  \param  timerEvt        Timer message event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void GyroStart(wsfHandlerId_t handlerId, uint8_t timerEvt);

/*************************************************************************************************/
/*!
 *  \brief  Stop service.
 *
 *  \return None.
 */
/*************************************************************************************************/
void GyroStop(void);

/*************************************************************************************************/
/*!
 *  \brief  Measurement stop handler.
 *
 *  \return None.
 */
/*************************************************************************************************/
void GyroMeasStop(void);

/*************************************************************************************************/
/*!
 *  \brief  Measurement start handler.
 *
 *  \return None.
 */
/*************************************************************************************************/
void GyroMeasStart(void);

/*************************************************************************************************/
/*!
 *  \brief  Measurement complete handler.
 *
 *  \param  connId    Connection ID.
 *  \param  x         Gyroscope x-axis reading.
 *  \param  y         Gyroscope y-axis reading.
 *  \param  z         Gyroscope z-axis reading.
 *
 *  \return None.
 */
/*************************************************************************************************/
void GyroMeasComplete(dmConnId_t connId, int16_t x, int16_t y, int16_t z);

/*! \} */    /* GYROSCOPE_SERVICE_PROFILE */

#ifdef __cplusplus
};
#endif

#endif /* GYRO_API_H */
