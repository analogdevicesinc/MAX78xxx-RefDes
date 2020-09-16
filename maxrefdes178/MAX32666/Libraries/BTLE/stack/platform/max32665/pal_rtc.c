/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      RTC timer driver.
 *
 *  Copyright (c) 2018 Arm Ltd.
 *
 *  Copyright (c) 2019 Packetcraft, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#include "pal_rtc.h"
#include "max32665.h"
#include "wut_regs.h"
#include "gcr_regs.h"
#include "wut.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/**************************************************************************************************
  Local Variables
**************************************************************************************************/
#ifndef __CROSSWORKS
static fnPalRtcIRQHook_t pfnRTCIntHook = NULL;
static void * pPalRtcIRQHookArg;
#endif /* __CROSSWORKS */

/**************************************************************************************************
  Global Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Interrupt service routine.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WUT_IRQHandler(void)
{
  WUT_IntClear();
  NVIC_ClearPendingIRQ(WUT_IRQn);
#ifndef __CROSSWORKS
  if (pfnRTCIntHook != NULL) {
    (pfnRTCIntHook)(pPalRtcIRQHookArg);
  }
#endif /* __CROSSWORKS */
}

/*************************************************************************************************/
/*!
 *  \brief  Wait for RTC edge.
 *
 *  \return None.
 */
/*************************************************************************************************/
void palRtcEdge(void)
{
  uint32_t preEdgeCnt;
  preEdgeCnt = WUT_GetCount();
  while (WUT_GetCount() == preEdgeCnt) {}
}

/*************************************************************************************************/
/*!
 *  \brief  Tickless timer initialization routine.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PalRtcInit(void)
{
  /* Enable WUT as a wakup source */
  MXC_GCR->pm |= MXC_F_GCR_PM_WUTWKEN;

  /* Init WUT */
  WUT_Shutdown();
  WUT_IntClear();
  //WUT_SetPres(MXC_S_WUT_CN_PRES_DIV1);
  //WUT_SetMode(WUT_MODE_COMPARE);
  MXC_WUT->cn = (MXC_S_WUT_CN_PRES_DIV1 | MXC_S_WUT_CN_TMODE_COMPARE);
  WUT_SetCompare(0xFFFFFFFF);

  NVIC_SetPriority(WUT_IRQn, 0x80);    /* medium priority */
  NVIC_ClearPendingIRQ(WUT_IRQn);

  /* Enable WUT */
  WUT_Enable();
}

#ifndef __CROSSWORKS
/*************************************************************************************************/
/*!
 *  \brief  Function for intercepting RTC interrupts.
 *
 *  \param pfnHook A user specified function to be called on RTC interrupts.
 *  \param pHookArg A user provided argument to be passed to the hook function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PalRtcRegisterHook(fnPalRtcIRQHook_t pfnHook, void * pHookArg)
{
    __disable_irq();
    pPalRtcIRQHookArg = pHookArg;
    pfnRTCIntHook = pfnHook;
    __enable_irq();
}
#endif /* __CROSSWORKS */

/*************************************************************************************************/
/*!
 *  \brief  Function for starting the RTC timer.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PalRtcEnableCompareIrq(void)
{
  NVIC_EnableIRQ(WUT_IRQn);
}

/*************************************************************************************************/
/*!
 *  \brief  Function for stopping the RTC timer.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PalRtcDisableCompareIrq(void)
{
  NVIC_DisableIRQ(WUT_IRQn);
}

/*************************************************************************************************/
/*!
 *  \brief  Get the current value of the RTC counter.
 *
 *  \return Current value of the RTC counter.
 */
/*************************************************************************************************/
uint32_t PalRtcCounterGet(void)
{
  uint32_t wutCnt;
  palRtcEdge();
  wutCnt = WUT_GetCount();
  return(wutCnt);
}

/*************************************************************************************************/
/*!
 *  \brief  Set the RTC capture compare value.
 *
 *  \param  value   Set new value for compare value.
 *
 *  \return None
 */
/*************************************************************************************************/
void PalRtcCompareSet(uint32_t value)
{
  WUT_SetCompare(value);
}

/*************************************************************************************************/
/*!
 *  \brief  Get the current value of the RTC capture compare.
 *
 *  \return Current value of the capture compare.
 */
/*************************************************************************************************/
uint32_t PalRtcCompareGet(void)
{
  return WUT_GetCompare();
}

