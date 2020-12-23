/*******************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All rights Reserved.
 *
 * This software is protected by copyright laws of the United States and
 * of foreign countries. This material may also be protected by patent laws
 * and technology transfer regulations of the United States and of foreign
 * countries. This software is furnished under a license agreement and/or a
 * nondisclosure agreement and may only be used or reproduced in accordance
 * with the terms of those agreements. Dissemination of this information to
 * any party or parties not specified in the license agreement and/or
 * nondisclosure agreement is expressly prohibited.
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
 *******************************************************************************
 */

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <app_api.h>
#include <app_db.h>
#include <app_main.h>
#include <app_terminal.h>
#include <app_ui.h>
#include <att_api.h>
#include <att_handler.h>
#include <dm_api.h>
#include <dm_handler.h>
#include <gatt/gatt_api.h>
#include <hci_api.h>
#include <hci_core.h>
#include <hci_defs.h>
#include <hci_handler.h>
#include <l2c_api.h>
#include <l2c_handler.h>
#include <pal_bb.h>
#include <pal_cfg.h>
#include <sec_api.h>
#include <smp_api.h>
#include <smp_handler.h>
#include <smp_handler.h>
#include <string.h>
#include <svc_ch.h>
#include <svc_core.h>
#include <svc_dis.h>
#include <svc_wp.h>
#include <util/bstream.h>
#include <util/calc128.h>
#include <wdxs/wdxs_api.h>
#include <wsf_assert.h>
#include <wsf_buf.h>
#include <wsf_bufio.h>
#include <wsf_cs.h>
#include <wsf_heap.h>
#include <wsf_msg.h>
#include <wsf_nvm.h>
#include <wsf_os.h>
#include <wsf_timer.h>
#include <wsf_trace.h>
#include <wsf_types.h>

#if defined(HCI_TR_EXACTLE) && (HCI_TR_EXACTLE == 1)
#include <ll_init_api.h>
#endif

#if WDXS_INCLUDED  == TRUE
#include <svc_wdxs.h>
#include <wdxs/wdxs_api.h>
#include <wdxs/wdxs_main.h>
#include <wdxs/wdxs_stream.h>
#include <wsf_efs.h>
#endif

#include "max32666_ble.h"
#include "max32666_debug.h"
#include "maxrefdes178_definitions.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME       "Ble"

/*! \brief UART TX buffer size */
#define PLATFORM_UART_TERMINAL_BUFFER_SIZE      2048U

#if (BT_VER > 8)
/* PHY Test Modes */
#define DATS_PHY_1M                       1
#define DATS_PHY_2M                       2
#define DATS_PHY_CODED                    3
#endif /* BT_VER */

#ifndef WDXS_INCLUDED
#define WDXS_INCLUDED              FALSE
#endif

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------
/*! Enumeration of client characteristic configuration descriptors */
enum
{
#if WDXS_INCLUDED == TRUE
  WDXS_DC_CH_CCC_IDX,             /*! WDXS DC service, service changed characteristic */
  WDXS_FTC_CH_CCC_IDX,            /*! WDXS FTC  service, service changed characteristic */
  WDXS_FTD_CH_CCC_IDX,            /*! WDXS FTD service, service changed characteristic */
  WDXS_AU_CH_CCC_IDX,             /*! WDXS AU service, service changed characteristic */
#endif /* WDXS_INCLUDED */
  DATS_GATT_SC_CCC_IDX,           /*! GATT service, service changed characteristic */
  DATS_WP_DAT_CCC_IDX,            /*! Arm Ltd. proprietary service, data transfer characteristic */
  DATS_NUM_CCC_IDX
};


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
/*! \brief  Pool runtime configuration. */
static wsfBufPoolDesc_t mainPoolDesc[] =
{
  { 16,              8 },
  { 32,              4 },
  { 192,             8 },
  { 256,             8 }
};

#if defined(HCI_TR_EXACTLE) && (HCI_TR_EXACTLE == 1)
static LlRtCfg_t mainLlRtCfg;
#endif

/*! configurable parameters for advertising */
static const appAdvCfg_t datsAdvCfg =
{
  {30000,     0,     0},                  /*! Advertising durations in ms */
  {   96,  1600,     0}                   /*! Advertising intervals in 0.625 ms units */
};

