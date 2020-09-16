/**
 * @file    pwrseq_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the PWRSEQ Peripheral Module.
 */

/* ****************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
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

#ifndef _PWRSEQ_REGS_H_
#define _PWRSEQ_REGS_H_

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
 * @ingroup     pwrseq
 * @defgroup    pwrseq_registers PWRSEQ_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the PWRSEQ Peripheral Module.
 * @details Power Sequencer / Low Power Control Register.
 */

/**
 * @ingroup pwrseq_registers
 * Structure type to access the PWRSEQ Registers.
 */
typedef struct {
    __IO uint32_t lpcn;                 /**< <tt>\b 0x00:</tt> PWRSEQ LPCN Register */
    __IO uint32_t lpwkst0;              /**< <tt>\b 0x04:</tt> PWRSEQ LPWKST0 Register */
    __IO uint32_t lpwken0;              /**< <tt>\b 0x08:</tt> PWRSEQ LPWKEN0 Register */
    __IO uint32_t lpwkst1;              /**< <tt>\b 0x0C:</tt> PWRSEQ LPWKST1 Register */
    __IO uint32_t lpwken1;              /**< <tt>\b 0x10:</tt> PWRSEQ LPWKEN1 Register */
    __R  uint32_t rsv_0x14_0x2f[7];
    __IO uint32_t lppwst;               /**< <tt>\b 0x30:</tt> PWRSEQ LPPWST Register */
    __IO uint32_t lppwen;               /**< <tt>\b 0x34:</tt> PWRSEQ LPPWEN Register */
    __R  uint32_t rsv_0x38_0x3f[2];
    __IO uint32_t lpmemsd;              /**< <tt>\b 0x40:</tt> PWRSEQ LPMEMSD Register */
    __R  uint32_t rsv_0x44;
    __IO uint32_t gp0;                  /**< <tt>\b 0x48:</tt> PWRSEQ GP0 Register */
    __IO uint32_t gp1;                  /**< <tt>\b 0x4C:</tt> PWRSEQ GP1 Register */
} mxc_pwrseq_regs_t;

/* Register offsets for module PWRSEQ */
/**
 * @ingroup    pwrseq_registers
 * @defgroup   PWRSEQ_Register_Offsets Register Offsets
 * @brief      PWRSEQ Peripheral Register Offsets from the PWRSEQ Base Peripheral Address. 
 * @{
 */
 #define MXC_R_PWRSEQ_LPCN                  ((uint32_t)0x00000000UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_PWRSEQ_LPWKST0               ((uint32_t)0x00000004UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_PWRSEQ_LPWKEN0               ((uint32_t)0x00000008UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_PWRSEQ_LPWKST1               ((uint32_t)0x0000000CUL) /**< Offset from PWRSEQ Base Address: <tt> 0x000C</tt> */ 
 #define MXC_R_PWRSEQ_LPWKEN1               ((uint32_t)0x00000010UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0010</tt> */ 
 #define MXC_R_PWRSEQ_LPPWST                ((uint32_t)0x00000030UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0030</tt> */ 
 #define MXC_R_PWRSEQ_LPPWEN                ((uint32_t)0x00000034UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0034</tt> */ 
 #define MXC_R_PWRSEQ_LPMEMSD               ((uint32_t)0x00000040UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0040</tt> */ 
 #define MXC_R_PWRSEQ_GP0                   ((uint32_t)0x00000048UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0048</tt> */ 
 #define MXC_R_PWRSEQ_GP1                   ((uint32_t)0x0000004CUL) /**< Offset from PWRSEQ Base Address: <tt> 0x004C</tt> */ 
/**@} end of group pwrseq_registers */

