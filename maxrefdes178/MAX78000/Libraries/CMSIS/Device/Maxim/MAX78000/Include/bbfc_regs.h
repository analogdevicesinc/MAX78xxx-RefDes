/**
 * @file    bbfc_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the BBFC Peripheral Module.
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

#ifndef _BBFC_REGS_H_
#define _BBFC_REGS_H_

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
 * @ingroup     bbfc
 * @defgroup    bbfc_registers BBFC_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the BBFC Peripheral Module.
 * @details Battery Backed Function Control Register.
 */

/**
 * @ingroup bbfc_registers
 * Structure type to access the BBFC Registers.
 */
typedef struct {
    __IO uint32_t reg0;                 /**< <tt>\b 0x00:</tt> BBFC REG0 Register */
    __IO uint32_t reg1;                 /**< <tt>\b 0x04:</tt> BBFC REG1 Register */
    __IO uint32_t reg2;                 /**< <tt>\b 0x08:</tt> BBFC REG2 Register */
    __IO uint32_t reg3;                 /**< <tt>\b 0x0C:</tt> BBFC REG3 Register */
} mxc_bbfc_regs_t;

/* Register offsets for module BBFC */
/**
 * @ingroup    bbfc_registers
 * @defgroup   BBFC_Register_Offsets Register Offsets
 * @brief      BBFC Peripheral Register Offsets from the BBFC Base Peripheral Address. 
 * @{
 */
 #define MXC_R_BBFC_REG0                    ((uint32_t)0x00000000UL) /**< Offset from BBFC Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_BBFC_REG1                    ((uint32_t)0x00000004UL) /**< Offset from BBFC Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_BBFC_REG2                    ((uint32_t)0x00000008UL) /**< Offset from BBFC Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_BBFC_REG3                    ((uint32_t)0x0000000CUL) /**< Offset from BBFC Base Address: <tt> 0x000C</tt> */ 
/**@} end of group bbfc_registers */

/**
 * @ingroup  bbfc_registers
 * @defgroup BBFC_REG0 BBFC_REG0
 * @brief    Register 0.
 * @{
 */
 #define MXC_F_BBFC_REG0_AIPWR_POS                      0 /**< REG0_AIPWR Position */
 #define MXC_F_BBFC_REG0_AIPWR                          ((uint32_t)(0xFUL << MXC_F_BBFC_REG0_AIPWR_POS)) /**< REG0_AIPWR Mask */

/**@} end of group BBFC_REG0_Register */

/**
 * @ingroup  bbfc_registers
 * @defgroup BBFC_REG1 BBFC_REG1
 * @brief    Register 1.
 * @{
 */
 #define MXC_F_BBFC_REG1_AIRAMPWR_POS                   0 /**< REG1_AIRAMPWR Position */
 #define MXC_F_BBFC_REG1_AIRAMPWR                       ((uint32_t)(0xFUL << MXC_F_BBFC_REG1_AIRAMPWR_POS)) /**< REG1_AIRAMPWR Mask */

/**@} end of group BBFC_REG1_Register */

/**
 * @ingroup  bbfc_registers
 * @defgroup BBFC_REG2 BBFC_REG2
 * @brief    Register 2.
 * @{
 */
 #define MXC_F_BBFC_REG2_AIISO_POS                      0 /**< REG2_AIISO Position */
 #define MXC_F_BBFC_REG2_AIISO                          ((uint32_t)(0xFUL << MXC_F_BBFC_REG2_AIISO_POS)) /**< REG2_AIISO Mask */

/**@} end of group BBFC_REG2_Register */

/**
 * @ingroup  bbfc_registers
 * @defgroup BBFC_REG3 BBFC_REG3
 * @brief    Register 3.
 * @{
 */
 #define MXC_F_BBFC_REG3_AIRST_POS                      0 /**< REG3_AIRST Position */
 #define MXC_F_BBFC_REG3_AIRST                          ((uint32_t)(0xFUL << MXC_F_BBFC_REG3_AIRST_POS)) /**< REG3_AIRST Mask */

/**@} end of group BBFC_REG3_Register */

#ifdef __cplusplus
}
#endif

#endif /* _BBFC_REGS_H_ */
