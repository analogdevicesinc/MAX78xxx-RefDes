/*******************************************************************************
 * Copyright (C) 2015 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2020-01-16 08:38:14 -0600 (Thu, 16 Jan 2020) $
 * $Revision: 50696 $
 *
 ******************************************************************************/

/**
 * @file    mxc_sys.h
 * @brief   System level header file.
 */

#ifndef _MXC_SYS_H_
#define _MXC_SYS_H_

#include "mxc_device.h"
#include "gcr_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief System reset0 and reset1 enumeration. Used in SYS_PeriphReset0 function */
typedef enum {
	MXC_SYS_RESET_DMA0      = MXC_F_GCR_RST0_DMA0_POS,         /**< Reset DMA */
	MXC_SYS_RESET_WDT0      = MXC_F_GCR_RST0_WDT0_POS,         /**< Reset WDT */
	MXC_SYS_RESET_GPIO0     = MXC_F_GCR_RST0_GPIO0_POS,       /**< Reset GPIO0 */
	MXC_SYS_RESET_GPIO1     = MXC_F_GCR_RST0_GPIO1_POS,       /**< Reset GPIO1 */
	MXC_SYS_RESET_TIMER0    = MXC_F_GCR_RST0_TIMER0_POS,      /**< Reset TIMER0 */
	MXC_SYS_RESET_TIMER1    = MXC_F_GCR_RST0_TIMER1_POS,      /**< Reset TIMER1 */
	MXC_SYS_RESET_TIMER2    = MXC_F_GCR_RST0_TIMER2_POS,      /**< Reset TIMER2 */
	MXC_SYS_RESET_TIMER3    = MXC_F_GCR_RST0_TIMER3_POS,      /**< Reset TIMER3 */
	MXC_SYS_RESET_TIMER4    = MXC_F_GCR_RST0_TIMER4_POS,      /**< Reset TIMER4 */
	MXC_SYS_RESET_TIMER5    = MXC_F_GCR_RST0_TIMER5_POS,      /**< Reset TIMER5 */
	MXC_SYS_RESET_UART0     = MXC_F_GCR_RST0_UART0_POS,       /**< Reset UART0 */
	MXC_SYS_RESET_UART1     = MXC_F_GCR_RST0_UART1_POS,       /**< Reset UART1 */
	MXC_SYS_RESET_SPI1      = MXC_F_GCR_RST0_SPI1_POS,        /**< Reset SPI0 */
	MXC_SYS_RESET_SPI2      = MXC_F_GCR_RST0_SPI2_POS,        /**< Reset SPI1 */
	MXC_SYS_RESET_I2C0      = MXC_F_GCR_RST0_I2C0_POS,        /**< Reset I2C0 */
	MXC_SYS_RESET_RTC       = MXC_F_GCR_RST0_RTC_POS,         /**< Reset RTC */
	MXC_SYS_RESET_CRYPTO    = MXC_F_GCR_RST0_CRYPTO_POS,      /**< Reset CRYPTO */
	MXC_SYS_RESET_SMPHR     = MXC_F_GCR_RST0_SMPHR_POS,    /**< Reset SMPHR */
	MXC_SYS_RESET_USB       = MXC_F_GCR_RST0_USB_POS,         /**< Reset USB */
	MXC_SYS_RESET_TRNG      = MXC_F_GCR_RST0_TRNG_POS,        /**< Reset TRNG */
	MXC_SYS_RESET_ADC       = MXC_F_GCR_RST0_ADC_POS,         /**< Reset ADC */
	MXC_SYS_RESET_DMA1      = MXC_F_GCR_RST0_DMA1_POS,         /**< Reset DMA1 */
	MXC_SYS_RESET_UART2     = MXC_F_GCR_RST0_UART2_POS,       /**< Reset UART2 */
	MXC_SYS_RESET_SRST      = MXC_F_GCR_RST0_SOFT_RST_POS,        /**< Soft reset */
	MXC_SYS_RESET_PRST      = MXC_F_GCR_RST0_PERIPH_RST_POS,        /**< Peripheral reset */
	MXC_SYS_RESET_SYSTEM    = MXC_F_GCR_RST0_SYS_RST_POS ,      /**< System reset */
    /* RESET1 Below this line we add 32 to separate RESET0 and RESET1 */
	MXC_SYS_RESET_I2C1      = (MXC_F_GCR_RST1_I2C1_POS + 32),        /**< Reset I2C1 */
	MXC_SYS_RESET_PT        = (MXC_F_GCR_RST1_PT_POS + 32),          /**< Reset PT */
	MXC_SYS_RESET_SPIXIP    = (MXC_F_GCR_RST1_SPIXIP_POS + 32),      /**< Reset SPIXIP */
	MXC_SYS_RESET_XSPIM     = (MXC_F_GCR_RST1_XSPIM_POS + 32),       /**< Reset XSPIM */
	MXC_SYS_RESET_SDHC      = (MXC_F_GCR_RST1_SDHC_POS + 32),        /**< Reset SDHC */
	MXC_SYS_RESET_OWIRE     = (MXC_F_GCR_RST1_OWIRE_POS + 32),       /**< Reset OWIRE */
	MXC_SYS_RESET_WDT1      = (MXC_F_GCR_RST1_WDT1_POS+32),         /**< Reset WDT1 */
	MXC_SYS_RESET_SPI3      = (MXC_F_GCR_RST1_SPI3_POS+32),       	 /**< Reset SPI2 */
	MXC_SYS_RESET_XIPR      = (MXC_F_GCR_RST1_XIPR_POS+32),       	 /**< Reset XIPR */
	MXC_SYS_RESET_SEMA      = (MXC_F_GCR_RST1_SEMA_POS+32),       	 /**< Reset SEMA */
	MXC_SYS_RESET_WDT2      = (MXC_F_GCR_RST1_WDT2_POS + 32),        /**< Reset WDT1 */
	MXC_SYS_RESET_BTLE      = (MXC_F_GCR_RST1_BTLE_POS + 32),        /**< Reset BTLE */
	MXC_SYS_RESET_AUDIO     = (MXC_F_GCR_RST1_AUDIO_POS + 32),        /**< Reset BTLE */
	MXC_SYS_RESET_RPU     	= (MXC_F_GCR_RST1_RPU_POS + 32),        /**< Reset BTLE */
	MXC_SYS_RESET_I2C2      = (MXC_F_GCR_RST1_I2C2_POS + 32),        /**< Reset BTLE */
	MXC_SYS_RESET_HTMR0     = (MXC_F_GCR_RST1_HTMR0_POS + 32),        /**< Reset HTMR0 */
	MXC_SYS_RESET_HTMR1     = (MXC_F_GCR_RST1_HTMR1_POS + 32),        /**< Reset HTMR1 */
	MXC_SYS_RESET_DVS      	= (MXC_F_GCR_RST1_DVS_POS + 32),   		/**< Reset DVS */
	MXC_SYS_RESET_SIMO   	= (MXC_F_GCR_RST1_SIMO_POS + 32),      /**< Reset SIMO */
} mxc_sys_reset_t;

