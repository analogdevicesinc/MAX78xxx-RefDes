/* Copyright (c) 2009-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#include "hci_defs.h"
#include "hci_api.h"
#include "wsf_os.h"
#include "wsf_trace.h"
#include "hci_main.h"
#include "hci_core.h"
#include "hci_vs.h"
#include "bb_ble_api.h"
#include "bstream.h"
#include "hci_drv_sdma.h"

#ifdef _RTE_
#include "RTE_Components.h"             // Component selection
#endif


/**************************************************************************************************
  Macros
**************************************************************************************************/
/* Cordio vendor specific OCF range is 0x3E0-0x3FF. */
// #define HCI_OPCODE_VS_SET_SCAN_CH_MAP           HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3E0)  /*!< Set Scan Channel Map opcode. */
// #define HCI_OPCODE_VS_SET_EVENT_MASK            HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3E1)  /*!< Set Vendor Specific Event Mask opcode. */
// #define HCI_OPCODE_VS_SET_RSRC_MGR_MODE         HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3E2)  /*!< DEPRECATED. */
// #define HCI_OPCODE_VS_ENA_ACL_SINK              HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3E3)  /*!< Enable ACL Packet Sink opcode. */
// #define HCI_OPCODE_VS_GENERATE_ACL              HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3E4)  /*!< Generate ACL Packets opcode. */
// #define HCI_OPCODE_VS_ENA_AUTO_GEN_ACL          HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3E5)  /*!< Enable Auto Generate ACL Packets opcode. */
// #define HCI_OPCODE_VS_SET_TX_TEST_ERR_PATT      HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3E6)  /*!< Set Tx Test Error Pattern opcode. */
// #define HCI_OPCODE_VS_SET_CONN_OP_FLAGS         HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3E7)  /*!< Set Connection Operational Flags opcode. */
// #define HCI_OPCODE_VS_SET_P256_PRIV_KEY         HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3E8)  /*!< Set P-256 Private Key opcode. */
// #define HCI_OPCODE_VS_GET_ACL_TEST_REPORT       HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3E9)  /*!< Get ACL Test Report opcode. */
// #define HCI_OPCODE_VS_SET_LOCAL_MIN_USED_CHAN   HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3EA)  /*!< Set local minimum number of used channels. */
// #define HCI_OPCODE_VS_GET_PEER_MIN_USED_CHAN    HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3EB)  /*!< Get peer minimum number of used channels. */
#define HCI_OPCODE_VS_SET_BD_ADDR               HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3F0)  /*!< Set BD address opcode. */
// #define HCI_OPCODE_VS_GET_RAND_ADDR             HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3F1)  /*!< Get Random Address opcode. */
#define HCI_OPCODE_VS_SET_LOCAL_FEAT            HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3F2)  /*!< Set Local Feature opcode. */
// #define HCI_OPCODE_VS_SET_OP_FLAGS              HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3F3)  /*!< Set Operational Flags opcode. */
// #define HCI_OPCODE_VS_SET_ADV_TX_PWR            HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3F5)  /*!< Set Advertising Tx Power opcode. */
#define HCI_OPCODE_VS_SET_CONN_TX_PWR           HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3F6)  /*!< Set Connection Tx Power opcode. */
// #define HCI_OPCODE_VS_SET_ENC_MODE              HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3F7)  /*!< Set Encryption Mode opcode. */
// #define HCI_OPCODE_VS_SET_CHAN_MAP              HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3F8)  /*!< Set Channel Map opcode. */

// #define HCI_OPCODE_VS_SET_DIAG_MODE             HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3F9)  /*!< Set Diagnostic Mode opcode. */