/*! configurable parameters for slave */
static const appSlaveCfg_t datsSlaveCfg =
{
  1,                                      /*! Maximum connections */
};

/*! configurable parameters for security */
static const appSecCfg_t datsSecCfg =
{
  DM_AUTH_BOND_FLAG | DM_AUTH_SC_FLAG,    /*! Authentication and bonding flags */
  DM_KEY_DIST_IRK,                        /*! Initiator key distribution flags */
  DM_KEY_DIST_LTK | DM_KEY_DIST_IRK,      /*! Responder key distribution flags */
  FALSE,                                  /*! TRUE if Out-of-band pairing data is present */
  TRUE                                    /*! TRUE to initiate security upon connection */
};

/*! TRUE if Out-of-band pairing data is to be sent */
static const bool_t datsSendOobData = FALSE;

/*! SMP security parameter configuration */
static const smpCfg_t datsSmpCfg =
{
  500,                                    /*! 'Repeated attempts' timeout in msec */
  SMP_IO_NO_IN_NO_OUT,                    /*! I/O Capability */
  7,                                      /*! Minimum encryption key length */
  16,                                     /*! Maximum encryption key length */
  1,                                      /*! Attempts to trigger 'repeated attempts' timeout */
  0,                                      /*! Device authentication requirements */
  64000,                                  /*! Maximum repeated attempts timeout in msec */
  64000,                                  /*! Time msec before attemptExp decreases */
  2                                       /*! Repeated attempts multiplier exponent */
};

/*! configurable parameters for connection parameter update */
static const appUpdateCfg_t datsUpdateCfg =
{
  0,                                      /*! Connection idle period in ms before attempting
                                              connection parameter update; set to zero to disable */
  640,                                    /*! Minimum connection interval in 1.25ms units */
  800,                                    /*! Maximum connection interval in 1.25ms units */
  3,                                      /*! Connection latency */
  900,                                    /*! Supervision timeout in 10ms units */
  5                                       /*! Number of update attempts before giving up */
};

/*! ATT configurable parameters (increase MTU) */
static const attCfg_t datsAttCfg =
{
  15,                               /* ATT server service discovery connection idle timeout in seconds */
  241,                              /* desired ATT MTU */
  ATT_MAX_TRANS_TIMEOUT,            /* transcation timeout in seconds */
  4                                 /* number of queued prepare writes supported by server */
};

/*! local IRK */
static uint8_t localIrk[] =
{
  0x95, 0xC8, 0xEE, 0x6F, 0xC5, 0x0D, 0xEF, 0x93, 0x35, 0x4E, 0x7C, 0x57, 0x08, 0xE2, 0xA3, 0x85
};

/*! advertising data, discoverable mode */
static const uint8_t datsAdvDataDisc[] =
{
  /*! flags */
  2,                                      /*! length */
  DM_ADV_TYPE_FLAGS,                      /*! AD type */
  DM_FLAG_LE_GENERAL_DISC |               /*! flags */
  DM_FLAG_LE_BREDR_NOT_SUP,

  /*! manufacturer specific data */
  3,                                      /*! length */
  DM_ADV_TYPE_MANUFACTURER,               /*! AD type */
  UINT16_TO_BYTES(HCI_ID_PACKETCRAFT)     /*! company ID */
};

/*! scan data, discoverable mode */
static const uint8_t datsScanDataDisc[] =
{
  /*! device name */
  8,                                      /*! length */
  DM_ADV_TYPE_LOCAL_NAME,                 /*! AD type */
  'D',
  'a',
  't',
  'a',
  ' ',
  'T',
  'X'
};

