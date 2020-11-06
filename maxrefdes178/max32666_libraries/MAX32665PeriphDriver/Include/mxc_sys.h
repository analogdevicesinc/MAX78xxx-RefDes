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
 * $Date: 2020-09-23 17:00:53 +0300 (Wed, 23 Sep 2020) $
 * $Revision: 55884 $
 *
 ******************************************************************************/

/**
 * @file    mxc_sys.h
 * @brief   System level header file.
 */

#ifndef _MXC_SYS_H_
#define _MXC_SYS_H_

#include "mxc_config.h"
#include "uart_regs.h"
#include "i2c_regs.h"
#include "ptg_regs.h"
#include "pt_regs.h"
#include "gcr_regs.h"
#include "tmr_regs.h"
#include "gpio.h"
#include "sdhc_regs.h"
#include "flc_regs.h"
#include "spixfc_regs.h"
#include "spi17y_regs.h"
#include "htmr_regs.h"
#include "wdt_regs.h"
#include "dma.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief System reset0 and reset1 enumeration. Used in SYS_PeriphReset0 function */
typedef enum {
    SYS_RESET_DMA       = MXC_F_GCR_RSTR0_DMA_POS,         /**< Reset DMA */
    SYS_RESET_WDT       = MXC_F_GCR_RSTR0_WDT0_POS,         /**< Reset WDT */
    SYS_RESET_GPIO0     = MXC_F_GCR_RSTR0_GPIO0_POS,       /**< Reset GPIO0 */
    SYS_RESET_GPIO1     = MXC_F_GCR_RSTR0_GPIO1_POS,       /**< Reset GPIO1 */
    SYS_RESET_TIMER0    = MXC_F_GCR_RSTR0_TIMER0_POS,      /**< Reset TIMER0 */
    SYS_RESET_TIMER1    = MXC_F_GCR_RSTR0_TIMER1_POS,      /**< Reset TIMER1 */
    SYS_RESET_TIMER2    = MXC_F_GCR_RSTR0_TIMER2_POS,      /**< Reset TIMER2 */
    SYS_RESET_TIMER3    = MXC_F_GCR_RSTR0_TIMER3_POS,      /**< Reset TIMER3 */
    SYS_RESET_TIMER4    = MXC_F_GCR_RSTR0_TIMER4_POS,      /**< Reset TIMER4 */
    SYS_RESET_TIMER5    = MXC_F_GCR_RSTR0_TIMER5_POS,      /**< Reset TIMER5 */
    SYS_RESET_UART0     = MXC_F_GCR_RSTR0_UART0_POS,       /**< Reset UART0 */
    SYS_RESET_UART1     = MXC_F_GCR_RSTR0_UART1_POS,       /**< Reset UART1 */
    SYS_RESET_SPI1      = MXC_F_GCR_RSTR0_SPI1_POS,        /**< Reset SPI0 */
    SYS_RESET_SPI2      = MXC_F_GCR_RSTR0_SPI2_POS,        /**< Reset SPI1 */
    SYS_RESET_I2C0      = MXC_F_GCR_RSTR0_I2C0_POS,        /**< Reset I2C0 */
    SYS_RESET_RTC       = MXC_F_GCR_RSTR0_RTC_POS,         /**< Reset RTC */
    SYS_RESET_CRYPTO    = MXC_F_GCR_RSTR0_CRYPTO_POS,      /**< Reset CRYPTO */
    SYS_RESET_USB       = MXC_F_GCR_RSTR0_USB_POS,         /**< Reset USB */
    SYS_RESET_TRNG      = MXC_F_GCR_RSTR0_TRNG_POS,        /**< Reset TRNG */
    SYS_RESET_ADC       = MXC_F_GCR_RSTR0_ADC_POS,         /**< Reset ADC */
    SYS_RESET_UART2     = MXC_F_GCR_RSTR0_UART2_POS,       /**< Reset UART2 */
    SYS_RESET_SRST      = MXC_F_GCR_RSTR0_SRST_POS,        /**< Soft reset */
    SYS_RESET_PRST      = MXC_F_GCR_RSTR0_PRST_POS,        /**< Peripheral reset */
    SYS_RESET_SYSTEM    = MXC_F_GCR_RSTR0_SYSTEM_POS,      /**< System reset */
    /* RESET1 Below this line we add 32 to separate RESET0 and RESET1 */
    SYS_RESET_I2C1      = (MXC_F_GCR_RSTR1_I2C1_POS + 32),        /**< Reset I2C1 */
    SYS_RESET_PT        = (MXC_F_GCR_RSTR1_PT_POS + 32),          /**< Reset PT */
    SYS_RESET_SPIXIP    = (MXC_F_GCR_RSTR1_SPIXIP_POS + 32),      /**< Reset SPIXIP */
    SYS_RESET_XSPIM     = (MXC_F_GCR_RSTR1_XSPIM_POS + 32),       /**< Reset XSPIM */
    SYS_RESET_SDHC      = (MXC_F_GCR_RSTR1_SDHC_POS + 32),        /**< Reset SDHC */
    SYS_RESET_OWIRE     = (MXC_F_GCR_RSTR1_OWIRE_POS + 32),       /**< Reset OWIRE */
    SYS_RESET_HTR0      = (MXC_F_GCR_RSTR1_HTMR0_POS + 32),        /**< Reset HTMR0 */
    SYS_RESET_HTMR1     = (MXC_F_GCR_RSTR1_HTMR1_POS + 32),        /**< Reset HTMR1 */
    SYS_RESET_WDT1      = (MXC_F_GCR_RSTR1_WDT1_POS + 32),        /**< Reset WDT1 */
    SYS_RESET_SPI0      = (MXC_F_GCR_RSTR1_SPI0_POS + 32),   /**< Reset QSPI0_AHB */
    SYS_RESET_SPIXMEM   = (MXC_F_GCR_RSTR1_SPIXMEM_POS + 32),     /**< Reset SPIXMEM */
    SYS_RESET_SMPHR     = (MXC_F_GCR_RSTR1_SMPHR_POS + 32)        /**< Reset SMPHR */
} sys_reset_t;