// #define HCI_OPCODE_VS_GET_PDU_FILT_STATS        HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3F4)  /*!< Get PDU Filter Statistics opcode. */
// #define HCI_OPCODE_VS_GET_SYS_STATS             HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3FA)  /*!< Get Memory Statistics opcode. */
#define HCI_OPCODE_VS_GET_ADV_STATS             HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3FB)  /*!< Get Advertising Statistics opcode. */
#define HCI_OPCODE_VS_GET_SCAN_STATS            HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3FC)  /*!< Get Scan Statistics opcode. */
#define HCI_OPCODE_VS_GET_CONN_STATS            HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3FD)  /*!< Get Connection Statistics opcode. */
#define HCI_OPCODE_VS_GET_TEST_STATS            HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3FE)  /*!< Get Test Statistics opcode. */
// #define HCI_OPCODE_VS_GET_POOL_STATS            HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3FF)  /*!< Get Pool Statistics opcode. */

// #define HCI_OPCODE_VS_SET_AUX_DELAY             HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3D0)  /*!< Set Additional AuxPtr offset. */
// #define HCI_OPCODE_VS_SET_EXT_ADV_FRAG_LEN      HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3D1)  /*!< Set extended advertising data fragmentation length. */
// #define HCI_OPCODE_VS_SET_EXT_ADV_PHY_OPTS      HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3D2)  /*!< Set extended advertising PHY options. */
// #define HCI_OPCODE_VS_SET_EXT_ADV_DEF_PHY_OPTS  HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3D3)  /*!< Set extended advertising default PHY options. */

// #define HCI_OPCODE_VS_GET_AUX_ADV_STATS         HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3DA)  /*!< Get Auxiliary Advertising Statistics opcode. */
// #define HCI_OPCODE_VS_GET_AUX_SCAN_STATS        HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3DB)  /*!< Get Auxiliary Scanning Statistics opcode. */
// #define HCI_OPCODE_VS_GET_PER_SCAN_STATS        HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3DC)  /*!< Get Periodic Scanning Statistics opcode. */

#define HCI_OPCODE_VS_SET_DEF_TX_POWER         HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3DD)  /*!< Set default transmit power. */
#define HCI_OPCODE_VS_CALIBRATE                HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3DE)  /*!< Run AFE calibration. */

/*************************************************************************************************/
/*!
 *  \fn     hciCoreResetStart
 *
 *  \brief  Start the HCI reset sequence.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciCoreResetStart(void)
{

#ifdef ENABLE_SDMA
  hciDrvInit();
#endif

  /* Reset the controller to start the sequence */
  HciResetCmd();
}

