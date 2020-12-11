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
#include <stdio.h>
#include <string.h>
#include <mxc_device.h>
#include <mxc_errors.h>
#include <mxc_sys.h>
#include <gcr_regs.h>
#include <wsf_types.h>
#include <util/bstream.h>
#include <wsf_msg.h>
#include <wsf_timer.h>
#include <wsf_types.h>
#include <wsf_os.h>
#include <wsf_trace.h>
#include <wsf_buf.h>
#include <wsf_assert.h>
#include <hci_api.h>
#include <hci_core.h>
#include <hci_handler.h>
#include <hci_vs.h>
#include <dm_api.h>
#include <dm_handler.h>
#include <l2c_api.h>
#include <l2c_handler.h>
#include <smp_api.h>
#include <att_api.h>
#include <att_handler.h>
#include <app_api.h>
#include <app_main.h>
#include <app_db.h>
#include <app_ui.h>
#include <svc_ch.h>
#include <svc_core.h>
#include <svc_dis.h>
#include <svc_wp.h>
#include <smp_handler.h>
#include <ll_init_api.h>
#include <util/bstream.h>
#include <util/calc128.h>
#include <dats/dats_api.h>
#include <bb_drv.h>
#include <mxc_delay.h>
#include <rtc.h>

#include "ble.h"
#include "max32666_debug.h"
#include "maxrefdes178_definitions.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME       "Ble"

/* Size of buffer for stdio functions */
#define WSF_BUF_POOLS       6
#define WSF_BUF_SIZE        0x1048

#define LL_IMPL_REV             0x2303
#define LL_MEMORY_FOOTPRINT     0xc152


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------
/*! Enumeration of client characteristic configuration descriptors */
enum
{
  DATS_GATT_SC_CCC_IDX,           /*! GATT service, service changed characteristic */
  DATS_WP_DAT_CCC_IDX,            /*! ARM Ltd. proprietary service, data transfer characteristic */
  DATS_NUM_CCC_IDX
};


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
uint32_t SystemHeapSize=WSF_BUF_SIZE;
uint32_t SystemHeap[WSF_BUF_SIZE/4];
uint32_t SystemHeapStart;

uint8_t LlMem[LL_MEMORY_FOOTPRINT];

LlRtCfg_t _ll_cfg = {
    /* Device */
    /*compId*/                  LL_COMP_ID_MAXIM,
    /*implRev*/                 LL_IMPL_REV,
    /*btVer*/                   LL_VER_BT_CORE_SPEC_5_0,
    /*_align32 */               0, // padding for alignment

    /* Advertiser */
    /*maxAdvSets*/              4, // 4 Extended Advertising Sets
    /*maxAdvReports*/           8,
    /*maxExtAdvDataLen*/        LL_MAX_ADV_DATA_LEN,
    /*defExtAdvDataFrag*/       64,
    /*auxDelayUsec*/            0,

    /* Scanner */
    /*maxScanReqRcvdEvt*/       4,
    /*maxExtScanDataLen*/       LL_MAX_ADV_DATA_LEN,

    /* Connection */
    /*maxConn*/                 2,
    /*numTxBufs*/               16,
    /*numRxBufs*/               16,
    /*maxAclLen*/               512,
    /*defTxPwrLvl*/             0,
    /*ceJitterUsec*/            0,

    /* DTM */
    /*dtmRxSyncMs*/             10000,

    /* PHY */
    /*phy2mSup*/                TRUE,
    /*phyCodedSup*/             FALSE,
    /*stableModIdxTxSup*/       FALSE,
    /*stableModIdxRxSup*/       FALSE
};

const BbRtCfg_t _bb_cfg = {
    /*clkPpm*/                  20,
    /*rfSetupDelayUsec*/        BB_RF_SETUP_DELAY_US,
    /*defaultTxPower*/          -10,
    /*maxScanPeriodMsec*/       BB_MAX_SCAN_PERIOD_MS,
    /*schSetupDelayUsec*/       BB_SCH_SETUP_DELAY_US
};

/*! Default pool descriptor. */
static wsfBufPoolDesc_t mainPoolDesc[WSF_BUF_POOLS] =
{
  {  16,  8 },
  {  32,  4 },
  {  64,  4 },
  { 128,  4 },
  { 256,  4 },
  { 512,  4 }
};
/**************************************************************************************************
  Configurable Parameters
**************************************************************************************************/

/*! configurable parameters for advertising */
/* These intervals directly impact energy usage during the non-connected/advertising mode */
#ifdef BTLE_APP_USE_LEGACY_API
static const appAdvCfg_t datsAdvCfg =
{
  { 1000,     0,     0},                  /*! Advertising durations in ms */
  {   96,   200,     0}                   /*! Advertising intervals in 0.625 ms units */
};
#else /* BTLE_APP_USE_LEGACY_API */
static const appExtAdvCfg_t datsExtAdvCfg =
{
  {    0,     0,     0}, /*! Advertising durations for extended advertising in ms */
  {   96,   200,     0}, /*! Advertising intervals for extended advertising in 0.625 ms units (20 ms to 10.24 s). */
  {    0,     0,     0}, /*! Maximum number of extended advertising events controller will send prior to terminating extended advertising */
  {    1,     1,     1}, /*! Whether to use legacy advertising PDUs with extended advertising. If set to TRUE then length of advertising data cannot exceed 31 octets. */
  {    6,     0,     0}  /*! Advertising intervals for periodic advertising in 1.25 ms units (7.5 ms to 81.91875 s). */
};

