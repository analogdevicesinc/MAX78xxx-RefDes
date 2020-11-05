/**
 * @file    rpu.h
 * @brief   RPU function prototypes and data types.
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
 * $Date: 2019-12-24 20:42:21 +0300 (Tue, 24 Dec 2019) $
 * $Revision: 50314 $
 *
 *************************************************************************** */

/* Define to prevent redundant inclusion */
#ifndef _RPU_H_
#define _RPU_H_

/* **** Includes **** */
#include "mxc_config.h"
#include "rpu_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup rpu Resource Protection Unit
 * @ingroup periphlibs
 * @{
 */

/* **** Definitions **** */

// Bus Masters whose access to peripherals is controlled by the RPU
typedef enum {
    RPU_DMA0_ALLOW  = 0x01,
    RPU_DMA1_ALLOW  = 0x02,
    RPU_USB_ALLOW   = 0x04,
    RPU_SYS0_ALLOW  = 0x08,
    RPU_SYS1_ALLOW  = 0x10,
    RPU_SDMAD_ALLOW = 0x20,
    RPU_SDMAI_ALLOW = 0x40,
    RPU_CRYPTO_ALLOW = 0x80,
    RPU_SDIO_ALLOW  = 0x100
} rpu_allow_t;

// Peripherals gated by the RPU
typedef enum {
    RPU_GCR          = MXC_R_RPU_GCR,
    RPU_FLC0         = MXC_R_RPU_FLC0,
    RPU_SDHCCTRL     = MXC_R_RPU_SDHCCTRL,
    RPU_SIR          = MXC_R_RPU_SIR,
    RPU_FCR          = MXC_R_RPU_FCR,
    RPU_CRYPTO       = MXC_R_RPU_CRYPTO,
    RPU_WDT0         = MXC_R_RPU_WDT0,
    RPU_WDT1         = MXC_R_RPU_WDT1,
    RPU_WDT2         = MXC_R_RPU_WDT2,
    RPU_SMON         = MXC_R_RPU_SMON,
    RPU_SIMO         = MXC_R_RPU_SIMO,
    RPU_DVS          = MXC_R_RPU_DVS,
    RPU_BBSIR        = MXC_R_RPU_BBSIR,
    RPU_RTC          = MXC_R_RPU_RTC,
    RPU_WUT          = MXC_R_RPU_WUT,
    RPU_PWRSEQ       = MXC_R_RPU_PWRSEQ,
    RPU_MCR          = MXC_R_RPU_MCR,
    RPU_GPIO0        = MXC_R_RPU_GPIO0,
    RPU_GPIO1        = MXC_R_RPU_GPIO1,
    RPU_TMR0         = MXC_R_RPU_TMR0,
    RPU_TMR1         = MXC_R_RPU_TMR1,
    RPU_TMR2         = MXC_R_RPU_TMR2,
    RPU_TMR3         = MXC_R_RPU_TMR3,
    RPU_TMR4         = MXC_R_RPU_TMR4,
    RPU_TMR5         = MXC_R_RPU_TMR5,
    RPU_HTIMER0      = MXC_R_RPU_HTIMER0,
    RPU_HTIMER1      = MXC_R_RPU_HTIMER1,
    RPU_I2C0_BUS0    = MXC_R_RPU_I2C0_BUS0,
    RPU_I2C1_BUS0    = MXC_R_RPU_I2C1_BUS0,
    RPU_I2C2_BUS0    = MXC_R_RPU_I2C2_BUS0,
    RPU_SPIXFM       = MXC_R_RPU_SPIXFM,
    RPU_SPIXFC       = MXC_R_RPU_SPIXFC,
    RPU_DMA0         = MXC_R_RPU_DMA0,
    RPU_FLC1         = MXC_R_RPU_FLC1,
    RPU_ICC0         = MXC_R_RPU_ICC0,
    RPU_ICC1         = MXC_R_RPU_ICC1,
    RPU_SFCC         = MXC_R_RPU_SFCC,
    RPU_SRCC         = MXC_R_RPU_SRCC,
    RPU_ADC          = MXC_R_RPU_ADC,
    RPU_DMA1         = MXC_R_RPU_DMA1,
    RPU_SDMA         = MXC_R_RPU_SDMA,
    RPU_SPIXR        = MXC_R_RPU_SPIXR,
    RPU_PTG_BUS0     = MXC_R_RPU_PTG_BUS0,
    RPU_OWM          = MXC_R_RPU_OWM,
    RPU_SEMA         = MXC_R_RPU_SEMA,
    RPU_UART0        = MXC_R_RPU_UART0,
    RPU_UART1        = MXC_R_RPU_UART1,
    RPU_UART2        = MXC_R_RPU_UART2,
    RPU_SPI1         = MXC_R_RPU_SPI1,
    RPU_SPI2         = MXC_R_RPU_SPI2,
    RPU_AUDIO        = MXC_R_RPU_AUDIO,
    RPU_TRNG         = MXC_R_RPU_TRNG,
    RPU_BTLE         = MXC_R_RPU_BTLE,
    RPU_USBHS        = MXC_R_RPU_USBHS,
    RPU_SDIO         = MXC_R_RPU_SDIO,
    RPU_SPIXM_FIFO   = MXC_R_RPU_SPIXM_FIFO,
    RPU_SPI0         = MXC_R_RPU_SPI0
} rpu_device_t;

/* **** Function Prototypes **** */
/**
 * @brief      Enable access to peripherals restricted by the RPU
 *             This function must be called from handler (privileged) mode
 * @param      periph the peripheral to allow access too
 * @param      allow_mask which bus masters to allow to access periph
 * @return     E_NO_ERROR If function is successful.
 */
int RPU_Allow(rpu_device_t periph, uint32_t allow_mask);

/**
 * @brief      Disable access to peripherals restricted by the RPU
 *             This function must be called from handler (privileged) mode
 * @param      periph the peripheral to revoke access too
 * @param      disallow_mask which bus masters to disallow access to periph
 * @return     E_NO_ERROR if function is successful.
 */
int RPU_Disallow(rpu_device_t periph, uint32_t disallow_mask);

/**
 * @brief      Check to see if this process is running in handler mode
 * @return     E_NO_RRROR If allowed.
 * @return     E_BAD_STATE If not allowed. 
 */ 
int RPU_IsAllowed(void);


/**@} end of group rpu */

#ifdef __cplusplus
}
#endif

#endif /* _RPU_H_ */
