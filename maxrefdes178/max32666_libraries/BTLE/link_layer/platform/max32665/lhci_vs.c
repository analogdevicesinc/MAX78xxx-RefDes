/*******************************************************************************
 * Copyright (C) 2019 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 * $Date: 2019-06-18 23:15:00 +0300 (Tue, 18 Jun 2019) $
 * $Revision: 44064 $
 *
 ******************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief 
 */
/*************************************************************************************************/

#include "lhci_int.h"
#include "bb_ble_api.h"
#include "bb_ble_drv_vs.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/
#define LHCI_OPCODE_VS_SET_DEF_TX_POWER           HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3DD)  /*!< Set default transmit power. */
#define LHCI_OPCODE_VS_CALIBRATE                  HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3DE)  /*!< Run AFE calibration. */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Build and send a command complete event packet.
 *
 *  \param  pCmdHdr     Command HCI header.
 *  \param  status      Status value.
 *  \param  paramLen    Parameter length of the command status event.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lhciVsCommonSendCmdCmplEvt(LhciHdr_t *pCmdHdr, uint8_t status, uint8_t paramLen)
{
  uint8_t *pEvtBuf;
  uint8_t *pBuf;

  if ((pEvtBuf = lhciAllocCmdCmplEvt(paramLen, pCmdHdr->opCode)) == NULL)
  {
    return;
  }

  pBuf = pEvtBuf;
  pBuf += lhciPackCmdCompleteEvtStatus(pEvtBuf, status);

  switch (pCmdHdr->opCode)
  {
        /* --- command completion with status only parameter --- */
    case LHCI_OPCODE_VS_SET_BD_ADDR:
    case LHCI_OPCODE_VS_SET_DEF_TX_POWER:
    case LHCI_OPCODE_VS_SET_CONN_TX_PWR:
    case LHCI_OPCODE_VS_SET_LOCAL_FEAT:
    case LHCI_OPCODE_VS_CALIBRATE:
    default:
      break;

    case LHCI_OPCODE_VS_GET_ADV_STATS:
    {
      BbBleAdvPktStats_t stats;
      BbBleGetAdvStats(&stats);
      UINT32_TO_BSTREAM(pBuf, stats.txAdv);
      UINT32_TO_BSTREAM(pBuf, stats.rxReq);
      UINT32_TO_BSTREAM(pBuf, stats.rxReqCrc);
      UINT32_TO_BSTREAM(pBuf, stats.rxReqTimeout);
      UINT32_TO_BSTREAM(pBuf, stats.txRsp);
      UINT32_TO_BSTREAM(pBuf, stats.errAdv);
      UINT16_TO_BSTREAM(pBuf, stats.rxSetupUsec);
      UINT16_TO_BSTREAM(pBuf, stats.txSetupUsec);
      UINT16_TO_BSTREAM(pBuf, stats.rxIsrUsec);
      UINT16_TO_BSTREAM(pBuf, stats.txIsrUsec);
      break;
    }
    case LHCI_OPCODE_VS_GET_SCAN_STATS:
    {
      BbBleScanPktStats_t stats;
      BbBleGetScanStats(&stats);
      UINT32_TO_BSTREAM(pBuf, stats.rxAdv);
      UINT32_TO_BSTREAM(pBuf, stats.rxAdvCrc);
      UINT32_TO_BSTREAM(pBuf, stats.rxAdvTimeout);
      UINT32_TO_BSTREAM(pBuf, stats.txReq);
      UINT32_TO_BSTREAM(pBuf, stats.rxRsp);
      UINT32_TO_BSTREAM(pBuf, stats.rxRspCrc);
      UINT32_TO_BSTREAM(pBuf, stats.rxRspTimeout);
      UINT32_TO_BSTREAM(pBuf, stats.errScan);
      UINT16_TO_BSTREAM(pBuf, stats.rxSetupUsec);
      UINT16_TO_BSTREAM(pBuf, stats.txSetupUsec);
      UINT16_TO_BSTREAM(pBuf, stats.rxIsrUsec);
      UINT16_TO_BSTREAM(pBuf, stats.txIsrUsec);
      break;
    }
    case LHCI_OPCODE_VS_GET_CONN_STATS:
    {
      BbBleDataPktStats_t stats;
      BbBleGetConnStats(&stats);
      UINT32_TO_BSTREAM(pBuf, stats.rxData);
      UINT32_TO_BSTREAM(pBuf, stats.rxDataCrc);
      UINT32_TO_BSTREAM(pBuf, stats.rxDataTimeout);
      UINT32_TO_BSTREAM(pBuf, stats.txData);
      UINT32_TO_BSTREAM(pBuf, stats.errData);
      UINT16_TO_BSTREAM(pBuf, stats.rxSetupUsec);
      UINT16_TO_BSTREAM(pBuf, stats.txSetupUsec);
      UINT16_TO_BSTREAM(pBuf, stats.rxIsrUsec);
      UINT16_TO_BSTREAM(pBuf, stats.txIsrUsec);
      break;
    }
    case LHCI_OPCODE_VS_GET_TEST_STATS:
    {
      BbBleDataPktStats_t stats;
      BbBleGetTestStats(&stats);
      UINT32_TO_BSTREAM(pBuf, stats.rxData);
      UINT32_TO_BSTREAM(pBuf, stats.rxDataCrc);
      UINT32_TO_BSTREAM(pBuf, stats.rxDataTimeout);
      UINT32_TO_BSTREAM(pBuf, stats.txData);
      UINT32_TO_BSTREAM(pBuf, stats.errData);
      UINT16_TO_BSTREAM(pBuf, stats.rxSetupUsec);
      UINT16_TO_BSTREAM(pBuf, stats.txSetupUsec);
      UINT16_TO_BSTREAM(pBuf, stats.rxIsrUsec);
      UINT16_TO_BSTREAM(pBuf, stats.txIsrUsec);
      break;
    }
  }

  lhciSendCmdCmplEvt(pEvtBuf);
}