/** @brief System clock disable enumeration. Used in SYS_ClockDisable and SYS_ClockEnable functions */
typedef enum {
	MXC_SYS_PERIPH_CLOCK_GPIO0       = MXC_F_GCR_PCLK_DIS0_GPIO0_POS,                    /**< Disable MXC_F_GCR_PERCKCN0_GPIO0D clock */
	MXC_SYS_PERIPH_CLOCK_GPIO1       = MXC_F_GCR_PCLK_DIS0_GPIO1_POS,                    /**< Disable MXC_F_GCR_PERCKCN0_GPIO1D clock */
	MXC_SYS_PERIPH_CLOCK_USB         = MXC_F_GCR_PCLK_DIS0_USB_POS,                      /**< Disable MXC_F_GCR_PERCKCN0_USBD clock */
	MXC_SYS_PERIPH_CLOCK_DMA         = MXC_F_GCR_PCLK_DIS0_DMA0_POS,                      /**< Disable MXC_F_GCR_PERCKCN0_DMAD clock */
	MXC_SYS_PERIPH_CLOCK_SPI1        = MXC_F_GCR_PCLK_DIS0_SPI0_POS,                     /**< Disable MXC_F_GCR_PERCKCN0_SPI1D clock */
	MXC_SYS_PERIPH_CLOCK_SPI2        = MXC_F_GCR_PCLK_DIS0_SPI1_POS,                     /**< Disable MXC_F_GCR_PERCKCN0_SPI2D clock */
	MXC_SYS_PERIPH_CLOCK_UART0       = MXC_F_GCR_PCLK_DIS0_UART0_POS,                    /**< Disable MXC_F_GCR_PERCKCN0_UART0D clock */
	MXC_SYS_PERIPH_CLOCK_UART1       = MXC_F_GCR_PCLK_DIS0_UART1_POS,                    /**< Disable MXC_F_GCR_PERCKCN0_UART1D clock */
	MXC_SYS_PERIPH_CLOCK_I2C0        = MXC_F_GCR_PCLK_DIS0_I2C0_POS,                     /**< Disable MXC_F_GCR_PERCKCN0_I2C0D clock */
	MXC_SYS_PERIPH_CLOCK_TPU         = MXC_F_GCR_PCLK_DIS0_CRYPTO_POS,                   /**< Disable MXC_F_GCR_PERCKCN0_CRYPTOD clock */
	MXC_SYS_PERIPH_CLOCK_T0          = MXC_F_GCR_PCLK_DIS0_TIMER0_POS,                       /**< Disable MXC_F_GCR_PERCKCN0_T0D clock */
	MXC_SYS_PERIPH_CLOCK_T1          = MXC_F_GCR_PCLK_DIS0_TIMER1_POS,                       /**< Disable MXC_F_GCR_PERCKCN0_T1D clock */
	MXC_SYS_PERIPH_CLOCK_T2          = MXC_F_GCR_PCLK_DIS0_TIMER2_POS,                       /**< Disable MXC_F_GCR_PERCKCN0_T2D clock */
	MXC_SYS_PERIPH_CLOCK_T3          = MXC_F_GCR_PCLK_DIS0_TIMER3_POS,                       /**< Disable MXC_F_GCR_PERCKCN0_T3D clock */
	MXC_SYS_PERIPH_CLOCK_T4          = MXC_F_GCR_PCLK_DIS0_TIMER4_POS,                       /**< Disable MXC_F_GCR_PERCKCN0_T4D clock */
	MXC_SYS_PERIPH_CLOCK_T5          = MXC_F_GCR_PCLK_DIS0_TIMER5_POS,                       /**< Disable MXC_F_GCR_PERCKCN0_T5D clock */
	MXC_SYS_PERIPH_CLOCK_ADC         = MXC_F_GCR_PCLK_DIS0_ADC_POS,                      /**< Disable MXC_F_GCR_PERCKCN0_ADCD clock */
	MXC_SYS_PERIPH_CLOCK_I2C1        = MXC_F_GCR_PCLK_DIS0_I2C1_POS,                     /**< Disable MXC_F_GCR_PERCKCN0_I2C1D clock */
	MXC_SYS_PERIPH_CLOCK_PT          = MXC_F_GCR_PCLK_DIS0_PTD_POS,                       /**< Disable MXC_F_GCR_PERCKCN0_PTD clock */
	MXC_SYS_PERIPH_CLOCK_SPIXIP      = MXC_F_GCR_PCLK_DIS0_SPIXIPF_POS,                   /**< Disable MXC_F_GCR_PERCKCN0_SPIXIPD clock */
	MXC_SYS_PERIPH_CLOCK_SPIXFC      = MXC_F_GCR_PCLK_DIS0_SPIXIPM_POS,                     /**< Disable MXC_F_GCR_PERCKCN0_SPIMD clock */
    /* PERCKCN1 Below this line we add 32 to separate PERCKCN0 and PERCKCN1 */
	MXC_SYS_PERIPH_CLOCK_BTLE        =(MXC_F_GCR_PCLK_DIS1_BTLE_POS + 32),
	MXC_SYS_PERIPH_CLOCK_UART2       =(MXC_F_GCR_PCLK_DIS1_UART2_POS + 32),            /**<Disable MXC_F_GCR_PERCKCN1_UART2D clock */
	MXC_SYS_PERIPH_CLOCK_TRNG        =(MXC_F_GCR_PCLK_DIS1_TRNG_POS + 32),             /**<Disable MXC_F_GCR_PERCKCN1_TRNGD clock */
	MXC_SYS_PERIPH_CLOCK_SCACHE      =(MXC_F_GCR_PCLK_DIS1_SCACHE_POS + 32),           /**<Disable MXC_F_GCR_PERCKCN1_SCACHED clock */
	MXC_SYS_PERIPH_CLOCK_SDMA        =(MXC_F_GCR_PCLK_DIS1_SDMA_POS + 32),             /**<Disable MXC_F_GCR_PERCKCN1_SDMAD clock */
	MXC_SYS_PERIPH_CLOCK_SMPHR       =(MXC_F_GCR_PCLK_DIS1_SMPHR_POS + 32),            /**<Disable MXC_F_GCR_PERCKCN1_SMPHRD clock */
	MXC_SYS_PERIPH_CLOCK_SDHC        =(MXC_F_GCR_PCLK_DIS1_SDHC_POS + 32),             /**<Disable MXC_F_GCR_PERCKCN1_SDHCD clock */
	MXC_SYS_PERIPH_CLOCK_ICACHEXIP   =(MXC_F_GCR_PCLK_DIS1_ICACHEXIP_POS + 32),        /**<Disable MXC_F_GCR_PERCKCN1_ICACHEXIPD clock */
	MXC_SYS_PERIPH_CLOCK_OWIRE       =(MXC_F_GCR_PCLK_DIS1_OW_POS + 32),            /**<Disable MXC_F_GCR_PERCKCN1_OWIRED clock */
	MXC_SYS_PERIPH_CLOCK_SPI0        =(MXC_F_GCR_PCLK_DIS1_SPI3_POS + 32),             /**< Disable QSPI Clock (API Calls QSPI SPI0) */
	MXC_SYS_PERIPH_CLOCK_SPIXIPD     =(MXC_F_GCR_PCLK_DIS1_SPIXIPR_POS + 32),
	MXC_SYS_PERIPH_CLOCK_DMA1        =(MXC_F_GCR_PCLK_DIS1_DMA1_POS + 32),
	MXC_SYS_PERIPH_CLOCK_AUDIO       =(MXC_F_GCR_PCLK_DIS1_AUDIO_POS + 32),
	MXC_SYS_PERIPH_CLOCK_I2C2        =(MXC_F_GCR_PCLK_DIS1_I2C2_POS + 32),
	MXC_SYS_PERIPH_CLOCK_HTMR0       =(MXC_F_GCR_PCLK_DIS1_HTMR0_POS + 32),           /**<Disable MXC_F_GCR_PERCKCN1_HTMR0 clock */
	MXC_SYS_PERIPH_CLOCK_HTMR1       =(MXC_F_GCR_PCLK_DIS1_HTMR1_POS + 32),           /**<Disable MXC_F_GCR_PERCKCN1_HTMR1 clock */
	MXC_SYS_PERIPH_CLOCK_WDT0        =(MXC_F_GCR_PCLK_DIS1_WDT0_POS + 32),
	MXC_SYS_PERIPH_CLOCK_WDT1        =(MXC_F_GCR_PCLK_DIS1_WDT1_POS + 32),
	MXC_SYS_PERIPH_CLOCK_WDT2        =(MXC_F_GCR_PCLK_DIS1_WDT2_POS + 32),
	MXC_SYS_PERIPH_CLOCK_CPU1        =(MXC_F_GCR_PCLK_DIS1_CPU1_POS + 32),
} mxc_sys_periph_clock_t;