/** @brief System clock disable enumeration. Used in SYS_ClockDisable and SYS_ClockEnable functions */
typedef enum {
    SYS_PERIPH_CLOCK_GPIO0       = MXC_F_GCR_PERCKCN0_GPIO0D_POS,                    /**< Disable MXC_F_GCR_PERCKCN0_GPIO0D clock */
    SYS_PERIPH_CLOCK_GPIO1       = MXC_F_GCR_PERCKCN0_GPIO1D_POS,                    /**< Disable MXC_F_GCR_PERCKCN0_GPIO1D clock */
    SYS_PERIPH_CLOCK_USB         = MXC_F_GCR_PERCKCN0_USBD_POS,                      /**< Disable MXC_F_GCR_PERCKCN0_USBD clock */
    SYS_PERIPH_CLOCK_DMA         = MXC_F_GCR_PERCKCN0_DMAD_POS,                      /**< Disable MXC_F_GCR_PERCKCN0_DMAD clock */
    SYS_PERIPH_CLOCK_SPI1        = MXC_F_GCR_PERCKCN0_SPI1D_POS,                     /**< Disable MXC_F_GCR_PERCKCN0_SPI1D clock */
    SYS_PERIPH_CLOCK_SPI2        = MXC_F_GCR_PERCKCN0_SPI2D_POS,                     /**< Disable MXC_F_GCR_PERCKCN0_SPI2D clock */
    SYS_PERIPH_CLOCK_UART0       = MXC_F_GCR_PERCKCN0_UART0D_POS,                    /**< Disable MXC_F_GCR_PERCKCN0_UART0D clock */
    SYS_PERIPH_CLOCK_UART1       = MXC_F_GCR_PERCKCN0_UART1D_POS,                    /**< Disable MXC_F_GCR_PERCKCN0_UART1D clock */
    SYS_PERIPH_CLOCK_I2C0        = MXC_F_GCR_PERCKCN0_I2C0D_POS,                     /**< Disable MXC_F_GCR_PERCKCN0_I2C0D clock */
    SYS_PERIPH_CLOCK_TPU         = MXC_F_GCR_PERCKCN0_CRYPTOD_POS,                   /**< Disable MXC_F_GCR_PERCKCN0_CRYPTOD clock */
    SYS_PERIPH_CLOCK_T0          = MXC_F_GCR_PERCKCN0_TIMER0D_POS,                       /**< Disable MXC_F_GCR_PERCKCN0_T0D clock */
    SYS_PERIPH_CLOCK_T1          = MXC_F_GCR_PERCKCN0_TIMER1D_POS,                       /**< Disable MXC_F_GCR_PERCKCN0_T1D clock */
    SYS_PERIPH_CLOCK_T2          = MXC_F_GCR_PERCKCN0_TIMER2D_POS,                       /**< Disable MXC_F_GCR_PERCKCN0_T2D clock */
    SYS_PERIPH_CLOCK_T3          = MXC_F_GCR_PERCKCN0_TIMER3D_POS,                       /**< Disable MXC_F_GCR_PERCKCN0_T3D clock */
    SYS_PERIPH_CLOCK_T4          = MXC_F_GCR_PERCKCN0_TIMER4D_POS,                       /**< Disable MXC_F_GCR_PERCKCN0_T4D clock */
    SYS_PERIPH_CLOCK_T5          = MXC_F_GCR_PERCKCN0_TIMER5D_POS,                       /**< Disable MXC_F_GCR_PERCKCN0_T5D clock */
    SYS_PERIPH_CLOCK_ADC         = MXC_F_GCR_PERCKCN0_ADCD_POS,                      /**< Disable MXC_F_GCR_PERCKCN0_ADCD clock */
    SYS_PERIPH_CLOCK_I2C1        = MXC_F_GCR_PERCKCN0_I2C1D_POS,                     /**< Disable MXC_F_GCR_PERCKCN0_I2C1D clock */
    SYS_PERIPH_CLOCK_PT          = MXC_F_GCR_PERCKCN0_PTD_POS,                       /**< Disable MXC_F_GCR_PERCKCN0_PTD clock */
    SYS_PERIPH_CLOCK_SPIXIP      = MXC_F_GCR_PERCKCN0_SPIXIPD_POS,                   /**< Disable MXC_F_GCR_PERCKCN0_SPIXIPD clock */
    SYS_PERIPH_CLOCK_SPIXFC       = MXC_F_GCR_PERCKCN0_SPIMD_POS,                     /**< Disable MXC_F_GCR_PERCKCN0_SPIMD clock */
    /* PERCKCN1 Below this line we add 32 to separate PERCKCN0 and PERCKCN1 */
    SYS_PERIPH_CLOCK_BTLE        =(MXC_F_GCR_PERCKCN1_BTLED_POS + 32),
    SYS_PERIPH_CLOCK_UART2       =(MXC_F_GCR_PERCKCN1_UART2D_POS + 32),            /**<Disable MXC_F_GCR_PERCKCN1_UART2D clock */
    SYS_PERIPH_CLOCK_TRNG        =(MXC_F_GCR_PERCKCN1_TRNGD_POS + 32),             /**<Disable MXC_F_GCR_PERCKCN1_TRNGD clock */
    SYS_PERIPH_CLOCK_SCACHE      =(MXC_F_GCR_PERCKCN1_SCACHED_POS + 32),           /**<Disable MXC_F_GCR_PERCKCN1_SCACHED clock */
    SYS_PERIPH_CLOCK_SDMA        =(MXC_F_GCR_PERCKCN1_SDMAD_POS + 32),             /**<Disable MXC_F_GCR_PERCKCN1_SDMAD clock */
    SYS_PERIPH_CLOCK_SMPHR       =(MXC_F_GCR_PERCKCN1_SMPHRD_POS + 32),            /**<Disable MXC_F_GCR_PERCKCN1_SMPHRD clock */
    SYS_PERIPH_CLOCK_SDHC        =(MXC_F_GCR_PERCKCN1_SDHCD_POS + 32),             /**<Disable MXC_F_GCR_PERCKCN1_SDHCD clock */
    SYS_PERIPH_CLOCK_ICACHEXIP   =(MXC_F_GCR_PERCKCN1_ICACHEXIPD_POS + 32),        /**<Disable MXC_F_GCR_PERCKCN1_ICACHEXIPD clock */
    SYS_PERIPH_CLOCK_OWIRE       =(MXC_F_GCR_PERCKCN1_OWIRED_POS + 32),            /**<Disable MXC_F_GCR_PERCKCN1_OWIRED clock */
    SYS_PERIPH_CLOCK_SPI0        =(MXC_F_GCR_PERCKCN1_SPI0D_POS + 32),             /**< Disable QSPI Clock (API Calls QSPI SPI0) */
    SYS_PERIPH_CLOCK_SPIXIPD     =(MXC_F_GCR_PERCKCN1_SPIXIPDD_POS + 32),
    SYS_PERIPH_CLOCK_DMA1        =(MXC_F_GCR_PERCKCN1_DMA1_POS + 32),
    SYS_PERIPH_CLOCK_AUDIO       =(MXC_F_GCR_PERCKCN1_AUDIO_POS + 32),
    SYS_PERIPH_CLOCK_I2C2        =(MXC_F_GCR_PERCKCN1_I2C2_POS + 32),
    SYS_PERIPH_CLOCK_HTMR0       =(MXC_F_GCR_PERCKCN1_HTMR0_POS + 32),           /**<Disable MXC_F_GCR_PERCKCN1_HTMR0 clock */
    SYS_PERIPH_CLOCK_HTMR1       =(MXC_F_GCR_PERCKCN1_HTMR1_POS + 32),           /**<Disable MXC_F_GCR_PERCKCN1_HTMR1 clock */
    SYS_PERIPH_CLOCK_WDT0        =(MXC_F_GCR_PERCKCN1_WDT0_POS + 32),
    SYS_PERIPH_CLOCK_WDT1        =(MXC_F_GCR_PERCKCN1_WDT1_POS + 32),
    SYS_PERIPH_CLOCK_WDT2        =(MXC_F_GCR_PERCKCN1_WDT2_POS + 32),
    SYS_PERIPH_CLOCK_CPU1        =(MXC_F_GCR_PERCKCN1_CPU1_POS + 32) 
} sys_periph_clock_t;

