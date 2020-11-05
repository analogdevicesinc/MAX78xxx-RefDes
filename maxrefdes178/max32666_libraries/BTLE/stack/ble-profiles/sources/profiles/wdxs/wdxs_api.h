/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Wireless Data Exchange profile application interface.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
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

#ifndef WDXS_API_H
#define WDXS_API_H

#include "att_api.h"
#include "wdx_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup WIRELESS_DATA_EXCHANGE_PROFILE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief Size of RAM Media used by the application */
#define WDXS_APP_RAM_MEDIA_SIZE         256

/*! \brief Device Model Name */
#ifndef WDXS_DEVICE_MODEL
#define WDXS_DEVICE_MODEL               "WDXS App"
#endif

/*************************************************************************************************/
/*!
 *  \brief  Called at startup to configure WDXS authentication.
 *
 *  \param  reqLevel  Level of authentication that is required for a client to use WDXS
 *  \param  pKey      Authentication key (set to NULL if no authentication is required)
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxsAuthenticationCfg(bool_t reqLevel, uint8_t *pKey);

/*************************************************************************************************/
/*!
 *  \brief  Handle WSF events for WDXS.
 *
 *  \param  event  event
 *  \param  pMsg   message assiciated with event
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxsHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  WSF Task Initialization for WDXS task.
 *
 *  \param  handlerId   ID of the WDXS task
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxsHandlerInit(wsfHandlerId_t handlerId);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the WDXS of ATT Events.
 *
 *  \param  pEvt   Pointer to the ATT Event
 *
 *  \return TRUE if the application should ignore the event, else FALSE.
 */
/*************************************************************************************************/
uint8_t WdxsAttCback(attEvt_t *pEvt);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the WDXS of DM Events.
 *
 *  \param  pEvt   Pointer to the DM Event
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxsProcDmMsg(dmEvt_t *pEvt);

/*************************************************************************************************/
/*!
 *  \brief  Set the CCCD index used by the application for WDXS service characteristics.
 *
 *  \param  dcCccIdx   Device Control CCCD index.
 *  \param  auCccIdx   Authentication CCCD index.
 *  \param  ftcCccIdx  File Transfer Control CCCD index.
 *  \param  ftdCccIdx  File Transfer Data CCCD index.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxsSetCccIdx(uint8_t dcCccIdx, uint8_t auCccIdx, uint8_t ftcCccIdx, uint8_t ftdCccIdx);

/*************************************************************************************************/
/*!
 *  \brief  Registers the platform dependent Flash Media with the Embedded File System (EFS)
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxsFlashMediaInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Registers the platform dependent OTA Media with the Embedded File System (EFS)
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxsOtaMediaInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Resets the system.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxsResetSystem(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize WDXS Device Configuration PHY.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxsPhyInit(void);

/*! \} */    /* WIRELESS_DATA_EXCHANGE_PROFILE */

#ifdef __cplusplus
}
#endif

#endif /* WDXS_API_H */
