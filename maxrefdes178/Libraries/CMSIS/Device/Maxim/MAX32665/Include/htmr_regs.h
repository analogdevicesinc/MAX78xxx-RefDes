/**
 * @file    htmr_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the HTMR Peripheral Module.
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

#ifndef _HTMR_REGS_H_
#define _HTMR_REGS_H_

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
 * @ingroup     htmr
 * @defgroup    htmr_registers HTMR_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the HTMR Peripheral Module.
 * @details High Speed Timer Module.
 */

/**
 * @ingroup htmr_registers
 * Structure type to access the HTMR Registers.
 */
typedef struct {
    __IO uint32_t sec;                  /**< <tt>\b 0x00:</tt> HTMR SEC Register */
    __IO uint32_t ssec;                 /**< <tt>\b 0x04:</tt> HTMR SSEC Register */
    __IO uint32_t ras;                  /**< <tt>\b 0x08:</tt> HTMR RAS Register */
    __IO uint32_t rssa;                 /**< <tt>\b 0x0C:</tt> HTMR RSSA Register */
    __IO uint32_t ctrl;                 /**< <tt>\b 0x10:</tt> HTMR CTRL Register */
} mxc_htmr_regs_t;

/* Register offsets for module HTMR */
/**
 * @ingroup    htmr_registers
 * @defgroup   HTMR_Register_Offsets Register Offsets
 * @brief      HTMR Peripheral Register Offsets from the HTMR Base Peripheral Address. 
 * @{
 */
 #define MXC_R_HTMR_SEC                     ((uint32_t)0x00000000UL) /**< Offset from HTMR Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_HTMR_SSEC                    ((uint32_t)0x00000004UL) /**< Offset from HTMR Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_HTMR_RAS                     ((uint32_t)0x00000008UL) /**< Offset from HTMR Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_HTMR_RSSA                    ((uint32_t)0x0000000CUL) /**< Offset from HTMR Base Address: <tt> 0x000C</tt> */ 
 #define MXC_R_HTMR_CTRL                    ((uint32_t)0x00000010UL) /**< Offset from HTMR Base Address: <tt> 0x0010</tt> */ 
/**@} end of group htmr_registers */

/**
 * @ingroup  htmr_registers
 * @defgroup HTMR_SEC HTMR_SEC
 * @brief    HTimer Long-Interval Counter. This register contains the 32 most significant
 *           bits of the counter.
 * @{
 */
 #define MXC_F_HTMR_SEC_RTS_POS                         0 /**< SEC_RTS Position */
 #define MXC_F_HTMR_SEC_RTS                             ((uint32_t)(0x7FFFFFFFUL << MXC_F_HTMR_SEC_RTS_POS)) /**< SEC_RTS Mask */

/**@} end of group HTMR_SEC_Register */

/**
 * @ingroup  htmr_registers
 * @defgroup HTMR_SSEC HTMR_SSEC
 * @brief    HTimer Short Interval Counter. This counter ticks ever t_htclk (16.48uS).
 *           HTIMER_SEC is incremented when this register rolls over from 0xFF to 0x00.
 * @{
 */
 #define MXC_F_HTMR_SSEC_RTSS_POS                       0 /**< SSEC_RTSS Position */
 #define MXC_F_HTMR_SSEC_RTSS                           ((uint32_t)(0xFFUL << MXC_F_HTMR_SSEC_RTSS_POS)) /**< SSEC_RTSS Mask */

/**@} end of group HTMR_SSEC_Register */

/**
 * @ingroup  htmr_registers
 * @defgroup HTMR_RAS HTMR_RAS
 * @brief    Long Interval Alarm.
 * @{
 */
 #define MXC_F_HTMR_RAS_RAS_POS                         0 /**< RAS_RAS Position */
 #define MXC_F_HTMR_RAS_RAS                             ((uint32_t)(0xFFFFFUL << MXC_F_HTMR_RAS_RAS_POS)) /**< RAS_RAS Mask */

/**@} end of group HTMR_RAS_Register */

