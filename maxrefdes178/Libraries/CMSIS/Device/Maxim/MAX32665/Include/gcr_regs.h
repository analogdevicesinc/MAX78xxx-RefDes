/**
 * @file    gcr_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the GCR Peripheral Module.
 */

/* ****************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
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
 *
 *************************************************************************** */

#ifndef _GCR_REGS_H_
#define _GCR_REGS_H_

/* **** Includes **** */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
 
#if defined (__ICCARM__)
  #pragma system_include
#endif
 
#if defined (__CC_ARM)
  #pragma anon_unions
#endif
/// @cond
/*
    If types are not defined elsewhere (CMSIS) define them here
*/
#ifndef __IO
#define __IO volatile
#endif
#ifndef __I
#define __I  volatile const
#endif
#ifndef __O
#define __O  volatile
#endif
#ifndef __R
#define __R  volatile const
#endif
/// @endcond

/* **** Definitions **** */

/**
 * @ingroup     gcr
 * @defgroup    gcr_registers GCR_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the GCR Peripheral Module.
 * @details Global Control Registers.
 */

/**
 * @ingroup gcr_registers
 * Structure type to access the GCR Registers.
 */
typedef struct {
    __IO uint32_t scon;                 /**< <tt>\b 0x00:</tt> GCR SCON Register */
    __IO uint32_t rst0;                 /**< <tt>\b 0x04:</tt> GCR RST0 Register */
    __IO uint32_t clk_ctrl;             /**< <tt>\b 0x08:</tt> GCR CLK_CTRL Register */
    __IO uint32_t pmr;                  /**< <tt>\b 0x0C:</tt> GCR PMR Register */
    __R  uint32_t rsv_0x10_0x17[2];
    __IO uint32_t pclk_div;             /**< <tt>\b 0x18:</tt> GCR PCLK_DIV Register */
    __R  uint32_t rsv_0x1c_0x23[2];
    __IO uint32_t pclk_dis0;            /**< <tt>\b 0x24:</tt> GCR PCLK_DIS0 Register */
    __IO uint32_t mem_clk;              /**< <tt>\b 0x28:</tt> GCR MEM_CLK Register */
    __IO uint32_t mem_zero;             /**< <tt>\b 0x2C:</tt> GCR MEM_ZERO Register */
    __R  uint32_t rsv_0x30;
    __IO uint32_t scck;                 /**< <tt>\b 0x34:</tt> GCR SCCK Register */
    __IO uint32_t mpri0;                /**< <tt>\b 0x38:</tt> GCR MPRI0 Register */
    __IO uint32_t mpri1;                /**< <tt>\b 0x3C:</tt> GCR MPRI1 Register */
    __IO uint32_t sys_stat;             /**< <tt>\b 0x40:</tt> GCR SYS_STAT Register */
    __IO uint32_t rst1;                 /**< <tt>\b 0x44:</tt> GCR RST1 Register */
    __IO uint32_t pclk_dis1;            /**< <tt>\b 0x48:</tt> GCR PCLK_DIS1 Register */
    __IO uint32_t event_en;             /**< <tt>\b 0x4C:</tt> GCR EVENT_EN Register */
    __I  uint32_t revision;             /**< <tt>\b 0x50:</tt> GCR REVISION Register */
    __IO uint32_t sys_stat_ie;          /**< <tt>\b 0x54:</tt> GCR SYS_STAT_IE Register */
    __R  uint32_t rsv_0x58_0x63[3];
    __IO uint32_t ecc_er;               /**< <tt>\b 0x64:</tt> GCR ECC_ER Register */
    __IO uint32_t ecc_nded;             /**< <tt>\b 0x68:</tt> GCR ECC_NDED Register */
    __IO uint32_t ecc_irqen;            /**< <tt>\b 0x6C:</tt> GCR ECC_IRQEN Register */
    __IO uint32_t ecc_errad;            /**< <tt>\b 0x70:</tt> GCR ECC_ERRAD Register */
    __IO uint32_t btle_ldocr;           /**< <tt>\b 0x74:</tt> GCR BTLE_LDOCR Register */
    __IO uint32_t btle_ldodcr;          /**< <tt>\b 0x78:</tt> GCR BTLE_LDODCR Register */
    __R  uint32_t rsv_0x7c;
    __IO uint32_t gpr0;                 /**< <tt>\b 0x80:</tt> GCR GPR0 Register */
    __IO uint32_t apb_async;            /**< <tt>\b 0x84:</tt> GCR APB_ASYNC Register */
} mxc_gcr_regs_t;

/* Register offsets for module GCR */
/**
 * @ingroup    gcr_registers
 * @defgroup   GCR_Register_Offsets Register Offsets
 * @brief      GCR Peripheral Register Offsets from the GCR Base Peripheral Address. 
 * @{
 */
 #define MXC_R_GCR_SCON                     ((uint32_t)0x00000000UL) /**< Offset from GCR Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_GCR_RST0                     ((uint32_t)0x00000004UL) /**< Offset from GCR Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_GCR_CLK_CTRL                 ((uint32_t)0x00000008UL) /**< Offset from GCR Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_GCR_PMR                      ((uint32_t)0x0000000CUL) /**< Offset from GCR Base Address: <tt> 0x000C</tt> */ 
 #define MXC_R_GCR_PCLK_DIV                 ((uint32_t)0x00000018UL) /**< Offset from GCR Base Address: <tt> 0x0018</tt> */ 
 #define MXC_R_GCR_PCLK_DIS0                ((uint32_t)0x00000024UL) /**< Offset from GCR Base Address: <tt> 0x0024</tt> */ 
 #define MXC_R_GCR_MEM_CLK                  ((uint32_t)0x00000028UL) /**< Offset from GCR Base Address: <tt> 0x0028</tt> */ 
 #define MXC_R_GCR_MEM_ZERO                 ((uint32_t)0x0000002CUL) /**< Offset from GCR Base Address: <tt> 0x002C</tt> */ 
 #define MXC_R_GCR_SCCK                     ((uint32_t)0x00000034UL) /**< Offset from GCR Base Address: <tt> 0x0034</tt> */ 
 #define MXC_R_GCR_MPRI0                    ((uint32_t)0x00000038UL) /**< Offset from GCR Base Address: <tt> 0x0038</tt> */ 
 #define MXC_R_GCR_MPRI1                    ((uint32_t)0x0000003CUL) /**< Offset from GCR Base Address: <tt> 0x003C</tt> */ 
 #define MXC_R_GCR_SYS_STAT                 ((uint32_t)0x00000040UL) /**< Offset from GCR Base Address: <tt> 0x0040</tt> */ 
 #define MXC_R_GCR_RST1                     ((uint32_t)0x00000044UL) /**< Offset from GCR Base Address: <tt> 0x0044</tt> */ 
 #define MXC_R_GCR_PCLK_DIS1                ((uint32_t)0x00000048UL) /**< Offset from GCR Base Address: <tt> 0x0048</tt> */ 
 #define MXC_R_GCR_EVENT_EN                 ((uint32_t)0x0000004CUL) /**< Offset from GCR Base Address: <tt> 0x004C</tt> */ 
 #define MXC_R_GCR_REVISION                 ((uint32_t)0x00000050UL) /**< Offset from GCR Base Address: <tt> 0x0050</tt> */ 
 #define MXC_R_GCR_SYS_STAT_IE              ((uint32_t)0x00000054UL) /**< Offset from GCR Base Address: <tt> 0x0054</tt> */ 
 #define MXC_R_GCR_ECC_ER                   ((uint32_t)0x00000064UL) /**< Offset from GCR Base Address: <tt> 0x0064</tt> */ 
 #define MXC_R_GCR_ECC_NDED                 ((uint32_t)0x00000068UL) /**< Offset from GCR Base Address: <tt> 0x0068</tt> */ 
 #define MXC_R_GCR_ECC_IRQEN                ((uint32_t)0x0000006CUL) /**< Offset from GCR Base Address: <tt> 0x006C</tt> */ 
 #define MXC_R_GCR_ECC_ERRAD                ((uint32_t)0x00000070UL) /**< Offset from GCR Base Address: <tt> 0x0070</tt> */ 
 #define MXC_R_GCR_BTLE_LDOCR               ((uint32_t)0x00000074UL) /**< Offset from GCR Base Address: <tt> 0x0074</tt> */ 
 #define MXC_R_GCR_BTLE_LDODCR              ((uint32_t)0x00000078UL) /**< Offset from GCR Base Address: <tt> 0x0078</tt> */ 
 #define MXC_R_GCR_GPR0                     ((uint32_t)0x00000080UL) /**< Offset from GCR Base Address: <tt> 0x0080</tt> */ 
 #define MXC_R_GCR_APB_ASYNC                ((uint32_t)0x00000084UL) /**< Offset from GCR Base Address: <tt> 0x0084</tt> */ 