typedef enum {
	MXC_SYS_CLOCK_HIRC96    = MXC_V_GCR_CLK_CTRL_SYSOSC_SEL_HIRC96,
	MXC_SYS_CLOCK_HIRC8     = MXC_V_GCR_CLK_CTRL_SYSOSC_SEL_HIRC8,
	MXC_SYS_CLOCK_HIRC      = MXC_V_GCR_CLK_CTRL_SYSOSC_SEL_HIRC,
	MXC_SYS_CLOCK_XTAL32M   = MXC_V_GCR_CLK_CTRL_SYSOSC_SEL_XTAL32M,
	MXC_SYS_CLOCK_LIRC8K    = MXC_V_GCR_CLK_CTRL_SYSOSC_SEL_LIRC8,
	MXC_SYS_CLOCK_XTAL32K   = MXC_V_GCR_CLK_CTRL_SYSOSC_SEL_XTAL32K,
} mxc_sys_system_clock_t;

#define MXC_SYS_SCACHE_CLK 1  // Enable SCACHE CLK
#define MXC_SYS_CTB_CLK 1     // Enable CTB CLK
  
/***** Function Prototypes *****/

/**
 * @brief Determines if the selected peripheral clock is enabled.
 * @param clock   Enumeration for desired clock.
 * @returns       0 is the clock is disabled, non 0 if the clock is enabled.
 */