#endif /* BTLE_APP_USE_LEGACY_API */

/*! configurable parameters for slave */
static const appSlaveCfg_t datsSlaveCfg =
{
  1,                                      /*! Maximum connections */
};

/*! configurable parameters for security */
static const appSecCfg_t datsSecCfg =
{
  DM_AUTH_BOND_FLAG | DM_AUTH_SC_FLAG,                        /*! Authentication and bonding flags */
  0,                                                         /*! Initiator key distribution flags */
  DM_KEY_DIST_LTK,                                           /*! Responder key distribution flags */
  FALSE,                                                     /*! TRUE if Out-of-band pairing data is present */
  FALSE                                                      /*! TRUE to initiate security upon connection */
};

/*! TRUE if Out-of-band pairing data is to be sent */
static const bool_t datsSendOobData = FALSE;

/*! SMP security parameter configuration */
static const smpCfg_t datsSmpCfg =
{
  3000,                                   /*! 'Repeated attempts' timeout in msec */
  SMP_IO_NO_IN_NO_OUT,                    /*! I/O Capability */
  7,                                      /*! Minimum encryption key length */
  16,                                     /*! Maximum encryption key length */
  3,                                      /*! Attempts to trigger 'repeated attempts' timeout */
  0,                                      /*! Device authentication requirements */
};

/*! configurable parameters for connection parameter update */
static const appUpdateCfg_t datsUpdateCfg =
{
  1000,                                   /*! Connection idle period in ms before attempting
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
  BLE_MAX_MTU_SIZE,                 /* desired ATT MTU */
  ATT_MAX_TRANS_TIMEOUT,            /* transcation timeout in seconds */
  4                                 /* number of queued prepare writes supported by server */
};

/*! local IRK */
//static uint8_t localIrk[] =
//{
//  0x95, 0xC8, 0xEE, 0x6F, 0xC5, 0x0D, 0xEF, 0x93, 0x35, 0x4E, 0x7C, 0x57, 0x08, 0xE2, 0xA3, 0x85
//};

/**************************************************************************************************
  Advertising Data
**************************************************************************************************/
#ifdef BTLE_APP_USE_LEGACY_API
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
  UINT16_TO_BYTES(HCI_ID_MAXIM)             /*! company ID */
};

/*! scan data, discoverable mode */
static const uint8_t datsScanDataDisc[] =
{
  /*! device name */
  7,                                      /*! length */
  DM_ADV_TYPE_LOCAL_NAME,                 /*! AD type */
  'M',
  'A',
  'X',
  'C',
  'A',
  'M'
};

#else /* BTLE_APP_USE_LEGACY_API */
/*! extended advertising and scan data, discoverable mode */
static const uint8_t datsExtAdvDataDisc[] =
{
  /*! flags */
  2,                                      /*! length */
  DM_ADV_TYPE_FLAGS,                      /*! AD type */
  DM_FLAG_LE_GENERAL_DISC |               /*! flags */
  DM_FLAG_LE_BREDR_NOT_SUP,

  /*! manufacturer specific data */
  3,                                      /*! length */
  DM_ADV_TYPE_MANUFACTURER,               /*! AD type */
  UINT16_TO_BYTES(HCI_ID_MAXIM),            /*! company ID */

  /*! device name */
  7,                                     /*! length */
  DM_ADV_TYPE_LOCAL_NAME,                 /*! AD type */
  'M',
  'A',
  'X',
  'C',
  'A',
  'M'
};

static const uint8_t datsExtScanDataDisc[] =
{
  /*! device name */
  7,                                     /*! length */
  DM_ADV_TYPE_LOCAL_NAME,                 /*! AD type */
  'M',
  'A',
  'X',
  'C',
  'A',
  'M'
};

#endif /* BTLE_APP_USE_LEGACY_API */

/*! client characteristic configuration descriptors settings, indexed by above enumeration */
static const attsCccSet_t datsCccSet[DATS_NUM_CCC_IDX] =
{
  /* cccd handle          value range               security level */
  {GATT_SC_CH_CCC_HDL,    ATT_CLIENT_CFG_INDICATE,  DM_SEC_LEVEL_NONE},   /* DATS_GATT_SC_CCC_IDX */
  {WP_DAT_CH_CCC_HDL,     ATT_CLIENT_CFG_NOTIFY | ATT_CLIENT_CFG_INDICATE,    DM_SEC_LEVEL_NONE}    /* DATS_WP_DAT_CCC_IDX */
};

/*! application control block */
static struct
{
  wsfHandlerId_t    handlerId;        /* WSF handler ID */
} datsCb;

/* LESC OOB configuration */
static dmSecLescOobCfg_t *datsOobCfg;

static dmConnId_t currentConnId = 0;
static uint8_t indicationStatus = 0;