/*! client characteristic configuration descriptors settings, indexed by above enumeration */
static const attsCccSet_t datsCccSet[DATS_NUM_CCC_IDX] =
{
  /* cccd handle          value range               security level */
#if WDXS_INCLUDED == TRUE
  {WDXS_DC_CH_CCC_HDL,    ATT_CLIENT_CFG_NOTIFY,    DM_SEC_LEVEL_NONE},   /* WDXS_DC_CH_CCC_IDX */
  {WDXS_FTC_CH_CCC_HDL,   ATT_CLIENT_CFG_NOTIFY,    DM_SEC_LEVEL_NONE},   /* WDXS_FTC_CH_CCC_IDX */
  {WDXS_FTD_CH_CCC_HDL,   ATT_CLIENT_CFG_NOTIFY,    DM_SEC_LEVEL_NONE},   /* WDXS_FTD_CH_CCC_IDX */
  {WDXS_AU_CH_CCC_HDL,    ATT_CLIENT_CFG_NOTIFY,    DM_SEC_LEVEL_NONE},   /* WDXS_AU_CH_CCC_IDX */
#endif /* WDXS_INCLUDED */
  {GATT_SC_CH_CCC_HDL,    ATT_CLIENT_CFG_INDICATE,  DM_SEC_LEVEL_NONE},   /* DATS_GATT_SC_CCC_IDX */
  {WP_DAT_CH_CCC_HDL,     ATT_CLIENT_CFG_NOTIFY,    DM_SEC_LEVEL_NONE}    /* DATS_WP_DAT_CCC_IDX */
};

/*! application control block */
static struct
{
  wsfHandlerId_t    handlerId;          /* WSF handler ID */
#if (BT_VER > 8)
  uint8_t           phyMode;            /*! PHY Test Mode */
#endif /* BT_VER */
  appDbHdl_t        resListRestoreHdl;  /*! Resolving List last restored handle */
  bool_t            restoringResList;   /*! Restoring resolving list from NVM */
} datsCb;

/* LESC OOB configuration */
static dmSecLescOobCfg_t *datsOobCfg;

//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static void StackInitDats(void);
static void mainWsfInit(void);
static void DatsStart(void);
static void DatsHandlerInit(wsfHandlerId_t handlerId);
static void DatsHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);

//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
static void datsSendData(dmConnId_t connId)
{
  uint8_t str[] = "hello back";

  if (AttsCccEnabled(connId, DATS_WP_DAT_CCC_IDX))
  {
    /* send notification */
    AttsHandleValueNtf(connId, WP_DAT_HDL, sizeof(str), str);
  }
}

static void datsDmCback(dmEvt_t *pDmEvt)
{
  dmEvt_t   *pMsg;
  uint16_t  len;

  if (pDmEvt->hdr.event == DM_SEC_ECC_KEY_IND)
  {
    DmSecSetEccKey(&pDmEvt->eccMsg.data.key);

    /* If the local device sends OOB data. */
    if (datsSendOobData)
    {
      uint8_t oobLocalRandom[SMP_RAND_LEN];
      SecRand(oobLocalRandom, SMP_RAND_LEN);
      DmSecCalcOobReq(oobLocalRandom, pDmEvt->eccMsg.data.key.pubKey_x);
    }
  }
  else if (pDmEvt->hdr.event == DM_SEC_CALC_OOB_IND)
  {
    if (datsOobCfg == NULL)
    {
      datsOobCfg = WsfBufAlloc(sizeof(dmSecLescOobCfg_t));
    }

    if (datsOobCfg)
    {
      Calc128Cpy(datsOobCfg->localConfirm, pDmEvt->oobCalcInd.confirm);
      Calc128Cpy(datsOobCfg->localRandom, pDmEvt->oobCalcInd.random);
    }
  }
  else
  {
    len = DmSizeOfEvt(pDmEvt);

    if ((pMsg = WsfMsgAlloc(len)) != NULL)
    {
      memcpy(pMsg, pDmEvt, len);
      WsfMsgSend(datsCb.handlerId, pMsg);
    }
  }
}

static void datsAttCback(attEvt_t *pEvt)
{
  attEvt_t  *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(attEvt_t) + pEvt->valueLen)) != NULL)
  {
    memcpy(pMsg, pEvt, sizeof(attEvt_t));
    pMsg->pValue = (uint8_t *)(pMsg + 1);
    memcpy(pMsg->pValue, pEvt->pValue, pEvt->valueLen);
    WsfMsgSend(datsCb.handlerId, pMsg);
  }

#if WDXS_INCLUDED == TRUE
  WdxsAttCback(pEvt);
#endif /* WDXS_INCLUDED */
}