typedef enum {
    SYS_CLOCK_HIRC96    = MXC_V_GCR_CLKCN_CLKSEL_HIRC96,
    SYS_CLOCK_HIRC8     = MXC_V_GCR_CLKCN_CLKSEL_HIRC8,
    SYS_CLOCK_HIRC      = MXC_V_GCR_CLKCN_CLKSEL_HIRC,
    SYS_CLOCK_XTAL32M   = MXC_V_GCR_CLKCN_CLKSEL_XTAL32M,
    SYS_CLOCK_LIRC8K    = MXC_V_GCR_CLKCN_CLKSEL_LIRC8,
    SYS_CLOCK_XTAL32K   = MXC_V_GCR_CLKCN_CLKSEL_XTAL32K 
} sys_system_clock_t;

#define SYS_SCACHE_CLK 1  // Enable SCACHE CLK
#define SYS_CRYPTO_CLK 1  // Enable CRYPTO CLK

#define SYS_USN_CHECKSUM_LEN    16

/** @brief Map control */
typedef enum {
    MAP_A,
    MAP_B
} sys_map_t;
/** @brief Enable control */
typedef enum {
    Disable,
    Enable
} sys_control_t;
typedef void* sys_cfg_t;

/** @brief SPI17Y Configuration Object */
typedef  struct 
{
    sys_map_t map;
    sys_control_t ss0;   
    sys_control_t ss1;   
    sys_control_t ss2;
    unsigned num_io;  
} sys_cfg_spi17y_t;