//-----------------------------------------------------------------------------
// Local Function declarations
//-----------------------------------------------------------------------------
static void Ble_Receive(uint16_t dataLen, uint8_t *data);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
/*************************************************************************************************/
/*!
 *  \brief      Initialize stack.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void StackInitDats(void)
{
    wsfHandlerId_t handlerId;

#ifndef ENABLE_SDMA
    uint32_t memUsed;

    /* Enable coded PHY */
    if(MXC_GCR->revision != 0xA1) {
        _ll_cfg.phyCodedSup = TRUE;
    }

    /* Initialize link layer. */
    LlInitRtCfg_t ll_init_cfg =
    {
            .pBbRtCfg     = &_bb_cfg,
            .wlSizeCfg    = 4,
            .rlSizeCfg    = 4,
            .plSizeCfg    = 4,
            .pLlRtCfg     = &_ll_cfg,
            .pFreeMem     = LlMem,
            .freeMemAvail = LL_MEMORY_FOOTPRINT
    };

    memUsed = LlInitControllerExtInit(&ll_init_cfg);
    if(memUsed != LL_MEMORY_FOOTPRINT)
    {
        PR_ERROR("mem_used: 0x%x LL_MEMORY_FOOTPRINT: 0x%x", memUsed,
                LL_MEMORY_FOOTPRINT);
    }
#endif

    handlerId = WsfOsSetNextHandler(HciHandler);
    HciHandlerInit(handlerId);

    SecInit();
    SecAesInit();
    SecCmacInit();
    SecEccInit();

    handlerId = WsfOsSetNextHandler(DmHandler);
    DmDevVsInit(0);
#ifdef BTLE_APP_USE_LEGACY_API
    DmAdvInit();
    DmConnInit();
    DmConnSlaveInit();
#else /* BTLE_APP_USE_LEGACY_API */
    DmExtAdvInit();
    DmConnInit();
    DmExtConnSlaveInit();
#endif /* BTLE_APP_USE_LEGACY_API */
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
    HciSetMaxRxAclLen(256);

    handlerId = WsfOsSetNextHandler(AppHandler);
    AppHandlerInit(handlerId);

    handlerId = WsfOsSetNextHandler(DatsHandler);
    DatsHandlerInit(handlerId);
}

/*************************************************************************************************/
/*!
 *  \brief  Application DM callback.
 *
 *  \param  pDmEvt  DM callback event
 *
 *  \return None.
 */
/*************************************************************************************************/
static void datsDmCback(dmEvt_t *pDmEvt)
{
    dmEvt_t   *pMsg;
    uint16_t  len;

    PR_INFO("Event: 0x%02hhX", pDmEvt->hdr.event);

    if (pDmEvt->hdr.event == DM_SEC_ECC_KEY_IND)
    {
        PR_INFO("DM_SEC_ECC_KEY_IND");

        DmSecSetEccKey(&pDmEvt->eccMsg.data.key);

        // If the local device sends OOB data.
        if (datsSendOobData)
        {
            uint8_t oobLocalRandom[SMP_RAND_LEN];
            SecRand(oobLocalRandom, SMP_RAND_LEN);
            DmSecCalcOobReq(oobLocalRandom, pDmEvt->eccMsg.data.key.pubKey_x);
        }
    }
    else if (pDmEvt->hdr.event == DM_SEC_CALC_OOB_IND)
    {
        PR_INFO("DM_SEC_CALC_OOB_IND");

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
        PR_INFO("Event: 0x%02hhX", pDmEvt->hdr.event);

        len = DmSizeOfEvt(pDmEvt);

        if ((pMsg = WsfMsgAlloc(len)) != NULL)
        {
            memcpy(pMsg, pDmEvt, len);
            WsfMsgSend(datsCb.handlerId, pMsg);
        }
    }
}

/*************************************************************************************************/
/*!
 *  \brief  Application ATT callback.
 *
 *  \param  pEvt    ATT callback event
 *
 *  \return None.
 */
/*************************************************************************************************/
static void datsAttCback(attEvt_t *pEvt)
{
    switch(pEvt->hdr.event)
    {
    case ATTS_HANDLE_VALUE_CNF:
        PR_DEBUG("ATTS_HANDLE_VALUE_CNF");

        if (pEvt->handle == WP_DAT_HDL) {
            indicationStatus = pEvt->hdr.status;
        }

        if (pEvt->hdr.status == ATT_SUCCESS) {
            PR_DEBUG("Handle %d confirmation success", pEvt->handle);
        } else if (pEvt->hdr.status == ATT_ERR_OVERFLOW) {
            PR_ERROR("Handle %d confirmation overflow", pEvt->handle);
        } else {
            PR_ERROR("Handle %d confirmation fail 0x%02hhX", pEvt->handle, pEvt->hdr.status);
        }
        break;

    default:
        PR_INFO("Event: 0x%02hhX Status: 0x%02hhX Handle: %d", pEvt->hdr.event, pEvt->hdr.status, pEvt->handle);
        break;
    }
}

/*************************************************************************************************/
/*!
 *  \brief  Application ATTS client characteristic configuration callback.
 *
 *  \param  pDmEvt  DM callback event
 *
 *  \return None.
 */
