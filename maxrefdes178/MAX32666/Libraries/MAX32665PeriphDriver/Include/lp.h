/**
 * @file    lp.h
 * @brief   Low power function prototypes and data types.
 */

/* ****************************************************************************
 * Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2019-11-07 22:07:17 +0300 (Thu, 07 Nov 2019) $
 * $Revision: 48499 $
 *
 *************************************************************************** */

/* Define to prevent redundant inclusion */
#ifndef _LP_H_
#define _LP_H_

/* **** Includes **** */
#include "gpio.h"
#include "pwrseq_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup pwrseq Low Power (Power Sequencer)
 * @ingroup periphlibs
 * @{
 */

typedef enum {
	RETAIN_NONE= MXC_S_PWRSEQ_LPCN_RAMRET_DIS,
	RETAIN_32k = MXC_S_PWRSEQ_LPCN_RAMRET_EN1,
	RETAIN_64k = MXC_S_PWRSEQ_LPCN_RAMRET_EN2,
	RETAIN_ALL = MXC_S_PWRSEQ_LPCN_RAMRET_EN3 
} ram_retained_t;

/**
 * @brief      Clears the wakup status bits.  
 */
void LP_ClearWakeStatus(void);

/**
 * @brief      Enables the selected GPIO port and its selected pins to wake up the device from any low power mode.  
 *             Call this function multiple times to enable pins on multiple ports.  This function does not configure
 *             the GPIO pins nor does it setup their interrupt functionality.
 * @param      wu_pins      The port and pins to configure as wakeup sources.  Only the gpio and mask fields of the
 *                          structure are used.  The func and pad fields are ignored.
 */
void LP_EnableGPIOWakeup(gpio_cfg_t *wu_pins);

/**
 * @brief      Disables the selected GPIO port and its selected pins as a wake up source.  
 *             Call this function multiple times to disable pins on multiple ports.
 * @param      wu_pins      The port and pins to disable as wakeup sources.  Only the gpio and mask fields of the
 *                          structure are used.  The func and pad fields are ignored.
 */
void LP_DisableGPIOWakeup(gpio_cfg_t *wu_pins);

/**
 * @brief      Enables the RTC alarm to wake up the device from any low power mode.  
 */
void LP_EnableRTCAlarmWakeup(void);

/**
 * @brief      Disables the RTC alarm from waking up the device.  
 */
void LP_DisableRTCAlarmWakeup(void);

/**
 * @brief Puts System Ram 0 in light sleep
 */
void LP_SysRam0LightSleep(void);

/**
 * @brief Puts System Ram 1 in light sleep
 */
void LP_SysRam1LightSleep(void);

/**
 * @brief Puts System Ram 2 in light sleep
 */
void LP_SysRam2LightSleep(void);

/**
 * @brief Puts System Ram 3 in light sleep
 */
void LP_SysRam3LightSleep(void);

/**
 * @brief Puts System Ram 4 in light sleep
 */
void LP_SysRam4LightSleep(void);

/**
 * @brief Puts System Ram 5 in light sleep
 */
void LP_SysRam5LightSleep(void);

/**
 * @brief Shutdown System Ram 0
 */
void LP_SysRam0Shutdown(void);

/**
 * @brief Wakeup System Ram 0
 */
void LP_SysRam0Wakeup(void);

/**
 * @brief Shutdown System Ram 1
 */
void LP_SysRam1Shutdown(void);

/**
 * @brief Wakeup System Ram 1
 */
void LP_SysRam1Wakeup(void);

/**
 * @brief Shutdown System Ram 2
 */
void LP_SysRam2Shutdown(void);

/**
 * @brief Wakeup System Ram 2
 */
void LP_SysRam2Wakeup(void);

/**
 * @brief Shutdown System Ram 3
 */
void LP_SysRam3Shutdown(void);

/**
 * @brief Wakeup System Ram 3
 */
void LP_SysRam3Wakeup(void);

/**
 * @brief Shutdown System Ram 4
 */
void LP_SysRam4Shutdown(void);

/**
 * @brief Wakeup System Ram 4
 */
void LP_SysRam4Wakeup(void);

/**
 * @brief Shutdown System Ram 5
 */
void LP_SysRam5Shutdown(void);

/**
 * @brief Wakeup System Ram 5
 */
void LP_SysRam5Wakeup(void);

/**
 * @brief Shutdown Internal Cache
 */
void LP_ICacheShutdown(void);

/**
 * @brief Wakeup Internal Cache
 */
void LP_ICacheWakeup(void);

/**
 * @brief Shutdown Internal Cache XIP
 */
void LP_ICacheXIPShutdown(void);

/**
 * @brief Wakeup Internal Cache XIP
 */
void LP_ICacheXIPWakeup(void);

/**
 * @brief Shutdown Crypto
 */
void LP_CryptoShutdown(void);

/**
 * @brief Wakeup Crypto
 */
void LP_CryptoWakeup(void);

/**
 * @brief Shutdown USB FIFO
 */
void LP_USBFIFOShutdown(void);

/**
 * @brief Wakeup USB FIFO
 */
void LP_USBFIFOWakeup(void);

/**
 * @brief Shutdown ROM
 */
void LP_ROMShutdown(void);

/**
 * @brief Wakeup ROM
 */
void LP_ROMWakeup(void);

/**
 * @brief Shutdown ROM 1
 */
void LP_ROM1Shutdown(void);

/**
 * @brief Wakeup ROM 1
 */
void LP_ROM1Wakeup(void);

/**
 * @brief Shutdown Internal Cache 1
 */
void LP_ICache1Shutdown(void);

/**
 * @brief Wakeup Internal Cache 1
 */
void LP_ICache1Wakeup(void);

/**
 * @brief Shutdown System Cache
 */
void LP_SysCacheShutdown(void);

/**
 * @brief Wakeup System Cache 
 */
void LP_SysCacheWakeup(void);

/**
 * @brief Disable USB Software Low Power
 */
void LP_USBSWLPDisable(void);

/**
 * @brief Enable USB Software Low Power 
 */
void LP_USBSWLPEnable(void);

/**
 * @brief Power Down VDD2
 */
void LP_VDD2PowerDown(void);

/**
 * @brief Power up VDD2
 */
void LP_VDD2PowerUp(void);

/**
 * @brief Power Down VDD3
 */
void LP_VDD3PowerDown(void);

/**
 * @brief Power Up VDD3
 */
void LP_VDD3PowerUp(void);

/**
 * @brief Power Down VDD4
 */
void LP_VDD4PowerDown(void);

/**
 * @brief Power Up VDD4
 */
void LP_VDD4PowerUp(void);

/**
 * @brief Power Down VDD5
 */
void LP_VDD5PowerDown(void);

/**
 * @brief Power Up VDD5
 */
void LP_VDD5PowerUp(void);

/**
 * @brief Power Down SIMOV regB
 */
void LP_SIMOVregBPowerDown(void);

/**
 * @brief Power Up SIMOV regB 
 */
void LP_SIMOVregBPowerUp(void);

/**
 * @brief Power Down SIMOV regD 
 */
void LP_SIMOVregDPowerDown(void);

/**
 * @brief Power Up SIMOV regD
 */
void LP_SIMOVregDPowerUp(void);

/**
 * @brief Enable Fast Wakeup
 */
void LP_FastWakeupEnable(void);

/**
 * @brief Disable Fast Wakeup
 */
void LP_FastWakeupDisable(void);

/**
 * @brief Enables the selected amount of RAM retention in backup mode
 *        Using any RAM retention removes the ability to shut down VcoreB
 */
void LP_SetRAMRetention(ram_retained_t ramRetained);

/**
 * @brief      Places the device into SLEEP mode.  This function returns once any interrupt occurs. 
 */
void LP_EnterSleepMode(void);

/**
 * @brief      Places the device into DEEPSLEEP mode.  This function returns once an RTC or external interrupt occur. 
 */
void LP_EnterDeepSleepMode(void);

/**
 * @brief      Places the device into BACKGROUND mode.  This function returns once an RTC or external interrupt occur.
 */
void LP_EnterBackgroundMode(void);

/**
 * @brief      Places the device into BACKUP mode.  CPU state is not maintained in this mode, so this function never returns.  
 *             Instead, the device will restart once an RTC or external interrupt occur. 
 * @param  	   func 	Function that backup mode returns to, if null, the part will return to Reset_Handler
 * @note 	   When returning from backup mode, depending on the RAM retention settings the processor
 * 			   could have no state information. It will not have a valid stack pointer. 
 * 			   This function also uses MXC_PWRSEQ->gp0 and gp1.
 */
void LP_EnterBackupMode(void* func(void));

/**
 * @brief      Places the device into Shutdown mode.  CPU state is not maintained in this mode, so this function never returns.  
 *             Instead, the device will restart once an RTC, USB wakeup, or external interrupt occur. 
 */
void LP_EnterShutDownMode(void);

#ifdef __cplusplus
}
#endif
/**@} end of group pwrseq */
#endif /* _LP_H_ */