/**@} end of group gcr_registers */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_SCON GCR_SCON
 * @brief    System Control.
 * @{
 */
 #define MXC_F_GCR_SCON_BSTAPEN_POS                     0 /**< SCON_BSTAPEN Position */
 #define MXC_F_GCR_SCON_BSTAPEN                         ((uint32_t)(0x1UL << MXC_F_GCR_SCON_BSTAPEN_POS)) /**< SCON_BSTAPEN Mask */

 #define MXC_F_GCR_SCON_SBUSARB_POS                     1 /**< SCON_SBUSARB Position */
 #define MXC_F_GCR_SCON_SBUSARB                         ((uint32_t)(0x3UL << MXC_F_GCR_SCON_SBUSARB_POS)) /**< SCON_SBUSARB Mask */
 #define MXC_V_GCR_SCON_SBUSARB_FIX                     ((uint32_t)0x0UL) /**< SCON_SBUSARB_FIX Value */
 #define MXC_S_GCR_SCON_SBUSARB_FIX                     (MXC_V_GCR_SCON_SBUSARB_FIX << MXC_F_GCR_SCON_SBUSARB_POS) /**< SCON_SBUSARB_FIX Setting */
 #define MXC_V_GCR_SCON_SBUSARB_ROUND                   ((uint32_t)0x1UL) /**< SCON_SBUSARB_ROUND Value */
 #define MXC_S_GCR_SCON_SBUSARB_ROUND                   (MXC_V_GCR_SCON_SBUSARB_ROUND << MXC_F_GCR_SCON_SBUSARB_POS) /**< SCON_SBUSARB_ROUND Setting */

 #define MXC_F_GCR_SCON_FLASH_PAGE_FLIP_POS             4 /**< SCON_FLASH_PAGE_FLIP Position */
 #define MXC_F_GCR_SCON_FLASH_PAGE_FLIP                 ((uint32_t)(0x1UL << MXC_F_GCR_SCON_FLASH_PAGE_FLIP_POS)) /**< SCON_FLASH_PAGE_FLIP Mask */

 #define MXC_F_GCR_SCON_CCACHE_FLUSH_POS                6 /**< SCON_CCACHE_FLUSH Position */
 #define MXC_F_GCR_SCON_CCACHE_FLUSH                    ((uint32_t)(0x1UL << MXC_F_GCR_SCON_CCACHE_FLUSH_POS)) /**< SCON_CCACHE_FLUSH Mask */

 #define MXC_F_GCR_SCON_DCACHE_FLUSH_POS                7 /**< SCON_DCACHE_FLUSH Position */
 #define MXC_F_GCR_SCON_DCACHE_FLUSH                    ((uint32_t)(0x1UL << MXC_F_GCR_SCON_DCACHE_FLUSH_POS)) /**< SCON_DCACHE_FLUSH Mask */

 #define MXC_F_GCR_SCON_DCACHE_DIS_POS                  9 /**< SCON_DCACHE_DIS Position */
 #define MXC_F_GCR_SCON_DCACHE_DIS                      ((uint32_t)(0x1UL << MXC_F_GCR_SCON_DCACHE_DIS_POS)) /**< SCON_DCACHE_DIS Mask */

 #define MXC_F_GCR_SCON_CCHK_POS                        13 /**< SCON_CCHK Position */
 #define MXC_F_GCR_SCON_CCHK                            ((uint32_t)(0x1UL << MXC_F_GCR_SCON_CCHK_POS)) /**< SCON_CCHK Mask */

 #define MXC_F_GCR_SCON_CHKRES_POS                      15 /**< SCON_CHKRES Position */
 #define MXC_F_GCR_SCON_CHKRES                          ((uint32_t)(0x1UL << MXC_F_GCR_SCON_CHKRES_POS)) /**< SCON_CHKRES Mask */

 #define MXC_F_GCR_SCON_OVR_POS                         16 /**< SCON_OVR Position */
 #define MXC_F_GCR_SCON_OVR                             ((uint32_t)(0x3UL << MXC_F_GCR_SCON_OVR_POS)) /**< SCON_OVR Mask */
 #define MXC_V_GCR_SCON_OVR_0_9V                        ((uint32_t)0x0UL) /**< SCON_OVR_0_9V Value */
 #define MXC_S_GCR_SCON_OVR_0_9V                        (MXC_V_GCR_SCON_OVR_0_9V << MXC_F_GCR_SCON_OVR_POS) /**< SCON_OVR_0_9V Setting */
 #define MXC_V_GCR_SCON_OVR_1_0V                        ((uint32_t)0x1UL) /**< SCON_OVR_1_0V Value */
 #define MXC_S_GCR_SCON_OVR_1_0V                        (MXC_V_GCR_SCON_OVR_1_0V << MXC_F_GCR_SCON_OVR_POS) /**< SCON_OVR_1_0V Setting */
 #define MXC_V_GCR_SCON_OVR_1_1V                        ((uint32_t)0x2UL) /**< SCON_OVR_1_1V Value */
 #define MXC_S_GCR_SCON_OVR_1_1V                        (MXC_V_GCR_SCON_OVR_1_1V << MXC_F_GCR_SCON_OVR_POS) /**< SCON_OVR_1_1V Setting */

/**@} end of group GCR_SCON_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_RST0 GCR_RST0
 * @brief    Reset.
 * @{
 */
 #define MXC_F_GCR_RST0_DMA0_POS                        0 /**< RST0_DMA0 Position */
 #define MXC_F_GCR_RST0_DMA0                            ((uint32_t)(0x1UL << MXC_F_GCR_RST0_DMA0_POS)) /**< RST0_DMA0 Mask */

 #define MXC_F_GCR_RST0_WDT0_POS                        1 /**< RST0_WDT0 Position */
 #define MXC_F_GCR_RST0_WDT0                            ((uint32_t)(0x1UL << MXC_F_GCR_RST0_WDT0_POS)) /**< RST0_WDT0 Mask */

 #define MXC_F_GCR_RST0_GPIO0_POS                       2 /**< RST0_GPIO0 Position */
 #define MXC_F_GCR_RST0_GPIO0                           ((uint32_t)(0x1UL << MXC_F_GCR_RST0_GPIO0_POS)) /**< RST0_GPIO0 Mask */

 #define MXC_F_GCR_RST0_GPIO1_POS                       3 /**< RST0_GPIO1 Position */
 #define MXC_F_GCR_RST0_GPIO1                           ((uint32_t)(0x1UL << MXC_F_GCR_RST0_GPIO1_POS)) /**< RST0_GPIO1 Mask */

 #define MXC_F_GCR_RST0_TIMER0_POS                      5 /**< RST0_TIMER0 Position */
 #define MXC_F_GCR_RST0_TIMER0                          ((uint32_t)(0x1UL << MXC_F_GCR_RST0_TIMER0_POS)) /**< RST0_TIMER0 Mask */

 #define MXC_F_GCR_RST0_TIMER1_POS                      6 /**< RST0_TIMER1 Position */
 #define MXC_F_GCR_RST0_TIMER1                          ((uint32_t)(0x1UL << MXC_F_GCR_RST0_TIMER1_POS)) /**< RST0_TIMER1 Mask */

 #define MXC_F_GCR_RST0_TIMER2_POS                      7 /**< RST0_TIMER2 Position */
 #define MXC_F_GCR_RST0_TIMER2                          ((uint32_t)(0x1UL << MXC_F_GCR_RST0_TIMER2_POS)) /**< RST0_TIMER2 Mask */

 #define MXC_F_GCR_RST0_TIMER3_POS                      8 /**< RST0_TIMER3 Position */
 #define MXC_F_GCR_RST0_TIMER3                          ((uint32_t)(0x1UL << MXC_F_GCR_RST0_TIMER3_POS)) /**< RST0_TIMER3 Mask */

 #define MXC_F_GCR_RST0_TIMER4_POS                      9 /**< RST0_TIMER4 Position */
 #define MXC_F_GCR_RST0_TIMER4                          ((uint32_t)(0x1UL << MXC_F_GCR_RST0_TIMER4_POS)) /**< RST0_TIMER4 Mask */

 #define MXC_F_GCR_RST0_TIMER5_POS                      10 /**< RST0_TIMER5 Position */
 #define MXC_F_GCR_RST0_TIMER5                          ((uint32_t)(0x1UL << MXC_F_GCR_RST0_TIMER5_POS)) /**< RST0_TIMER5 Mask */

 #define MXC_F_GCR_RST0_UART0_POS                       11 /**< RST0_UART0 Position */
 #define MXC_F_GCR_RST0_UART0                           ((uint32_t)(0x1UL << MXC_F_GCR_RST0_UART0_POS)) /**< RST0_UART0 Mask */

 #define MXC_F_GCR_RST0_UART1_POS                       12 /**< RST0_UART1 Position */
 #define MXC_F_GCR_RST0_UART1                           ((uint32_t)(0x1UL << MXC_F_GCR_RST0_UART1_POS)) /**< RST0_UART1 Mask */

 #define MXC_F_GCR_RST0_SPI1_POS                        13 /**< RST0_SPI1 Position */
 #define MXC_F_GCR_RST0_SPI1                            ((uint32_t)(0x1UL << MXC_F_GCR_RST0_SPI1_POS)) /**< RST0_SPI1 Mask */

 #define MXC_F_GCR_RST0_SPI2_POS                        14 /**< RST0_SPI2 Position */
 #define MXC_F_GCR_RST0_SPI2                            ((uint32_t)(0x1UL << MXC_F_GCR_RST0_SPI2_POS)) /**< RST0_SPI2 Mask */

 #define MXC_F_GCR_RST0_I2C0_POS                        16 /**< RST0_I2C0 Position */
 #define MXC_F_GCR_RST0_I2C0                            ((uint32_t)(0x1UL << MXC_F_GCR_RST0_I2C0_POS)) /**< RST0_I2C0 Mask */

 #define MXC_F_GCR_RST0_RTC_POS                         17 /**< RST0_RTC Position */
 #define MXC_F_GCR_RST0_RTC                             ((uint32_t)(0x1UL << MXC_F_GCR_RST0_RTC_POS)) /**< RST0_RTC Mask */

 #define MXC_F_GCR_RST0_CRYPTO_POS                      18 /**< RST0_CRYPTO Position */
 #define MXC_F_GCR_RST0_CRYPTO                          ((uint32_t)(0x1UL << MXC_F_GCR_RST0_CRYPTO_POS)) /**< RST0_CRYPTO Mask */

 #define MXC_F_GCR_RST0_SMPHR_POS                       22 /**< RST0_SMPHR Position */
 #define MXC_F_GCR_RST0_SMPHR                           ((uint32_t)(0x1UL << MXC_F_GCR_RST0_SMPHR_POS)) /**< RST0_SMPHR Mask */

 #define MXC_F_GCR_RST0_USB_POS                         23 /**< RST0_USB Position */
 #define MXC_F_GCR_RST0_USB                             ((uint32_t)(0x1UL << MXC_F_GCR_RST0_USB_POS)) /**< RST0_USB Mask */

 #define MXC_F_GCR_RST0_TRNG_POS                        24 /**< RST0_TRNG Position */
 #define MXC_F_GCR_RST0_TRNG                            ((uint32_t)(0x1UL << MXC_F_GCR_RST0_TRNG_POS)) /**< RST0_TRNG Mask */

 #define MXC_F_GCR_RST0_ADC_POS                         26 /**< RST0_ADC Position */
 #define MXC_F_GCR_RST0_ADC                             ((uint32_t)(0x1UL << MXC_F_GCR_RST0_ADC_POS)) /**< RST0_ADC Mask */

 #define MXC_F_GCR_RST0_DMA1_POS                        27 /**< RST0_DMA1 Position */
 #define MXC_F_GCR_RST0_DMA1                            ((uint32_t)(0x1UL << MXC_F_GCR_RST0_DMA1_POS)) /**< RST0_DMA1 Mask */

 #define MXC_F_GCR_RST0_UART2_POS                       28 /**< RST0_UART2 Position */
 #define MXC_F_GCR_RST0_UART2                           ((uint32_t)(0x1UL << MXC_F_GCR_RST0_UART2_POS)) /**< RST0_UART2 Mask */

 #define MXC_F_GCR_RST0_SOFT_RST_POS                    29 /**< RST0_SOFT_RST Position */
 #define MXC_F_GCR_RST0_SOFT_RST                        ((uint32_t)(0x1UL << MXC_F_GCR_RST0_SOFT_RST_POS)) /**< RST0_SOFT_RST Mask */

 #define MXC_F_GCR_RST0_PERIPH_RST_POS                  30 /**< RST0_PERIPH_RST Position */
 #define MXC_F_GCR_RST0_PERIPH_RST                      ((uint32_t)(0x1UL << MXC_F_GCR_RST0_PERIPH_RST_POS)) /**< RST0_PERIPH_RST Mask */

 #define MXC_F_GCR_RST0_SYS_RST_POS                     31 /**< RST0_SYS_RST Position */
 #define MXC_F_GCR_RST0_SYS_RST                         ((uint32_t)(0x1UL << MXC_F_GCR_RST0_SYS_RST_POS)) /**< RST0_SYS_RST Mask */

/**@} end of group GCR_RST0_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_CLK_CTRL GCR_CLK_CTRL
 * @brief    Clock Control.
 * @{
 */
 #define MXC_F_GCR_CLK_CTRL_SYSCLK_PRESCALE_POS         6 /**< CLK_CTRL_SYSCLK_PRESCALE Position */
 #define MXC_F_GCR_CLK_CTRL_SYSCLK_PRESCALE             ((uint32_t)(0x7UL << MXC_F_GCR_CLK_CTRL_SYSCLK_PRESCALE_POS)) /**< CLK_CTRL_SYSCLK_PRESCALE Mask */
 #define MXC_V_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV1        ((uint32_t)0x0UL) /**< CLK_CTRL_SYSCLK_PRESCALE_DIV1 Value */
 #define MXC_S_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV1        (MXC_V_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV1 << MXC_F_GCR_CLK_CTRL_SYSCLK_PRESCALE_POS) /**< CLK_CTRL_SYSCLK_PRESCALE_DIV1 Setting */
 #define MXC_V_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV2        ((uint32_t)0x1UL) /**< CLK_CTRL_SYSCLK_PRESCALE_DIV2 Value */
 #define MXC_S_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV2        (MXC_V_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV2 << MXC_F_GCR_CLK_CTRL_SYSCLK_PRESCALE_POS) /**< CLK_CTRL_SYSCLK_PRESCALE_DIV2 Setting */
 #define MXC_V_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV4        ((uint32_t)0x2UL) /**< CLK_CTRL_SYSCLK_PRESCALE_DIV4 Value */
 #define MXC_S_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV4        (MXC_V_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV4 << MXC_F_GCR_CLK_CTRL_SYSCLK_PRESCALE_POS) /**< CLK_CTRL_SYSCLK_PRESCALE_DIV4 Setting */
 #define MXC_V_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV8        ((uint32_t)0x3UL) /**< CLK_CTRL_SYSCLK_PRESCALE_DIV8 Value */
 #define MXC_S_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV8        (MXC_V_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV8 << MXC_F_GCR_CLK_CTRL_SYSCLK_PRESCALE_POS) /**< CLK_CTRL_SYSCLK_PRESCALE_DIV8 Setting */
 #define MXC_V_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV16       ((uint32_t)0x4UL) /**< CLK_CTRL_SYSCLK_PRESCALE_DIV16 Value */
 #define MXC_S_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV16       (MXC_V_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV16 << MXC_F_GCR_CLK_CTRL_SYSCLK_PRESCALE_POS) /**< CLK_CTRL_SYSCLK_PRESCALE_DIV16 Setting */
 #define MXC_V_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV32       ((uint32_t)0x5UL) /**< CLK_CTRL_SYSCLK_PRESCALE_DIV32 Value */
 #define MXC_S_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV32       (MXC_V_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV32 << MXC_F_GCR_CLK_CTRL_SYSCLK_PRESCALE_POS) /**< CLK_CTRL_SYSCLK_PRESCALE_DIV32 Setting */
 #define MXC_V_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV64       ((uint32_t)0x6UL) /**< CLK_CTRL_SYSCLK_PRESCALE_DIV64 Value */
 #define MXC_S_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV64       (MXC_V_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV64 << MXC_F_GCR_CLK_CTRL_SYSCLK_PRESCALE_POS) /**< CLK_CTRL_SYSCLK_PRESCALE_DIV64 Setting */
 #define MXC_V_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV128      ((uint32_t)0x7UL) /**< CLK_CTRL_SYSCLK_PRESCALE_DIV128 Value */
 #define MXC_S_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV128      (MXC_V_GCR_CLK_CTRL_SYSCLK_PRESCALE_DIV128 << MXC_F_GCR_CLK_CTRL_SYSCLK_PRESCALE_POS) /**< CLK_CTRL_SYSCLK_PRESCALE_DIV128 Setting */

 #define MXC_F_GCR_CLK_CTRL_SYSOSC_SEL_POS              9 /**< CLK_CTRL_SYSOSC_SEL Position */
 #define MXC_F_GCR_CLK_CTRL_SYSOSC_SEL                  ((uint32_t)(0x7UL << MXC_F_GCR_CLK_CTRL_SYSOSC_SEL_POS)) /**< CLK_CTRL_SYSOSC_SEL Mask */
 #define MXC_V_GCR_CLK_CTRL_SYSOSC_SEL_HIRC             ((uint32_t)0x0UL) /**< CLK_CTRL_SYSOSC_SEL_HIRC Value */
 #define MXC_S_GCR_CLK_CTRL_SYSOSC_SEL_HIRC             (MXC_V_GCR_CLK_CTRL_SYSOSC_SEL_HIRC << MXC_F_GCR_CLK_CTRL_SYSOSC_SEL_POS) /**< CLK_CTRL_SYSOSC_SEL_HIRC Setting */
 #define MXC_V_GCR_CLK_CTRL_SYSOSC_SEL_XTAL32M          ((uint32_t)0x2UL) /**< CLK_CTRL_SYSOSC_SEL_XTAL32M Value */
 #define MXC_S_GCR_CLK_CTRL_SYSOSC_SEL_XTAL32M          (MXC_V_GCR_CLK_CTRL_SYSOSC_SEL_XTAL32M << MXC_F_GCR_CLK_CTRL_SYSOSC_SEL_POS) /**< CLK_CTRL_SYSOSC_SEL_XTAL32M Setting */
 #define MXC_V_GCR_CLK_CTRL_SYSOSC_SEL_LIRC8            ((uint32_t)0x3UL) /**< CLK_CTRL_SYSOSC_SEL_LIRC8 Value */
 #define MXC_S_GCR_CLK_CTRL_SYSOSC_SEL_LIRC8            (MXC_V_GCR_CLK_CTRL_SYSOSC_SEL_LIRC8 << MXC_F_GCR_CLK_CTRL_SYSOSC_SEL_POS) /**< CLK_CTRL_SYSOSC_SEL_LIRC8 Setting */
 #define MXC_V_GCR_CLK_CTRL_SYSOSC_SEL_HIRC96           ((uint32_t)0x4UL) /**< CLK_CTRL_SYSOSC_SEL_HIRC96 Value */
 #define MXC_S_GCR_CLK_CTRL_SYSOSC_SEL_HIRC96           (MXC_V_GCR_CLK_CTRL_SYSOSC_SEL_HIRC96 << MXC_F_GCR_CLK_CTRL_SYSOSC_SEL_POS) /**< CLK_CTRL_SYSOSC_SEL_HIRC96 Setting */
 #define MXC_V_GCR_CLK_CTRL_SYSOSC_SEL_HIRC8            ((uint32_t)0x5UL) /**< CLK_CTRL_SYSOSC_SEL_HIRC8 Value */
 #define MXC_S_GCR_CLK_CTRL_SYSOSC_SEL_HIRC8            (MXC_V_GCR_CLK_CTRL_SYSOSC_SEL_HIRC8 << MXC_F_GCR_CLK_CTRL_SYSOSC_SEL_POS) /**< CLK_CTRL_SYSOSC_SEL_HIRC8 Setting */
 #define MXC_V_GCR_CLK_CTRL_SYSOSC_SEL_XTAL32K          ((uint32_t)0x6UL) /**< CLK_CTRL_SYSOSC_SEL_XTAL32K Value */
 #define MXC_S_GCR_CLK_CTRL_SYSOSC_SEL_XTAL32K          (MXC_V_GCR_CLK_CTRL_SYSOSC_SEL_XTAL32K << MXC_F_GCR_CLK_CTRL_SYSOSC_SEL_POS) /**< CLK_CTRL_SYSOSC_SEL_XTAL32K Setting */

 #define MXC_F_GCR_CLK_CTRL_SYSOSC_RDY_POS              13 /**< CLK_CTRL_SYSOSC_RDY Position */
 #define MXC_F_GCR_CLK_CTRL_SYSOSC_RDY                  ((uint32_t)(0x1UL << MXC_F_GCR_CLK_CTRL_SYSOSC_RDY_POS)) /**< CLK_CTRL_SYSOSC_RDY Mask */

 #define MXC_F_GCR_CLK_CTRL_CCD_POS                     15 /**< CLK_CTRL_CCD Position */
 #define MXC_F_GCR_CLK_CTRL_CCD                         ((uint32_t)(0x1UL << MXC_F_GCR_CLK_CTRL_CCD_POS)) /**< CLK_CTRL_CCD Mask */

 #define MXC_F_GCR_CLK_CTRL_X32M_EN_POS                 16 /**< CLK_CTRL_X32M_EN Position */
 #define MXC_F_GCR_CLK_CTRL_X32M_EN                     ((uint32_t)(0x1UL << MXC_F_GCR_CLK_CTRL_X32M_EN_POS)) /**< CLK_CTRL_X32M_EN Mask */

 #define MXC_F_GCR_CLK_CTRL_X32K_EN_POS                 17 /**< CLK_CTRL_X32K_EN Position */
 #define MXC_F_GCR_CLK_CTRL_X32K_EN                     ((uint32_t)(0x1UL << MXC_F_GCR_CLK_CTRL_X32K_EN_POS)) /**< CLK_CTRL_X32K_EN Mask */

 #define MXC_F_GCR_CLK_CTRL_HIRC60M_EN_POS              18 /**< CLK_CTRL_HIRC60M_EN Position */
 #define MXC_F_GCR_CLK_CTRL_HIRC60M_EN                  ((uint32_t)(0x1UL << MXC_F_GCR_CLK_CTRL_HIRC60M_EN_POS)) /**< CLK_CTRL_HIRC60M_EN Mask */

 #define MXC_F_GCR_CLK_CTRL_HIRCMM_EN_POS               19 /**< CLK_CTRL_HIRCMM_EN Position */
 #define MXC_F_GCR_CLK_CTRL_HIRCMM_EN                   ((uint32_t)(0x1UL << MXC_F_GCR_CLK_CTRL_HIRCMM_EN_POS)) /**< CLK_CTRL_HIRCMM_EN Mask */

 #define MXC_F_GCR_CLK_CTRL_HIRC8M_EN_POS               20 /**< CLK_CTRL_HIRC8M_EN Position */
 #define MXC_F_GCR_CLK_CTRL_HIRC8M_EN                   ((uint32_t)(0x1UL << MXC_F_GCR_CLK_CTRL_HIRC8M_EN_POS)) /**< CLK_CTRL_HIRC8M_EN Mask */

 #define MXC_F_GCR_CLK_CTRL_HIRC8M_VS_POS               21 /**< CLK_CTRL_HIRC8M_VS Position */
 #define MXC_F_GCR_CLK_CTRL_HIRC8M_VS                   ((uint32_t)(0x1UL << MXC_F_GCR_CLK_CTRL_HIRC8M_VS_POS)) /**< CLK_CTRL_HIRC8M_VS Mask */

 #define MXC_F_GCR_CLK_CTRL_X32M_RDY_POS                24 /**< CLK_CTRL_X32M_RDY Position */
 #define MXC_F_GCR_CLK_CTRL_X32M_RDY                    ((uint32_t)(0x1UL << MXC_F_GCR_CLK_CTRL_X32M_RDY_POS)) /**< CLK_CTRL_X32M_RDY Mask */

 #define MXC_F_GCR_CLK_CTRL_X32K_RDY_POS                25 /**< CLK_CTRL_X32K_RDY Position */
 #define MXC_F_GCR_CLK_CTRL_X32K_RDY                    ((uint32_t)(0x1UL << MXC_F_GCR_CLK_CTRL_X32K_RDY_POS)) /**< CLK_CTRL_X32K_RDY Mask */

 #define MXC_F_GCR_CLK_CTRL_HIRC60M_RDY_POS             26 /**< CLK_CTRL_HIRC60M_RDY Position */
 #define MXC_F_GCR_CLK_CTRL_HIRC60M_RDY                 ((uint32_t)(0x1UL << MXC_F_GCR_CLK_CTRL_HIRC60M_RDY_POS)) /**< CLK_CTRL_HIRC60M_RDY Mask */

 #define MXC_F_GCR_CLK_CTRL_HIRCMM_RDY_POS              27 /**< CLK_CTRL_HIRCMM_RDY Position */
 #define MXC_F_GCR_CLK_CTRL_HIRCMM_RDY                  ((uint32_t)(0x1UL << MXC_F_GCR_CLK_CTRL_HIRCMM_RDY_POS)) /**< CLK_CTRL_HIRCMM_RDY Mask */

 #define MXC_F_GCR_CLK_CTRL_HIRC8M_RDY_POS              28 /**< CLK_CTRL_HIRC8M_RDY Position */
 #define MXC_F_GCR_CLK_CTRL_HIRC8M_RDY                  ((uint32_t)(0x1UL << MXC_F_GCR_CLK_CTRL_HIRC8M_RDY_POS)) /**< CLK_CTRL_HIRC8M_RDY Mask */

 #define MXC_F_GCR_CLK_CTRL_LIRC8K_RDY_POS              29 /**< CLK_CTRL_LIRC8K_RDY Position */
 #define MXC_F_GCR_CLK_CTRL_LIRC8K_RDY                  ((uint32_t)(0x1UL << MXC_F_GCR_CLK_CTRL_LIRC8K_RDY_POS)) /**< CLK_CTRL_LIRC8K_RDY Mask */

 #define MXC_F_GCR_CLK_CTRL_LIRC6K_RDY_POS              30 /**< CLK_CTRL_LIRC6K_RDY Position */
 #define MXC_F_GCR_CLK_CTRL_LIRC6K_RDY                  ((uint32_t)(0x1UL << MXC_F_GCR_CLK_CTRL_LIRC6K_RDY_POS)) /**< CLK_CTRL_LIRC6K_RDY Mask */

/**@} end of group GCR_CLK_CTRL_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_PMR GCR_PMR
 * @brief    Power Management.
 * @{
 */
 #define MXC_F_GCR_PMR_MODE_POS                         0 /**< PMR_MODE Position */
 #define MXC_F_GCR_PMR_MODE                             ((uint32_t)(0x7UL << MXC_F_GCR_PMR_MODE_POS)) /**< PMR_MODE Mask */
 #define MXC_V_GCR_PMR_MODE_ACTIVE                      ((uint32_t)0x0UL) /**< PMR_MODE_ACTIVE Value */
 #define MXC_S_GCR_PMR_MODE_ACTIVE                      (MXC_V_GCR_PMR_MODE_ACTIVE << MXC_F_GCR_PMR_MODE_POS) /**< PMR_MODE_ACTIVE Setting */
 #define MXC_V_GCR_PMR_MODE_DEEPSLEEP                   ((uint32_t)0x2UL) /**< PMR_MODE_DEEPSLEEP Value */
 #define MXC_S_GCR_PMR_MODE_DEEPSLEEP                   (MXC_V_GCR_PMR_MODE_DEEPSLEEP << MXC_F_GCR_PMR_MODE_POS) /**< PMR_MODE_DEEPSLEEP Setting */
 #define MXC_V_GCR_PMR_MODE_SHUTDOWN                    ((uint32_t)0x3UL) /**< PMR_MODE_SHUTDOWN Value */
 #define MXC_S_GCR_PMR_MODE_SHUTDOWN                    (MXC_V_GCR_PMR_MODE_SHUTDOWN << MXC_F_GCR_PMR_MODE_POS) /**< PMR_MODE_SHUTDOWN Setting */
 #define MXC_V_GCR_PMR_MODE_BACKUP                      ((uint32_t)0x4UL) /**< PMR_MODE_BACKUP Value */
 #define MXC_S_GCR_PMR_MODE_BACKUP                      (MXC_V_GCR_PMR_MODE_BACKUP << MXC_F_GCR_PMR_MODE_POS) /**< PMR_MODE_BACKUP Setting */

 #define MXC_F_GCR_PMR_GPIOWKEN_POS                     4 /**< PMR_GPIOWKEN Position */
 #define MXC_F_GCR_PMR_GPIOWKEN                         ((uint32_t)(0x1UL << MXC_F_GCR_PMR_GPIOWKEN_POS)) /**< PMR_GPIOWKEN Mask */

 #define MXC_F_GCR_PMR_RTCWKEN_POS                      5 /**< PMR_RTCWKEN Position */
 #define MXC_F_GCR_PMR_RTCWKEN                          ((uint32_t)(0x1UL << MXC_F_GCR_PMR_RTCWKEN_POS)) /**< PMR_RTCWKEN Mask */

 #define MXC_F_GCR_PMR_USBWKEN_POS                      6 /**< PMR_USBWKEN Position */
 #define MXC_F_GCR_PMR_USBWKEN                          ((uint32_t)(0x1UL << MXC_F_GCR_PMR_USBWKEN_POS)) /**< PMR_USBWKEN Mask */

 #define MXC_F_GCR_PMR_WUTWKEN_POS                      7 /**< PMR_WUTWKEN Position */
 #define MXC_F_GCR_PMR_WUTWKEN                          ((uint32_t)(0x1UL << MXC_F_GCR_PMR_WUTWKEN_POS)) /**< PMR_WUTWKEN Mask */

 #define MXC_F_GCR_PMR_SDMAWKEN_POS                     8 /**< PMR_SDMAWKEN Position */
 #define MXC_F_GCR_PMR_SDMAWKEN                         ((uint32_t)(0x1UL << MXC_F_GCR_PMR_SDMAWKEN_POS)) /**< PMR_SDMAWKEN Mask */

 #define MXC_F_GCR_PMR_COMPWKEN_POS                     8 /**< PMR_COMPWKEN Position */
 #define MXC_F_GCR_PMR_COMPWKEN                         ((uint32_t)(0x1UL << MXC_F_GCR_PMR_COMPWKEN_POS)) /**< PMR_COMPWKEN Mask */

 #define MXC_F_GCR_PMR_HIRCPD_POS                       15 /**< PMR_HIRCPD Position */
 #define MXC_F_GCR_PMR_HIRCPD                           ((uint32_t)(0x1UL << MXC_F_GCR_PMR_HIRCPD_POS)) /**< PMR_HIRCPD Mask */

 #define MXC_F_GCR_PMR_HIRC96MPD_POS                    16 /**< PMR_HIRC96MPD Position */
 #define MXC_F_GCR_PMR_HIRC96MPD                        ((uint32_t)(0x1UL << MXC_F_GCR_PMR_HIRC96MPD_POS)) /**< PMR_HIRC96MPD Mask */

 #define MXC_F_GCR_PMR_HIRC8MPD_POS                     17 /**< PMR_HIRC8MPD Position */
 #define MXC_F_GCR_PMR_HIRC8MPD                         ((uint32_t)(0x1UL << MXC_F_GCR_PMR_HIRC8MPD_POS)) /**< PMR_HIRC8MPD Mask */

 #define MXC_F_GCR_PMR_XTALPB_POS                       20 /**< PMR_XTALPB Position */
 #define MXC_F_GCR_PMR_XTALPB                           ((uint32_t)(0x1UL << MXC_F_GCR_PMR_XTALPB_POS)) /**< PMR_XTALPB Mask */

/**@} end of group GCR_PMR_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_PCLK_DIV GCR_PCLK_DIV
 * @brief    Peripheral Clock Divider.
 * @{
 */
 #define MXC_F_GCR_PCLK_DIV_PCF_POS                     0 /**< PCLK_DIV_PCF Position */
 #define MXC_F_GCR_PCLK_DIV_PCF                         ((uint32_t)(0x7UL << MXC_F_GCR_PCLK_DIV_PCF_POS)) /**< PCLK_DIV_PCF Mask */
 #define MXC_V_GCR_PCLK_DIV_PCF_96MHZ                   ((uint32_t)0x2UL) /**< PCLK_DIV_PCF_96MHZ Value */
 #define MXC_S_GCR_PCLK_DIV_PCF_96MHZ                   (MXC_V_GCR_PCLK_DIV_PCF_96MHZ << MXC_F_GCR_PCLK_DIV_PCF_POS) /**< PCLK_DIV_PCF_96MHZ Setting */
 #define MXC_V_GCR_PCLK_DIV_PCF_48MHZ                   ((uint32_t)0x3UL) /**< PCLK_DIV_PCF_48MHZ Value */
 #define MXC_S_GCR_PCLK_DIV_PCF_48MHZ                   (MXC_V_GCR_PCLK_DIV_PCF_48MHZ << MXC_F_GCR_PCLK_DIV_PCF_POS) /**< PCLK_DIV_PCF_48MHZ Setting */
 #define MXC_V_GCR_PCLK_DIV_PCF_24MHZ                   ((uint32_t)0x4UL) /**< PCLK_DIV_PCF_24MHZ Value */
 #define MXC_S_GCR_PCLK_DIV_PCF_24MHZ                   (MXC_V_GCR_PCLK_DIV_PCF_24MHZ << MXC_F_GCR_PCLK_DIV_PCF_POS) /**< PCLK_DIV_PCF_24MHZ Setting */
 #define MXC_V_GCR_PCLK_DIV_PCF_12MHZ                   ((uint32_t)0x5UL) /**< PCLK_DIV_PCF_12MHZ Value */
 #define MXC_S_GCR_PCLK_DIV_PCF_12MHZ                   (MXC_V_GCR_PCLK_DIV_PCF_12MHZ << MXC_F_GCR_PCLK_DIV_PCF_POS) /**< PCLK_DIV_PCF_12MHZ Setting */
 #define MXC_V_GCR_PCLK_DIV_PCF_6MHZ                    ((uint32_t)0x6UL) /**< PCLK_DIV_PCF_6MHZ Value */
 #define MXC_S_GCR_PCLK_DIV_PCF_6MHZ                    (MXC_V_GCR_PCLK_DIV_PCF_6MHZ << MXC_F_GCR_PCLK_DIV_PCF_POS) /**< PCLK_DIV_PCF_6MHZ Setting */
 #define MXC_V_GCR_PCLK_DIV_PCF_3MHZ                    ((uint32_t)0x7UL) /**< PCLK_DIV_PCF_3MHZ Value */
 #define MXC_S_GCR_PCLK_DIV_PCF_3MHZ                    (MXC_V_GCR_PCLK_DIV_PCF_3MHZ << MXC_F_GCR_PCLK_DIV_PCF_POS) /**< PCLK_DIV_PCF_3MHZ Setting */

 #define MXC_F_GCR_PCLK_DIV_SDHCFRQ_POS                 7 /**< PCLK_DIV_SDHCFRQ Position */
 #define MXC_F_GCR_PCLK_DIV_SDHCFRQ                     ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIV_SDHCFRQ_POS)) /**< PCLK_DIV_SDHCFRQ Mask */

 #define MXC_F_GCR_PCLK_DIV_ADCFRQ_POS                  10 /**< PCLK_DIV_ADCFRQ Position */
 #define MXC_F_GCR_PCLK_DIV_ADCFRQ                      ((uint32_t)(0xFUL << MXC_F_GCR_PCLK_DIV_ADCFRQ_POS)) /**< PCLK_DIV_ADCFRQ Mask */

 #define MXC_F_GCR_PCLK_DIV_AONDIV_POS                  14 /**< PCLK_DIV_AONDIV Position */
 #define MXC_F_GCR_PCLK_DIV_AONDIV                      ((uint32_t)(0x3UL << MXC_F_GCR_PCLK_DIV_AONDIV_POS)) /**< PCLK_DIV_AONDIV Mask */
 #define MXC_V_GCR_PCLK_DIV_AONDIV_DIV_4                ((uint32_t)0x0UL) /**< PCLK_DIV_AONDIV_DIV_4 Value */
 #define MXC_S_GCR_PCLK_DIV_AONDIV_DIV_4                (MXC_V_GCR_PCLK_DIV_AONDIV_DIV_4 << MXC_F_GCR_PCLK_DIV_AONDIV_POS) /**< PCLK_DIV_AONDIV_DIV_4 Setting */
 #define MXC_V_GCR_PCLK_DIV_AONDIV_DIV_8                ((uint32_t)0x1UL) /**< PCLK_DIV_AONDIV_DIV_8 Value */
 #define MXC_S_GCR_PCLK_DIV_AONDIV_DIV_8                (MXC_V_GCR_PCLK_DIV_AONDIV_DIV_8 << MXC_F_GCR_PCLK_DIV_AONDIV_POS) /**< PCLK_DIV_AONDIV_DIV_8 Setting */
 #define MXC_V_GCR_PCLK_DIV_AONDIV_DIV_16               ((uint32_t)0x2UL) /**< PCLK_DIV_AONDIV_DIV_16 Value */
 #define MXC_S_GCR_PCLK_DIV_AONDIV_DIV_16               (MXC_V_GCR_PCLK_DIV_AONDIV_DIV_16 << MXC_F_GCR_PCLK_DIV_AONDIV_POS) /**< PCLK_DIV_AONDIV_DIV_16 Setting */
 #define MXC_V_GCR_PCLK_DIV_AONDIV_DIV_32               ((uint32_t)0x3UL) /**< PCLK_DIV_AONDIV_DIV_32 Value */
 #define MXC_S_GCR_PCLK_DIV_AONDIV_DIV_32               (MXC_V_GCR_PCLK_DIV_AONDIV_DIV_32 << MXC_F_GCR_PCLK_DIV_AONDIV_POS) /**< PCLK_DIV_AONDIV_DIV_32 Setting */

/**@} end of group GCR_PCLK_DIV_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_PCLK_DIS0 GCR_PCLK_DIS0
 * @brief    Peripheral Clock Disable.
 * @{
 */
 #define MXC_F_GCR_PCLK_DIS0_GPIO0_POS                  0 /**< PCLK_DIS0_GPIO0 Position */
 #define MXC_F_GCR_PCLK_DIS0_GPIO0                      ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_GPIO0_POS)) /**< PCLK_DIS0_GPIO0 Mask */

 #define MXC_F_GCR_PCLK_DIS0_GPIO1_POS                  1 /**< PCLK_DIS0_GPIO1 Position */
 #define MXC_F_GCR_PCLK_DIS0_GPIO1                      ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_GPIO1_POS)) /**< PCLK_DIS0_GPIO1 Mask */

 #define MXC_F_GCR_PCLK_DIS0_USB_POS                    3 /**< PCLK_DIS0_USB Position */
 #define MXC_F_GCR_PCLK_DIS0_USB                        ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_USB_POS)) /**< PCLK_DIS0_USB Mask */

 #define MXC_F_GCR_PCLK_DIS0_DMA0_POS                   5 /**< PCLK_DIS0_DMA0 Position */
 #define MXC_F_GCR_PCLK_DIS0_DMA0                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_DMA0_POS)) /**< PCLK_DIS0_DMA0 Mask */

 #define MXC_F_GCR_PCLK_DIS0_SPI0_POS                   6 /**< PCLK_DIS0_SPI0 Position */
 #define MXC_F_GCR_PCLK_DIS0_SPI0                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_SPI0_POS)) /**< PCLK_DIS0_SPI0 Mask */

 #define MXC_F_GCR_PCLK_DIS0_SPI1_POS                   7 /**< PCLK_DIS0_SPI1 Position */
 #define MXC_F_GCR_PCLK_DIS0_SPI1                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_SPI1_POS)) /**< PCLK_DIS0_SPI1 Mask */

 #define MXC_F_GCR_PCLK_DIS0_UART0_POS                  9 /**< PCLK_DIS0_UART0 Position */
 #define MXC_F_GCR_PCLK_DIS0_UART0                      ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_UART0_POS)) /**< PCLK_DIS0_UART0 Mask */

 #define MXC_F_GCR_PCLK_DIS0_UART1_POS                  10 /**< PCLK_DIS0_UART1 Position */
 #define MXC_F_GCR_PCLK_DIS0_UART1                      ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_UART1_POS)) /**< PCLK_DIS0_UART1 Mask */

 #define MXC_F_GCR_PCLK_DIS0_I2C0_POS                   13 /**< PCLK_DIS0_I2C0 Position */
 #define MXC_F_GCR_PCLK_DIS0_I2C0                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_I2C0_POS)) /**< PCLK_DIS0_I2C0 Mask */

 #define MXC_F_GCR_PCLK_DIS0_CRYPTO_POS                 14 /**< PCLK_DIS0_CRYPTO Position */
 #define MXC_F_GCR_PCLK_DIS0_CRYPTO                     ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_CRYPTO_POS)) /**< PCLK_DIS0_CRYPTO Mask */

 #define MXC_F_GCR_PCLK_DIS0_TIMER0_POS                 15 /**< PCLK_DIS0_TIMER0 Position */
 #define MXC_F_GCR_PCLK_DIS0_TIMER0                     ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_TIMER0_POS)) /**< PCLK_DIS0_TIMER0 Mask */

 #define MXC_F_GCR_PCLK_DIS0_TIMER1_POS                 16 /**< PCLK_DIS0_TIMER1 Position */
 #define MXC_F_GCR_PCLK_DIS0_TIMER1                     ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_TIMER1_POS)) /**< PCLK_DIS0_TIMER1 Mask */

 #define MXC_F_GCR_PCLK_DIS0_TIMER2_POS                 17 /**< PCLK_DIS0_TIMER2 Position */
 #define MXC_F_GCR_PCLK_DIS0_TIMER2                     ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_TIMER2_POS)) /**< PCLK_DIS0_TIMER2 Mask */

 #define MXC_F_GCR_PCLK_DIS0_TIMER3_POS                 18 /**< PCLK_DIS0_TIMER3 Position */
 #define MXC_F_GCR_PCLK_DIS0_TIMER3                     ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_TIMER3_POS)) /**< PCLK_DIS0_TIMER3 Mask */

 #define MXC_F_GCR_PCLK_DIS0_TIMER4_POS                 19 /**< PCLK_DIS0_TIMER4 Position */
 #define MXC_F_GCR_PCLK_DIS0_TIMER4                     ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_TIMER4_POS)) /**< PCLK_DIS0_TIMER4 Mask */

 #define MXC_F_GCR_PCLK_DIS0_TIMER5_POS                 20 /**< PCLK_DIS0_TIMER5 Position */
 #define MXC_F_GCR_PCLK_DIS0_TIMER5                     ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_TIMER5_POS)) /**< PCLK_DIS0_TIMER5 Mask */

 #define MXC_F_GCR_PCLK_DIS0_ADC_POS                    23 /**< PCLK_DIS0_ADC Position */
 #define MXC_F_GCR_PCLK_DIS0_ADC                        ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_ADC_POS)) /**< PCLK_DIS0_ADC Mask */

 #define MXC_F_GCR_PCLK_DIS0_I2C1_POS                   28 /**< PCLK_DIS0_I2C1 Position */
 #define MXC_F_GCR_PCLK_DIS0_I2C1                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_I2C1_POS)) /**< PCLK_DIS0_I2C1 Mask */

 #define MXC_F_GCR_PCLK_DIS0_PTD_POS                    29 /**< PCLK_DIS0_PTD Position */
 #define MXC_F_GCR_PCLK_DIS0_PTD                        ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_PTD_POS)) /**< PCLK_DIS0_PTD Mask */

 #define MXC_F_GCR_PCLK_DIS0_SPIXIPF_POS                30 /**< PCLK_DIS0_SPIXIPF Position */
 #define MXC_F_GCR_PCLK_DIS0_SPIXIPF                    ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_SPIXIPF_POS)) /**< PCLK_DIS0_SPIXIPF Mask */

 #define MXC_F_GCR_PCLK_DIS0_SPIXIPM_POS                31 /**< PCLK_DIS0_SPIXIPM Position */
 #define MXC_F_GCR_PCLK_DIS0_SPIXIPM                    ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_SPIXIPM_POS)) /**< PCLK_DIS0_SPIXIPM Mask */

/**@} end of group GCR_PCLK_DIS0_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_MEM_CLK GCR_MEM_CLK
 * @brief    Memory Clock Control Register.
 * @{
 */
 #define MXC_F_GCR_MEM_CLK_FWS_POS                      0 /**< MEM_CLK_FWS Position */
 #define MXC_F_GCR_MEM_CLK_FWS                          ((uint32_t)(0x7UL << MXC_F_GCR_MEM_CLK_FWS_POS)) /**< MEM_CLK_FWS Mask */

 #define MXC_F_GCR_MEM_CLK_SYSRAM0LS_POS                16 /**< MEM_CLK_SYSRAM0LS Position */
 #define MXC_F_GCR_MEM_CLK_SYSRAM0LS                    ((uint32_t)(0x1UL << MXC_F_GCR_MEM_CLK_SYSRAM0LS_POS)) /**< MEM_CLK_SYSRAM0LS Mask */

 #define MXC_F_GCR_MEM_CLK_SYSRAM1LS_POS                17 /**< MEM_CLK_SYSRAM1LS Position */
 #define MXC_F_GCR_MEM_CLK_SYSRAM1LS                    ((uint32_t)(0x1UL << MXC_F_GCR_MEM_CLK_SYSRAM1LS_POS)) /**< MEM_CLK_SYSRAM1LS Mask */

 #define MXC_F_GCR_MEM_CLK_SYSRAM2LS_POS                18 /**< MEM_CLK_SYSRAM2LS Position */
 #define MXC_F_GCR_MEM_CLK_SYSRAM2LS                    ((uint32_t)(0x1UL << MXC_F_GCR_MEM_CLK_SYSRAM2LS_POS)) /**< MEM_CLK_SYSRAM2LS Mask */

 #define MXC_F_GCR_MEM_CLK_SYSRAM3LS_POS                19 /**< MEM_CLK_SYSRAM3LS Position */
 #define MXC_F_GCR_MEM_CLK_SYSRAM3LS                    ((uint32_t)(0x1UL << MXC_F_GCR_MEM_CLK_SYSRAM3LS_POS)) /**< MEM_CLK_SYSRAM3LS Mask */

 #define MXC_F_GCR_MEM_CLK_SYSRAM4LS_POS                20 /**< MEM_CLK_SYSRAM4LS Position */
 #define MXC_F_GCR_MEM_CLK_SYSRAM4LS                    ((uint32_t)(0x1UL << MXC_F_GCR_MEM_CLK_SYSRAM4LS_POS)) /**< MEM_CLK_SYSRAM4LS Mask */

 #define MXC_F_GCR_MEM_CLK_SYSRAM5LS_POS                21 /**< MEM_CLK_SYSRAM5LS Position */
 #define MXC_F_GCR_MEM_CLK_SYSRAM5LS                    ((uint32_t)(0x1UL << MXC_F_GCR_MEM_CLK_SYSRAM5LS_POS)) /**< MEM_CLK_SYSRAM5LS Mask */

 #define MXC_F_GCR_MEM_CLK_SYSRAM6LS_POS                22 /**< MEM_CLK_SYSRAM6LS Position */
 #define MXC_F_GCR_MEM_CLK_SYSRAM6LS                    ((uint32_t)(0x1UL << MXC_F_GCR_MEM_CLK_SYSRAM6LS_POS)) /**< MEM_CLK_SYSRAM6LS Mask */

 #define MXC_F_GCR_MEM_CLK_ICACHE0LS_POS                24 /**< MEM_CLK_ICACHE0LS Position */
 #define MXC_F_GCR_MEM_CLK_ICACHE0LS                    ((uint32_t)(0x1UL << MXC_F_GCR_MEM_CLK_ICACHE0LS_POS)) /**< MEM_CLK_ICACHE0LS Mask */

 #define MXC_F_GCR_MEM_CLK_ICACHEXIPLS_POS              25 /**< MEM_CLK_ICACHEXIPLS Position */
 #define MXC_F_GCR_MEM_CLK_ICACHEXIPLS                  ((uint32_t)(0x1UL << MXC_F_GCR_MEM_CLK_ICACHEXIPLS_POS)) /**< MEM_CLK_ICACHEXIPLS Mask */

 #define MXC_F_GCR_MEM_CLK_SCACHELS_POS                 26 /**< MEM_CLK_SCACHELS Position */
 #define MXC_F_GCR_MEM_CLK_SCACHELS                     ((uint32_t)(0x1UL << MXC_F_GCR_MEM_CLK_SCACHELS_POS)) /**< MEM_CLK_SCACHELS Mask */

 #define MXC_F_GCR_MEM_CLK_CRYPTOLS_POS                 27 /**< MEM_CLK_CRYPTOLS Position */
 #define MXC_F_GCR_MEM_CLK_CRYPTOLS                     ((uint32_t)(0x1UL << MXC_F_GCR_MEM_CLK_CRYPTOLS_POS)) /**< MEM_CLK_CRYPTOLS Mask */

 #define MXC_F_GCR_MEM_CLK_USBLS_POS                    28 /**< MEM_CLK_USBLS Position */
 #define MXC_F_GCR_MEM_CLK_USBLS                        ((uint32_t)(0x1UL << MXC_F_GCR_MEM_CLK_USBLS_POS)) /**< MEM_CLK_USBLS Mask */

 #define MXC_F_GCR_MEM_CLK_ROM0LS_POS                   29 /**< MEM_CLK_ROM0LS Position */
 #define MXC_F_GCR_MEM_CLK_ROM0LS                       ((uint32_t)(0x1UL << MXC_F_GCR_MEM_CLK_ROM0LS_POS)) /**< MEM_CLK_ROM0LS Mask */

 #define MXC_F_GCR_MEM_CLK_ROM1LS_POS                   30 /**< MEM_CLK_ROM1LS Position */
 #define MXC_F_GCR_MEM_CLK_ROM1LS                       ((uint32_t)(0x1UL << MXC_F_GCR_MEM_CLK_ROM1LS_POS)) /**< MEM_CLK_ROM1LS Mask */

 #define MXC_F_GCR_MEM_CLK_ICACHE1LS_POS                31 /**< MEM_CLK_ICACHE1LS Position */
 #define MXC_F_GCR_MEM_CLK_ICACHE1LS                    ((uint32_t)(0x1UL << MXC_F_GCR_MEM_CLK_ICACHE1LS_POS)) /**< MEM_CLK_ICACHE1LS Mask */

/**@} end of group GCR_MEM_CLK_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_MEM_ZERO GCR_MEM_ZERO
 * @brief    Memory Zeroize Control.
 * @{
 */
 #define MXC_F_GCR_MEM_ZERO_SRAM0Z_POS                  0 /**< MEM_ZERO_SRAM0Z Position */
 #define MXC_F_GCR_MEM_ZERO_SRAM0Z                      ((uint32_t)(0x1UL << MXC_F_GCR_MEM_ZERO_SRAM0Z_POS)) /**< MEM_ZERO_SRAM0Z Mask */

 #define MXC_F_GCR_MEM_ZERO_SRAM1Z_POS                  1 /**< MEM_ZERO_SRAM1Z Position */
 #define MXC_F_GCR_MEM_ZERO_SRAM1Z                      ((uint32_t)(0x1UL << MXC_F_GCR_MEM_ZERO_SRAM1Z_POS)) /**< MEM_ZERO_SRAM1Z Mask */

 #define MXC_F_GCR_MEM_ZERO_SRAM2_POS                   2 /**< MEM_ZERO_SRAM2 Position */
 #define MXC_F_GCR_MEM_ZERO_SRAM2                       ((uint32_t)(0x1UL << MXC_F_GCR_MEM_ZERO_SRAM2_POS)) /**< MEM_ZERO_SRAM2 Mask */

 #define MXC_F_GCR_MEM_ZERO_SRAM3Z_POS                  3 /**< MEM_ZERO_SRAM3Z Position */
 #define MXC_F_GCR_MEM_ZERO_SRAM3Z                      ((uint32_t)(0x1UL << MXC_F_GCR_MEM_ZERO_SRAM3Z_POS)) /**< MEM_ZERO_SRAM3Z Mask */

 #define MXC_F_GCR_MEM_ZERO_SRAM4Z_POS                  4 /**< MEM_ZERO_SRAM4Z Position */
 #define MXC_F_GCR_MEM_ZERO_SRAM4Z                      ((uint32_t)(0x1UL << MXC_F_GCR_MEM_ZERO_SRAM4Z_POS)) /**< MEM_ZERO_SRAM4Z Mask */

 #define MXC_F_GCR_MEM_ZERO_SRAM5Z_POS                  5 /**< MEM_ZERO_SRAM5Z Position */
 #define MXC_F_GCR_MEM_ZERO_SRAM5Z                      ((uint32_t)(0x1UL << MXC_F_GCR_MEM_ZERO_SRAM5Z_POS)) /**< MEM_ZERO_SRAM5Z Mask */

 #define MXC_F_GCR_MEM_ZERO_SRAM6Z_POS                  6 /**< MEM_ZERO_SRAM6Z Position */
 #define MXC_F_GCR_MEM_ZERO_SRAM6Z                      ((uint32_t)(0x1UL << MXC_F_GCR_MEM_ZERO_SRAM6Z_POS)) /**< MEM_ZERO_SRAM6Z Mask */

 #define MXC_F_GCR_MEM_ZERO_ICACHE0Z_POS                8 /**< MEM_ZERO_ICACHE0Z Position */
 #define MXC_F_GCR_MEM_ZERO_ICACHE0Z                    ((uint32_t)(0x1UL << MXC_F_GCR_MEM_ZERO_ICACHE0Z_POS)) /**< MEM_ZERO_ICACHE0Z Mask */

 #define MXC_F_GCR_MEM_ZERO_ICACHEXIPZ_POS              9 /**< MEM_ZERO_ICACHEXIPZ Position */
 #define MXC_F_GCR_MEM_ZERO_ICACHEXIPZ                  ((uint32_t)(0x1UL << MXC_F_GCR_MEM_ZERO_ICACHEXIPZ_POS)) /**< MEM_ZERO_ICACHEXIPZ Mask */

 #define MXC_F_GCR_MEM_ZERO_SCACHEDATAZ_POS             10 /**< MEM_ZERO_SCACHEDATAZ Position */
 #define MXC_F_GCR_MEM_ZERO_SCACHEDATAZ                 ((uint32_t)(0x1UL << MXC_F_GCR_MEM_ZERO_SCACHEDATAZ_POS)) /**< MEM_ZERO_SCACHEDATAZ Mask */

 #define MXC_F_GCR_MEM_ZERO_SCACHETAGZ_POS              11 /**< MEM_ZERO_SCACHETAGZ Position */
 #define MXC_F_GCR_MEM_ZERO_SCACHETAGZ                  ((uint32_t)(0x1UL << MXC_F_GCR_MEM_ZERO_SCACHETAGZ_POS)) /**< MEM_ZERO_SCACHETAGZ Mask */

 #define MXC_F_GCR_MEM_ZERO_CRYPTOZ_POS                 12 /**< MEM_ZERO_CRYPTOZ Position */
 #define MXC_F_GCR_MEM_ZERO_CRYPTOZ                     ((uint32_t)(0x1UL << MXC_F_GCR_MEM_ZERO_CRYPTOZ_POS)) /**< MEM_ZERO_CRYPTOZ Mask */

 #define MXC_F_GCR_MEM_ZERO_USBFIFOZ_POS                13 /**< MEM_ZERO_USBFIFOZ Position */
 #define MXC_F_GCR_MEM_ZERO_USBFIFOZ                    ((uint32_t)(0x1UL << MXC_F_GCR_MEM_ZERO_USBFIFOZ_POS)) /**< MEM_ZERO_USBFIFOZ Mask */

 #define MXC_F_GCR_MEM_ZERO_ICACHE1Z_POS                14 /**< MEM_ZERO_ICACHE1Z Position */
 #define MXC_F_GCR_MEM_ZERO_ICACHE1Z                    ((uint32_t)(0x1UL << MXC_F_GCR_MEM_ZERO_ICACHE1Z_POS)) /**< MEM_ZERO_ICACHE1Z Mask */

/**@} end of group GCR_MEM_ZERO_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_SYS_STAT GCR_SYS_STAT
 * @brief    System Status Register.
 * @{
 */
 #define MXC_F_GCR_SYS_STAT_ICELOCK_POS                 0 /**< SYS_STAT_ICELOCK Position */
 #define MXC_F_GCR_SYS_STAT_ICELOCK                     ((uint32_t)(0x1UL << MXC_F_GCR_SYS_STAT_ICELOCK_POS)) /**< SYS_STAT_ICELOCK Mask */

 #define MXC_F_GCR_SYS_STAT_CODEINTERR_POS              1 /**< SYS_STAT_CODEINTERR Position */
 #define MXC_F_GCR_SYS_STAT_CODEINTERR                  ((uint32_t)(0x1UL << MXC_F_GCR_SYS_STAT_CODEINTERR_POS)) /**< SYS_STAT_CODEINTERR Mask */

 #define MXC_F_GCR_SYS_STAT_SCMEMF_POS                  5 /**< SYS_STAT_SCMEMF Position */
 #define MXC_F_GCR_SYS_STAT_SCMEMF                      ((uint32_t)(0x1UL << MXC_F_GCR_SYS_STAT_SCMEMF_POS)) /**< SYS_STAT_SCMEMF Mask */

/**@} end of group GCR_SYS_STAT_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_RST1 GCR_RST1
 * @brief    Reset 1.
 * @{
 */
 #define MXC_F_GCR_RST1_I2C1_POS                        0 /**< RST1_I2C1 Position */
 #define MXC_F_GCR_RST1_I2C1                            ((uint32_t)(0x1UL << MXC_F_GCR_RST1_I2C1_POS)) /**< RST1_I2C1 Mask */

 #define MXC_F_GCR_RST1_PT_POS                          1 /**< RST1_PT Position */
 #define MXC_F_GCR_RST1_PT                              ((uint32_t)(0x1UL << MXC_F_GCR_RST1_PT_POS)) /**< RST1_PT Mask */

 #define MXC_F_GCR_RST1_SPIXIP_POS                      3 /**< RST1_SPIXIP Position */
 #define MXC_F_GCR_RST1_SPIXIP                          ((uint32_t)(0x1UL << MXC_F_GCR_RST1_SPIXIP_POS)) /**< RST1_SPIXIP Mask */

 #define MXC_F_GCR_RST1_XSPIM_POS                       4 /**< RST1_XSPIM Position */
 #define MXC_F_GCR_RST1_XSPIM                           ((uint32_t)(0x1UL << MXC_F_GCR_RST1_XSPIM_POS)) /**< RST1_XSPIM Mask */

 #define MXC_F_GCR_RST1_SDHC_POS                        6 /**< RST1_SDHC Position */
 #define MXC_F_GCR_RST1_SDHC                            ((uint32_t)(0x1UL << MXC_F_GCR_RST1_SDHC_POS)) /**< RST1_SDHC Mask */

 #define MXC_F_GCR_RST1_OWIRE_POS                       7 /**< RST1_OWIRE Position */
 #define MXC_F_GCR_RST1_OWIRE                           ((uint32_t)(0x1UL << MXC_F_GCR_RST1_OWIRE_POS)) /**< RST1_OWIRE Mask */

 #define MXC_F_GCR_RST1_WDT1_POS                        8 /**< RST1_WDT1 Position */
 #define MXC_F_GCR_RST1_WDT1                            ((uint32_t)(0x1UL << MXC_F_GCR_RST1_WDT1_POS)) /**< RST1_WDT1 Mask */

 #define MXC_F_GCR_RST1_SPI3_POS                        9 /**< RST1_SPI3 Position */
 #define MXC_F_GCR_RST1_SPI3                            ((uint32_t)(0x1UL << MXC_F_GCR_RST1_SPI3_POS)) /**< RST1_SPI3 Mask */

 #define MXC_F_GCR_RST1_XIPR_POS                        15 /**< RST1_XIPR Position */
 #define MXC_F_GCR_RST1_XIPR                            ((uint32_t)(0x1UL << MXC_F_GCR_RST1_XIPR_POS)) /**< RST1_XIPR Mask */

 #define MXC_F_GCR_RST1_SEMA_POS                        16 /**< RST1_SEMA Position */
 #define MXC_F_GCR_RST1_SEMA                            ((uint32_t)(0x1UL << MXC_F_GCR_RST1_SEMA_POS)) /**< RST1_SEMA Mask */

 #define MXC_F_GCR_RST1_WDT2_POS                        17 /**< RST1_WDT2 Position */
 #define MXC_F_GCR_RST1_WDT2                            ((uint32_t)(0x1UL << MXC_F_GCR_RST1_WDT2_POS)) /**< RST1_WDT2 Mask */

 #define MXC_F_GCR_RST1_BTLE_POS                        18 /**< RST1_BTLE Position */
 #define MXC_F_GCR_RST1_BTLE                            ((uint32_t)(0x1UL << MXC_F_GCR_RST1_BTLE_POS)) /**< RST1_BTLE Mask */

 #define MXC_F_GCR_RST1_AUDIO_POS                       19 /**< RST1_AUDIO Position */
 #define MXC_F_GCR_RST1_AUDIO                           ((uint32_t)(0x1UL << MXC_F_GCR_RST1_AUDIO_POS)) /**< RST1_AUDIO Mask */

 #define MXC_F_GCR_RST1_I2C2_POS                        20 /**< RST1_I2C2 Position */
 #define MXC_F_GCR_RST1_I2C2                            ((uint32_t)(0x1UL << MXC_F_GCR_RST1_I2C2_POS)) /**< RST1_I2C2 Mask */

 #define MXC_F_GCR_RST1_RPU_POS                         21 /**< RST1_RPU Position */
 #define MXC_F_GCR_RST1_RPU                             ((uint32_t)(0x1UL << MXC_F_GCR_RST1_RPU_POS)) /**< RST1_RPU Mask */

 #define MXC_F_GCR_RST1_HTMR0_POS                       22 /**< RST1_HTMR0 Position */
 #define MXC_F_GCR_RST1_HTMR0                           ((uint32_t)(0x1UL << MXC_F_GCR_RST1_HTMR0_POS)) /**< RST1_HTMR0 Mask */

 #define MXC_F_GCR_RST1_HTMR1_POS                       23 /**< RST1_HTMR1 Position */
 #define MXC_F_GCR_RST1_HTMR1                           ((uint32_t)(0x1UL << MXC_F_GCR_RST1_HTMR1_POS)) /**< RST1_HTMR1 Mask */

 #define MXC_F_GCR_RST1_DVS_POS                         24 /**< RST1_DVS Position */
 #define MXC_F_GCR_RST1_DVS                             ((uint32_t)(0x1UL << MXC_F_GCR_RST1_DVS_POS)) /**< RST1_DVS Mask */

 #define MXC_F_GCR_RST1_SIMO_POS                        25 /**< RST1_SIMO Position */
 #define MXC_F_GCR_RST1_SIMO                            ((uint32_t)(0x1UL << MXC_F_GCR_RST1_SIMO_POS)) /**< RST1_SIMO Mask */

/**@} end of group GCR_RST1_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_PCLK_DIS1 GCR_PCLK_DIS1
 * @brief    Peripheral Clock Disable.
 * @{
 */
 #define MXC_F_GCR_PCLK_DIS1_BTLE_POS                   0 /**< PCLK_DIS1_BTLE Position */
 #define MXC_F_GCR_PCLK_DIS1_BTLE                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS1_BTLE_POS)) /**< PCLK_DIS1_BTLE Mask */

 #define MXC_F_GCR_PCLK_DIS1_UART2_POS                  1 /**< PCLK_DIS1_UART2 Position */
 #define MXC_F_GCR_PCLK_DIS1_UART2                      ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS1_UART2_POS)) /**< PCLK_DIS1_UART2 Mask */

 #define MXC_F_GCR_PCLK_DIS1_TRNG_POS                   2 /**< PCLK_DIS1_TRNG Position */
 #define MXC_F_GCR_PCLK_DIS1_TRNG                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS1_TRNG_POS)) /**< PCLK_DIS1_TRNG Mask */

 #define MXC_F_GCR_PCLK_DIS1_SCACHE_POS                 7 /**< PCLK_DIS1_SCACHE Position */
 #define MXC_F_GCR_PCLK_DIS1_SCACHE                     ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS1_SCACHE_POS)) /**< PCLK_DIS1_SCACHE Mask */

 #define MXC_F_GCR_PCLK_DIS1_SDMA_POS                   8 /**< PCLK_DIS1_SDMA Position */
 #define MXC_F_GCR_PCLK_DIS1_SDMA                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS1_SDMA_POS)) /**< PCLK_DIS1_SDMA Mask */

 #define MXC_F_GCR_PCLK_DIS1_SMPHR_POS                  9 /**< PCLK_DIS1_SMPHR Position */
 #define MXC_F_GCR_PCLK_DIS1_SMPHR                      ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS1_SMPHR_POS)) /**< PCLK_DIS1_SMPHR Mask */

 #define MXC_F_GCR_PCLK_DIS1_SDHC_POS                   10 /**< PCLK_DIS1_SDHC Position */
 #define MXC_F_GCR_PCLK_DIS1_SDHC                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS1_SDHC_POS)) /**< PCLK_DIS1_SDHC Mask */

 #define MXC_F_GCR_PCLK_DIS1_ICACHEXIP_POS              12 /**< PCLK_DIS1_ICACHEXIP Position */
 #define MXC_F_GCR_PCLK_DIS1_ICACHEXIP                  ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS1_ICACHEXIP_POS)) /**< PCLK_DIS1_ICACHEXIP Mask */

 #define MXC_F_GCR_PCLK_DIS1_OW_POS                     13 /**< PCLK_DIS1_OW Position */
 #define MXC_F_GCR_PCLK_DIS1_OW                         ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS1_OW_POS)) /**< PCLK_DIS1_OW Mask */

 #define MXC_F_GCR_PCLK_DIS1_SPI3_POS                   14 /**< PCLK_DIS1_SPI3 Position */
 #define MXC_F_GCR_PCLK_DIS1_SPI3                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS1_SPI3_POS)) /**< PCLK_DIS1_SPI3 Mask */

 #define MXC_F_GCR_PCLK_DIS1_SPIXIPR_POS                20 /**< PCLK_DIS1_SPIXIPR Position */
 #define MXC_F_GCR_PCLK_DIS1_SPIXIPR                    ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS1_SPIXIPR_POS)) /**< PCLK_DIS1_SPIXIPR Mask */

 #define MXC_F_GCR_PCLK_DIS1_DMA1_POS                   21 /**< PCLK_DIS1_DMA1 Position */
 #define MXC_F_GCR_PCLK_DIS1_DMA1                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS1_DMA1_POS)) /**< PCLK_DIS1_DMA1 Mask */

 #define MXC_F_GCR_PCLK_DIS1_AUDIO_POS                  23 /**< PCLK_DIS1_AUDIO Position */
 #define MXC_F_GCR_PCLK_DIS1_AUDIO                      ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS1_AUDIO_POS)) /**< PCLK_DIS1_AUDIO Mask */

 #define MXC_F_GCR_PCLK_DIS1_I2C2_POS                   24 /**< PCLK_DIS1_I2C2 Position */
 #define MXC_F_GCR_PCLK_DIS1_I2C2                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS1_I2C2_POS)) /**< PCLK_DIS1_I2C2 Mask */

 #define MXC_F_GCR_PCLK_DIS1_HTMR0_POS                  25 /**< PCLK_DIS1_HTMR0 Position */
 #define MXC_F_GCR_PCLK_DIS1_HTMR0                      ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS1_HTMR0_POS)) /**< PCLK_DIS1_HTMR0 Mask */

 #define MXC_F_GCR_PCLK_DIS1_HTMR1_POS                  26 /**< PCLK_DIS1_HTMR1 Position */
 #define MXC_F_GCR_PCLK_DIS1_HTMR1                      ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS1_HTMR1_POS)) /**< PCLK_DIS1_HTMR1 Mask */

 #define MXC_F_GCR_PCLK_DIS1_WDT0_POS                   27 /**< PCLK_DIS1_WDT0 Position */
 #define MXC_F_GCR_PCLK_DIS1_WDT0                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS1_WDT0_POS)) /**< PCLK_DIS1_WDT0 Mask */

 #define MXC_F_GCR_PCLK_DIS1_WDT1_POS                   28 /**< PCLK_DIS1_WDT1 Position */
 #define MXC_F_GCR_PCLK_DIS1_WDT1                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS1_WDT1_POS)) /**< PCLK_DIS1_WDT1 Mask */

 #define MXC_F_GCR_PCLK_DIS1_WDT2_POS                   29 /**< PCLK_DIS1_WDT2 Position */
 #define MXC_F_GCR_PCLK_DIS1_WDT2                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS1_WDT2_POS)) /**< PCLK_DIS1_WDT2 Mask */

 #define MXC_F_GCR_PCLK_DIS1_CPU1_POS                   31 /**< PCLK_DIS1_CPU1 Position */
 #define MXC_F_GCR_PCLK_DIS1_CPU1                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS1_CPU1_POS)) /**< PCLK_DIS1_CPU1 Mask */

/**@} end of group GCR_PCLK_DIS1_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_EVENT_EN GCR_EVENT_EN
 * @brief    Event Enable Register.
 * @{
 */
 #define MXC_F_GCR_EVENT_EN_CPU0DMA0EVENT_POS           0 /**< EVENT_EN_CPU0DMA0EVENT Position */
 #define MXC_F_GCR_EVENT_EN_CPU0DMA0EVENT               ((uint32_t)(0x1UL << MXC_F_GCR_EVENT_EN_CPU0DMA0EVENT_POS)) /**< EVENT_EN_CPU0DMA0EVENT Mask */

 #define MXC_F_GCR_EVENT_EN_CPU0DMA1EVENT_POS           1 /**< EVENT_EN_CPU0DMA1EVENT Position */
 #define MXC_F_GCR_EVENT_EN_CPU0DMA1EVENT               ((uint32_t)(0x1UL << MXC_F_GCR_EVENT_EN_CPU0DMA1EVENT_POS)) /**< EVENT_EN_CPU0DMA1EVENT Mask */

 #define MXC_F_GCR_EVENT_EN_CPU0TXEVENT_POS             2 /**< EVENT_EN_CPU0TXEVENT Position */
 #define MXC_F_GCR_EVENT_EN_CPU0TXEVENT                 ((uint32_t)(0x1UL << MXC_F_GCR_EVENT_EN_CPU0TXEVENT_POS)) /**< EVENT_EN_CPU0TXEVENT Mask */

 #define MXC_F_GCR_EVENT_EN_CPU1DMA0EVENT_POS           3 /**< EVENT_EN_CPU1DMA0EVENT Position */
 #define MXC_F_GCR_EVENT_EN_CPU1DMA0EVENT               ((uint32_t)(0x1UL << MXC_F_GCR_EVENT_EN_CPU1DMA0EVENT_POS)) /**< EVENT_EN_CPU1DMA0EVENT Mask */

 #define MXC_F_GCR_EVENT_EN_CPU1DMA1EVENT_POS           4 /**< EVENT_EN_CPU1DMA1EVENT Position */
 #define MXC_F_GCR_EVENT_EN_CPU1DMA1EVENT               ((uint32_t)(0x1UL << MXC_F_GCR_EVENT_EN_CPU1DMA1EVENT_POS)) /**< EVENT_EN_CPU1DMA1EVENT Mask */

 #define MXC_F_GCR_EVENT_EN_CPU1TXEVENT_POS             5 /**< EVENT_EN_CPU1TXEVENT Position */
 #define MXC_F_GCR_EVENT_EN_CPU1TXEVENT                 ((uint32_t)(0x1UL << MXC_F_GCR_EVENT_EN_CPU1TXEVENT_POS)) /**< EVENT_EN_CPU1TXEVENT Mask */

/**@} end of group GCR_EVENT_EN_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_REVISION GCR_REVISION
 * @brief    Revision Register.
 * @{
 */
 #define MXC_F_GCR_REVISION_REVISION_POS                0 /**< REVISION_REVISION Position */
 #define MXC_F_GCR_REVISION_REVISION                    ((uint32_t)(0xFFFFUL << MXC_F_GCR_REVISION_REVISION_POS)) /**< REVISION_REVISION Mask */

/**@} end of group GCR_REVISION_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_SYS_STAT_IE GCR_SYS_STAT_IE
 * @brief    System Status Interrupt Enable Register.
 * @{
 */
 #define MXC_F_GCR_SYS_STAT_IE_ICEULIE_POS              0 /**< SYS_STAT_IE_ICEULIE Position */
 #define MXC_F_GCR_SYS_STAT_IE_ICEULIE                  ((uint32_t)(0x1UL << MXC_F_GCR_SYS_STAT_IE_ICEULIE_POS)) /**< SYS_STAT_IE_ICEULIE Mask */

 #define MXC_F_GCR_SYS_STAT_IE_CIEIE_POS                1 /**< SYS_STAT_IE_CIEIE Position */
 #define MXC_F_GCR_SYS_STAT_IE_CIEIE                    ((uint32_t)(0x1UL << MXC_F_GCR_SYS_STAT_IE_CIEIE_POS)) /**< SYS_STAT_IE_CIEIE Mask */

 #define MXC_F_GCR_SYS_STAT_IE_SCMFIE_POS               5 /**< SYS_STAT_IE_SCMFIE Position */
 #define MXC_F_GCR_SYS_STAT_IE_SCMFIE                   ((uint32_t)(0x1UL << MXC_F_GCR_SYS_STAT_IE_SCMFIE_POS)) /**< SYS_STAT_IE_SCMFIE Mask */

/**@} end of group GCR_SYS_STAT_IE_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_ECC_ER GCR_ECC_ER
 * @brief    ECC Error Register
 * @{
 */
 #define MXC_F_GCR_ECC_ER_SYSRAM0ECCERR_POS             0 /**< ECC_ER_SYSRAM0ECCERR Position */
 #define MXC_F_GCR_ECC_ER_SYSRAM0ECCERR                 ((uint32_t)(0x1UL << MXC_F_GCR_ECC_ER_SYSRAM0ECCERR_POS)) /**< ECC_ER_SYSRAM0ECCERR Mask */

 #define MXC_F_GCR_ECC_ER_SYSRAM1ECCERR_POS             1 /**< ECC_ER_SYSRAM1ECCERR Position */
 #define MXC_F_GCR_ECC_ER_SYSRAM1ECCERR                 ((uint32_t)(0x1UL << MXC_F_GCR_ECC_ER_SYSRAM1ECCERR_POS)) /**< ECC_ER_SYSRAM1ECCERR Mask */

 #define MXC_F_GCR_ECC_ER_SYSRAM2ECCERR_POS             2 /**< ECC_ER_SYSRAM2ECCERR Position */
 #define MXC_F_GCR_ECC_ER_SYSRAM2ECCERR                 ((uint32_t)(0x1UL << MXC_F_GCR_ECC_ER_SYSRAM2ECCERR_POS)) /**< ECC_ER_SYSRAM2ECCERR Mask */

 #define MXC_F_GCR_ECC_ER_SYSRAM3ECCERR_POS             3 /**< ECC_ER_SYSRAM3ECCERR Position */
 #define MXC_F_GCR_ECC_ER_SYSRAM3ECCERR                 ((uint32_t)(0x1UL << MXC_F_GCR_ECC_ER_SYSRAM3ECCERR_POS)) /**< ECC_ER_SYSRAM3ECCERR Mask */

 #define MXC_F_GCR_ECC_ER_SYSRAM4ECCERR_POS             4 /**< ECC_ER_SYSRAM4ECCERR Position */
 #define MXC_F_GCR_ECC_ER_SYSRAM4ECCERR                 ((uint32_t)(0x1UL << MXC_F_GCR_ECC_ER_SYSRAM4ECCERR_POS)) /**< ECC_ER_SYSRAM4ECCERR Mask */

 #define MXC_F_GCR_ECC_ER_SYSRAM5ECCERR_POS             5 /**< ECC_ER_SYSRAM5ECCERR Position */
 #define MXC_F_GCR_ECC_ER_SYSRAM5ECCERR                 ((uint32_t)(0x1UL << MXC_F_GCR_ECC_ER_SYSRAM5ECCERR_POS)) /**< ECC_ER_SYSRAM5ECCERR Mask */

 #define MXC_F_GCR_ECC_ER_SYSRAM6ECCERR_POS             6 /**< ECC_ER_SYSRAM6ECCERR Position */
 #define MXC_F_GCR_ECC_ER_SYSRAM6ECCERR                 ((uint32_t)(0x1UL << MXC_F_GCR_ECC_ER_SYSRAM6ECCERR_POS)) /**< ECC_ER_SYSRAM6ECCERR Mask */

 #define MXC_F_GCR_ECC_ER_IC0ECCERR_POS                 8 /**< ECC_ER_IC0ECCERR Position */
 #define MXC_F_GCR_ECC_ER_IC0ECCERR                     ((uint32_t)(0x1UL << MXC_F_GCR_ECC_ER_IC0ECCERR_POS)) /**< ECC_ER_IC0ECCERR Mask */

 #define MXC_F_GCR_ECC_ER_IC1ECCERR_POS                 9 /**< ECC_ER_IC1ECCERR Position */
 #define MXC_F_GCR_ECC_ER_IC1ECCERR                     ((uint32_t)(0x1UL << MXC_F_GCR_ECC_ER_IC1ECCERR_POS)) /**< ECC_ER_IC1ECCERR Mask */

 #define MXC_F_GCR_ECC_ER_ICXIPECCERR_POS               10 /**< ECC_ER_ICXIPECCERR Position */
 #define MXC_F_GCR_ECC_ER_ICXIPECCERR                   ((uint32_t)(0x1UL << MXC_F_GCR_ECC_ER_ICXIPECCERR_POS)) /**< ECC_ER_ICXIPECCERR Mask */

 #define MXC_F_GCR_ECC_ER_FL0ECCERR_POS                 11 /**< ECC_ER_FL0ECCERR Position */
 #define MXC_F_GCR_ECC_ER_FL0ECCERR                     ((uint32_t)(0x1UL << MXC_F_GCR_ECC_ER_FL0ECCERR_POS)) /**< ECC_ER_FL0ECCERR Mask */

 #define MXC_F_GCR_ECC_ER_FL1ECCERR_POS                 12 /**< ECC_ER_FL1ECCERR Position */
 #define MXC_F_GCR_ECC_ER_FL1ECCERR                     ((uint32_t)(0x1UL << MXC_F_GCR_ECC_ER_FL1ECCERR_POS)) /**< ECC_ER_FL1ECCERR Mask */

/**@} end of group GCR_ECC_ER_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_ECC_NDED GCR_ECC_NDED
 * @brief    ECC Not Double Error Detect Register
 * @{
 */
 #define MXC_F_GCR_ECC_NDED_SYSRAM0ECCNDED_POS          0 /**< ECC_NDED_SYSRAM0ECCNDED Position */
 #define MXC_F_GCR_ECC_NDED_SYSRAM0ECCNDED              ((uint32_t)(0x1UL << MXC_F_GCR_ECC_NDED_SYSRAM0ECCNDED_POS)) /**< ECC_NDED_SYSRAM0ECCNDED Mask */

 #define MXC_F_GCR_ECC_NDED_SYSRAM1ECCNDED_POS          1 /**< ECC_NDED_SYSRAM1ECCNDED Position */
 #define MXC_F_GCR_ECC_NDED_SYSRAM1ECCNDED              ((uint32_t)(0x1UL << MXC_F_GCR_ECC_NDED_SYSRAM1ECCNDED_POS)) /**< ECC_NDED_SYSRAM1ECCNDED Mask */

 #define MXC_F_GCR_ECC_NDED_SYSRAM2ECCNDED_POS          2 /**< ECC_NDED_SYSRAM2ECCNDED Position */
 #define MXC_F_GCR_ECC_NDED_SYSRAM2ECCNDED              ((uint32_t)(0x1UL << MXC_F_GCR_ECC_NDED_SYSRAM2ECCNDED_POS)) /**< ECC_NDED_SYSRAM2ECCNDED Mask */

 #define MXC_F_GCR_ECC_NDED_SYSRAM3ECCNDED_POS          3 /**< ECC_NDED_SYSRAM3ECCNDED Position */
 #define MXC_F_GCR_ECC_NDED_SYSRAM3ECCNDED              ((uint32_t)(0x1UL << MXC_F_GCR_ECC_NDED_SYSRAM3ECCNDED_POS)) /**< ECC_NDED_SYSRAM3ECCNDED Mask */

 #define MXC_F_GCR_ECC_NDED_SYSRAM4ECCNDED_POS          4 /**< ECC_NDED_SYSRAM4ECCNDED Position */
 #define MXC_F_GCR_ECC_NDED_SYSRAM4ECCNDED              ((uint32_t)(0x1UL << MXC_F_GCR_ECC_NDED_SYSRAM4ECCNDED_POS)) /**< ECC_NDED_SYSRAM4ECCNDED Mask */

 #define MXC_F_GCR_ECC_NDED_SYSRAM5ECCNDED_POS          5 /**< ECC_NDED_SYSRAM5ECCNDED Position */
 #define MXC_F_GCR_ECC_NDED_SYSRAM5ECCNDED              ((uint32_t)(0x1UL << MXC_F_GCR_ECC_NDED_SYSRAM5ECCNDED_POS)) /**< ECC_NDED_SYSRAM5ECCNDED Mask */

 #define MXC_F_GCR_ECC_NDED_SYSRAM6ECCNDED_POS          6 /**< ECC_NDED_SYSRAM6ECCNDED Position */
 #define MXC_F_GCR_ECC_NDED_SYSRAM6ECCNDED              ((uint32_t)(0x1UL << MXC_F_GCR_ECC_NDED_SYSRAM6ECCNDED_POS)) /**< ECC_NDED_SYSRAM6ECCNDED Mask */

 #define MXC_F_GCR_ECC_NDED_IC0ECCNDED_POS              8 /**< ECC_NDED_IC0ECCNDED Position */
 #define MXC_F_GCR_ECC_NDED_IC0ECCNDED                  ((uint32_t)(0x1UL << MXC_F_GCR_ECC_NDED_IC0ECCNDED_POS)) /**< ECC_NDED_IC0ECCNDED Mask */

 #define MXC_F_GCR_ECC_NDED_IC1ECCNDED_POS              9 /**< ECC_NDED_IC1ECCNDED Position */
 #define MXC_F_GCR_ECC_NDED_IC1ECCNDED                  ((uint32_t)(0x1UL << MXC_F_GCR_ECC_NDED_IC1ECCNDED_POS)) /**< ECC_NDED_IC1ECCNDED Mask */

 #define MXC_F_GCR_ECC_NDED_ICXIPECCNDED_POS            10 /**< ECC_NDED_ICXIPECCNDED Position */
 #define MXC_F_GCR_ECC_NDED_ICXIPECCNDED                ((uint32_t)(0x1UL << MXC_F_GCR_ECC_NDED_ICXIPECCNDED_POS)) /**< ECC_NDED_ICXIPECCNDED Mask */

 #define MXC_F_GCR_ECC_NDED_FL0ECCNDED_POS              11 /**< ECC_NDED_FL0ECCNDED Position */
 #define MXC_F_GCR_ECC_NDED_FL0ECCNDED                  ((uint32_t)(0x1UL << MXC_F_GCR_ECC_NDED_FL0ECCNDED_POS)) /**< ECC_NDED_FL0ECCNDED Mask */

 #define MXC_F_GCR_ECC_NDED_FL1ECCNDED_POS              12 /**< ECC_NDED_FL1ECCNDED Position */
 #define MXC_F_GCR_ECC_NDED_FL1ECCNDED                  ((uint32_t)(0x1UL << MXC_F_GCR_ECC_NDED_FL1ECCNDED_POS)) /**< ECC_NDED_FL1ECCNDED Mask */

/**@} end of group GCR_ECC_NDED_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_ECC_IRQEN GCR_ECC_IRQEN
 * @brief    ECC IRQ Enable Register
 * @{
 */
 #define MXC_F_GCR_ECC_IRQEN_ECCSYSRAM0EN_POS           0 /**< ECC_IRQEN_ECCSYSRAM0EN Position */
 #define MXC_F_GCR_ECC_IRQEN_ECCSYSRAM0EN               ((uint32_t)(0x1UL << MXC_F_GCR_ECC_IRQEN_ECCSYSRAM0EN_POS)) /**< ECC_IRQEN_ECCSYSRAM0EN Mask */

 #define MXC_F_GCR_ECC_IRQEN_ECCSYSRAM1EN_POS           1 /**< ECC_IRQEN_ECCSYSRAM1EN Position */
 #define MXC_F_GCR_ECC_IRQEN_ECCSYSRAM1EN               ((uint32_t)(0x1UL << MXC_F_GCR_ECC_IRQEN_ECCSYSRAM1EN_POS)) /**< ECC_IRQEN_ECCSYSRAM1EN Mask */

 #define MXC_F_GCR_ECC_IRQEN_ECCSYSRAM2EN_POS           2 /**< ECC_IRQEN_ECCSYSRAM2EN Position */
 #define MXC_F_GCR_ECC_IRQEN_ECCSYSRAM2EN               ((uint32_t)(0x1UL << MXC_F_GCR_ECC_IRQEN_ECCSYSRAM2EN_POS)) /**< ECC_IRQEN_ECCSYSRAM2EN Mask */

 #define MXC_F_GCR_ECC_IRQEN_ECCSYSRAM3EN_POS           3 /**< ECC_IRQEN_ECCSYSRAM3EN Position */
 #define MXC_F_GCR_ECC_IRQEN_ECCSYSRAM3EN               ((uint32_t)(0x1UL << MXC_F_GCR_ECC_IRQEN_ECCSYSRAM3EN_POS)) /**< ECC_IRQEN_ECCSYSRAM3EN Mask */

 #define MXC_F_GCR_ECC_IRQEN_ECCSYSRAM4EN_POS           4 /**< ECC_IRQEN_ECCSYSRAM4EN Position */
 #define MXC_F_GCR_ECC_IRQEN_ECCSYSRAM4EN               ((uint32_t)(0x1UL << MXC_F_GCR_ECC_IRQEN_ECCSYSRAM4EN_POS)) /**< ECC_IRQEN_ECCSYSRAM4EN Mask */

 #define MXC_F_GCR_ECC_IRQEN_ECCSYSRAM5EN_POS           5 /**< ECC_IRQEN_ECCSYSRAM5EN Position */
 #define MXC_F_GCR_ECC_IRQEN_ECCSYSRAM5EN               ((uint32_t)(0x1UL << MXC_F_GCR_ECC_IRQEN_ECCSYSRAM5EN_POS)) /**< ECC_IRQEN_ECCSYSRAM5EN Mask */

 #define MXC_F_GCR_ECC_IRQEN_SYSRAM6ECCEN_POS           6 /**< ECC_IRQEN_SYSRAM6ECCEN Position */
 #define MXC_F_GCR_ECC_IRQEN_SYSRAM6ECCEN               ((uint32_t)(0x1UL << MXC_F_GCR_ECC_IRQEN_SYSRAM6ECCEN_POS)) /**< ECC_IRQEN_SYSRAM6ECCEN Mask */

 #define MXC_F_GCR_ECC_IRQEN_EC0ECCEN_POS               8 /**< ECC_IRQEN_EC0ECCEN Position */
 #define MXC_F_GCR_ECC_IRQEN_EC0ECCEN                   ((uint32_t)(0x1UL << MXC_F_GCR_ECC_IRQEN_EC0ECCEN_POS)) /**< ECC_IRQEN_EC0ECCEN Mask */

 #define MXC_F_GCR_ECC_IRQEN_EC1ECCEN_POS               9 /**< ECC_IRQEN_EC1ECCEN Position */
 #define MXC_F_GCR_ECC_IRQEN_EC1ECCEN                   ((uint32_t)(0x1UL << MXC_F_GCR_ECC_IRQEN_EC1ECCEN_POS)) /**< ECC_IRQEN_EC1ECCEN Mask */

 #define MXC_F_GCR_ECC_IRQEN_ICXIPECCEN_POS             10 /**< ECC_IRQEN_ICXIPECCEN Position */
 #define MXC_F_GCR_ECC_IRQEN_ICXIPECCEN                 ((uint32_t)(0x1UL << MXC_F_GCR_ECC_IRQEN_ICXIPECCEN_POS)) /**< ECC_IRQEN_ICXIPECCEN Mask */

 #define MXC_F_GCR_ECC_IRQEN_FL0ECCEN_POS               11 /**< ECC_IRQEN_FL0ECCEN Position */
 #define MXC_F_GCR_ECC_IRQEN_FL0ECCEN                   ((uint32_t)(0x1UL << MXC_F_GCR_ECC_IRQEN_FL0ECCEN_POS)) /**< ECC_IRQEN_FL0ECCEN Mask */

 #define MXC_F_GCR_ECC_IRQEN_FL1ECCEN_POS               12 /**< ECC_IRQEN_FL1ECCEN Position */
 #define MXC_F_GCR_ECC_IRQEN_FL1ECCEN                   ((uint32_t)(0x1UL << MXC_F_GCR_ECC_IRQEN_FL1ECCEN_POS)) /**< ECC_IRQEN_FL1ECCEN Mask */

/**@} end of group GCR_ECC_IRQEN_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_ECC_ERRAD GCR_ECC_ERRAD
 * @brief    ECC Error Address Register
 * @{
 */
 #define MXC_F_GCR_ECC_ERRAD_DATARAMADDR_POS            0 /**< ECC_ERRAD_DATARAMADDR Position */
 #define MXC_F_GCR_ECC_ERRAD_DATARAMADDR                ((uint32_t)(0x1FFFUL << MXC_F_GCR_ECC_ERRAD_DATARAMADDR_POS)) /**< ECC_ERRAD_DATARAMADDR Mask */

 #define MXC_F_GCR_ECC_ERRAD_DATARAMBANK_POS            14 /**< ECC_ERRAD_DATARAMBANK Position */
 #define MXC_F_GCR_ECC_ERRAD_DATARAMBANK                ((uint32_t)(0x1UL << MXC_F_GCR_ECC_ERRAD_DATARAMBANK_POS)) /**< ECC_ERRAD_DATARAMBANK Mask */

 #define MXC_F_GCR_ECC_ERRAD_DATARAMERR_POS             15 /**< ECC_ERRAD_DATARAMERR Position */
 #define MXC_F_GCR_ECC_ERRAD_DATARAMERR                 ((uint32_t)(0x1UL << MXC_F_GCR_ECC_ERRAD_DATARAMERR_POS)) /**< ECC_ERRAD_DATARAMERR Mask */

 #define MXC_F_GCR_ECC_ERRAD_TAGRAMADDR_POS             16 /**< ECC_ERRAD_TAGRAMADDR Position */
 #define MXC_F_GCR_ECC_ERRAD_TAGRAMADDR                 ((uint32_t)(0x1FFFUL << MXC_F_GCR_ECC_ERRAD_TAGRAMADDR_POS)) /**< ECC_ERRAD_TAGRAMADDR Mask */

 #define MXC_F_GCR_ECC_ERRAD_TAGRAMBANK_POS             30 /**< ECC_ERRAD_TAGRAMBANK Position */
 #define MXC_F_GCR_ECC_ERRAD_TAGRAMBANK                 ((uint32_t)(0x1UL << MXC_F_GCR_ECC_ERRAD_TAGRAMBANK_POS)) /**< ECC_ERRAD_TAGRAMBANK Mask */

 #define MXC_F_GCR_ECC_ERRAD_TAGRAMERR_POS              31 /**< ECC_ERRAD_TAGRAMERR Position */
 #define MXC_F_GCR_ECC_ERRAD_TAGRAMERR                  ((uint32_t)(0x1UL << MXC_F_GCR_ECC_ERRAD_TAGRAMERR_POS)) /**< ECC_ERRAD_TAGRAMERR Mask */

/**@} end of group GCR_ECC_ERRAD_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_BTLE_LDOCR GCR_BTLE_LDOCR
 * @brief    BTLE LDO Control Register
 * @{
 */
 #define MXC_F_GCR_BTLE_LDOCR_LDOTXEN_POS               0 /**< BTLE_LDOCR_LDOTXEN Position */
 #define MXC_F_GCR_BTLE_LDOCR_LDOTXEN                   ((uint32_t)(0x1UL << MXC_F_GCR_BTLE_LDOCR_LDOTXEN_POS)) /**< BTLE_LDOCR_LDOTXEN Mask */

 #define MXC_F_GCR_BTLE_LDOCR_LDOTXOPULLD_POS           1 /**< BTLE_LDOCR_LDOTXOPULLD Position */
 #define MXC_F_GCR_BTLE_LDOCR_LDOTXOPULLD               ((uint32_t)(0x1UL << MXC_F_GCR_BTLE_LDOCR_LDOTXOPULLD_POS)) /**< BTLE_LDOCR_LDOTXOPULLD Mask */

 #define MXC_F_GCR_BTLE_LDOCR_LDOTXVSEL_POS             2 /**< BTLE_LDOCR_LDOTXVSEL Position */
 #define MXC_F_GCR_BTLE_LDOCR_LDOTXVSEL                 ((uint32_t)(0x3UL << MXC_F_GCR_BTLE_LDOCR_LDOTXVSEL_POS)) /**< BTLE_LDOCR_LDOTXVSEL Mask */
 #define MXC_V_GCR_BTLE_LDOCR_LDOTXVSEL_0_7             ((uint32_t)0x0UL) /**< BTLE_LDOCR_LDOTXVSEL_0_7 Value */
 #define MXC_S_GCR_BTLE_LDOCR_LDOTXVSEL_0_7             (MXC_V_GCR_BTLE_LDOCR_LDOTXVSEL_0_7 << MXC_F_GCR_BTLE_LDOCR_LDOTXVSEL_POS) /**< BTLE_LDOCR_LDOTXVSEL_0_7 Setting */
 #define MXC_V_GCR_BTLE_LDOCR_LDOTXVSEL_0_85            ((uint32_t)0x1UL) /**< BTLE_LDOCR_LDOTXVSEL_0_85 Value */
 #define MXC_S_GCR_BTLE_LDOCR_LDOTXVSEL_0_85            (MXC_V_GCR_BTLE_LDOCR_LDOTXVSEL_0_85 << MXC_F_GCR_BTLE_LDOCR_LDOTXVSEL_POS) /**< BTLE_LDOCR_LDOTXVSEL_0_85 Setting */
 #define MXC_V_GCR_BTLE_LDOCR_LDOTXVSEL_0_9             ((uint32_t)0x2UL) /**< BTLE_LDOCR_LDOTXVSEL_0_9 Value */
 #define MXC_S_GCR_BTLE_LDOCR_LDOTXVSEL_0_9             (MXC_V_GCR_BTLE_LDOCR_LDOTXVSEL_0_9 << MXC_F_GCR_BTLE_LDOCR_LDOTXVSEL_POS) /**< BTLE_LDOCR_LDOTXVSEL_0_9 Setting */
 #define MXC_V_GCR_BTLE_LDOCR_LDOTXVSEL_1_1             ((uint32_t)0x3UL) /**< BTLE_LDOCR_LDOTXVSEL_1_1 Value */
 #define MXC_S_GCR_BTLE_LDOCR_LDOTXVSEL_1_1             (MXC_V_GCR_BTLE_LDOCR_LDOTXVSEL_1_1 << MXC_F_GCR_BTLE_LDOCR_LDOTXVSEL_POS) /**< BTLE_LDOCR_LDOTXVSEL_1_1 Setting */

 #define MXC_F_GCR_BTLE_LDOCR_LDORXEN_POS               4 /**< BTLE_LDOCR_LDORXEN Position */
 #define MXC_F_GCR_BTLE_LDOCR_LDORXEN                   ((uint32_t)(0x1UL << MXC_F_GCR_BTLE_LDOCR_LDORXEN_POS)) /**< BTLE_LDOCR_LDORXEN Mask */

 #define MXC_F_GCR_BTLE_LDOCR_LDORXPULLD_POS            5 /**< BTLE_LDOCR_LDORXPULLD Position */
 #define MXC_F_GCR_BTLE_LDOCR_LDORXPULLD                ((uint32_t)(0x1UL << MXC_F_GCR_BTLE_LDOCR_LDORXPULLD_POS)) /**< BTLE_LDOCR_LDORXPULLD Mask */

 #define MXC_F_GCR_BTLE_LDOCR_LDORXVSEL_POS             6 /**< BTLE_LDOCR_LDORXVSEL Position */
 #define MXC_F_GCR_BTLE_LDOCR_LDORXVSEL                 ((uint32_t)(0x3UL << MXC_F_GCR_BTLE_LDOCR_LDORXVSEL_POS)) /**< BTLE_LDOCR_LDORXVSEL Mask */
 #define MXC_V_GCR_BTLE_LDOCR_LDORXVSEL_0_7             ((uint32_t)0x0UL) /**< BTLE_LDOCR_LDORXVSEL_0_7 Value */
 #define MXC_S_GCR_BTLE_LDOCR_LDORXVSEL_0_7             (MXC_V_GCR_BTLE_LDOCR_LDORXVSEL_0_7 << MXC_F_GCR_BTLE_LDOCR_LDORXVSEL_POS) /**< BTLE_LDOCR_LDORXVSEL_0_7 Setting */
 #define MXC_V_GCR_BTLE_LDOCR_LDORXVSEL_0_85            ((uint32_t)0x1UL) /**< BTLE_LDOCR_LDORXVSEL_0_85 Value */
 #define MXC_S_GCR_BTLE_LDOCR_LDORXVSEL_0_85            (MXC_V_GCR_BTLE_LDOCR_LDORXVSEL_0_85 << MXC_F_GCR_BTLE_LDOCR_LDORXVSEL_POS) /**< BTLE_LDOCR_LDORXVSEL_0_85 Setting */
 #define MXC_V_GCR_BTLE_LDOCR_LDORXVSEL_0_9             ((uint32_t)0x2UL) /**< BTLE_LDOCR_LDORXVSEL_0_9 Value */
 #define MXC_S_GCR_BTLE_LDOCR_LDORXVSEL_0_9             (MXC_V_GCR_BTLE_LDOCR_LDORXVSEL_0_9 << MXC_F_GCR_BTLE_LDOCR_LDORXVSEL_POS) /**< BTLE_LDOCR_LDORXVSEL_0_9 Setting */
 #define MXC_V_GCR_BTLE_LDOCR_LDORXVSEL_1_1             ((uint32_t)0x3UL) /**< BTLE_LDOCR_LDORXVSEL_1_1 Value */
 #define MXC_S_GCR_BTLE_LDOCR_LDORXVSEL_1_1             (MXC_V_GCR_BTLE_LDOCR_LDORXVSEL_1_1 << MXC_F_GCR_BTLE_LDOCR_LDORXVSEL_POS) /**< BTLE_LDOCR_LDORXVSEL_1_1 Setting */

 #define MXC_F_GCR_BTLE_LDOCR_LDORXBYP_POS              8 /**< BTLE_LDOCR_LDORXBYP Position */
 #define MXC_F_GCR_BTLE_LDOCR_LDORXBYP                  ((uint32_t)(0x1UL << MXC_F_GCR_BTLE_LDOCR_LDORXBYP_POS)) /**< BTLE_LDOCR_LDORXBYP Mask */

 #define MXC_F_GCR_BTLE_LDOCR_LDORXDISCH_POS            9 /**< BTLE_LDOCR_LDORXDISCH Position */
 #define MXC_F_GCR_BTLE_LDOCR_LDORXDISCH                ((uint32_t)(0x1UL << MXC_F_GCR_BTLE_LDOCR_LDORXDISCH_POS)) /**< BTLE_LDOCR_LDORXDISCH Mask */

 #define MXC_F_GCR_BTLE_LDOCR_LDOTXBYP_POS              10 /**< BTLE_LDOCR_LDOTXBYP Position */
 #define MXC_F_GCR_BTLE_LDOCR_LDOTXBYP                  ((uint32_t)(0x1UL << MXC_F_GCR_BTLE_LDOCR_LDOTXBYP_POS)) /**< BTLE_LDOCR_LDOTXBYP Mask */

 #define MXC_F_GCR_BTLE_LDOCR_LDOTXDISCH_POS            11 /**< BTLE_LDOCR_LDOTXDISCH Position */
 #define MXC_F_GCR_BTLE_LDOCR_LDOTXDISCH                ((uint32_t)(0x1UL << MXC_F_GCR_BTLE_LDOCR_LDOTXDISCH_POS)) /**< BTLE_LDOCR_LDOTXDISCH Mask */

 #define MXC_F_GCR_BTLE_LDOCR_LDOTXENDLY_POS            12 /**< BTLE_LDOCR_LDOTXENDLY Position */
 #define MXC_F_GCR_BTLE_LDOCR_LDOTXENDLY                ((uint32_t)(0x1UL << MXC_F_GCR_BTLE_LDOCR_LDOTXENDLY_POS)) /**< BTLE_LDOCR_LDOTXENDLY Mask */

 #define MXC_F_GCR_BTLE_LDOCR_LDORXENDLY_POS            13 /**< BTLE_LDOCR_LDORXENDLY Position */
 #define MXC_F_GCR_BTLE_LDOCR_LDORXENDLY                ((uint32_t)(0x1UL << MXC_F_GCR_BTLE_LDOCR_LDORXENDLY_POS)) /**< BTLE_LDOCR_LDORXENDLY Mask */

/**@} end of group GCR_BTLE_LDOCR_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_BTLE_LDODCR GCR_BTLE_LDODCR
 * @brief    BTLE LDO Delay Register
 * @{
 */
 #define MXC_F_GCR_BTLE_LDODCR_BYPDLYCNT_POS            0 /**< BTLE_LDODCR_BYPDLYCNT Position */
 #define MXC_F_GCR_BTLE_LDODCR_BYPDLYCNT                ((uint32_t)(0xFFUL << MXC_F_GCR_BTLE_LDODCR_BYPDLYCNT_POS)) /**< BTLE_LDODCR_BYPDLYCNT Mask */

 #define MXC_F_GCR_BTLE_LDODCR_LDORXDLYCNT_POS          8 /**< BTLE_LDODCR_LDORXDLYCNT Position */
 #define MXC_F_GCR_BTLE_LDODCR_LDORXDLYCNT              ((uint32_t)(0x1FFUL << MXC_F_GCR_BTLE_LDODCR_LDORXDLYCNT_POS)) /**< BTLE_LDODCR_LDORXDLYCNT Mask */

 #define MXC_F_GCR_BTLE_LDODCR_LDOTXDLYCNT_POS          20 /**< BTLE_LDODCR_LDOTXDLYCNT Position */
 #define MXC_F_GCR_BTLE_LDODCR_LDOTXDLYCNT              ((uint32_t)(0x1FFUL << MXC_F_GCR_BTLE_LDODCR_LDOTXDLYCNT_POS)) /**< BTLE_LDODCR_LDOTXDLYCNT Mask */

/**@} end of group GCR_BTLE_LDODCR_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_APB_ASYNC GCR_APB_ASYNC
 * @brief    APB Asynchronous Bridge Select Register
 * @{
 */
 #define MXC_F_GCR_APB_ASYNC_APBASYNCI2C0_POS           0 /**< APB_ASYNC_APBASYNCI2C0 Position */
 #define MXC_F_GCR_APB_ASYNC_APBASYNCI2C0               ((uint32_t)(0x1UL << MXC_F_GCR_APB_ASYNC_APBASYNCI2C0_POS)) /**< APB_ASYNC_APBASYNCI2C0 Mask */

 #define MXC_F_GCR_APB_ASYNC_APBASYNCI2C1_POS           1 /**< APB_ASYNC_APBASYNCI2C1 Position */
 #define MXC_F_GCR_APB_ASYNC_APBASYNCI2C1               ((uint32_t)(0x1UL << MXC_F_GCR_APB_ASYNC_APBASYNCI2C1_POS)) /**< APB_ASYNC_APBASYNCI2C1 Mask */

 #define MXC_F_GCR_APB_ASYNC_APBASYNCI2C2_POS           2 /**< APB_ASYNC_APBASYNCI2C2 Position */
 #define MXC_F_GCR_APB_ASYNC_APBASYNCI2C2               ((uint32_t)(0x1UL << MXC_F_GCR_APB_ASYNC_APBASYNCI2C2_POS)) /**< APB_ASYNC_APBASYNCI2C2 Mask */

 #define MXC_F_GCR_APB_ASYNC_APBASYNCPT_POS             3 /**< APB_ASYNC_APBASYNCPT Position */
 #define MXC_F_GCR_APB_ASYNC_APBASYNCPT                 ((uint32_t)(0x1UL << MXC_F_GCR_APB_ASYNC_APBASYNCPT_POS)) /**< APB_ASYNC_APBASYNCPT Mask */

/**@} end of group GCR_APB_ASYNC_Register */

#ifdef __cplusplus
}
#endif

#endif /* _GCR_REGS_H_ */