/**
 * @ingroup  pwrseq_registers
 * @defgroup PWRSEQ_LPCN PWRSEQ_LPCN
 * @brief    Low Power Control Register.
 * @{
 */
 #define MXC_F_PWRSEQ_LPCN_RAMRET0_POS                  0 /**< LPCN_RAMRET0 Position */
 #define MXC_F_PWRSEQ_LPCN_RAMRET0                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_RAMRET0_POS)) /**< LPCN_RAMRET0 Mask */

 #define MXC_F_PWRSEQ_LPCN_RAMRET1_POS                  1 /**< LPCN_RAMRET1 Position */
 #define MXC_F_PWRSEQ_LPCN_RAMRET1                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_RAMRET1_POS)) /**< LPCN_RAMRET1 Mask */

 #define MXC_F_PWRSEQ_LPCN_RAMRET2_POS                  2 /**< LPCN_RAMRET2 Position */
 #define MXC_F_PWRSEQ_LPCN_RAMRET2                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_RAMRET2_POS)) /**< LPCN_RAMRET2 Mask */

 #define MXC_F_PWRSEQ_LPCN_RAMRET3_POS                  3 /**< LPCN_RAMRET3 Position */
 #define MXC_F_PWRSEQ_LPCN_RAMRET3                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_RAMRET3_POS)) /**< LPCN_RAMRET3 Mask */

 #define MXC_F_PWRSEQ_LPCN_OVR_POS                      4 /**< LPCN_OVR Position */
 #define MXC_F_PWRSEQ_LPCN_OVR                          ((uint32_t)(0x3UL << MXC_F_PWRSEQ_LPCN_OVR_POS)) /**< LPCN_OVR Mask */
 #define MXC_V_PWRSEQ_LPCN_OVR_0_9V                     ((uint32_t)0x0UL) /**< LPCN_OVR_0_9V Value */
 #define MXC_S_PWRSEQ_LPCN_OVR_0_9V                     (MXC_V_PWRSEQ_LPCN_OVR_0_9V << MXC_F_PWRSEQ_LPCN_OVR_POS) /**< LPCN_OVR_0_9V Setting */
 #define MXC_V_PWRSEQ_LPCN_OVR_1_0V                     ((uint32_t)0x1UL) /**< LPCN_OVR_1_0V Value */
 #define MXC_S_PWRSEQ_LPCN_OVR_1_0V                     (MXC_V_PWRSEQ_LPCN_OVR_1_0V << MXC_F_PWRSEQ_LPCN_OVR_POS) /**< LPCN_OVR_1_0V Setting */
 #define MXC_V_PWRSEQ_LPCN_OVR_1_1V                     ((uint32_t)0x2UL) /**< LPCN_OVR_1_1V Value */
 #define MXC_S_PWRSEQ_LPCN_OVR_1_1V                     (MXC_V_PWRSEQ_LPCN_OVR_1_1V << MXC_F_PWRSEQ_LPCN_OVR_POS) /**< LPCN_OVR_1_1V Setting */

 #define MXC_F_PWRSEQ_LPCN_BLKDET_POS                   6 /**< LPCN_BLKDET Position */
 #define MXC_F_PWRSEQ_LPCN_BLKDET                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_BLKDET_POS)) /**< LPCN_BLKDET Mask */

 #define MXC_F_PWRSEQ_LPCN_FVDDEN_POS                   7 /**< LPCN_FVDDEN Position */
 #define MXC_F_PWRSEQ_LPCN_FVDDEN                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_FVDDEN_POS)) /**< LPCN_FVDDEN Mask */

 #define MXC_F_PWRSEQ_LPCN_RREGEN_POS                   8 /**< LPCN_RREGEN Position */
 #define MXC_F_PWRSEQ_LPCN_RREGEN                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_RREGEN_POS)) /**< LPCN_RREGEN Mask */

 #define MXC_F_PWRSEQ_LPCN_STORAGE_POS                  9 /**< LPCN_STORAGE Position */
 #define MXC_F_PWRSEQ_LPCN_STORAGE                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_STORAGE_POS)) /**< LPCN_STORAGE Mask */

 #define MXC_F_PWRSEQ_LPCN_FWKM_POS                     10 /**< LPCN_FWKM Position */
 #define MXC_F_PWRSEQ_LPCN_FWKM                         ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_FWKM_POS)) /**< LPCN_FWKM Mask */

 #define MXC_F_PWRSEQ_LPCN_BGOFF_POS                    11 /**< LPCN_BGOFF Position */
 #define MXC_F_PWRSEQ_LPCN_BGOFF                        ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_BGOFF_POS)) /**< LPCN_BGOFF Mask */

 #define MXC_F_PWRSEQ_LPCN_PORVCOREMD_POS               12 /**< LPCN_PORVCOREMD Position */
 #define MXC_F_PWRSEQ_LPCN_PORVCOREMD                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_PORVCOREMD_POS)) /**< LPCN_PORVCOREMD Mask */

 #define MXC_F_PWRSEQ_LPCN_LDO_DIS_POS                  16 /**< LPCN_LDO_DIS Position */
 #define MXC_F_PWRSEQ_LPCN_LDO_DIS                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_LDO_DIS_POS)) /**< LPCN_LDO_DIS Mask */

 #define MXC_F_PWRSEQ_LPCN_VCORE_EXT_POS                17 /**< LPCN_VCORE_EXT Position */
 #define MXC_F_PWRSEQ_LPCN_VCORE_EXT                    ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_VCORE_EXT_POS)) /**< LPCN_VCORE_EXT Mask */

 #define MXC_F_PWRSEQ_LPCN_VCOREMD_POS                  20 /**< LPCN_VCOREMD Position */
 #define MXC_F_PWRSEQ_LPCN_VCOREMD                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_VCOREMD_POS)) /**< LPCN_VCOREMD Mask */

 #define MXC_F_PWRSEQ_LPCN_VDDAMD_POS                   22 /**< LPCN_VDDAMD Position */
 #define MXC_F_PWRSEQ_LPCN_VDDAMD                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_VDDAMD_POS)) /**< LPCN_VDDAMD Mask */

 #define MXC_F_PWRSEQ_LPCN_VDDIOMD_POS                  23 /**< LPCN_VDDIOMD Position */
 #define MXC_F_PWRSEQ_LPCN_VDDIOMD                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_VDDIOMD_POS)) /**< LPCN_VDDIOMD Mask */

 #define MXC_F_PWRSEQ_LPCN_VDDIOHMD_POS                 24 /**< LPCN_VDDIOHMD Position */
 #define MXC_F_PWRSEQ_LPCN_VDDIOHMD                     ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_VDDIOHMD_POS)) /**< LPCN_VDDIOHMD Mask */

 #define MXC_F_PWRSEQ_LPCN_PORVDDIOMD_POS               25 /**< LPCN_PORVDDIOMD Position */
 #define MXC_F_PWRSEQ_LPCN_PORVDDIOMD                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_PORVDDIOMD_POS)) /**< LPCN_PORVDDIOMD Mask */

 #define MXC_F_PWRSEQ_LPCN_PORVDDIOHMD_POS              26 /**< LPCN_PORVDDIOHMD Position */
 #define MXC_F_PWRSEQ_LPCN_PORVDDIOHMD                  ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_PORVDDIOHMD_POS)) /**< LPCN_PORVDDIOHMD Mask */

 #define MXC_F_PWRSEQ_LPCN_VDDBMD_POS                   27 /**< LPCN_VDDBMD Position */
 #define MXC_F_PWRSEQ_LPCN_VDDBMD                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_VDDBMD_POS)) /**< LPCN_VDDBMD Mask */

 #define MXC_F_PWRSEQ_LPCN_INROEN_POS                   28 /**< LPCN_INROEN Position */
 #define MXC_F_PWRSEQ_LPCN_INROEN                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_INROEN_POS)) /**< LPCN_INROEN Mask */

 #define MXC_F_PWRSEQ_LPCN_XRTCOEN_POS                  29 /**< LPCN_XRTCOEN Position */
 #define MXC_F_PWRSEQ_LPCN_XRTCOEN                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_XRTCOEN_POS)) /**< LPCN_XRTCOEN Mask */

 #define MXC_F_PWRSEQ_LPCN_TM_LPMODE_POS                30 /**< LPCN_TM_LPMODE Position */
 #define MXC_F_PWRSEQ_LPCN_TM_LPMODE                    ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_TM_LPMODE_POS)) /**< LPCN_TM_LPMODE Mask */

 #define MXC_F_PWRSEQ_LPCN_TM_PWRSEQ_POS                31 /**< LPCN_TM_PWRSEQ Position */
 #define MXC_F_PWRSEQ_LPCN_TM_PWRSEQ                    ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_TM_PWRSEQ_POS)) /**< LPCN_TM_PWRSEQ Mask */