/*************************************************************************************************/
/*!
 *  \fn     hciCoreResetSequence
 *
 *  \brief  Implement the HCI reset sequence.
 *
 *  \param  pMsg    HCI event message from previous command in the sequence.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciCoreResetSequence(uint8_t *pMsg) 
{ 
 uint16_t       opcode;
  wsfMsgHdr_t    hdr;
  static uint8_t randCnt;

  /* if event is a command complete event */
  if (*pMsg == HCI_CMD_CMPL_EVT)
  {
    /* parse parameters */
    pMsg += HCI_EVT_HDR_LEN;
    pMsg++;                   /* skip num packets */
    BSTREAM_TO_UINT16(opcode, pMsg);
    pMsg++;                   /* skip status */
    
    /* decode opcode */
    switch (opcode)
    {
      case HCI_OPCODE_RESET:
        /* initialize rand command count */
        randCnt = 0;

        /* send next command in sequence */
        HciSetEventMaskCmd((uint8_t *) hciEventMask);
        break;

      case HCI_OPCODE_SET_EVENT_MASK:
        /* send next command in sequence */
        HciSetEventMaskPage2Cmd((uint8_t *)hciEventMaskPage2);
        break;

      case HCI_OPCODE_SET_EVENT_MASK_PAGE2:
        /* send next command in sequence */
        HciLeSetEventMaskCmd((uint8_t*)hciLeEventMask);
        break;

      case HCI_OPCODE_LE_SET_EVENT_MASK:
        /* send next command in sequence */
        HciReadBdAddrCmd();
        break;

      case HCI_OPCODE_READ_BD_ADDR:
        /* parse and store event parameters */
        BdaCpy(hciCoreCb.bdAddr, pMsg);

        /* send next command in sequence */
        HciLeReadBufSizeCmd();
        break;

      case HCI_OPCODE_LE_READ_BUF_SIZE:
        /* parse and store event parameters */
        BSTREAM_TO_UINT16(hciCoreCb.bufSize, pMsg);
        BSTREAM_TO_UINT8(hciCoreCb.numBufs, pMsg);

        /* initialize ACL buffer accounting */
        hciCoreCb.availBufs = hciCoreCb.numBufs;

        /* send next command in sequence */
        HciLeReadSupStatesCmd();
        break;

      case HCI_OPCODE_LE_READ_SUP_STATES:
        /* parse and store event parameters */
        memcpy(hciCoreCb.leStates, pMsg, HCI_LE_STATES_LEN);

        /* send next command in sequence */
        HciLeReadWhiteListSizeCmd();
        break;

      case HCI_OPCODE_LE_READ_WHITE_LIST_SIZE:
        /* parse and store event parameters */
        BSTREAM_TO_UINT8(hciCoreCb.whiteListSize, pMsg);

        /* send next command in sequence */
        HciLeRandCmd();
        break;

      case HCI_OPCODE_LE_RAND:
        /* check if need to send second rand command */
        if (randCnt == 0)
        {
          randCnt++;
          HciLeRandCmd();
        }
        else
        {
          /* last command in sequence; set resetting state and call callback */
          hciCb.resetting = FALSE;
          hdr.param = 1;
          hdr.event = HCI_RESET_SEQ_CMPL_CBACK_EVT;
          (*hciCb.evtCback)((hciEvt_t *) &hdr);
        }
        break;

      default:
        break;
    }
  }
  /* else if vendor specific event */
  else if (*pMsg == HCI_VENDOR_SPEC_EVT)
  {
  }
}

/*************************************************************************************************/
/*!
 *  \fn     hciCoreVsCmdCmplRcvd
 *
 *  \brief  Perform internal HCI processing of vendor specific command complete events.
 *
 *  \param  opcode  HCI command opcode.
 *  \param  pMsg    Pointer to input HCI event parameter byte stream.
 *  \param  len     Parameter byte stream length.
 *
 *  \return HCI callback event code or zero.
 */