int MXC_SYS_IsClockEnabled (mxc_sys_periph_clock_t clock);

/**
 * @brief Disables the selected peripheral clock.
 * @param clock   Enumeration for desired clock.
 */
void MXC_SYS_ClockDisable (mxc_sys_periph_clock_t clock);

/**
 * @brief Enables the selected peripheral clock.
 * @param clock   Enumeration for desired clock.
 */
void MXC_SYS_ClockEnable (mxc_sys_periph_clock_t clock);

/**
 * @brief Enables the 32kHz oscillator
 * @param mxc_sys_cfg   Not used, may be NULL.
 */
void MXC_SYS_RTCClockEnable (void);

/**
 * @brief Disables the 32kHz oscillator
 * @returns         E_NO_ERROR if everything is successful
 */
int MXC_SYS_RTCClockDisable();

/**
 * @brief Enable System Clock Source without switching to it
 * @param      clock The clock to enable
 * @return     E_NO_ERROR if everything is successful
 */
int MXC_SYS_ClockSourceEnable (mxc_sys_system_clock_t clock);

/**
 * @brief Disable System Clock Source
 * @param      clock The clock to disable
 * @return     E_NO_ERROR if everything is successful
 */
int MXC_SYS_ClockSourceDisable (mxc_sys_system_clock_t clock);

