/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Battery service server.
 *
 *  Copyright (c) 2012-2018 Arm Ltd. All Rights Reserved.
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
#ifndef BAS_API_H
#define BAS_API_H

#include "wsf_timer.h"
#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup BATTERY_PROFILE
 *  \{ */

/**************************************************************************************************
  Data Types
**************************************************************************************************/


/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*! \brief Battery service configurable parameters */
typedef struct
{
  wsfTimerTicks_t     period;     /*!< \brief Battery measurement timer expiration period in seconds */
  uint16_t            count;      /*!< \brief Perform battery measurement after this many timer periods */
  uint8_t             threshold;  /*!< \brief Send battery level notification to peer when below this level. */
} basCfg_t;

/*************************************************************************************************/
/*!
 *  \brief  Initialize the battery service server.
 *
 *  \param  handlerId    WSF handler ID of the application using this service.
 *  \param  pCfg        Battery service configurable parameters.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BasInit(wsfHandlerId_t handlerId, basCfg_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief  Start periodic battery level measurement.  This function starts a timer to perform
 *          periodic battery measurements.
 *
 *  \param  connId      DM connection identifier.
 *  \param  timerEvt    WSF event designated by the application for the timer.
 *  \param  battCccIdx  Index of battery level CCC descriptor in CCC descriptor handle table.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BasMeasBattStart(dmConnId_t connId, uint8_t timerEvt, uint8_t battCccIdx);

/*************************************************************************************************/
/*!
 *  \brief  Stop periodic battery level measurement.
 *
 *  \param  connId      DM connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BasMeasBattStop(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Process received WSF message.
 *
 *  \param  pMsg     Event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BasProcMsg(wsfMsgHdr_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Send the battery level to the peer device.
 *
 *  \param  connId      DM connection identifier.
 *  \param  idx         Index of battery level CCC descriptor in CCC descriptor handle table.
 *  \param  level       The battery level.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BasSendBattLevel(dmConnId_t connId, uint8_t idx, uint8_t level);

/*************************************************************************************************/
/*!
 *  \brief  ATTS read callback for battery service used to read the battery level.  Use this
 *          function as a parameter to SvcBattCbackRegister().
 *
 *  \param  connId    DM connection identifier.
 *  \param  handle    ATT handle.
 *  \param  operation ATT operation.
 *  \param  offset    read offset.
 *  \param  pAttr     pointer to Attribute
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t BasReadCback(dmConnId_t connId, uint16_t handle, uint8_t operation,
                     uint16_t offset, attsAttr_t *pAttr);

/*! \} */    /* BATTERY_PROFILE */

#ifdef __cplusplus
};
#endif

#endif /* BAS_API_H */
