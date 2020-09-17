/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  System hooks.
 *
 *  Copyright (c) 2016-2019 Arm Ltd.
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

#include "pal_sys.h"
#include "pal_led.h"
#include "pal_rtc.h"

#ifdef __IAR_SYSTEMS_ICC__
#include <intrinsics.h>
#endif

/*! \brief      Number of assertions. */
static uint32_t palSysAssertCount;

/*! \brief      Trap enabled flag. */
static volatile bool_t PalSysAssertTrapEnable;

static uint32_t palSysBusyCount;

static PalSysSleep_t sleepState;

/*************************************************************************************************/
/*!
 *  \brief  Enter a critical section.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void palEnterCs(void)
{
#ifdef __IAR_SYSTEMS_ICC__
  __disable_interrupt();
#endif
#ifdef __GNUC__
  __asm volatile ("cpsid i");
#endif
#ifdef __CC_ARM
  __disable_irq();
#endif
}

/*************************************************************************************************/
/*!
 *  \brief  Exit a critical section.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void palExitCs(void)
{
#ifdef __IAR_SYSTEMS_ICC__
  __enable_interrupt();
#endif
#ifdef __GNUC__
  __asm volatile ("cpsie i");
#endif
#ifdef __CC_ARM
  __enable_irq();
#endif
}

/*************************************************************************************************/
/*!
 *  \brief      System fault trap.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalSysAssertTrap(void)
{
  PalLedOn(PAL_LED_ID_ERROR);

  palSysAssertCount++;

  while (PalSysAssertTrapEnable);
}

/*************************************************************************************************/
/*!
 *  \brief      Set system trap.
 *
 *  \param      enable    Enable assert trap or not.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalSysSetTrap(bool_t enable)
{
  PalSysAssertTrapEnable = enable;
}

/*************************************************************************************************/
/*!
 *  \brief      Get assert count.
 *
 *  \return     None.
 */
/*************************************************************************************************/
uint32_t PalSysGetAssertCount(void)
{
  return palSysAssertCount;
}

/*************************************************************************************************/
/*!
 *  \brief      Count stack usage.
 *
 *  \return     Number of bytes used by the stack.
 */
/*************************************************************************************************/
uint32_t PalSysGetStackUsage(void)
{
  /* Not available; stub routine. */
  return 0;
}

/*************************************************************************************************/
/*!
 *  \brief      System sleep.
 *
 *  \return     none.
 */
/*************************************************************************************************/
void PalSysSleep(void)
{
#ifdef __IAR_SYSTEMS_ICC__
  __WFI();
#endif
#ifdef __GNUC__
  __asm volatile ("wfi");
#endif
#ifdef __CC_ARM
  __wfi();
#endif
}

/*************************************************************************************************/
/*!
 *  \brief      Check if system is busy.
 *
 *  \return     True if system is busy.
 */
/*************************************************************************************************/
bool_t PalSysIsBusy(void)
{
  bool_t sysIsBusy = FALSE;
  palEnterCs();
  sysIsBusy = ((palSysBusyCount == 0) ? FALSE : TRUE);
  palExitCs();
  return sysIsBusy;
}

/*************************************************************************************************/
/*!
 *  \brief      Set system busy.
 *
 *  \return     none.
 */
/*************************************************************************************************/
void PalSysSetBusy(void)
{
  palEnterCs();
  palSysBusyCount++;
  palExitCs();
}

/*************************************************************************************************/
/*!
 *  \brief      Set system idle.
 *
 *  \return     none.
 */
/*************************************************************************************************/
void PalSysSetIdle(void)
{
  palEnterCs();
  if (palSysBusyCount) {
    palSysBusyCount--;
  }
  palExitCs();
}

/*************************************************************************************************/
/*!
 *  \brief      Common platform initialization.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalSysInit(void)
{
  PalRtcInit();
  PalLedOff(PAL_LED_ID_ERROR);
  PalLedOn(PAL_LED_ID_CPU_ACTIVE);
  sleepState = PAL_SYS_SLEEP;
}

/*************************************************************************************************/
/*!
 *  \brief      Get the sleep state.
 *
 *  \return     Sleep state for the next sleep event.
 */
/*************************************************************************************************/
PalSysSleep_t PalSysGetSleepState(void)
{
  return sleepState;
}

/*************************************************************************************************/
/*!
 *  \brief      Set the sleep state for the next sleep event.
 *
 *  \return     None.
 */
/*************************************************************************************************/

void PalSysSetSleepState(PalSysSleep_t state)
{
  sleepState = state;
}