/** @brief UART Configuration Object */
typedef  struct 
{
    sys_map_t map;
    sys_control_t flow; 
} sys_cfg_uart_t;

/** @brief SPIXR Configuration Object */
typedef struct
{
    uint8_t scache_flag;
    uint8_t crypto_flag;
} sys_cfg_spixr_t;


/** @brief I2C System Configuration Object */
typedef sys_cfg_t sys_cfg_i2c_t; 

/** @brief SDHC System Configuration Object */
typedef sys_cfg_t sys_cfg_sdhc_t; 

/** @brief OWM System Configuration Object */
typedef sys_cfg_t sys_cfg_owm_t; 

/** @brief System Cache System Configuration Object */
typedef sys_cfg_t sys_cfg_scache_t; 

/** @brief USB High Speed System Configuration Object */
typedef sys_cfg_t sys_cfg_usbhs_t; 

/** @brief Real Time Clock System Configuration Object */
typedef sys_cfg_t sys_cfg_rtc_t; 

/** @brief TPU System Configuration Object */
typedef sys_cfg_t sys_cfg_tpu_t; 

/** @brief Timer System Configuration Object */
typedef sys_cfg_t sys_cfg_tmr_t; 

/** @brief ADC System Configuration Object */
typedef sys_cfg_t sys_cfg_adc_t; 