/*************************************************************************************************/
static void datsCccCback(attsCccEvt_t *pEvt)
{
    appDbHdl_t    dbHdl;

    PR_INFO("Event: 0x%02hhX idx: %d Value: %d", pEvt->hdr.event, pEvt->idx, pEvt->value);

    /* if CCC not set from initialization and there's a device record */
    if ((pEvt->handle != ATT_HANDLE_NONE) &&
            ((dbHdl = AppDbGetHdl((dmConnId_t) pEvt->hdr.param)) != APP_DB_HDL_NONE))
    {
        /* store value in device database */
        AppDbSetCccTblValue(dbHdl, pEvt->idx, pEvt->value);
    }
}

/*************************************************************************************************/
/*!
 *  \brief  ATTS write callback for proprietary data service.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t datsWpWriteCback(dmConnId_t connId, uint16_t handle, uint8_t operation,
                          uint16_t offset, uint16_t len, uint8_t *pValue, attsAttr_t *pAttr)
{
//    PR_DEBUG("connId: 0x%02hhX", connId);

    Ble_Receive(len, pValue);

    return ATT_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief  ATTS read callback for proprietary data service.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t datsWpReadCback(dmConnId_t connId, uint16_t handle, uint8_t operation,
                                   uint16_t offset, attsAttr_t *pAttr)
{
    PR_DEBUG("connId: 0x%02hhX", connId);

    *(pAttr->pLen) = 1;
    for (int i = 0; i < 1; i++ ){
        pAttr->pValue[i] = i + 5;
    }
    return ATT_SUCCESS;
}

/*************************************************************************************************/
/*!
*
*  \brief  Get peer key from a device database record.
*
*  \param  pMsg    Pointer to DM callback event message.
*
*  \return Pointer to peer key if key is valid or NULL if not valid.
*/
/*************************************************************************************************/
//static dmSecKey_t *datsGetPeerKey(dmEvt_t *pMsg)
//{
//  appConnCb_t *pCb;
//
//  PR_INFO("%d", pMsg->hdr.param);
//
//  /* look up app connection control block from DM connection ID */
//  pCb = &appConnCb[pMsg->hdr.param - 1];
//
//  /* if database record handle valid */
//  if (pCb->dbHdl != APP_DB_HDL_NONE)
//  {
//    return AppDbGetKey(pCb->dbHdl, DM_KEY_IRK, NULL);
//  }
//
//  return NULL;
//}

/*************************************************************************************************/
/*!
*
*  \brief  Handle add device to resolving list indication.
*
*  \param  pMsg    Pointer to DM callback event message.
*
*  \return None.
*/
/*************************************************************************************************/
//static void datsPrivAddDevToResListInd(dmEvt_t *pMsg)
//{
//  PR_INFO("%d", pMsg->hdr.param);
//
//#ifndef BTLE_APP_USE_LEGACY_API
//  uint8_t addrZeros[BDA_ADDR_LEN] = { 0 };
//#endif /* BTLE_APP_USE_LEGACY_API */
//
//  dmSecKey_t *pPeerKey;
//
//  /* if peer IRK present */
//  if ((pPeerKey = datsGetPeerKey(pMsg)) != NULL)
//  {
//    /* set advertising peer address */
//#ifdef BTLE_APP_USE_LEGACY_API
//    AppSetAdvPeerAddr(pPeerKey->irk.addrType, pPeerKey->irk.bdAddr);
//#else /* BTLE_APP_USE_LEGACY_API */
//    AppExtSetAdvPeerAddr(DM_ADV_HANDLE_DEFAULT, HCI_ADDR_TYPE_PUBLIC, addrZeros);
//#endif /* BTLE_APP_USE_LEGACY_API */
//  }
//}

/*************************************************************************************************/
/*!
*
*  \brief  Handle remove device from resolving list indication.
*
*  \param  pMsg    Pointer to DM callback event message.
*
*  \return None.
*/
/*************************************************************************************************/
//static void datsPrivRemDevFromResListInd(dmEvt_t *pMsg)
//{
//  PR_INFO("%d", pMsg->hdr.param);
//
//  if (pMsg->hdr.status == HCI_SUCCESS)
//  {
//    appConnCb_t *pCb;
//
//    /* look up app connection control block from DM connection ID */
//    pCb = &appConnCb[pMsg->hdr.param - 1];
//
//    if (pCb->dbHdl != APP_DB_HDL_NONE)
//    {
//      uint8_t addrZeros[BDA_ADDR_LEN] = { 0 };
//
//      /* clear advertising peer address and its type */
//#ifdef BTLE_APP_USE_LEGACY_API
//      AppSetAdvPeerAddr(HCI_ADDR_TYPE_PUBLIC, addrZeros);
//#else /* BTLE_APP_USE_LEGACY_API */
//      AppExtSetAdvPeerAddr(DM_ADV_HANDLE_DEFAULT, HCI_ADDR_TYPE_PUBLIC, addrZeros);
//#endif /* BTLE_APP_USE_LEGACY_API */
//    }
//  }
//}

/*************************************************************************************************/
/*!
 *
 *  \brief  Display stack version.
 *
 *  \param  version    version number.
 *
 *  \return None.
 */