/**
 * @brief Select the system clock.
 * @param clock     Enumeration for desired clock.
 * @param tmr       Optional tmr pointer for timeout. NULL if undesired.
 * @returns         E_NO_ERROR if everything is successful.
 */
int MXC_SYS_Clock_Select (mxc_sys_system_clock_t clock);

/**
 * @brief Wait for a clock to enable with timeout
 * @param      ready The clock to wait for
 * @return     E_NO_ERROR if ready, E_TIME_OUT if timeout
 */
int MXC_SYS_Clock_Timeout (uint32_t ready);

/**
 * @brief Reset the peripherals and/or CPU in the RST0 or RST1 register.
 * @param           Enumeration for what to reset. Can reset multiple items at once.
 */
void MXC_SYS_Reset_Periph (mxc_sys_reset_t reset);

/**
 * @brief      Get the revision of the chip
 * @returns    the chip revision
 */
uint8_t MXC_SYS_GetRev (void);

/**
 * @brief      Get the USN of the chip
 * @param      serialNumber buffer to store the USN
 * @param      len          length of the USN buffer
 * @returns    #E_NO_ERROR if everything is successful.
 */
int MXC_SYS_GetUSN (uint8_t* serialNumber, int len);

#ifdef __cplusplus
}
#endif

#endif /* _MXC_MXC_SYS_H_*/