/**
 * @ingroup  htmr_registers
 * @defgroup HTMR_RSSA HTMR_RSSA
 * @brief    HTimer Short Interval Alarm. This register contains the reload value for the
 *           short interval alarm, HTIMER_CTRL.alarm_ss_fl is raised on rollover.
 * @{
 */
 #define MXC_F_HTMR_RSSA_RSSA_POS                       0 /**< RSSA_RSSA Position */
 #define MXC_F_HTMR_RSSA_RSSA                           ((uint32_t)(0xFFFFFFFFUL << MXC_F_HTMR_RSSA_RSSA_POS)) /**< RSSA_RSSA Mask */

/**@} end of group HTMR_RSSA_Register */

/**
 * @ingroup  htmr_registers
 * @defgroup HTMR_CTRL HTMR_CTRL
 * @brief    HTimer Control Register.
 * @{
 */
 #define MXC_F_HTMR_CTRL_ENABLE_POS                     0 /**< CTRL_ENABLE Position */
 #define MXC_F_HTMR_CTRL_ENABLE                         ((uint32_t)(0x1UL << MXC_F_HTMR_CTRL_ENABLE_POS)) /**< CTRL_ENABLE Mask */

 #define MXC_F_HTMR_CTRL_ALARM_TOD_EN_POS               1 /**< CTRL_ALARM_TOD_EN Position */
 #define MXC_F_HTMR_CTRL_ALARM_TOD_EN                   ((uint32_t)(0x1UL << MXC_F_HTMR_CTRL_ALARM_TOD_EN_POS)) /**< CTRL_ALARM_TOD_EN Mask */

 #define MXC_F_HTMR_CTRL_ALARM_SS_EN_POS                2 /**< CTRL_ALARM_SS_EN Position */
 #define MXC_F_HTMR_CTRL_ALARM_SS_EN                    ((uint32_t)(0x1UL << MXC_F_HTMR_CTRL_ALARM_SS_EN_POS)) /**< CTRL_ALARM_SS_EN Mask */

 #define MXC_F_HTMR_CTRL_BUSY_POS                       3 /**< CTRL_BUSY Position */
 #define MXC_F_HTMR_CTRL_BUSY                           ((uint32_t)(0x1UL << MXC_F_HTMR_CTRL_BUSY_POS)) /**< CTRL_BUSY Mask */

 #define MXC_F_HTMR_CTRL_READY_POS                      4 /**< CTRL_READY Position */
 #define MXC_F_HTMR_CTRL_READY                          ((uint32_t)(0x1UL << MXC_F_HTMR_CTRL_READY_POS)) /**< CTRL_READY Mask */

 #define MXC_F_HTMR_CTRL_READY_INT_EN_POS               5 /**< CTRL_READY_INT_EN Position */
 #define MXC_F_HTMR_CTRL_READY_INT_EN                   ((uint32_t)(0x1UL << MXC_F_HTMR_CTRL_READY_INT_EN_POS)) /**< CTRL_READY_INT_EN Mask */

 #define MXC_F_HTMR_CTRL_ALARM_TOD_FL_POS               6 /**< CTRL_ALARM_TOD_FL Position */
 #define MXC_F_HTMR_CTRL_ALARM_TOD_FL                   ((uint32_t)(0x1UL << MXC_F_HTMR_CTRL_ALARM_TOD_FL_POS)) /**< CTRL_ALARM_TOD_FL Mask */

 #define MXC_F_HTMR_CTRL_ALARM_SS_FL_POS                7 /**< CTRL_ALARM_SS_FL Position */
 #define MXC_F_HTMR_CTRL_ALARM_SS_FL                    ((uint32_t)(0x1UL << MXC_F_HTMR_CTRL_ALARM_SS_FL_POS)) /**< CTRL_ALARM_SS_FL Mask */

 #define MXC_F_HTMR_CTRL_ACRE_POS                       14 /**< CTRL_ACRE Position */
 #define MXC_F_HTMR_CTRL_ACRE                           ((uint32_t)(0x1UL << MXC_F_HTMR_CTRL_ACRE_POS)) /**< CTRL_ACRE Mask */

 #define MXC_F_HTMR_CTRL_WRITE_EN_POS                   15 /**< CTRL_WRITE_EN Position */
 #define MXC_F_HTMR_CTRL_WRITE_EN                       ((uint32_t)(0x1UL << MXC_F_HTMR_CTRL_WRITE_EN_POS)) /**< CTRL_WRITE_EN Mask */

/**@} end of group HTMR_CTRL_Register */

#ifdef __cplusplus
}
#endif

#endif /* _HTMR_REGS_H_ */
