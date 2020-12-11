/*************************************************************************************************/
/*!
 * \file
 *
 * \brief  System hooks.
 *
 * Copyright (c) 2019-2020 Packetcraft, Inc.  All rights reserved.
 * Packetcraft, Inc. confidential and proprietary.
 *
 * IMPORTANT.  Your use of this file is governed by a Software License Agreement
 * ("Agreement") that must be accepted in order to download or otherwise receive a
 * copy of this file.  You may not use or copy this file for any purpose other than
 * as described in the Agreement.  If you do not agree to all of the terms of the
 * Agreement do not use this file and delete all copies in your possession or control;
 * if you do not have a copy of the Agreement, you must contact Packetcraft, Inc. prior
 * to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/
#include <string.h>

#include "pal_sys.h"
#include "pal_led.h"
#include "pal_rtc.h"
#include "pal_timer.h"
#include "pal_crypto.h"
#include "pal_uart.h"
#include "pal_bb.h"
#include "mxc_device.h"
#include "mcr_regs.h"
#include "gcr_regs.h"
#include "lp.h"
#include "wut.h"
#include "uart.h"
#include "dma.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

#ifdef __GNUC__

/*! \brief      Stack initial values. */
#define INIT_STACK_VAL    0xAFAFAFAF

/*! \brief      Starting memory location of free memory. */
#define FREE_MEM_START    ((uint8_t *)&__heap_start__)

/*! \brief      Total size in bytes of free memory. */
#define FREE_MEM_SIZE     ((uint32_t)&__heap_end__ - (uint32_t)&__heap_start__)

uint8_t *SystemHeapStart;
uint32_t SystemHeapSize;

extern unsigned int __HeapBase;
extern unsigned int __HeapLimit;
unsigned long __heap_end__;
unsigned long __heap_start__;

extern unsigned int __StackTop;
extern unsigned int __StackLimit;
unsigned long __stack_limit__;
unsigned long __stack_top__;

#else

/*! \brief      Starting memory location of free memory. */
#define FREE_MEM_START          	((uint8_t *)palSysFreeMem)

/*! \brief      Total size in bytes of free memory. */
#define FREE_MEM_SIZE           	(1024 * 196)

#endif

#ifndef PAL_SYS_ENABLE_DS
/* Disable deep sleep when debugging */
#ifdef  DEBUG
#define PAL_SYS_ENABLE_DS           0
#else
#define PAL_SYS_ENABLE_DS           1
#endif
#endif

#define PAL_SYS_MIN_DS_USEC       	5000
#define PAL_SYS_DS_WAKEUP_USEC    	2000

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Number of assertions. */
static uint32_t palSysAssertCount;

/*! \brief      Trap enabled flag. */
static volatile bool_t PalSysAssertTrapEnable;

/*! \brief      Busy client count. */
static uint32_t palSysBusyCount;

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  DMA interrupt handlers.
 */
/*************************************************************************************************/
void DMA0_IRQHandler(void)
{
  PalLedOn(PAL_LED_ID_CPU_ACTIVE);
  MXC_DMA_Handler();
}
void DMA1_IRQHandler(void)
{
  PalLedOn(PAL_LED_ID_CPU_ACTIVE);
  MXC_DMA_Handler();
}
void DMA2_IRQHandler(void)
{
  PalLedOn(PAL_LED_ID_CPU_ACTIVE);
  MXC_DMA_Handler();
}
void DMA3_IRQHandler(void)
{
  PalLedOn(PAL_LED_ID_CPU_ACTIVE);
  MXC_DMA_Handler();
}

/*************************************************************************************************/
/*!
 *  \brief  Enter a critical section.
 */
/*************************************************************************************************/
void PalEnterCs(void)
{
  /* Use CMSIS compiler abstraction. */
  __disable_irq();
}

/*************************************************************************************************/
/*!
 *  \brief  Exit a critical section.
 */
/*************************************************************************************************/
void PalExitCs(void)
{
  /* Use CMSIS compiler abstraction. */
    __enable_irq();
}

/*************************************************************************************************/
/*!
 *  \brief      Common platform initialization.
 */
/*************************************************************************************************/
void PalSysInit(void)
{
  /* Delay to prevent lockup when debugging */
#ifdef DEBUG
  volatile int i;
  for(i = 0; i < 0x3FFFFF; i++) {}
#endif

  /* Initialize heap variables. */
  SystemHeapStart = (uint8_t *)&__HeapBase;
  SystemHeapSize  = (uint8_t *)&__HeapLimit - (uint8_t *)&__HeapBase;

  __heap_end__ = (uint32_t)&__HeapLimit;
  __heap_start__ = (uint32_t)&__HeapBase;

  /* Initialize stack variables. */
  __stack_limit__ = (uint32_t)&__StackLimit;
  __stack_top__ = (uint32_t)& __StackTop;

  palSysAssertCount = 0;
  PalSysAssertTrapEnable = TRUE;
  palSysBusyCount = 0;

  /* Switch to ISO clock */
  MXC_SETFIELD(MXC_GCR->clkctrl, MXC_F_GCR_CLKCTRL_SYSCLK_DIV, MXC_S_GCR_CLKCTRL_SYSCLK_DIV_DIV1);
  MXC_SYS_Clock_Select(MXC_SYS_CLOCK_ISO);
  MXC_SYS_ClockSourceDisable(MXC_SYS_CLOCK_IPO);

  PalLedInit();
  PalLedOff(PAL_LED_ID_ERROR);
  PalLedOn(PAL_LED_ID_CPU_ACTIVE);
  PalCryptoInit();
  PalRtcInit();

  MXC_DMA_Init();
  NVIC_EnableIRQ(DMA0_IRQn);
  NVIC_EnableIRQ(DMA1_IRQn);
  NVIC_EnableIRQ(DMA2_IRQn);
  NVIC_EnableIRQ(DMA3_IRQn);

#ifdef DEBUG
  /* Reset free memory. */
  memset(SystemHeapStart, 0, SystemHeapSize);
#endif
}