/**@} end of group PWRSEQ_LPCN_Register */

/**
 * @ingroup  pwrseq_registers
 * @defgroup PWRSEQ_LPWKST0 PWRSEQ_LPWKST0
 * @brief    Low Power I/O Wakeup Status Register 0. This register indicates the low power
 *           wakeup status for GPIO0.
 * @{
 */
 #define MXC_F_PWRSEQ_LPWKST0_WAKEST_POS                0 /**< LPWKST0_WAKEST Position */
 #define MXC_F_PWRSEQ_LPWKST0_WAKEST                    ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPWKST0_WAKEST_POS)) /**< LPWKST0_WAKEST Mask */

/**@} end of group PWRSEQ_LPWKST0_Register */

/**
 * @ingroup  pwrseq_registers
 * @defgroup PWRSEQ_LPWKEN0 PWRSEQ_LPWKEN0
 * @brief    Low Power I/O Wakeup Enable Register 0. This register enables low power wakeup
 *           functionality for GPIO0.
 * @{
 */
 #define MXC_F_PWRSEQ_LPWKEN0_WAKEEN_POS                0 /**< LPWKEN0_WAKEEN Position */
 #define MXC_F_PWRSEQ_LPWKEN0_WAKEEN                    ((uint32_t)(0x7FFFFFFFUL << MXC_F_PWRSEQ_LPWKEN0_WAKEEN_POS)) /**< LPWKEN0_WAKEEN Mask */