/*************************************************************************************************/
uint8_t hciCoreVsCmdCmplRcvd(uint16_t opcode, uint8_t *pMsg, uint8_t len)
{
  switch (opcode) {
    case HCI_OPCODE_VS_GET_ADV_STATS:
    {
#if WSF_TRACE_ENABLED
      uint8_t status;
      BbBleAdvPktStats_t stats;

      BSTREAM_TO_UINT8(status, pMsg);
      if(status != HCI_SUCCESS) {
        HCI_TRACE_ERR0("Error with HCI_OPCODE_VS_GET_ADV_STATS");
        break;
      }

      BSTREAM_TO_UINT32(stats.txAdv, pMsg);
      BSTREAM_TO_UINT32(stats.rxReq, pMsg);
      BSTREAM_TO_UINT32(stats.rxReqCrc, pMsg);
      BSTREAM_TO_UINT32(stats.rxReqTimeout, pMsg);
      BSTREAM_TO_UINT32(stats.txRsp, pMsg);
      BSTREAM_TO_UINT32(stats.errAdv, pMsg);
      BSTREAM_TO_UINT16(stats.rxSetupUsec, pMsg);
      BSTREAM_TO_UINT16(stats.txSetupUsec, pMsg);
      BSTREAM_TO_UINT16(stats.rxIsrUsec, pMsg);
      BSTREAM_TO_UINT16(stats.txIsrUsec, pMsg);

      HCI_TRACE_INFO0("Advertising stats:");
      HCI_TRACE_INFO3("txAdv        = 0x%04x rxReq       = 0x%04x rxReqCrc = 0x%04x", stats.txAdv, stats.rxReq, stats.rxReqCrc);
      HCI_TRACE_INFO3("rxReqTimeout = 0x%04x txRsp       = 0x%04x errAdv   = 0x%04x", stats.rxReqTimeout, stats.txRsp, stats.errAdv);
      HCI_TRACE_INFO2("rxSetupUsec  = 0x%04x txSetupUsec = 0x%04x", stats.rxSetupUsec, stats.txSetupUsec);
      HCI_TRACE_INFO2("rxIsrUsec    = 0x%04x txIsrUsec   = 0x%04x", stats.rxSetupUsec, stats.txSetupUsec);
#endif
      break;
    }

    case HCI_OPCODE_VS_GET_SCAN_STATS:
    {
#if WSF_TRACE_ENABLED
      uint8_t status;
      BbBleScanPktStats_t stats;

      BSTREAM_TO_UINT8(status, pMsg);
      if(status != HCI_SUCCESS) {
        HCI_TRACE_ERR0("Error with HCI_OPCODE_VS_GET_SCAN_STATS");
        break;
      }

      BSTREAM_TO_UINT32(stats.rxAdv, pMsg);
      BSTREAM_TO_UINT32(stats.rxAdvCrc, pMsg);
      BSTREAM_TO_UINT32(stats.rxAdvTimeout, pMsg);
      BSTREAM_TO_UINT32(stats.txReq, pMsg);
      BSTREAM_TO_UINT32(stats.rxRsp, pMsg);
      BSTREAM_TO_UINT32(stats.rxRspCrc, pMsg);
      BSTREAM_TO_UINT32(stats.rxRspTimeout, pMsg);
      BSTREAM_TO_UINT32(stats.errScan, pMsg);
      BSTREAM_TO_UINT16(stats.rxSetupUsec, pMsg);
      BSTREAM_TO_UINT16(stats.txSetupUsec, pMsg);
      BSTREAM_TO_UINT16(stats.rxIsrUsec, pMsg);
      BSTREAM_TO_UINT16(stats.txIsrUsec, pMsg);

      HCI_TRACE_INFO0("Scanning stats:");
      HCI_TRACE_INFO3("rxAdv        = 0x%04x rxAdvCrc    = 0x%04x rxAdvTimeout = 0x%04x", stats.rxAdv, stats.rxAdvCrc, stats.rxAdvTimeout);
      HCI_TRACE_INFO3("txReq        = 0x%04x rxRsp       = 0x%04x rxRspCrc       = 0x%04x", stats.txReq, stats.rxRsp, stats.rxRspCrc);
      HCI_TRACE_INFO2("rxRspTimeout = 0x%04x errScan     = 0x%04x", stats.rxRspTimeout, stats.errScan);
      HCI_TRACE_INFO2("rxSetupUsec  = 0x%04x txSetupUsec = 0x%04x", stats.rxSetupUsec, stats.txSetupUsec);
      HCI_TRACE_INFO2("rxIsrUsec    = 0x%04x txIsrUsec   = 0x%04x", stats.rxSetupUsec, stats.txSetupUsec);
#endif
      break;
    }

    case HCI_OPCODE_VS_GET_CONN_STATS:
    {
#if WSF_TRACE_ENABLED
      float crc,timeout,ok, per;
      unsigned decimalError, intError;
      uint8_t status;
      BbBleDataPktStats_t stats;

      BSTREAM_TO_UINT8(status, pMsg);
      if(status != HCI_SUCCESS) {
        HCI_TRACE_ERR0("Error with HCI_OPCODE_VS_GET_CONN_STATS");
        break;
      }

      BSTREAM_TO_UINT32(stats.rxData, pMsg);
      BSTREAM_TO_UINT32(stats.rxDataCrc, pMsg);
      BSTREAM_TO_UINT32(stats.rxDataTimeout, pMsg);
      BSTREAM_TO_UINT32(stats.txData, pMsg);
      BSTREAM_TO_UINT32(stats.errData, pMsg);
      BSTREAM_TO_UINT16(stats.rxSetupUsec, pMsg);
      BSTREAM_TO_UINT16(stats.txSetupUsec, pMsg);
      BSTREAM_TO_UINT16(stats.rxIsrUsec, pMsg);
      BSTREAM_TO_UINT16(stats.txIsrUsec, pMsg);

      HCI_TRACE_INFO0("Connection stats:");
      HCI_TRACE_INFO3("rxData = 0x%04x rxDataCrc = 0x%04x rxDataTimeout = 0x%04x", stats.rxData, stats.rxDataCrc, stats.rxDataTimeout);
      HCI_TRACE_INFO2("txData = 0x%04x drrData   = 0x%04x ", stats.txData, stats.errData);
    
      /* Calculate PER */
      crc = (float)stats.rxDataCrc;
      timeout = (float)stats.rxDataTimeout;
      ok = (float)stats.rxData;
      per = 100.0 * (crc + timeout)/(crc + timeout + ok);
      intError = (unsigned)per;
      per = per * 100.0;
      decimalError = (unsigned)per;
      
      /* Print the leading zero */
      if(decimalError < 10) {
        HCI_TRACE_INFO2("PER = %d.0%d %\n", intError, decimalError);
      } else {
        HCI_TRACE_INFO2("PER = %d.%d %\n", intError, decimalError);
      }
      HCI_TRACE_INFO2("rxSetupUsec  = 0x%04x txSetupUsec = 0x%04x", stats.rxSetupUsec, stats.txSetupUsec);
      HCI_TRACE_INFO2("rxIsrUsec    = 0x%04x txIsrUsec   = 0x%04x", stats.rxSetupUsec, stats.txSetupUsec);
#endif
      break;
    }

    case HCI_OPCODE_VS_GET_TEST_STATS:
    {
#if WSF_TRACE_ENABLED
      uint8_t status;
      BbBleDataPktStats_t stats;

      BSTREAM_TO_UINT8(status, pMsg);
      if(status != HCI_SUCCESS) {
        HCI_TRACE_ERR0("Error with HCI_OPCODE_VS_GET_TEST_STATS");
        break;
      }

      BSTREAM_TO_UINT32(stats.rxData, pMsg);
      BSTREAM_TO_UINT32(stats.rxDataCrc, pMsg);
      BSTREAM_TO_UINT32(stats.rxDataTimeout, pMsg);
      BSTREAM_TO_UINT32(stats.txData, pMsg);
      BSTREAM_TO_UINT32(stats.errData, pMsg);
      BSTREAM_TO_UINT16(stats.rxSetupUsec, pMsg);
      BSTREAM_TO_UINT16(stats.txSetupUsec, pMsg);
      BSTREAM_TO_UINT16(stats.rxIsrUsec, pMsg);
      BSTREAM_TO_UINT16(stats.txIsrUsec, pMsg);

      HCI_TRACE_INFO0("Test stats:");
      HCI_TRACE_INFO3("rxData = 0x%04x rxDataCrc = 0x%04x rxDataTimeout = 0x%04x", stats.rxData, stats.rxDataCrc, stats.rxDataTimeout);
      HCI_TRACE_INFO2("txData = 0x%04x drrData   = 0x%04x ", stats.txData, stats.errData);
      HCI_TRACE_INFO2("rxSetupUsec  = 0x%04x txSetupUsec = 0x%04x", stats.rxSetupUsec, stats.txSetupUsec);
      HCI_TRACE_INFO2("rxIsrUsec    = 0x%04x txIsrUsec   = 0x%04x", stats.rxSetupUsec, stats.txSetupUsec);
#endif  
      break;
    }

    default:
      break;
  }
  return HCI_VENDOR_SPEC_CMD_CMPL_CBACK_EVT;
}

