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
#if defined(HCI_TR_EXACTLE) && (HCI_TR_EXACTLE == 1)
#include <ll_init_api.h>
#endif
#include <pal_bb.h>
#include <pal_cfg.h>
#include <sec_api.h>
#include <smp_api.h>
#include <smp_handler.h>
#include <sema.h>
#include <stdio.h>
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

#include "max32666_ble.h"
#include "max32666_debug.h"
#include "maxrefdes178_definitions.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME       "Ble"

/*! \brief UART TX buffer size */
#define PLATFORM_UART_TERMINAL_BUFFER_SIZE      2048U


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------
/*! Enumeration of client characteristic configuration descriptors */
enum
{
    PERIPH_GATT_SC_CCC_IDX,           /*! GATT service, service changed characteristic */
    PERIPH_WP_DAT_CCC_IDX,            /*! Arm Ltd. proprietary service, data transfer characteristic */
    PERIPH_NUM_CCC_IDX
};


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
#if defined(HCI_TR_EXACTLE) && (HCI_TR_EXACTLE == 1)
static LlRtCfg_t mainLlRtCfg;
#endif

/*! \brief  Pool runtime configuration. */
static wsfBufPoolDesc_t mainPoolDesc[] =
{
    { 16,              8 },
    { 32,              4 },
    { 192,             8 },
    { 256,             8 }
};

/*! configurable parameters for advertising */
static const appAdvCfg_t periphAdvCfg =
{
    {30000,     0,     0},                  /*! Advertising durations in ms, 0 corresponds to infinite */
    {   96,  1600,     0}                   /*! Advertising intervals in 0.625 ms units */
};

/*! configurable parameters for slave */
static const appSlaveCfg_t periphSlaveCfg =
{
    1,                                      /*! Maximum connections */
};

/*! ATT configurable parameters (increase MTU) */
static const attCfg_t periphAttCfg =
{
    15,                               /* ATT server service discovery connection idle timeout in seconds */
//ty  241,                              /* desired ATT MTU */
    BLE_MAX_MTU_SIZE,                 /* desired ATT MTU */
    ATT_MAX_TRANS_TIMEOUT,            /* transcation timeout in seconds */
    4                                 /* number of queued prepare writes supported by server */
};

/*! advertising data, discoverable mode */
static const uint8_t periphAdvDataDisc[] =
{
    /*! flags */
    2,                                      /*! length */
    DM_ADV_TYPE_FLAGS,                      /*! AD type */
    DM_FLAG_LE_GENERAL_DISC |               /*! flags */
    DM_FLAG_LE_BREDR_NOT_SUP,
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

/*! client characteristic configuration descriptors settings, indexed by above enumeration */
static const attsCccSet_t periphCccSet[PERIPH_NUM_CCC_IDX] =
{
    /* cccd handle          value range               security level */
    {GATT_SC_CH_CCC_HDL,    ATT_CLIENT_CFG_INDICATE,  DM_SEC_LEVEL_NONE},   /* PERIPH_GATT_SC_CCC_IDX */
//ty  {WP_DAT_CH_CCC_HDL,     ATT_CLIENT_CFG_NOTIFY,    DM_SEC_LEVEL_NONE}    /* DATS_WP_DAT_CCC_IDX */
    {WP_DAT_CH_CCC_HDL,     ATT_CLIENT_CFG_INDICATE,    DM_SEC_LEVEL_NONE}    /* DATS_WP_DAT_CCC_IDX */
};

/*! application control block */
static struct
{
    wsfHandlerId_t    handlerId;          /* WSF handler ID */
    dmConnId_t        connId;             /* Connection ID */
    bool_t            connected;          /* Connection state */
    uint8_t           indicationStatus;   /* Connection state */
} periphCb;


//-----------------------------------------------------------------------------
// Local Function declarations
//-----------------------------------------------------------------------------
static void periphDmCback(dmEvt_t *pDmEvt);
static void periphAttCback(attEvt_t *pEvt);
static void periphCccCback(attsCccEvt_t *pEvt);
static uint8_t periphWpWriteCback(dmConnId_t connId, uint16_t handle, uint8_t operation,
                          uint16_t offset, uint16_t len, uint8_t *pValue, attsAttr_t *pAttr);
static void periphSetup(dmEvt_t *pMsg);
static void periphProcMsg(dmEvt_t *pMsg);
static void PeriphHandlerInit(wsfHandlerId_t handlerId);
static void PeriphHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);
static void PeriphStart(void);
static void StackInitPeriph(void);
static void mainWsfInit(void);
static void ble_receive(uint16_t dataLen, uint8_t *data);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
// Application DM callback.
static void periphDmCback(dmEvt_t *pDmEvt)
{
    dmEvt_t   *pMsg;
    uint16_t  len;

    PR_INFO("Event: 0x%02hhX", pDmEvt->hdr.event);

    len = DmSizeOfEvt(pDmEvt);

    if ((pMsg = WsfMsgAlloc(len)) != NULL)
    {
        memcpy(pMsg, pDmEvt, len);
        WsfMsgSend(periphCb.handlerId, pMsg);
    }
}

