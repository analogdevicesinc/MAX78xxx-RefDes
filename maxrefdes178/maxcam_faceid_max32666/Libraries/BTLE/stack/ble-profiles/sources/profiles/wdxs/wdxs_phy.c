/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Wireless Data Exchange profile implementation - Device PHY Configuration.
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

#include <string.h>
#include "wsf_types.h"
#include "util/wstr.h"
#include "wsf_trace.h"
#include "wsf_assert.h"
#include "wsf_efs.h"
#include "util/bstream.h"
#include "svc_wdxs.h"
#include "wdxs/wdxs_api.h"
#include "wdxs/wdxs_main.h"
#include "dm_api.h"
#include "app_api.h"
#include "app_hw.h"

#if WDXS_DC_ENABLED == TRUE

/*************************************************************************************************/
/*!
*  \brief  Process set PHY request.
*
*  \return ATT status.
*/
/*************************************************************************************************/
static uint8_t wdxsDcSetPhyReq(dmConnId_t connId, uint16_t len, uint8_t *pValue)
{
  uint8_t allPhys;
  uint8_t txPhys;
  uint8_t rxPhys;
  uint16_t phyOptions;

  /* verify parameter length */
  if (len != WDX_DC_LEN_PHY_UPDATE_REQ)
  {
    return ATT_ERR_LENGTH;
  }

  /* parse parameters */
  BSTREAM_TO_UINT8(allPhys, pValue);
  BSTREAM_TO_UINT8(txPhys, pValue);
  BSTREAM_TO_UINT8(rxPhys, pValue);
  BSTREAM_TO_UINT16(phyOptions, pValue);

  /* request update to PHY */
  DmSetPhy(connId, allPhys, txPhys, rxPhys, phyOptions);

  return ATT_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief  Process a Get PHY request.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
static uint8_t wdxsDcGetPhy(dmConnId_t connId, uint16_t len, uint8_t *pValue)
{
  return wdxsDcUpdatePhy(connId, HCI_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief  Process a PHY write to the device configuration characteristic.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
static uint8_t wdxsDcPhyWrite(dmConnId_t connId, uint8_t op, uint8_t id, uint16_t len, uint8_t *pValue)
{
  uint8_t status;

  /* set operation */
  if (op == WDX_DC_OP_SET)
  {
    switch (id)
    {
      case WDX_DC_ID_PHY_UPDATE_REQ:
        status = wdxsDcSetPhyReq(connId, len, pValue);
        break;

     default:
        status = ATT_ERR_RANGE;
        break;
     }
  }
  /* get operation */
  else if (op == WDX_DC_OP_GET)
  {
    switch (id)
    {
      case WDX_DC_ID_PHY:
        status = wdxsDcGetPhy(connId, len, pValue);
        break;

      default:
        status = ATT_ERR_RANGE;
        break;
    }
  }
  else
  {
    status = ATT_ERR_RANGE;
  }

  return status;
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize WDXS Device Configuration PHY.
 *
 *  \param  None
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxsPhyInit(void)
{
  /* register device configuration phy write callback */
  wdxsDcPhyRegister(wdxsDcPhyWrite);
}

#endif  /* WDXS_DC_ENABLED */