/*************************************************************************************************/
void datsDisplayStackVersion(const char *pVersion)
{
    PR_INFO("%s", pVersion);

    APP_TRACE_INFO1("Stack Version: %s", pVersion);
}

/*************************************************************************************************/
/*!
 *  \brief  Set up advertising and other procedures that need to be performed after
 *          device reset.
 *
 *  \param  pMsg    Pointer to message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void datsSetup(dmEvt_t *pMsg)
{
    PR_INFO("%d", pMsg->hdr.param);

#ifndef BTLE_APP_USE_LEGACY_API
    uint8_t advHandle;
#endif /* BTLE_APP_USE_LEGACY_API */

    /* set advertising and scan response data for discoverable mode */
#ifdef BTLE_APP_USE_LEGACY_API
    AppAdvSetData(APP_ADV_DATA_DISCOVERABLE, sizeof(datsAdvDataDisc), (uint8_t *) datsAdvDataDisc);
    AppAdvSetData(APP_SCAN_DATA_DISCOVERABLE, sizeof(datsScanDataDisc), (uint8_t *) datsScanDataDisc);
#else /* BTLE_APP_USE_LEGACY_API */
    AppExtAdvSetData(DM_ADV_HANDLE_DEFAULT, APP_ADV_DATA_DISCOVERABLE, sizeof(datsExtAdvDataDisc), (uint8_t *) datsExtAdvDataDisc, HCI_EXT_ADV_DATA_LEN);
    AppExtAdvSetData(DM_ADV_HANDLE_DEFAULT, APP_SCAN_DATA_DISCOVERABLE, sizeof(datsExtScanDataDisc), (uint8_t *) datsExtScanDataDisc, HCI_EXT_ADV_DATA_LEN);
#endif /* BTLE_APP_USE_LEGACY_API */


    /* set advertising and scan response data for connectable mode */
#ifdef BTLE_APP_USE_LEGACY_API
    AppAdvSetData(APP_ADV_DATA_CONNECTABLE, sizeof(datsAdvDataDisc), (uint8_t *) datsAdvDataDisc);
    AppAdvSetData(APP_SCAN_DATA_CONNECTABLE, sizeof(datsScanDataDisc), (uint8_t *) datsScanDataDisc);
    //  AppAdvSetData(APP_ADV_DATA_CONNECTABLE, 0, NULL);
    //  AppAdvSetData(APP_SCAN_DATA_CONNECTABLE, 0, NULL);
#else /* BTLE_APP_USE_LEGACY_API */
    AppExtAdvSetData(DM_ADV_HANDLE_DEFAULT, APP_ADV_DATA_CONNECTABLE, sizeof(datsExtAdvDataDisc), (uint8_t *) datsExtAdvDataDisc, HCI_EXT_ADV_DATA_LEN);
    AppExtAdvSetData(DM_ADV_HANDLE_DEFAULT, APP_SCAN_DATA_CONNECTABLE, sizeof(datsExtScanDataDisc), (uint8_t *) datsExtScanDataDisc, HCI_EXT_ADV_DATA_LEN);
#endif /* BTLE_APP_USE_LEGACY_API */

    /* Enable coded PHY */
    if(MXC_GCR->revision != 0xA1) {
        DmSetDefaultPhy(0, HCI_PHY_LE_1M_BIT | HCI_PHY_LE_2M_BIT | HCI_PHY_LE_CODED_BIT,
                HCI_PHY_LE_1M_BIT | HCI_PHY_LE_2M_BIT | HCI_PHY_LE_CODED_BIT);
    } else {
        DmSetDefaultPhy(0, HCI_PHY_LE_1M_BIT | HCI_PHY_LE_2M_BIT, HCI_PHY_LE_1M_BIT | HCI_PHY_LE_2M_BIT);
    }

    /* start advertising; automatically set connectable/discoverable mode and bondable mode */
#ifndef BTLE_APP_USE_LEGACY_API
    advHandle = DM_ADV_HANDLE_DEFAULT;
#endif /* BTLE_APP_USE_LEGACY_API */

#ifdef BTLE_APP_USE_LEGACY_API
    AppAdvStart(APP_MODE_AUTO_INIT);
#else /* BTLE_APP_USE_LEGACY_API */
    AppExtAdvStart(1, &advHandle, APP_MODE_AUTO_INIT);
#endif /* BTLE_APP_USE_LEGACY_API */
}

/*************************************************************************************************/
/*!
 *  \brief  Process messages from the event handler.
 *
 *  \param  pMsg    Pointer to message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void datsProcMsg(dmEvt_t *pMsg)
{
  uint8_t uiEvent = APP_UI_NONE;

  switch(pMsg->hdr.event)
  {
    case ATTS_HANDLE_VALUE_CNF:
      PR_INFO("ATTS_HANDLE_VALUE_CNF");
      break;

    case ATTS_CCC_STATE_IND:
      PR_INFO("ATTS_CCC_STATE_IND");
      break;

    case DM_RESET_CMPL_IND:
      PR_INFO("DM_RESET_CMPL_IND");
      DmSecGenerateEccKeyReq();
      datsSetup(pMsg);
      uiEvent = APP_UI_RESET_CMPL;
      break;

    case DM_ADV_START_IND:
      PR_INFO("DM_ADV_START_IND");
      uiEvent = APP_UI_ADV_START;
      break;

#ifndef BTLE_APP_IGNORE_EXT_EVENTS
    case DM_ADV_SET_START_IND:
      PR_INFO("DM_ADV_SET_START_IND");
      uiEvent = APP_UI_ADV_SET_START_IND;
      break;
#endif /* BTLE_APP_IGNORE_EXT_EVENTS */

    case DM_ADV_STOP_IND:
      PR_INFO("DM_ADV_STOP_IND");
      uiEvent = APP_UI_ADV_STOP;
      break;