/*************************************************************************************************/
/*!
 *  \fn     hciCoreVsEvtRcvd
 *
 *  \brief  Perform internal HCI processing of vendor specific HCI events.
 *
 *  \param  p       Pointer to input HCI event parameter byte stream.
 *  \param  len     Parameter byte stream length.
 *
 *  \return HCI callback event code or zero.
 */
/*************************************************************************************************/
uint8_t hciCoreVsEvtRcvd(uint8_t *p, uint8_t len)
{
  return HCI_VENDOR_SPEC_EVT;
}

/*************************************************************************************************/
/*!
 *  \fn     hciCoreHwErrorRcvd
 *
 *  \brief  Perform internal HCI processing of hardware error event.
 *
 *  \param  p       Pointer to input HCI event parameter byte stream.
 *
 *  \return HCI callback event code or zero.
 */
/*************************************************************************************************/
uint8_t hciCoreHwErrorRcvd(uint8_t *p)
{
  return 0;
}

/*************************************************************************************************/
/*!
 *  \fn     HciVsInit
 *
 *  \brief  Vendor-specific controller initialization function.
 *
 *  \param  param    Vendor-specific parameter.
 *
 *  \return None
 */
/*************************************************************************************************/
void HciVsInit(uint8_t param)
{
  // TODO: maxAdvDataLen should be increased and set inside of reset sequence.
  hciCoreCb.maxAdvDataLen = HCI_EXT_ADV_DATA_LEN;
  hciCoreCb.extResetSeq = NULL;
}