/*************************************************************************************************/
/*!
 *  \brief      System fault trap.
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
 */
/*************************************************************************************************/
void PalSysSetTrap(bool_t enable)
{
  PalSysAssertTrapEnable = enable;
}

/*************************************************************************************************/
/*!
 *  \brief      Get assert count.
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
#ifdef __GNUC__
  unsigned long *pUnused = &__stack_limit__;

  while (pUnused < &__stack_top__) {
    if (*pUnused != INIT_STACK_VAL) {
      break;
    }

    pUnused++;
  }

  return (uint32_t)(&__stack_top__ - pUnused) * sizeof(*pUnused);
#else
  /* Not available; stub routine. */
  return 0;
#endif
}

/*************************************************************************************************/
/*!
 *  \brief      System sleep.
 *
 *  \param      nextWakeMs  Next CPU wakeup time.
 *
 *  \note       Caller of this routine must ensure IRQ are disabled before entering this call.
 */
/*************************************************************************************************/
void PalSysSleep(void)
{
  uint32_t rtcCount, schUsec, palBbSnap, targetTick, rtcElapsed, schUsecElapsed;
  bool_t palBbSnapValid = FALSE;

  if (palSysBusyCount) {
    /* Work pending; do not sleep yet. */
    return;
  }

  /* Figure out if the UART is active */
  if(PalUartGetState(PAL_UART_ID_TERMINAL) == PAL_UART_STATE_BUSY) {
    MXC_LP_EnterSleepMode();
    return;
  }

  /* Prevent characters from being corrupted if still transmitting, 
    UART will shutdown in deep sleep  */
  if(MXC_UART_GetActive(MXC_UART_GET_UART(TERMINAL_UART)) != E_NO_ERROR) {
    MXC_LP_EnterSleepMode();
    return;
  }

  /* Figure out if the scheduler timer is active */
  if(PalTimerGetState() != PAL_TIMER_STATE_BUSY) {
    MXC_LP_EnterSleepMode();
    return;
  }

  if(!PAL_SYS_ENABLE_DS) {
    MXC_LP_EnterSleepMode();
    return;
  }

  /* Get the time until the next event */
  MXC_WUT_Edge();
  rtcCount = MXC_WUT->cnt;
  palBbSnapValid = PalBbGetTimestamp(&palBbSnap);
  schUsec = PalTimerGetExpTime();

  /* TODO: Figure out if RTC is active */

  /* Regular sleep if we don't have time for deep sleep */
  if (schUsec < PAL_SYS_MIN_DS_USEC) {
    MXC_LP_EnterSleepMode();
    return;
  }

  /* Arm WUT for wakeup from scheduler timer */
  targetTick = rtcCount;
  targetTick += (uint64_t)(schUsec - PAL_SYS_DS_WAKEUP_USEC) * (uint64_t)PAL_RTC_TICKS_PER_SEC / (uint64_t)1000000;
  MXC_WUT->cmp = targetTick;

  /* Enable wakeup from WUT */
  NVIC_EnableIRQ(WUT_IRQn);
  MXC_LP_EnableWUTAlarmWakeup();

  /* Stop the scheduler timer */
  PalTimerStop();

  /* Shutdown PalBb */
  if(palBbSnapValid) {
    PalBbDisable();
  }
  
  /* Re-enable interrupts for wakeup */
  PalExitCs();

  MXC_LP_EnterStandbyMode();

  /* Disable interrupts until we complete the recovery */
  PalEnterCs();

  /* Reset the BB counter */
  if(palBbSnapValid) {
    PalBbEnable();
    PalBbRestore();

    /* restore BB clock from WUT */
    MXC_WUT_Edge();
    rtcElapsed = MXC_WUT->cnt + targetTick - rtcCount;

    MXC_WUT->preset = palBbSnap + ((uint64_t)rtcElapsed * (uint64_t)BB_CLK_RATE_HZ / (uint64_t)PAL_RTC_TICKS_PER_SEC);
    MXC_WUT->reload = 1;
    MXC_WUT_Edge();
  }

  /* Update the scheduler timer */
  MXC_WUT_Edge();
  rtcElapsed = MXC_WUT->cnt + targetTick - rtcCount;
  schUsecElapsed = (uint64_t)rtcElapsed * (uint64_t)1000000 / (uint64_t)PAL_RTC_TICKS_PER_SEC;
  PalTimerRestore(schUsec - schUsecElapsed);

  /* Reset RTC compare value to prevent unintended rollover */
  MXC_WUT->cmp = PAL_MAX_RTC_COUNTER_VAL;
}

/*************************************************************************************************/
/*!
 *  \brief      Set system busy.
 */
/*************************************************************************************************/
void PalSysSetBusy(void)
{
  PalEnterCs();
  palSysBusyCount++;
  PalExitCs();
}

/*************************************************************************************************/
/*!
 *  \brief      Set system idle.
 */
/*************************************************************************************************/
void PalSysSetIdle(void)
{
  PalEnterCs();
  if (palSysBusyCount) {
    palSysBusyCount--;
  }
  PalExitCs();
}

/*************************************************************************************************/
/*!
 *  \brief      Check if system is busy.
 *
 *  \return     TRUE if system is busy.
 */
/*************************************************************************************************/
bool_t PalSysIsBusy(void)
{
  bool_t sysIsBusy = FALSE;
  PalEnterCs();
  sysIsBusy = ((palSysBusyCount == 0) ? FALSE : TRUE);
  PalExitCs();
  return sysIsBusy;
}