#ifndef BTLE_APP_IGNORE_EXT_EVENTS
     case DM_ADV_SET_STOP_IND:
      PR_INFO("DM_ADV_SET_STOP_IND");
      uiEvent = APP_UI_ADV_SET_STOP_IND;
      break;
#endif /* BTLE_APP_IGNORE_EXT_EVENTS */

    case DM_CONN_OPEN_IND:
      PR_INFO("Connection opened");
      PR_INFO("connId: %d", pMsg->connOpen.hdr.param);
      PR_INFO("peerAddr: %02X:%02X:%02X:%02X:%02X:%02X",
          pMsg->connOpen.peerAddr[5], pMsg->connOpen.peerAddr[4], pMsg->connOpen.peerAddr[3],
          pMsg->connOpen.peerAddr[2], pMsg->connOpen.peerAddr[1], pMsg->connOpen.peerAddr[0]);
      currentConnId = (dmConnId_t) pMsg->connOpen.hdr.param;
      PR_INFO("MTU: %d", Ble_GetCurrentMtuSize());
      uiEvent = APP_UI_CONN_OPEN;
      break;

    case DM_CONN_CLOSE_IND:
      PR_INFO("Connection closed status 0x%02hhX, reason 0x%02hhX", pMsg->connClose.status, pMsg->connClose.reason);
      switch (pMsg->connClose.reason)
      {
        case HCI_ERR_CONN_TIMEOUT:      PR_INFO(" TIMEOUT");         break;
        case HCI_ERR_LOCAL_TERMINATED:  PR_INFO(" LOCAL TERM");      break;
        case HCI_ERR_REMOTE_TERMINATED: PR_INFO(" REMOTE TERM");     break;
        case HCI_ERR_CONN_FAIL:         PR_INFO(" FAIL ESTABLISH");  break;
        case HCI_ERR_MIC_FAILURE:       PR_INFO(" MIC FAILURE");     break;
      }
      currentConnId = 0;
      uiEvent = APP_UI_CONN_CLOSE;
      break;

    case DM_CONN_DATA_LEN_CHANGE_IND:
      PR_INFO("Data len changed maxTxOctets: %d, maxRxOctets: %d", pMsg->dataLenChange.maxTxOctets, pMsg->dataLenChange.maxRxOctets);
      PR_INFO("MTU: %d", Ble_GetCurrentMtuSize());
      uiEvent = DM_CONN_DATA_LEN_CHANGE_IND;
      break;

    case DM_SEC_PAIR_CMPL_IND:
      PR_INFO("Pairing completed successfully.");
      uiEvent = APP_UI_SEC_PAIR_CMPL;
      break;

    case DM_SEC_PAIR_FAIL_IND:
      PR_INFO("DM_SEC_PAIR_FAIL_IND");
      uiEvent = APP_UI_SEC_PAIR_FAIL;
      break;

    case DM_SEC_ENCRYPT_IND:
      PR_INFO("DM_SEC_ENCRYPT_IND");
      uiEvent = APP_UI_SEC_ENCRYPT;
      break;

    case DM_SEC_ENCRYPT_FAIL_IND:
      PR_INFO("DM_SEC_ENCRYPT_FAIL_IND");
      uiEvent = APP_UI_SEC_ENCRYPT_FAIL;
      break;

    case DM_SEC_AUTH_REQ_IND:
      PR_INFO("DM_SEC_AUTH_REQ_IND");

//      if (pMsg->authReq.oob)
//      {
//        dmConnId_t connId = (dmConnId_t) pMsg->hdr.param;
//
//        /* TODO: Perform OOB Exchange with the peer. */
//
//
//        /* TODO: Fill datsOobCfg peerConfirm and peerRandom with value passed out of band */
//
//        if (datsOobCfg != NULL)
//        {
//          DmSecSetOob(connId, datsOobCfg);
//        }
//
//        DmSecAuthRsp(connId, 0, NULL);
//      }
//      else
//      {
        AppHandlePasskey(&pMsg->authReq);
//      }
      break;

    case DM_SEC_COMPARE_IND:
      PR_INFO("DM_SEC_COMPARE_IND");
      AppHandleNumericComparison(&pMsg->cnfInd);
      break;

    case DM_PRIV_ADD_DEV_TO_RES_LIST_IND:
      PR_INFO("DM_PRIV_ADD_DEV_TO_RES_LIST_IND");
//      datsPrivAddDevToResListInd(pMsg);
      break;

    case DM_PRIV_REM_DEV_FROM_RES_LIST_IND:
      PR_INFO("DM_PRIV_REM_DEV_FROM_RES_LIST_IND");