/** @brief Flash Controller System Configuration Object */
typedef sys_cfg_t sys_cfg_flc_t; 

/** @brief Random Number Generator System Configuration Object */
typedef sys_cfg_t sys_cfg_trng_t; 

/** @brief SPI External Memory Controller System Configuration Object */
typedef sys_cfg_t sys_cfg_spixfc_t;

/** @brief Pulse Train System Configuration Object */
typedef gpio_cfg_t sys_cfg_pt_t;

/** @brief Pulse Train System Configuration Object */
typedef sys_cfg_t sys_cfg_ptg_t;

/** @brief HTimer System Configuration Object */
typedef sys_cfg_t sys_cfg_htmr_t;

/** @brief Semaphore System Configuration Object */
typedef sys_cfg_t sys_cfg_sema_t;

/** @brief Watchdog System Configuration Object */
typedef sys_cfg_t sys_cfg_wdt_t;

/** @brief Clock scaling is not supported on this device, and any parameter using this type is ignored */
typedef unsigned int sys_pt_clk_scale;
  
/***** Function Prototypes *****/

/**
 * @brief Returns the Unique Serial Number.
 * @param usn       Pointer to store the USN, 16 bytes.
 * @param checksum  Pointer to store the computed checksum, 16 bytes. NULL if 
 *                  no checksum verification required. TPU requried for verficiation.
 * @returns         E_NO_ERROR if the checksum matches, otherwise error. 
 */
int SYS_GetUSN(uint8_t *usn, uint8_t *checksum);

/**
 * @brief Determines if the selected peripheral clock is enabled.
 * @param clock   Enumeration for desired clock.
 * @returns       0 is the clock is disabled, non 0 if the clock is enabled.
 */
int SYS_IsClockEnabled(sys_periph_clock_t clock);

/**
 * @brief Disables the selected peripheral clock.
 * @param clock   Enumeration for desired clock.
 */
void SYS_ClockDisable(sys_periph_clock_t clock);

/**
 * @brief Enables the selected peripheral clock.
 * @param clock   Enumeration for desired clock.
 */
void SYS_ClockEnable(sys_periph_clock_t clock);

/**
 * @brief Enables the 32kHz oscillator
 * @param sys_cfg   Not used, may be NULL.
 */
void SYS_RTCClockEnable(sys_cfg_rtc_t *sys_cfg);

/**
 * @brief Disables the 32kHz oscillator
 * @returns         E_NO_ERROR if everything is successful
 */
int SYS_RTCClockDisable(void);
  
/**
 * @brief Enable System Clock Source without switching to it
 * @param      clock The clock to enable
 * @return     E_NO_ERROR if everything is successful
 */ 
int SYS_ClockSourceEnable(sys_system_clock_t clock);

/**
 * @brief Disable System Clock Source
 * @param      clock The clock to disable
 * @return     E_NO_ERROR if everything is successful
 */
int SYS_ClockSourceDisable(sys_system_clock_t clock);

/**
 * @brief Select the system clock.
 * @param clock     Enumeration for desired clock.
 * @param tmr       Optional tmr pointer for timeout. NULL if undesired.
 * @returns         E_NO_ERROR if everything is successful.
 */
int SYS_Clock_Select(sys_system_clock_t clock, mxc_tmr_regs_t* tmr);

/**
 * @brief System level initialization for UART module.
 * @param uart      Pointer to UART module registers
 * @param           System configuration object
 * @returns         E_NO_ERROR if everything is successful
 */
int SYS_UART_Init(mxc_uart_regs_t *uart, const sys_cfg_uart_t* sys_cfg);

/**
 * @brief      System level shutdown for UART module
 * @param      uart  Pointer to UART module registers
 * @return     #E_NO_ERROR if everything is successful
 * @ingroup    uart
 */
int SYS_UART_Shutdown(mxc_uart_regs_t *uart);


/**
 * @brief System level initialization for I2C module.
 * @param i2c      Pointer to I2C module registers
 * @param          System configuration object
 * @returns        #E_NO_ERROR if everything is successful
 */
int SYS_I2C_Init(mxc_i2c_regs_t *i2c, const sys_cfg_i2c_t* sys_cfg);

/**
 * @brief System level Shutdown for I2C module.
 * @param i2c      Pointer to I2C module registers
 * @returns        #E_NO_ERROR if everything is successful
 */
int SYS_I2C_Shutdown(mxc_i2c_regs_t *i2c);