static void datsCccCback(attsCccEvt_t *pEvt)
{
  appDbHdl_t    dbHdl;

  /* If CCC not set from initialization and there's a device record and currently bonded */
  if ((pEvt->handle != ATT_HANDLE_NONE) &&
      ((dbHdl = AppDbGetHdl((dmConnId_t) pEvt->hdr.param)) != APP_DB_HDL_NONE) &&
      AppCheckBonded((dmConnId_t)pEvt->hdr.param))
  {
    /* Store value in device database. */
    AppDbSetCccTblValue(dbHdl, pEvt->idx, pEvt->value);
    AppDbNvmStoreCccTbl(dbHdl);
  }
}

uint8_t datsWpWriteCback(dmConnId_t connId, uint16_t handle, uint8_t operation,
                          uint16_t offset, uint16_t len, uint8_t *pValue, attsAttr_t *pAttr)
{
  /* print received data */
  APP_TRACE_INFO0((const char*) pValue);

  /* send back some data */
  datsSendData(connId);

  return ATT_SUCCESS;
}

static void datsPrivAddDevToResList(appDbHdl_t dbHdl)
{
  dmSecKey_t *pPeerKey;

  /* if peer IRK present */
  if ((pPeerKey = AppDbGetKey(dbHdl, DM_KEY_IRK, NULL)) != NULL)
  {
    /* set advertising peer address */
    AppSetAdvPeerAddr(pPeerKey->irk.addrType, pPeerKey->irk.bdAddr);
  }
}

static void datsPrivRemDevFromResListInd(dmEvt_t *pMsg)
{
  if (pMsg->hdr.status == HCI_SUCCESS)
  {
    if (AppDbGetHdl((dmConnId_t) pMsg->hdr.param) != APP_DB_HDL_NONE)
    {
      uint8_t addrZeros[BDA_ADDR_LEN] = { 0 };

      /* clear advertising peer address and its type */
      AppSetAdvPeerAddr(HCI_ADDR_TYPE_PUBLIC, addrZeros);
    }
  }
}

void datsDisplayStackVersion(const char *pVersion)
{
  APP_TRACE_INFO1("Stack Version: %s", pVersion);
}

static void datsSetup(dmEvt_t *pMsg)
{

  /* Initialize control information */
  datsCb.restoringResList = FALSE;

  /* set advertising and scan response data for discoverable mode */
  AppAdvSetData(APP_ADV_DATA_DISCOVERABLE, sizeof(datsAdvDataDisc), (uint8_t *) datsAdvDataDisc);
  AppAdvSetData(APP_SCAN_DATA_DISCOVERABLE, sizeof(datsScanDataDisc), (uint8_t *) datsScanDataDisc);

  /* set advertising and scan response data for connectable mode */
  AppAdvSetData(APP_ADV_DATA_CONNECTABLE, sizeof(datsAdvDataDisc), (uint8_t *) datsAdvDataDisc);
  AppAdvSetData(APP_SCAN_DATA_CONNECTABLE, sizeof(datsScanDataDisc), (uint8_t *) datsScanDataDisc);

  /* start advertising; automatically set connectable/discoverable mode and bondable mode */
  AppAdvStart(APP_MODE_AUTO_INIT);
}

static void datsRestoreResolvingList(dmEvt_t *pMsg)
{
  /* Restore first device to resolving list in Controller. */
  datsCb.resListRestoreHdl = AppAddNextDevToResList(APP_DB_HDL_NONE);

  if (datsCb.resListRestoreHdl == APP_DB_HDL_NONE)
  {
    /* No device to restore.  Setup application. */
    datsSetup(pMsg);
  }
  else
  {
    datsCb.restoringResList = TRUE;
  }
}

static void datsPrivAddDevToResListInd(dmEvt_t *pMsg)
{
  /* Check if in the process of restoring the Device List from NV */
  if (datsCb.restoringResList)
  {
    /* Set the advertising peer address. */
    datsPrivAddDevToResList(datsCb.resListRestoreHdl);

    /* Retore next device to resolving list in Controller. */
    datsCb.resListRestoreHdl = AppAddNextDevToResList(datsCb.resListRestoreHdl);

    if (datsCb.resListRestoreHdl == APP_DB_HDL_NONE)
    {
      /* No additional device to restore. Setup application. */
      datsSetup(pMsg);
    }
  }
  else
  {
    datsPrivAddDevToResList(AppDbGetHdl((dmConnId_t) pMsg->hdr.param));
  }
}