//      datsPrivRemDevFromResListInd(pMsg);
      break;

    case DM_ADV_NEW_ADDR_IND:
      PR_INFO("DM_ADV_NEW_ADDR_IND");
      break;

    case DM_VENDOR_SPEC_IND:
      PR_INFO("DM_VENDOR_SPEC_IND");
      break;

    default:
      PR_INFO("Event 0x%02hhX", pMsg->hdr.event);
      break;
  }

  if (uiEvent != APP_UI_NONE)
  {
      AppUiAction(uiEvent);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Application handler init function called during system initialization.
 *
 *  \param  handlerID  WSF handler ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DatsHandlerInit(wsfHandlerId_t handlerId)
{
    APP_TRACE_INFO0("DatsHandlerInit");

    /* store handler ID */
    datsCb.handlerId = handlerId;

    /* Set configuration pointers */
    pAppSlaveCfg = (appSlaveCfg_t *) &datsSlaveCfg;
#ifdef BTLE_APP_USE_LEGACY_API
    pAppAdvCfg = (appAdvCfg_t *) &datsAdvCfg;
#else /* BTLE_APP_USE_LEGACY_API */
    pAppExtAdvCfg = (appExtAdvCfg_t *) &datsExtAdvCfg;
#endif /* BTLE_APP_USE_LEGACY_API */
    pAppSecCfg = (appSecCfg_t *) &datsSecCfg;
    pAppUpdateCfg = (appUpdateCfg_t *) &datsUpdateCfg;
    pSmpCfg = (smpCfg_t *) &datsSmpCfg;
    pAttCfg = (attCfg_t *) &datsAttCfg;

    /* Initialize application framework */
    AppSlaveInit();

    /* Set IRK for the local device */
    //  DmSecSetLocalIrk(localIrk);
}

/*************************************************************************************************/
/*!
 *  \brief  Callback for WSF buffer diagnostic messages.
 *
 *  \param  pInfo     Diagnostics message
 *
 *  \return None.
 */
/*************************************************************************************************/
static void datsWsfBufDiagnostics(WsfBufDiag_t *pInfo)
{
    if (pInfo->type == WSF_BUF_ALLOC_FAILED)
    {
        APP_TRACE_INFO2("Dats got WSF Buffer Allocation Failure - Task: %d Len: %d",
                pInfo->param.alloc.taskId, pInfo->param.alloc.len);
    }
}

/*************************************************************************************************/
/*!
 *  \brief  WSF event handler for application.
 *
 *  \param  event   WSF event mask.
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DatsHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
    if (pMsg != NULL)
    {
        APP_TRACE_INFO1("Dats got evt 0x%x", pMsg->event);

        if (pMsg->event >= DM_CBACK_START && pMsg->event <= DM_CBACK_END)
        {
            /* process advertising and connection-related messages */
            AppSlaveProcDmMsg((dmEvt_t *) pMsg);

            /* process security-related messages */
            AppSlaveSecProcDmMsg((dmEvt_t *) pMsg);
        }

        /* perform profile and user interface-related operations */
        datsProcMsg((dmEvt_t *) pMsg);
    }
}

/*************************************************************************************************/
/*!
 *  \brief  Start the application.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DatsStart(void)
{
    /* Register for stack callbacks */
    DmRegister(datsDmCback);
    DmConnRegister(DM_CLIENT_ID_APP, datsDmCback);
    AttRegister(datsAttCback);
    AttConnRegister(AppServerConnCback);
    AttsCccRegister(DATS_NUM_CCC_IDX, (attsCccSet_t *) datsCccSet, datsCccCback);

    /* Initialize attribute server database */
    SvcCoreAddGroup();
    SvcWpCbackRegister(datsWpReadCback, datsWpWriteCback);
    SvcWpAddGroup();

    WsfBufDiagRegister(datsWsfBufDiagnostics);

    /* Reset the device */
    DmDevReset();
}

#ifdef WSF_TRACE_ENABLED
static bool_t myTrace(const uint8_t *pBuf, uint32_t len)
{
    extern uint8_t wsfCsNesting;

    if (wsfCsNesting == 0)
    {
        WsfTaskLock();
        while(SEMA_GetSema(MAX32666_SEMAPHORE_PRINT) == E_BUSY) {}
        fwrite(pBuf, len, 1, stdout);
        fflush(stdout);
        SEMA_FreeSema(MAX32666_SEMAPHORE_PRINT);
        WsfTaskUnlock();
        return TRUE;
    }

    return FALSE;
}
#endif

/*************************************************************************************************/
/*!
 *  \brief  Initialize WSF.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void WsfInit(void)
{
    uint32_t bytesUsed;
    /* setup the systick for 1MS timer*/
    SysTick->LOAD = (SystemCoreClock / 1000) * WSF_MS_PER_TICK;
    SysTick->VAL = 0;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);

    WsfTimerInit();

    SystemHeapStart = (uint32_t)&SystemHeap;
    memset(SystemHeap, 0, sizeof(SystemHeap));

    PR_INFO("SystemHeapStart = 0x%x", SystemHeapStart);
    PR_INFO("SystemHeapSize = 0x%x", SystemHeapSize);
    bytesUsed = WsfBufInit(WSF_BUF_POOLS, mainPoolDesc);
    PR_INFO("bytesUsed = 0x%x", bytesUsed);