/**
 * @brief Get the frequency of the I2C module source clock
 * @param spim      Pointer to I2C module registers
 * @returns         frequency in Hz
 */
unsigned SYS_I2C_GetFreq(mxc_i2c_regs_t *i2c);

/**
 * @brief System level initialization for Pulse Train module.
 * @param cfg       System configuration object
 * @returns         E_NO_ERROR if everything is successful
 */
int SYS_PT_Config(mxc_pt_regs_t *pt, const sys_cfg_pt_t *cfg);

/**
 * @brief      Initialize pulse train system settings
 * @param      System configuration object
 */
void SYS_PT_Init(const sys_cfg_ptg_t* sys_cfg);

/**
 * @brief      Disable pulse train system settings
 */
void SYS_PT_Shutdown(void);

/**
 * @brief Get the frequency of the PT module source clock.
 * @returns         frequency in Hz
 */
unsigned SYS_PT_GetFreq(void);

/**
 * @brief Get the frequency of the Timer module source clock.
 * @returns         frequency in Hz
 */
unsigned SYS_TMR_GetFreq(mxc_tmr_regs_t *tmr);
/**
 * @brief Clear Cache and Line buffer.
 */
void SYS_Flash_Operation(void);
/**
 * @brief Reset the peripherals and/or CPU in the rstr0 or rstr1 register.
 * @param           Enumeration for what to reset. Can reset multiple items at once.
 */
void SYS_Reset_Periph(sys_reset_t reset);
/**
 * @brief Get the frequency of the PT module source clock.
 * @param           System configuration object
 * @returns         frequency in Hz
 */
int SYS_SDHC_Init(const sys_cfg_sdhc_t* sys_cfg);

/**
 * @brief System level shutdown for SDHC module
 * @returns         E_NO_ERROR if everything is successful
 */
int SYS_SDHC_Shutdown(void);

/**
 * @brief Enables the Seamphore peripheral clock
 * @returns         frequency in Hz
 */
int SYS_SEMA_Init(const sys_cfg_sema_t* sys_cfg);

/**
 * @brief Shutdown the semaphore clock
 * @returns         frequency in Hz
 */
int SYS_SEMA_Shutdown(void);

/**
 * @brief System level initialization for SPIXFC module.
 * @param spixf      Pointer to SPIXFC module registers
 * @param           System configuration object
 * @returns         E_NO_ERROR if everything is successful
 */
int SYS_SPIXFC_Init(mxc_spixfc_regs_t *spixfc, const sys_cfg_spixfc_t* sys_cfg);

/**
 * @brief System level initialization for SPIXFC module.
 * @param spixf      Pointer to SPIM module registers
 * @returns         E_NO_ERROR if everything is successful
 */
int SYS_SPIXFC_Shutdown(mxc_spixfc_regs_t *spixfc);


/**
 * @brief Get the frequency of the SPIXFC module source clock
 * @param spixfc      Pointer to SPIXFC module registers
 * @returns         frequency in Hz
 */
int SYS_SPIXFC_GetFreq(mxc_spixfc_regs_t *spixfc);


/**
 * @brief      System level initialization for OWM module.
 * @param      System configuration object
 * @return     #E_NO_ERROR if everything is successful
 */
uint32_t SYS_OWM_Init(const sys_cfg_owm_t* sys_cfg);

/**
 * @brief      System level Shutdown for OWM module.
 */
void SYS_OWM_Shutdown(void);

/**
 * @brief      Get the frequency of the OWM module source clock
 * @return     frequency in Hz
 */
uint32_t SYS_OWM_GetFreq(void);

/**
 * @brief System level initialization for SPIXR module.
 * @param           System configuration object
 * @returns         E_NO_ERROR if everything is successful.
 */
int SYS_SPIXR_Init(const sys_cfg_spixr_t* sys_cfg);

/**
 * @brief System level shutdown for SPIXR module
 */
void SYS_SPIXR_Shutdown(void);

/**
 * @brief    System level initialization for SCHACE module.
 * @param    System configuration object
 */
void SYS_SCACHE_Init(const sys_cfg_scache_t* sys_cfg);

/**
 * @brief      System level Shutdown for SCACHE module.
 */
void SYS_SCACHE_Shutdown(void);


/**
 * @brief System level initialization for SPI module.
 * @param           System configuration object
 * @returns         E_NO_ERROR if everything is successful.
 */