/*************************************************************************************************/
bool_t lhciCommonVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
    uint8_t status = HCI_SUCCESS;
    uint8_t paramLen = 0;

    switch(pHdr->opCode) {

      case LHCI_OPCODE_VS_SET_BD_ADDR:
        LlSetBdAddr(pBuf);
        break;
      case LHCI_OPCODE_VS_SET_DEF_TX_POWER:
      {
        int8_t power;
        BSTREAM_TO_INT8(power, pBuf);
        BbBleDrvSetTxPower(power);
        break;
      }
      case LHCI_OPCODE_VS_SET_LOCAL_FEAT:
        status = LlSetFeatures(pBuf);
        break;
      case LHCI_OPCODE_VS_GET_TEST_STATS:
        paramLen += sizeof(BbBleDataPktStats_t);
        break;
      case LHCI_OPCODE_VS_CALIBRATE:
        BbBleDrvCalibrate();
        break;

      default:
          return FALSE;
    }

    lhciVsCommonSendCmdCmplEvt(pHdr, status, paramLen);
    return TRUE;
}

/*************************************************************************************************/
bool_t lhciConnVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
    uint8_t status = HCI_SUCCESS;
    uint8_t paramLen = 0;

    switch(pHdr->opCode) {

      case LHCI_OPCODE_VS_SET_CONN_TX_PWR:
      {
        uint16_t handle;
        int8_t level;
        BSTREAM_TO_UINT16(handle, pBuf);
        BSTREAM_TO_INT8 (level, pBuf);
        status = LlSetTxPowerLevel(handle, level);
        break;
      }
      case LHCI_OPCODE_VS_GET_CONN_STATS:
        paramLen += sizeof(BbBleDataPktStats_t);
        break;

  /* --- default --- */

        default:
            return FALSE;
    }

    lhciVsCommonSendCmdCmplEvt(pHdr, status, paramLen);
    return TRUE;
}

/*************************************************************************************************/
bool_t lhciMstConnVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
  return FALSE;
}

/*************************************************************************************************/
bool_t lhciMstScanVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
  uint8_t status = HCI_SUCCESS;
  uint8_t paramLen = 0;

  switch(pHdr->opCode) {

    /* --- local device management --- */

    case LHCI_OPCODE_VS_GET_SCAN_STATS:
      paramLen += sizeof(BbBleScanPktStats_t);
      break;

    /* --- default --- */

    default:
      return FALSE;
  }

  lhciVsCommonSendCmdCmplEvt(pHdr, status, paramLen);
  return TRUE;
}

/*************************************************************************************************/
bool_t lhciSlvAdvVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
  uint8_t status = HCI_SUCCESS;
  uint8_t paramLen = 0;

  switch(pHdr->opCode) {

    /* --- local device management --- */

    case LHCI_OPCODE_VS_SET_ADV_TX_PWR:
      LlSetAdvTxPower(pBuf[0]);
      break;
    case LHCI_OPCODE_VS_GET_ADV_STATS:
      paramLen += sizeof(BbBleAdvPktStats_t);
      break;

    /* --- default --- */

    default:
      return FALSE;
  }

  lhciVsCommonSendCmdCmplEvt(pHdr, status, paramLen);
  return TRUE;
}

/*************************************************************************************************/
bool_t lhciSlvEncVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
    return FALSE;
}

/*************************************************************************************************/
bool_t lhciScVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
    return FALSE;
}

/*************************************************************************************************/
bool_t lhciMstExtScanVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
    return FALSE;
}

/*************************************************************************************************/
bool_t lhciSlvExtAdvVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
    return FALSE;
}