#ifdef WSF_TRACE_ENABLED
    WsfTraceRegisterHandler(myTrace);
    WsfTraceEnable(TRUE);
#endif
}

/*
 * In two-chip solutions, setting the address must wait until the HCI interface is initialized.
 * This handler can also catch other Application events, but none are currently implemented.
 * See ble-profiles/sources/apps/app/common/app_ui.c for further details.
 *
 */
void uiHandler(uint8_t event)
{
  uint8_t usn[SYS_USN_CHECKSUM_LEN];
  uint8_t checksum[SYS_USN_CHECKSUM_LEN];
  // MA-L assigend by IEEE to Maxim Integrated Products
  uint8_t bdAddr[6] = {0x00, 0x00, 0x00, 0x80, 0x18, 0x00};

  PR_INFO("Event: 0x%02hhX", event);

  switch (event) {
  case APP_UI_RESET_CMPL:
    if(SYS_GetUSN(usn, checksum) != E_NO_ERROR) {
      PR_ERROR("Error getting Checksum");
    }

    if(SYS_GetUSN(usn, checksum) != E_NO_ERROR) {
      PR_ERROR("Error getting Checksum");
    }

    PR_INFO("USN: ");
    for (int i = 0; i < SYS_USN_CHECKSUM_LEN; i++) {
        PR("0x%02hhX ", usn[i]);
    }
    PR("\n");

    PR_INFO("USN Checksum: ");
    for (int i = 0; i < SYS_USN_CHECKSUM_LEN; i++) {
        PR("0x%02hhX ", checksum[i]);
    }
    PR("\n");

    // USN checksum
    bdAddr[2] = checksum[2];
    bdAddr[1] = checksum[1];
    bdAddr[0] = checksum[0];

    PR_INFO("Setting address -- MAC %02X:%02X:%02X:%02X:%02X:%02X", bdAddr[5], bdAddr[4], bdAddr[3], bdAddr[2], bdAddr[1], bdAddr[0]);
    HciVsSetBdAddr(bdAddr);
    break;
  default:
      break;
  }
}

int Ble_GetCurrentMtuSize(void)
{
    if (!currentConnId) {
        return 0;
    }

    return AttGetMtu(currentConnId);
}

int Ble_GetCurrentPacketSize(void)
{
    if (!currentConnId) {
        return 0;
    }

    return AttGetMtu(currentConnId) - 3;
}

int Ble_IsConnected(void)
{
    if (currentConnId) {
        return TRUE;
    }

    return FALSE;
}

int Ble_SendNotification(uint16_t dataLen, uint8_t *data)
{
    if (!currentConnId) {
      PR_ERROR("No connection");
      return E_NO_DEVICE;
    }

    if (!AttsCccEnabled(currentConnId, DATS_WP_DAT_CCC_IDX)) {
      PR_ERROR("Notification is not enabled");
      return E_BAD_STATE;
    }

    PR_INFO("TX notification len: %d, data:", dataLen);
    for (int i = 0; i < dataLen; i++) {
        PR("0x%02hhX ", data[i]);
    }
    PR("\n");

    for (int i = 0; i < 10000; i++) {
        wsfOsDispatcher();
    }

    AttsHandleValueNtf(currentConnId, WP_DAT_HDL, dataLen, data);

    return E_SUCCESS;
}

int Ble_SendIndication(uint16_t dataLen, uint8_t *data)
{
    if (!currentConnId) {
      PR_ERROR("No connection");
      return E_NO_DEVICE;
    }

    if (!AttsCccEnabled(currentConnId, DATS_WP_DAT_CCC_IDX)) {
      PR_ERROR("Indications is not enabled");
      return E_BAD_STATE;
    }

    PR_INFO("TX indication len: %d", dataLen);
    for (int i = 0; i < dataLen; i++) {
        PR("0x%02hhX ", data[i]);
    }
    PR("\n");

    for (int try = 0; try < 3; try++) {
        indicationStatus = ATT_RSP_PENDING;

        AttsHandleValueInd(currentConnId, WP_DAT_HDL, dataLen, data);

        for (int i = 0; (i < 1000000) && (indicationStatus == ATT_RSP_PENDING); i++) {
            wsfOsDispatcher();
        }

        if (indicationStatus == ATT_SUCCESS) {
            return E_SUCCESS;
        }
    }

    return E_COMM_ERR;
}

static void Ble_Receive(uint16_t dataLen, uint8_t *data)
{
    PR_INFO("RX len: %d", dataLen);
    for (int i = 0; i < dataLen; i++) {
      PR("0x%02hhX ", data[i]);
    }
    PR("\n");
}

int Ble_Init(void)
{
    /* Initialize Radio */
    WsfInit();
    StackInitDats();
    DatsStart();

    /* Register a handler for Application events */
    AppUiActionRegister(uiHandler);

    PR_INFO("BLE init completed");

    return E_NO_ERROR;
}

int Ble_Worker(void)
{
    wsfOsDispatcher();
    return E_NO_ERROR;
}