static void datsProcMsg(dmEvt_t *pMsg)
{
  uint8_t uiEvent = APP_UI_NONE;

  switch(pMsg->hdr.event)
  {
    case DM_RESET_CMPL_IND:
      AttsCalculateDbHash();
      DmSecGenerateEccKeyReq();
      AppDbNvmReadAll();
      datsRestoreResolvingList(pMsg);
      uiEvent = APP_UI_RESET_CMPL;
      break;

    case DM_ADV_START_IND:
      uiEvent = APP_UI_ADV_START;
      break;

    case DM_ADV_STOP_IND:
      uiEvent = APP_UI_ADV_STOP;
      break;

    case DM_CONN_OPEN_IND:
      uiEvent = APP_UI_CONN_OPEN;
#if (BT_VER > 8)
      datsCb.phyMode = DATS_PHY_1M;
#endif /* BT_VER */
      break;

    case DM_CONN_CLOSE_IND:
      uiEvent = APP_UI_CONN_CLOSE;
      break;

    case DM_SEC_PAIR_CMPL_IND:
      DmSecGenerateEccKeyReq();
      AppDbNvmStoreBond(AppDbGetHdl((dmConnId_t) pMsg->hdr.param));
      uiEvent = APP_UI_SEC_PAIR_CMPL;
      break;

    case DM_SEC_PAIR_FAIL_IND:
      DmSecGenerateEccKeyReq();
      uiEvent = APP_UI_SEC_PAIR_FAIL;
      break;

    case DM_SEC_ENCRYPT_IND:
      uiEvent = APP_UI_SEC_ENCRYPT;
      break;

    case DM_SEC_ENCRYPT_FAIL_IND:
      uiEvent = APP_UI_SEC_ENCRYPT_FAIL;
      break;

    case DM_SEC_AUTH_REQ_IND:

      if (pMsg->authReq.oob)
      {
        dmConnId_t connId = (dmConnId_t) pMsg->hdr.param;

        /* TODO: Perform OOB Exchange with the peer. */


        /* TODO: Fill datsOobCfg peerConfirm and peerRandom with value passed out of band */

        if (datsOobCfg != NULL)
        {
          DmSecSetOob(connId, datsOobCfg);
        }

        DmSecAuthRsp(connId, 0, NULL);
      }
      else
      {
        AppHandlePasskey(&pMsg->authReq);
      }
      break;

    case DM_SEC_COMPARE_IND:
      AppHandleNumericComparison(&pMsg->cnfInd);
      break;

    case DM_PRIV_ADD_DEV_TO_RES_LIST_IND:
      datsPrivAddDevToResListInd(pMsg);
      break;

    case DM_PRIV_REM_DEV_FROM_RES_LIST_IND:
      datsPrivRemDevFromResListInd(pMsg);
      break;

    case DM_ADV_NEW_ADDR_IND:
      break;

    case DM_PRIV_CLEAR_RES_LIST_IND:
      APP_TRACE_INFO1("Clear resolving list status 0x%02x", pMsg->hdr.status);
      break;

#if (BT_VER > 8)
    case DM_PHY_UPDATE_IND:
      APP_TRACE_INFO2("DM_PHY_UPDATE_IND - RX: %d, TX: %d", pMsg->phyUpdate.rxPhy, pMsg->phyUpdate.txPhy);
      datsCb.phyMode = pMsg->phyUpdate.rxPhy;
      break;
#endif /* BT_VER */

    default:
      break;
  }

  if (uiEvent != APP_UI_NONE)
  {
    AppUiAction(uiEvent);
  }
}