/**@} end of group PWRSEQ_LPWKEN0_Register */

/**
 * @ingroup  pwrseq_registers
 * @defgroup PWRSEQ_LPPWST PWRSEQ_LPPWST
 * @brief    Low Power Peripheral Wakeup Status Register.
 * @{
 */
 #define MXC_F_PWRSEQ_LPPWST_AINCOMP0_POS               4 /**< LPPWST_AINCOMP0 Position */
 #define MXC_F_PWRSEQ_LPPWST_AINCOMP0                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWST_AINCOMP0_POS)) /**< LPPWST_AINCOMP0 Mask */

 #define MXC_F_PWRSEQ_LPPWST_BACKUP_POS                 16 /**< LPPWST_BACKUP Position */
 #define MXC_F_PWRSEQ_LPPWST_BACKUP                     ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWST_BACKUP_POS)) /**< LPPWST_BACKUP Mask */

 #define MXC_F_PWRSEQ_LPPWST_RESET_POS                  17 /**< LPPWST_RESET Position */
 #define MXC_F_PWRSEQ_LPPWST_RESET                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWST_RESET_POS)) /**< LPPWST_RESET Mask */

/**@} end of group PWRSEQ_LPPWST_Register */

/**
 * @ingroup  pwrseq_registers
 * @defgroup PWRSEQ_LPPWEN PWRSEQ_LPPWEN
 * @brief    Low Power Peripheral Wakeup Enable Register.
 * @{
 */
 #define MXC_F_PWRSEQ_LPPWEN_AINCOMP0_POS               4 /**< LPPWEN_AINCOMP0 Position */
 #define MXC_F_PWRSEQ_LPPWEN_AINCOMP0                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWEN_AINCOMP0_POS)) /**< LPPWEN_AINCOMP0 Mask */

 #define MXC_F_PWRSEQ_LPPWEN_WDT0_POS                   8 /**< LPPWEN_WDT0 Position */
 #define MXC_F_PWRSEQ_LPPWEN_WDT0                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWEN_WDT0_POS)) /**< LPPWEN_WDT0 Mask */

 #define MXC_F_PWRSEQ_LPPWEN_WDT1_POS                   9 /**< LPPWEN_WDT1 Position */
 #define MXC_F_PWRSEQ_LPPWEN_WDT1                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWEN_WDT1_POS)) /**< LPPWEN_WDT1 Mask */

 #define MXC_F_PWRSEQ_LPPWEN_CPU1_POS                   10 /**< LPPWEN_CPU1 Position */
 #define MXC_F_PWRSEQ_LPPWEN_CPU1                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWEN_CPU1_POS)) /**< LPPWEN_CPU1 Mask */

 #define MXC_F_PWRSEQ_LPPWEN_TMR0_POS                   11 /**< LPPWEN_TMR0 Position */
 #define MXC_F_PWRSEQ_LPPWEN_TMR0                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWEN_TMR0_POS)) /**< LPPWEN_TMR0 Mask */

 #define MXC_F_PWRSEQ_LPPWEN_TMR1_POS                   12 /**< LPPWEN_TMR1 Position */
 #define MXC_F_PWRSEQ_LPPWEN_TMR1                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWEN_TMR1_POS)) /**< LPPWEN_TMR1 Mask */

 #define MXC_F_PWRSEQ_LPPWEN_TMR2_POS                   13 /**< LPPWEN_TMR2 Position */
 #define MXC_F_PWRSEQ_LPPWEN_TMR2                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWEN_TMR2_POS)) /**< LPPWEN_TMR2 Mask */

 #define MXC_F_PWRSEQ_LPPWEN_TMR3_POS                   14 /**< LPPWEN_TMR3 Position */
 #define MXC_F_PWRSEQ_LPPWEN_TMR3                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWEN_TMR3_POS)) /**< LPPWEN_TMR3 Mask */

 #define MXC_F_PWRSEQ_LPPWEN_TMR4_POS                   15 /**< LPPWEN_TMR4 Position */
 #define MXC_F_PWRSEQ_LPPWEN_TMR4                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWEN_TMR4_POS)) /**< LPPWEN_TMR4 Mask */

 #define MXC_F_PWRSEQ_LPPWEN_TMR5_POS                   16 /**< LPPWEN_TMR5 Position */
 #define MXC_F_PWRSEQ_LPPWEN_TMR5                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWEN_TMR5_POS)) /**< LPPWEN_TMR5 Mask */

 #define MXC_F_PWRSEQ_LPPWEN_UART0_POS                  17 /**< LPPWEN_UART0 Position */
 #define MXC_F_PWRSEQ_LPPWEN_UART0                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWEN_UART0_POS)) /**< LPPWEN_UART0 Mask */

 #define MXC_F_PWRSEQ_LPPWEN_UART1_POS                  18 /**< LPPWEN_UART1 Position */
 #define MXC_F_PWRSEQ_LPPWEN_UART1                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWEN_UART1_POS)) /**< LPPWEN_UART1 Mask */

 #define MXC_F_PWRSEQ_LPPWEN_UART2_POS                  19 /**< LPPWEN_UART2 Position */
 #define MXC_F_PWRSEQ_LPPWEN_UART2                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWEN_UART2_POS)) /**< LPPWEN_UART2 Mask */

 #define MXC_F_PWRSEQ_LPPWEN_UART3_POS                  20 /**< LPPWEN_UART3 Position */
 #define MXC_F_PWRSEQ_LPPWEN_UART3                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWEN_UART3_POS)) /**< LPPWEN_UART3 Mask */

 #define MXC_F_PWRSEQ_LPPWEN_I2C0_POS                   21 /**< LPPWEN_I2C0 Position */
 #define MXC_F_PWRSEQ_LPPWEN_I2C0                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWEN_I2C0_POS)) /**< LPPWEN_I2C0 Mask */

 #define MXC_F_PWRSEQ_LPPWEN_I2C1_POS                   22 /**< LPPWEN_I2C1 Position */
 #define MXC_F_PWRSEQ_LPPWEN_I2C1                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWEN_I2C1_POS)) /**< LPPWEN_I2C1 Mask */

 #define MXC_F_PWRSEQ_LPPWEN_I2C2_POS                   23 /**< LPPWEN_I2C2 Position */
 #define MXC_F_PWRSEQ_LPPWEN_I2C2                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWEN_I2C2_POS)) /**< LPPWEN_I2C2 Mask */

 #define MXC_F_PWRSEQ_LPPWEN_I2S_POS                    24 /**< LPPWEN_I2S Position */
 #define MXC_F_PWRSEQ_LPPWEN_I2S                        ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWEN_I2S_POS)) /**< LPPWEN_I2S Mask */

 #define MXC_F_PWRSEQ_LPPWEN_SPI0_POS                   25 /**< LPPWEN_SPI0 Position */
 #define MXC_F_PWRSEQ_LPPWEN_SPI0                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWEN_SPI0_POS)) /**< LPPWEN_SPI0 Mask */

 #define MXC_F_PWRSEQ_LPPWEN_LPCMP_POS                  26 /**< LPPWEN_LPCMP Position */
 #define MXC_F_PWRSEQ_LPPWEN_LPCMP                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWEN_LPCMP_POS)) /**< LPPWEN_LPCMP Mask */