int SYS_SPI17Y_Init( mxc_spi17y_regs_t *spi, const sys_cfg_spi17y_t* sys_cfg);

/**
 * @brief System level shutdown for SPI module
 * @returns         E_NO_ERROR if everything is successful
 */
int SYS_SPI17Y_Shutdown(mxc_spi17y_regs_t *spi);

/**
 * @brief Set the Output pin (SQW) for the RTC
 * @param System configuration object
 */
void SYS_RTC_SqwavInit(const sys_cfg_rtc_t* sys_cfg);


/**
 * @brief System-level initialization for the USBHS module
 * @returns         E_NO_ERROR upon success, or appropriate failure code
 */
int SYS_USBHS_Init(const sys_cfg_usbhs_t* sys_cfg);

/**
 * @brief System-level shutdown for the USBHS module
 * @returns         E_NO_ERROR upon success, or appropriate failure code
 */
int SYS_USBHS_Shutdown(void);
/**
 * @brief      Init DMA system settings
 */
void SYS_DMA_Init(void);

/**
 * @brief      Shutdown DMA system specific settings
 */
void SYS_DMA_Shutdown(void);

/**
 * @brief      Init TMR system settings
 * @param      System configuration object
 */
int SYS_TMR_Init(mxc_tmr_regs_t *tmr, const sys_cfg_tmr_t* sys_cfg);

/**
 * @brief      Shutdown TMR system specific settings
 */
int SYS_TMR_Shutdown(mxc_tmr_regs_t *tmr);

/**
 * @brief      Init CRYPTO system settings
 * @param      System configuration object
 */
int SYS_TPU_Init(const sys_cfg_tpu_t* sys_cfg);

/**
 * @brief      Shutdown CRYPTO system specific settings
 */
int SYS_TPU_Shutdown(void);

/**
 * @brief      Init ADC system settings
 * @param      System configuration object
 */
int SYS_ADC_Init(const sys_cfg_adc_t* sys_cfg);

/**
 * @brief      Shutdown ADC system specific settings
 */
int SYS_ADC_Shutdown(void);

/**
 * @brief      Init flash system settings
 */
int SYS_FLC_Init(const sys_cfg_flc_t* sys_cfg);
/**
 * @brief      Get FLC registers by the address of flash being written
 *
 * @param      flc   The flc
 * @param[in]  addr  The address of flash being written
 *
 * @return     E_NO_ERROR upon success, or appropriate failure code
 */
int SYS_FLC_GetByAddress(mxc_flc_regs_t **flc, uint32_t addr);

/**
 * @brief      gets physical address from logical address
 *
 * @param[in]  addr    logical address of flash
 * @param      result  returns physical address to this variable
 *
 * @return     E_NO_ERROR upon success, or appropriate failure code
 */
int SYS_FLC_GetPhysicalAddress( uint32_t addr, uint32_t *result);

/**
 * @brief      Shutdown flash system specific settings
 */
int SYS_FLC_Shutdown(void);

/**
 * @brief      Init TRNG system settings
 * @param      System configuration object
 */
int SYS_TRNG_Init(const sys_cfg_trng_t* sys_cfg);

/**
 * @brief      Shutdown TRNG system specific settings
 */
int SYS_TRNG_Shutdown(void);

/**
 * @brief      Init HTimer system specific settings
 * @param      htmr The HTMR being initialized
 */
int SYS_HTMR_Init(mxc_htmr_regs_t *htmr);

/**
 * @brief      Shutdown  system specific settings
 * @param      htmr The HTMR being shutdown
*/
int SYS_HTMR_Shutdown(mxc_htmr_regs_t *htmr);

/**
 * @brief      Init Watchdog system specific settings
 * @param      wdt The WDT being initialized
 * @param      sys_cfg System configuration object
 */
int SYS_WDT_Init(mxc_wdt_regs_t *wdt, const sys_cfg_wdt_t* sys_cfg);

/**
 * @brief      Get the frequency of the WUT module source clock
 * @return     frequency in Hz
 */
uint32_t SYS_WUT_GetFreq(void);

/**
 * @brief      Set the frequency of the WUT module source clock
 * @param      freq     frequency in Hz
 */
void SYS_WUT_SetFreq(uint32_t freq);

#ifdef __cplusplus
}
#endif

#endif /* _MXC_SYS_H_*/