static void datsBtnCback(uint8_t btn)
{
#if WDXS_INCLUDED == TRUE
  static uint8_t waveform = WDXS_STREAM_WAVEFORM_SINE;
#endif /* WDXS_INCLUDED */

#if (BT_VER > 8)
  dmConnId_t      connId;
  if ((connId = AppConnIsOpen()) != DM_CONN_ID_NONE)
#else
  if (AppConnIsOpen() != DM_CONN_ID_NONE)
#endif /* BT_VER */
  {
    switch (btn)
    {
#if (BT_VER > 8)
      case APP_UI_BTN_2_SHORT:

        /* Toggle PHY Test Mode */
        if (datsCb.phyMode == DATS_PHY_1M)
        {
          APP_TRACE_INFO0("2 MBit TX and RX PHY Requested");
          DmSetPhy(connId, HCI_ALL_PHY_ALL_PREFERENCES, HCI_PHY_LE_2M_BIT, HCI_PHY_LE_2M_BIT, HCI_PHY_OPTIONS_NONE);
        }
        else if (datsCb.phyMode == DATS_PHY_2M)
        {
          APP_TRACE_INFO0("LE Coded TX and RX PHY Requested");
          DmSetPhy(connId, HCI_ALL_PHY_ALL_PREFERENCES, HCI_PHY_LE_CODED_BIT, HCI_PHY_LE_CODED_BIT, HCI_PHY_OPTIONS_NONE);
        }
        else
        {
          APP_TRACE_INFO0("1 MBit TX and RX PHY Requested");
          DmSetPhy(connId, HCI_ALL_PHY_ALL_PREFERENCES, HCI_PHY_LE_1M_BIT, HCI_PHY_LE_1M_BIT, HCI_PHY_OPTIONS_NONE);
        }
        break;

#endif /* BT_VER */

#if WDXS_INCLUDED == TRUE
      case APP_UI_BTN_1_SHORT:
        /* Change stream waveform */
        waveform++;

        if (waveform > WDXS_STREAM_WAVEFORM_SAWTOOTH)
          waveform = WDXS_STREAM_WAVEFORM_SINE;

        wdxsSetStreamWaveform(waveform);
        break;
#endif /* WDXS_INCLUDED */

      default:
        break;
    }
  }
  else
  {
    switch (btn)
    {
      case APP_UI_BTN_1_SHORT:
        /* start advertising */
        AppAdvStart(APP_MODE_AUTO_INIT);
        break;

      case APP_UI_BTN_1_MED:
        /* Enter bondable mode */
        AppSetBondable(TRUE);
        break;

      case APP_UI_BTN_1_LONG:
        /* clear all bonding info */
        AppSlaveClearAllBondingInfo();
        AppDbNvmDeleteAll();
        break;

      case APP_UI_BTN_1_EX_LONG:
        {
          const char *pVersion;
          StackGetVersionNumber(&pVersion);
          datsDisplayStackVersion(pVersion);
        }
        break;

      case APP_UI_BTN_2_SHORT:
        /* stop advertising */
        AppAdvStop();
        break;

      default:
        break;
    }
  }
}

static void datsWsfBufDiagnostics(WsfBufDiag_t *pInfo)
{
  if (pInfo->type == WSF_BUF_ALLOC_FAILED)
  {
    APP_TRACE_INFO2("Dats got WSF Buffer Allocation Failure - Task: %d Len: %d",
                     pInfo->param.alloc.taskId, pInfo->param.alloc.len);
  }
}

void DatsHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
  if (pMsg != NULL)
  {
    APP_TRACE_INFO1("Dats got evt %d", pMsg->event);

    /* process ATT messages */
    if (pMsg->event >= ATT_CBACK_START && pMsg->event <= ATT_CBACK_END)
    {
      /* process server-related ATT messages */
      AppServerProcAttMsg(pMsg);
    }
    /* process DM messages */
    else if (pMsg->event >= DM_CBACK_START && pMsg->event <= DM_CBACK_END)
    {
      /* process advertising and connection-related messages */
      AppSlaveProcDmMsg((dmEvt_t *) pMsg);

      /* process security-related messages */
      AppSlaveSecProcDmMsg((dmEvt_t *) pMsg);

#if WDXS_INCLUDED == TRUE
      /* process WDXS-related messages */
      WdxsProcDmMsg((dmEvt_t*) pMsg);
#endif /* WDXS_INCLUDED */
    }

    /* perform profile and user interface-related operations */
    datsProcMsg((dmEvt_t *) pMsg);
  }
}