/**@} end of group PWRSEQ_LPPWEN_Register */

/**
 * @ingroup  pwrseq_registers
 * @defgroup PWRSEQ_LPMEMSD PWRSEQ_LPMEMSD
 * @brief    Low Power Memory Shutdown Control.
 * @{
 */
 #define MXC_F_PWRSEQ_LPMEMSD_SRAM0SD_POS               0 /**< LPMEMSD_SRAM0SD Position */
 #define MXC_F_PWRSEQ_LPMEMSD_SRAM0SD                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_SRAM0SD_POS)) /**< LPMEMSD_SRAM0SD Mask */

 #define MXC_F_PWRSEQ_LPMEMSD_SRAM1SD_POS               1 /**< LPMEMSD_SRAM1SD Position */
 #define MXC_F_PWRSEQ_LPMEMSD_SRAM1SD                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_SRAM1SD_POS)) /**< LPMEMSD_SRAM1SD Mask */

 #define MXC_F_PWRSEQ_LPMEMSD_SRAM2SD_POS               2 /**< LPMEMSD_SRAM2SD Position */
 #define MXC_F_PWRSEQ_LPMEMSD_SRAM2SD                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_SRAM2SD_POS)) /**< LPMEMSD_SRAM2SD Mask */

 #define MXC_F_PWRSEQ_LPMEMSD_SRAM3SD_POS               3 /**< LPMEMSD_SRAM3SD Position */
 #define MXC_F_PWRSEQ_LPMEMSD_SRAM3SD                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_SRAM3SD_POS)) /**< LPMEMSD_SRAM3SD Mask */

/**@} end of group PWRSEQ_LPMEMSD_Register */

#ifdef __cplusplus
}
#endif

#endif /* _PWRSEQ_REGS_H_ */