// Application ATT callback.
static void periphAttCback(attEvt_t *pEvt)
{
    attEvt_t  *pMsg;

    switch(pEvt->hdr.event)
    {
    case ATTS_HANDLE_VALUE_CNF:
        PR_DEBUG("ATTS_HANDLE_VALUE_CNF");

        if (pEvt->handle == WP_DAT_HDL) {
            periphCb.indicationStatus = pEvt->hdr.status;
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

    if ((pMsg = WsfMsgAlloc(sizeof(attEvt_t) + pEvt->valueLen)) != NULL)
    {
        memcpy(pMsg, pEvt, sizeof(attEvt_t));
        pMsg->pValue = (uint8_t *)(pMsg + 1);
        memcpy(pMsg->pValue, pEvt->pValue, pEvt->valueLen);
        WsfMsgSend(periphCb.handlerId, pMsg);
    }
}

// Application ATTS client characteristic configuration callback.
static void periphCccCback(attsCccEvt_t *pEvt)
{
    PR_INFO("Event: 0x%02hhX idx: %d Value: %d", pEvt->hdr.event, pEvt->idx, pEvt->value);
}

// ATTS write callback for proprietary data service.
static uint8_t periphWpWriteCback(dmConnId_t connId, uint16_t handle, uint8_t operation,
                          uint16_t offset, uint16_t len, uint8_t *pValue, attsAttr_t *pAttr)
{
//  PR_DEBUG("connId: 0x%02hhX", connId);

    ble_receive(len, pValue);

    return ATT_SUCCESS;
}

// ATTS read callback for proprietary data service.
static uint8_t periphWpReadCback(dmConnId_t connId, uint16_t handle, uint8_t operation,
                                  uint16_t offset, attsAttr_t *pAttr)
{
    PR_DEBUG("connId: 0x%02hhX", connId);

    *(pAttr->pLen) = 1;
    for (int i = 0; i < 1; i++ ){
        pAttr->pValue[i] = i + 5;
    }
    return ATT_SUCCESS;
}

// Set up advertising and other procedures that need to be performed after device reset.
static void periphSetup(dmEvt_t *pMsg)
{
    /* set advertising data for discoverable mode */
    AppAdvSetData(APP_ADV_DATA_DISCOVERABLE, sizeof(periphAdvDataDisc), (uint8_t *) periphAdvDataDisc);

    /* set advertising data or connectable mode */
    AppAdvSetData(APP_ADV_DATA_CONNECTABLE, sizeof(periphAdvDataDisc), (uint8_t *) periphAdvDataDisc);

    /* start advertising; automatically set connectable/discoverable mode and bondable mode */
    AppAdvStart(APP_MODE_AUTO_INIT);
}

// Process messages from the event handler.
static void periphProcMsg(dmEvt_t *pMsg)
{
    uint8_t uiEvent = APP_UI_NONE;

    switch(pMsg->hdr.event)
    {
    case DM_RESET_CMPL_IND:
        periphSetup(pMsg);
        uiEvent = APP_UI_RESET_CMPL;
        break;

    case DM_ADV_START_IND:
        uiEvent = APP_UI_ADV_START;
        break;

    case DM_ADV_STOP_IND:
        uiEvent = APP_UI_ADV_STOP;
        break;

    case DM_CONN_OPEN_IND:
        /* Save connId  */
        periphCb.connId = (dmConnId_t) pMsg->hdr.param;
        periphCb.connected = TRUE;

        PR_INFO("Connection opened");
        PR_INFO("connId: %d", pMsg->connOpen.hdr.param);
        PR_INFO("peerAddr: %02X:%02X:%02X:%02X:%02X:%02X",
                pMsg->connOpen.peerAddr[5], pMsg->connOpen.peerAddr[4], pMsg->connOpen.peerAddr[3],
                pMsg->connOpen.peerAddr[2], pMsg->connOpen.peerAddr[1], pMsg->connOpen.peerAddr[0]);
        PR_INFO("MTU: %d", AttGetMtu(periphCb.connId));

        uiEvent = APP_UI_CONN_OPEN;
        break;

    case DM_CONN_CLOSE_IND:
        periphCb.connected = FALSE;

        PR_INFO("Connection closed status 0x%02hhX, reason 0x%02hhX", pMsg->connClose.status, pMsg->connClose.reason);
        switch (pMsg->connClose.reason)
        {
        case HCI_ERR_CONN_TIMEOUT:      PR_INFO(" TIMEOUT");         break;
        case HCI_ERR_LOCAL_TERMINATED:  PR_INFO(" LOCAL TERM");      break;
        case HCI_ERR_REMOTE_TERMINATED: PR_INFO(" REMOTE TERM");     break;
        case HCI_ERR_CONN_FAIL:         PR_INFO(" FAIL ESTABLISH");  break;
        case HCI_ERR_MIC_FAILURE:       PR_INFO(" MIC FAILURE");     break;
        }

        uiEvent = APP_UI_CONN_CLOSE;
        break;

     case DM_CONN_DATA_LEN_CHANGE_IND:
         PR_INFO("Data len changed maxTxOctets: %d, maxRxOctets: %d", pMsg->dataLenChange.maxTxOctets, pMsg->dataLenChange.maxRxOctets);
         PR_INFO("MTU: %d", AttGetMtu(periphCb.connId));
         uiEvent = DM_CONN_DATA_LEN_CHANGE_IND;
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

// Application handler init function called during system initialization.
static void PeriphHandlerInit(wsfHandlerId_t handlerId)
{
    APP_TRACE_INFO0("PeriphHandlerInit");

    /* store handler ID */
    periphCb.handlerId = handlerId;

    /* Set configuration pointers */
    pAppSlaveCfg = (appSlaveCfg_t *) &periphSlaveCfg;
    pAppAdvCfg = (appAdvCfg_t *) &periphAdvCfg;
    pAttCfg = (attCfg_t *) &periphAttCfg;

    /* Initialize application framework */
    AppSlaveInit();
    AppServerInit();
}

// WSF event handler for application.
static void PeriphHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
    if (pMsg != NULL)
    {
        APP_TRACE_INFO1("Periph got evt %d", pMsg->event);

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
        }

        /* perform profile and user interface-related operations */
        periphProcMsg((dmEvt_t *) pMsg);
    }
}

// Start the application.
static void PeriphStart(void)
{
    /* Register for stack callbacks */
    DmRegister(periphDmCback);
    DmConnRegister(DM_CLIENT_ID_APP, periphDmCback);
    AttRegister(periphAttCback);
    AttConnRegister(AppServerConnCback);
    AttsCccRegister(PERIPH_NUM_CCC_IDX, (attsCccSet_t *) periphCccSet, periphCccCback);

    /* Initialize attribute server database */
    SvcCoreGattCbackRegister(GattReadCback, GattWriteCback);
    SvcCoreAddGroup();
    SvcWpCbackRegister(periphWpReadCback, periphWpWriteCback);
    SvcWpAddGroup();

    /* Set Service Changed CCCD index. */
    GattSetSvcChangedIdx(PERIPH_GATT_SC_CCC_IDX);

    /* Reset the device */
    DmDevReset();
}

// Initialize stack.
static void StackInitPeriph(void)
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
    HciSetMaxRxAclLen(BLE_MAX_MTU_SIZE);

    handlerId = WsfOsSetNextHandler(AppHandler);
    AppHandlerInit(handlerId);

    handlerId = WsfOsSetNextHandler(PeriphHandler);
    PeriphHandlerInit(handlerId);
}

#ifdef WSF_TRACE_ENABLED
static bool_t max32666_trace(const uint8_t *pBuf, uint32_t len)
{
    extern uint8_t wsfCsNesting;

    if (wsfCsNesting == 0)
    {
        WsfTaskLock();
        while(MXC_SEMA_GetSema(MAX32666_SEMAPHORE_PRINT) == E_BUSY) {};
        fwrite(pBuf, len, 1, stdout);
        fflush(stdout);
        MXC_SEMA_FreeSema(MAX32666_SEMAPHORE_PRINT);
        WsfTaskUnlock();
        return TRUE;
    }

    return FALSE;
}
#endif

// Initialize WSF.
static void mainWsfInit(void)
{
#if defined(HCI_TR_EXACTLE) && (HCI_TR_EXACTLE == 1)
    /* Configurations must be persistent. */
    static BbRtCfg_t mainBbRtCfg;

    PalBbLoadCfg((PalBbCfg_t *)&mainBbRtCfg);
    LlGetDefaultRunTimeCfg(&mainLlRtCfg);
    PalCfgLoadData(PAL_CFG_ID_LL_PARAM, &mainLlRtCfg.maxAdvSets, sizeof(LlRtCfg_t) - 9);
#endif

    uint32_t memUsed;
    memUsed = WsfBufIoUartInit(WsfHeapGetFreeStartAddress(), PLATFORM_UART_TERMINAL_BUFFER_SIZE);
    WsfHeapAlloc(memUsed);

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

    memUsed = WsfBufInit(numPools, mainPoolDesc);
    WsfHeapAlloc(memUsed);
    WsfOsInit();
    WsfTimerInit();
#if (WSF_TOKEN_ENABLED == TRUE) || (WSF_TRACE_ENABLED == TRUE)
    //ty  WsfTraceRegisterHandler(WsfBufIoWrite);
    WsfTraceRegisterHandler(max32666_trace);
    WsfTraceEnable(TRUE);
#endif

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
    PR_INFO("Setting address -- MAC %02X:%02X:%02X:%02X:%02X:%02X", bdAddr[5], bdAddr[4], bdAddr[3], bdAddr[2], bdAddr[1], bdAddr[0]);
    LlSetBdAddr((uint8_t *)&bdAddr);
    LlMathSetSeed((uint32_t *)&bdAddr);
#endif

    StackInitPeriph();
    PeriphStart();
}

static void ble_receive(uint16_t dataLen, uint8_t *data)
{
    PR_INFO("RX len: %d", dataLen);
    for (int i = 0; i < dataLen; i++) {
      PR("0x%02hhX ", data[i]);
    }
    PR("\n");

    // Increase every byte by 1 and send it back
    for (uint16_t i = 0; i < dataLen; i++) {
        data[i] = data[i] + 1;
    }
    ble_send_indication(dataLen, data);
}

int ble_mtu_size(void)
{
    if (!periphCb.connected) {
        return 0;
    }

    return AttGetMtu(periphCb.connId);
}

int ble_packet_size(void)
{
    if (!periphCb.connected) {
        return 0;
    }

    return AttGetMtu(periphCb.connId) - 3;
}

int ble_connected(void)
{
    return periphCb.connected;
}

int ble_send_indication(uint16_t dataLen, uint8_t *data)
{
    if (!periphCb.connected) {
      PR_ERROR("No connection");
      return E_NO_DEVICE;
    }

    if (!AttsCccEnabled(periphCb.connId, PERIPH_WP_DAT_CCC_IDX)) {
      PR_ERROR("Indications is not enabled");
      return E_BAD_STATE;
    }

    PR_INFO("TX indication len: %d", dataLen);
    for (int i = 0; i < dataLen; i++) {
        PR("0x%02hhX ", data[i]);
    }
    PR("\n");

    for (int try = 0; try < 3; try++) {
        periphCb.indicationStatus = ATT_RSP_PENDING;

        AttsHandleValueInd(periphCb.connId, WP_DAT_HDL, dataLen, data);

        for (int i = 0; (i < 1000000) && (periphCb.indicationStatus == ATT_RSP_PENDING); i++) {
            wsfOsDispatcher();
        }

        if (periphCb.indicationStatus == ATT_SUCCESS) {
            return E_SUCCESS;
        }
    }

    return E_COMM_ERR;
}

int ble_init(void)
{
    mainWsfInit();

    PR_INFO("ble_init completed");

    return E_NO_ERROR;
}

int ble_worker(void)
{
    /* Run the WSF OS */
    wsfOsDispatcher();

    if(!WsfOsActive()) {
      /* No WSF tasks are active, optionally sleep */
    }

    return E_NO_ERROR;
}