void DatsStart(void)
{
  /* Register for stack callbacks */
  DmRegister(datsDmCback);
  DmConnRegister(DM_CLIENT_ID_APP, datsDmCback);
  AttRegister(datsAttCback);
  AttConnRegister(AppServerConnCback);
  AttsCccRegister(DATS_NUM_CCC_IDX, (attsCccSet_t *) datsCccSet, datsCccCback);

  /* Initialize attribute server database */
  SvcCoreGattCbackRegister(GattReadCback, GattWriteCback);
  SvcCoreAddGroup();
  SvcWpCbackRegister(NULL, datsWpWriteCback);
  SvcWpAddGroup();

  /* Set Service Changed CCCD index. */
  GattSetSvcChangedIdx(DATS_GATT_SC_CCC_IDX);

  /* Register for app framework button callbacks */
  AppUiBtnRegister(datsBtnCback);

#if WDXS_INCLUDED == TRUE

  /* Initialize the OTA File Media */
  WdxsOtaMediaInit();

  /* Initialize the WDXS Stream */
  wdxsStreamInit();

  /* Set the WDXS CCC Identifiers */
  WdxsSetCccIdx(WDXS_DC_CH_CCC_IDX, WDXS_AU_CH_CCC_IDX, WDXS_FTC_CH_CCC_IDX, WDXS_FTD_CH_CCC_IDX);

#if (BT_VER > 8)
  WdxsPhyInit();
#endif /* BT_VER */

#endif /* WDXS_INCLUDED */

#if (BT_VER > 8)
  DmPhyInit();
#endif /* BT_VER */

  WsfNvmInit();

  WsfBufDiagRegister(datsWsfBufDiagnostics);

  /* Reset the device */
  DmDevReset();
}

void DatsHandlerInit(wsfHandlerId_t handlerId)
{
  APP_TRACE_INFO0("DatsHandlerInit");

  /* store handler ID */
  datsCb.handlerId = handlerId;

  /* Set configuration pointers */
  pAppSlaveCfg = (appSlaveCfg_t *) &datsSlaveCfg;
  pAppAdvCfg = (appAdvCfg_t *) &datsAdvCfg;
  pAppSecCfg = (appSecCfg_t *) &datsSecCfg;
  pAppUpdateCfg = (appUpdateCfg_t *) &datsUpdateCfg;
  pSmpCfg = (smpCfg_t *) &datsSmpCfg;
  pAttCfg = (attCfg_t *) &datsAttCfg;

  /* Initialize application framework */
  AppSlaveInit();
  AppServerInit();

  /* Set IRK for the local device */
  DmSecSetLocalIrk(localIrk);
}

static void StackInitDats(void)
{
    wsfHandlerId_t handlerId;

    SecInit();
    SecAesInit();
    SecCmacInit();
    SecEccInit();

    handlerId = WsfOsSetNextHandler(HciHandler);
    HciHandlerInit(handlerId);

    handlerId = WsfOsSetNextHandler(DmHandler);
    DmDevVsInit(0);
    DmConnInit();
    DmAdvInit();
    DmConnSlaveInit();
    DmSecInit();
    DmSecLescInit();
    DmPrivInit();
    DmHandlerInit(handlerId);

    handlerId = WsfOsSetNextHandler(L2cSlaveHandler);
    L2cSlaveHandlerInit(handlerId);
    L2cInit();
    L2cSlaveInit();

    handlerId = WsfOsSetNextHandler(AttHandler);
    AttHandlerInit(handlerId);
    AttsInit();
    AttsIndInit();

    handlerId = WsfOsSetNextHandler(SmpHandler);
    SmpHandlerInit(handlerId);
    SmprInit();
    SmprScInit();
    HciSetMaxRxAclLen(100);

    handlerId = WsfOsSetNextHandler(AppHandler);
    AppHandlerInit(handlerId);

    handlerId = WsfOsSetNextHandler(DatsHandler);
    DatsHandlerInit(handlerId);

#if WDXS_INCLUDED == TRUE
    handlerId = WsfOsSetNextHandler(WdxsHandler);
    WdxsHandlerInit(handlerId);
#endif
}