/*************************************************************************************************/
void HciVsSetBdAddr(uint8_t *pAddr)
{
  BdaCpy(hciCoreCb.bdAddr, pAddr); 
  HciVendorSpecificCmd(HCI_OPCODE_VS_SET_BD_ADDR, sizeof(bdAddr_t), pAddr);
}

/*************************************************************************************************/
void HciVsSetDefaultTxPower(int8_t power)
{
  HciVendorSpecificCmd(HCI_OPCODE_VS_SET_DEF_TX_POWER, sizeof(int8_t), (uint8_t*)&power);
}

/*************************************************************************************************/
void HciVsCalibrate(void)
{
  HciVendorSpecificCmd(HCI_OPCODE_VS_CALIBRATE, 0, NULL);
}

/*************************************************************************************************/
void HciVsSetLocalFeatures(uint8_t *pFeatures)
{
  HciVendorSpecificCmd(HCI_OPCODE_VS_SET_LOCAL_FEAT, HCI_FEAT_LEN, pFeatures);
}

/*************************************************************************************************/
void HciVsSetConnTxPower(uint16_t handle, int8_t power)
{
  uint8_t params[3];

  memcpy(&params[0], &handle, sizeof(uint16_t));
  params[2] = (uint8_t)power;

  HciVendorSpecificCmd(HCI_OPCODE_VS_SET_CONN_TX_PWR, 3, params);
}

/*************************************************************************************************/
void HciVsGetAdvStats(void)
{
  HciVendorSpecificCmd(HCI_OPCODE_VS_GET_ADV_STATS, 0, NULL);
}

/*************************************************************************************************/
void HciVsGetScanStats(void)
{
  HciVendorSpecificCmd(HCI_OPCODE_VS_GET_SCAN_STATS, 0, NULL);
}

/*************************************************************************************************/
void HciVsGetConnStats(void)
{
  HciVendorSpecificCmd(HCI_OPCODE_VS_GET_CONN_STATS, 0, NULL);
}

/*************************************************************************************************/
void HciVsGetTestStats(void)
{
  HciVendorSpecificCmd(HCI_OPCODE_VS_GET_TEST_STATS, 0, NULL);
}