#ifdef WSF_TRACE_ENABLED
static bool_t myTrace(const uint8_t *pBuf, uint32_t len)
{
    extern uint8_t wsfCsNesting;

    if (wsfCsNesting == 0)
    {
        WsfTaskLock();
//        while(SEMA_GetSema(MAX32666_SEMAPHORE_PRINT) == E_BUSY) {}
        fwrite(pBuf, len, 1, stdout);
        fflush(stdout);
//        SEMA_FreeSema(MAX32666_SEMAPHORE_PRINT);
        WsfTaskUnlock();
        return TRUE;
    }

    return FALSE;
}
#endif

static void mainWsfInit(void)
{
#if defined(HCI_TR_EXACTLE) && (HCI_TR_EXACTLE == 1)
    /* +12 for message headroom, + 2 event header, +255 maximum parameter length. */
    const uint16_t maxRptBufSize = 12 + 2 + 255;

    /* +12 for message headroom, +4 for header. */
    const uint16_t aclBufSize = 12 + mainLlRtCfg.maxAclLen + 4 + BB_DATA_PDU_TAILROOM;

    /* Adjust buffer allocation based on platform configuration. */
    mainPoolDesc[2].len = maxRptBufSize;
    mainPoolDesc[2].num = mainLlRtCfg.maxAdvReports;
    mainPoolDesc[3].len = aclBufSize;
    mainPoolDesc[3].num = mainLlRtCfg.numTxBufs + mainLlRtCfg.numRxBufs;
#endif

    const uint8_t numPools = sizeof(mainPoolDesc) / sizeof(mainPoolDesc[0]);

    uint16_t memUsed;
    memUsed = WsfBufInit(numPools, mainPoolDesc);
    WsfHeapAlloc(memUsed);
    WsfOsInit();
    WsfTimerInit();
#if (WSF_TOKEN_ENABLED == TRUE) || (WSF_TRACE_ENABLED == TRUE)
    WsfTraceRegisterHandler(myTrace);
    WsfTraceEnable(TRUE);
#endif
}

int ble_init(void)
{
#if defined(HCI_TR_EXACTLE) && (HCI_TR_EXACTLE == 1)
    /* Configurations must be persistent. */
    static BbRtCfg_t mainBbRtCfg;

    PalBbLoadCfg((PalBbCfg_t *)&mainBbRtCfg);
    LlGetDefaultRunTimeCfg(&mainLlRtCfg);
#if (BT_VER >= LL_VER_BT_CORE_SPEC_5_0)
    /* Set 5.0 requirements. */
    mainLlRtCfg.btVer = LL_VER_BT_CORE_SPEC_5_0;
#endif
    PalCfgLoadData(PAL_CFG_ID_LL_PARAM, &mainLlRtCfg.maxAdvSets, sizeof(LlRtCfg_t) - 9);
#if (BT_VER >= LL_VER_BT_CORE_SPEC_5_0)
    PalCfgLoadData(PAL_CFG_ID_BLE_PHY, &mainLlRtCfg.phy2mSup, 4);
#endif
#endif

    uint32_t memUsed;
    memUsed = WsfBufIoUartInit(WsfHeapGetFreeStartAddress(), PLATFORM_UART_TERMINAL_BUFFER_SIZE);
    WsfHeapAlloc(memUsed);

    mainWsfInit();
    AppTerminalInit();

#if defined(HCI_TR_EXACTLE) && (HCI_TR_EXACTLE == 1)
    LlInitRtCfg_t llCfg =
    {
            .pBbRtCfg     = &mainBbRtCfg,
            .wlSizeCfg    = 4,
            .rlSizeCfg    = 4,
            .plSizeCfg    = 4,
            .pLlRtCfg     = &mainLlRtCfg,
            .pFreeMem     = WsfHeapGetFreeStartAddress(),
            .freeMemAvail = WsfHeapCountAvailable()
    };

    memUsed = LlInit(&llCfg);
    WsfHeapAlloc(memUsed);

    bdAddr_t bdAddr;
    PalCfgLoadData(PAL_CFG_ID_BD_ADDR, bdAddr, sizeof(bdAddr_t));
    LlSetBdAddr((uint8_t *)&bdAddr);
    LlMathSetSeed((uint32_t *)&bdAddr);
#endif

    StackInitDats();
    DatsStart();

    PR_INFO("BLE init completed");

    return E_NO_ERROR;
}

int ble_worker(void)
{
    wsfOsDispatcher();
    return E_NO_ERROR;
}
