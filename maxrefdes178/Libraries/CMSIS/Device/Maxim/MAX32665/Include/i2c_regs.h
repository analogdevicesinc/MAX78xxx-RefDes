/**
 * @file    i2c_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the I2C Peripheral Module.
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

#ifndef _I2C_REGS_H_
#define _I2C_REGS_H_

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
 * @ingroup     i2c
 * @defgroup    i2c_registers I2C_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the I2C Peripheral Module.
 * @details Inter-Integrated Circuit.
 */

/**
 * @ingroup i2c_registers
 * Structure type to access the I2C Registers.
 */
typedef struct {
    __IO uint32_t ctrl0;                /**< <tt>\b 0x00:</tt> I2C CTRL0 Register */
    __IO uint32_t stat;                 /**< <tt>\b 0x04:</tt> I2C STAT Register */
    __IO uint32_t int_fl0;              /**< <tt>\b 0x08:</tt> I2C INT_FL0 Register */
    __IO uint32_t int_en0;              /**< <tt>\b 0x0C:</tt> I2C INT_EN0 Register */
    __IO uint32_t int_fl1;              /**< <tt>\b 0x10:</tt> I2C INT_FL1 Register */
    __IO uint32_t int_en1;              /**< <tt>\b 0x14:</tt> I2C INT_EN1 Register */
    __IO uint32_t fifo_len;             /**< <tt>\b 0x18:</tt> I2C FIFO_LEN Register */
    __IO uint32_t rx_ctrl0;             /**< <tt>\b 0x1C:</tt> I2C RX_CTRL0 Register */
    __IO uint32_t rx_ctrl1;             /**< <tt>\b 0x20:</tt> I2C RX_CTRL1 Register */
    __IO uint32_t tx_ctrl0;             /**< <tt>\b 0x24:</tt> I2C TX_CTRL0 Register */
    __IO uint32_t tx_ctrl1;             /**< <tt>\b 0x28:</tt> I2C TX_CTRL1 Register */
    __IO uint32_t fifo;                 /**< <tt>\b 0x2C:</tt> I2C FIFO Register */
    __IO uint32_t mstr_mode;            /**< <tt>\b 0x30:</tt> I2C MSTR_MODE Register */
    __IO uint32_t clk_lo;               /**< <tt>\b 0x34:</tt> I2C CLK_LO Register */
    __IO uint32_t clk_hi;               /**< <tt>\b 0x38:</tt> I2C CLK_HI Register */
    __IO uint32_t hs_clk;               /**< <tt>\b 0x3C:</tt> I2C HS_CLK Register */
    __IO uint32_t timeout;              /**< <tt>\b 0x40:</tt> I2C TIMEOUT Register */
    __R  uint32_t rsv_0x44;
    __IO uint32_t dma;                  /**< <tt>\b 0x48:</tt> I2C DMA Register */
    __IO uint32_t slv_addr;             /**< <tt>\b 0x4C:</tt> I2C SLV_ADDR Register */
} mxc_i2c_regs_t;

/* Register offsets for module I2C */
/**
 * @ingroup    i2c_registers
 * @defgroup   I2C_Register_Offsets Register Offsets
 * @brief      I2C Peripheral Register Offsets from the I2C Base Peripheral Address. 
 * @{
 */
 #define MXC_R_I2C_CTRL0                    ((uint32_t)0x00000000UL) /**< Offset from I2C Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_I2C_STAT                     ((uint32_t)0x00000004UL) /**< Offset from I2C Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_I2C_INT_FL0                  ((uint32_t)0x00000008UL) /**< Offset from I2C Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_I2C_INT_EN0                  ((uint32_t)0x0000000CUL) /**< Offset from I2C Base Address: <tt> 0x000C</tt> */ 
 #define MXC_R_I2C_INT_FL1                  ((uint32_t)0x00000010UL) /**< Offset from I2C Base Address: <tt> 0x0010</tt> */ 
 #define MXC_R_I2C_INT_EN1                  ((uint32_t)0x00000014UL) /**< Offset from I2C Base Address: <tt> 0x0014</tt> */ 
 #define MXC_R_I2C_FIFO_LEN                 ((uint32_t)0x00000018UL) /**< Offset from I2C Base Address: <tt> 0x0018</tt> */ 
 #define MXC_R_I2C_RX_CTRL0                 ((uint32_t)0x0000001CUL) /**< Offset from I2C Base Address: <tt> 0x001C</tt> */ 
 #define MXC_R_I2C_RX_CTRL1                 ((uint32_t)0x00000020UL) /**< Offset from I2C Base Address: <tt> 0x0020</tt> */ 
 #define MXC_R_I2C_TX_CTRL0                 ((uint32_t)0x00000024UL) /**< Offset from I2C Base Address: <tt> 0x0024</tt> */ 
 #define MXC_R_I2C_TX_CTRL1                 ((uint32_t)0x00000028UL) /**< Offset from I2C Base Address: <tt> 0x0028</tt> */ 
 #define MXC_R_I2C_FIFO                     ((uint32_t)0x0000002CUL) /**< Offset from I2C Base Address: <tt> 0x002C</tt> */ 
 #define MXC_R_I2C_MSTR_MODE                ((uint32_t)0x00000030UL) /**< Offset from I2C Base Address: <tt> 0x0030</tt> */ 
 #define MXC_R_I2C_CLK_LO                   ((uint32_t)0x00000034UL) /**< Offset from I2C Base Address: <tt> 0x0034</tt> */ 
 #define MXC_R_I2C_CLK_HI                   ((uint32_t)0x00000038UL) /**< Offset from I2C Base Address: <tt> 0x0038</tt> */ 
 #define MXC_R_I2C_HS_CLK                   ((uint32_t)0x0000003CUL) /**< Offset from I2C Base Address: <tt> 0x003C</tt> */ 
 #define MXC_R_I2C_TIMEOUT                  ((uint32_t)0x00000040UL) /**< Offset from I2C Base Address: <tt> 0x0040</tt> */ 
 #define MXC_R_I2C_DMA                      ((uint32_t)0x00000048UL) /**< Offset from I2C Base Address: <tt> 0x0048</tt> */ 
 #define MXC_R_I2C_SLV_ADDR                 ((uint32_t)0x0000004CUL) /**< Offset from I2C Base Address: <tt> 0x004C</tt> */ 
/**@} end of group i2c_registers */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_CTRL0 I2C_CTRL0
 * @brief    Control Register0.
 * @{
 */
 #define MXC_F_I2C_CTRL0_I2CEN_POS                      0 /**< CTRL0_I2CEN Position */
 #define MXC_F_I2C_CTRL0_I2CEN                          ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_I2CEN_POS)) /**< CTRL0_I2CEN Mask */

 #define MXC_F_I2C_CTRL0_MST_POS                        1 /**< CTRL0_MST Position */
 #define MXC_F_I2C_CTRL0_MST                            ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_MST_POS)) /**< CTRL0_MST Mask */

 #define MXC_F_I2C_CTRL0_GCEN_POS                       2 /**< CTRL0_GCEN Position */
 #define MXC_F_I2C_CTRL0_GCEN                           ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_GCEN_POS)) /**< CTRL0_GCEN Mask */

 #define MXC_F_I2C_CTRL0_IRXM_POS                       3 /**< CTRL0_IRXM Position */
 #define MXC_F_I2C_CTRL0_IRXM                           ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_IRXM_POS)) /**< CTRL0_IRXM Mask */

 #define MXC_F_I2C_CTRL0_ACK_POS                        4 /**< CTRL0_ACK Position */
 #define MXC_F_I2C_CTRL0_ACK                            ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_ACK_POS)) /**< CTRL0_ACK Mask */

 #define MXC_F_I2C_CTRL0_SCLO_POS                       6 /**< CTRL0_SCLO Position */
 #define MXC_F_I2C_CTRL0_SCLO                           ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_SCLO_POS)) /**< CTRL0_SCLO Mask */

 #define MXC_F_I2C_CTRL0_SDAO_POS                       7 /**< CTRL0_SDAO Position */
 #define MXC_F_I2C_CTRL0_SDAO                           ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_SDAO_POS)) /**< CTRL0_SDAO Mask */

 #define MXC_F_I2C_CTRL0_SCL_POS                        8 /**< CTRL0_SCL Position */
 #define MXC_F_I2C_CTRL0_SCL                            ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_SCL_POS)) /**< CTRL0_SCL Mask */

 #define MXC_F_I2C_CTRL0_SDA_POS                        9 /**< CTRL0_SDA Position */
 #define MXC_F_I2C_CTRL0_SDA                            ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_SDA_POS)) /**< CTRL0_SDA Mask */

 #define MXC_F_I2C_CTRL0_SWOE_POS                       10 /**< CTRL0_SWOE Position */
 #define MXC_F_I2C_CTRL0_SWOE                           ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_SWOE_POS)) /**< CTRL0_SWOE Mask */

 #define MXC_F_I2C_CTRL0_READ_POS                       11 /**< CTRL0_READ Position */
 #define MXC_F_I2C_CTRL0_READ                           ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_READ_POS)) /**< CTRL0_READ Mask */

 #define MXC_F_I2C_CTRL0_SCL_STRD_POS                   12 /**< CTRL0_SCL_STRD Position */
 #define MXC_F_I2C_CTRL0_SCL_STRD                       ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_SCL_STRD_POS)) /**< CTRL0_SCL_STRD Mask */

 #define MXC_F_I2C_CTRL0_SCL_PPM_POS                    13 /**< CTRL0_SCL_PPM Position */
 #define MXC_F_I2C_CTRL0_SCL_PPM                        ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_SCL_PPM_POS)) /**< CTRL0_SCL_PPM Mask */

 #define MXC_F_I2C_CTRL0_HSMODE_POS                     15 /**< CTRL0_HSMODE Position */
 #define MXC_F_I2C_CTRL0_HSMODE                         ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_HSMODE_POS)) /**< CTRL0_HSMODE Mask */

/**@} end of group I2C_CTRL0_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_STAT I2C_STAT
 * @brief    Status Register.
 * @{
 */
 #define MXC_F_I2C_STAT_BUSY_POS                        0 /**< STAT_BUSY Position */
 #define MXC_F_I2C_STAT_BUSY                            ((uint32_t)(0x1UL << MXC_F_I2C_STAT_BUSY_POS)) /**< STAT_BUSY Mask */

 #define MXC_F_I2C_STAT_RX_EMPTY_POS                    1 /**< STAT_RX_EMPTY Position */
 #define MXC_F_I2C_STAT_RX_EMPTY                        ((uint32_t)(0x1UL << MXC_F_I2C_STAT_RX_EMPTY_POS)) /**< STAT_RX_EMPTY Mask */

 #define MXC_F_I2C_STAT_RXF_POS                         2 /**< STAT_RXF Position */
 #define MXC_F_I2C_STAT_RXF                             ((uint32_t)(0x1UL << MXC_F_I2C_STAT_RXF_POS)) /**< STAT_RXF Mask */

 #define MXC_F_I2C_STAT_TXE_POS                         3 /**< STAT_TXE Position */
 #define MXC_F_I2C_STAT_TXE                             ((uint32_t)(0x1UL << MXC_F_I2C_STAT_TXE_POS)) /**< STAT_TXE Mask */

 #define MXC_F_I2C_STAT_TXF_POS                         4 /**< STAT_TXF Position */
 #define MXC_F_I2C_STAT_TXF                             ((uint32_t)(0x1UL << MXC_F_I2C_STAT_TXF_POS)) /**< STAT_TXF Mask */

 #define MXC_F_I2C_STAT_CLKMD_POS                       5 /**< STAT_CLKMD Position */
 #define MXC_F_I2C_STAT_CLKMD                           ((uint32_t)(0x1UL << MXC_F_I2C_STAT_CLKMD_POS)) /**< STAT_CLKMD Mask */

/**@} end of group I2C_STAT_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_INT_FL0 I2C_INT_FL0
 * @brief    Interrupt Status Register.
 * @{
 */
 #define MXC_F_I2C_INT_FL0_DONEI_POS                    0 /**< INT_FL0_DONEI Position */
 #define MXC_F_I2C_INT_FL0_DONEI                        ((uint32_t)(0x1UL << MXC_F_I2C_INT_FL0_DONEI_POS)) /**< INT_FL0_DONEI Mask */

 #define MXC_F_I2C_INT_FL0_IRXMI_POS                    1 /**< INT_FL0_IRXMI Position */
 #define MXC_F_I2C_INT_FL0_IRXMI                        ((uint32_t)(0x1UL << MXC_F_I2C_INT_FL0_IRXMI_POS)) /**< INT_FL0_IRXMI Mask */

 #define MXC_F_I2C_INT_FL0_GCI_POS                      2 /**< INT_FL0_GCI Position */
 #define MXC_F_I2C_INT_FL0_GCI                          ((uint32_t)(0x1UL << MXC_F_I2C_INT_FL0_GCI_POS)) /**< INT_FL0_GCI Mask */

 #define MXC_F_I2C_INT_FL0_AMI_POS                      3 /**< INT_FL0_AMI Position */
 #define MXC_F_I2C_INT_FL0_AMI                          ((uint32_t)(0x1UL << MXC_F_I2C_INT_FL0_AMI_POS)) /**< INT_FL0_AMI Mask */

 #define MXC_F_I2C_INT_FL0_RXTHI_POS                    4 /**< INT_FL0_RXTHI Position */
 #define MXC_F_I2C_INT_FL0_RXTHI                        ((uint32_t)(0x1UL << MXC_F_I2C_INT_FL0_RXTHI_POS)) /**< INT_FL0_RXTHI Mask */

 #define MXC_F_I2C_INT_FL0_TXTHI_POS                    5 /**< INT_FL0_TXTHI Position */
 #define MXC_F_I2C_INT_FL0_TXTHI                        ((uint32_t)(0x1UL << MXC_F_I2C_INT_FL0_TXTHI_POS)) /**< INT_FL0_TXTHI Mask */

 #define MXC_F_I2C_INT_FL0_STOPI_POS                    6 /**< INT_FL0_STOPI Position */
 #define MXC_F_I2C_INT_FL0_STOPI                        ((uint32_t)(0x1UL << MXC_F_I2C_INT_FL0_STOPI_POS)) /**< INT_FL0_STOPI Mask */

 #define MXC_F_I2C_INT_FL0_ADRACKI_POS                  7 /**< INT_FL0_ADRACKI Position */
 #define MXC_F_I2C_INT_FL0_ADRACKI                      ((uint32_t)(0x1UL << MXC_F_I2C_INT_FL0_ADRACKI_POS)) /**< INT_FL0_ADRACKI Mask */

 #define MXC_F_I2C_INT_FL0_ARBERI_POS                   8 /**< INT_FL0_ARBERI Position */
 #define MXC_F_I2C_INT_FL0_ARBERI                       ((uint32_t)(0x1UL << MXC_F_I2C_INT_FL0_ARBERI_POS)) /**< INT_FL0_ARBERI Mask */

 #define MXC_F_I2C_INT_FL0_TOERI_POS                    9 /**< INT_FL0_TOERI Position */
 #define MXC_F_I2C_INT_FL0_TOERI                        ((uint32_t)(0x1UL << MXC_F_I2C_INT_FL0_TOERI_POS)) /**< INT_FL0_TOERI Mask */

 #define MXC_F_I2C_INT_FL0_ADRERI_POS                   10 /**< INT_FL0_ADRERI Position */
 #define MXC_F_I2C_INT_FL0_ADRERI                       ((uint32_t)(0x1UL << MXC_F_I2C_INT_FL0_ADRERI_POS)) /**< INT_FL0_ADRERI Mask */

 #define MXC_F_I2C_INT_FL0_DATERI_POS                   11 /**< INT_FL0_DATERI Position */
 #define MXC_F_I2C_INT_FL0_DATERI                       ((uint32_t)(0x1UL << MXC_F_I2C_INT_FL0_DATERI_POS)) /**< INT_FL0_DATERI Mask */

 #define MXC_F_I2C_INT_FL0_DNRERI_POS                   12 /**< INT_FL0_DNRERI Position */
 #define MXC_F_I2C_INT_FL0_DNRERI                       ((uint32_t)(0x1UL << MXC_F_I2C_INT_FL0_DNRERI_POS)) /**< INT_FL0_DNRERI Mask */

 #define MXC_F_I2C_INT_FL0_STRTERI_POS                  13 /**< INT_FL0_STRTERI Position */
 #define MXC_F_I2C_INT_FL0_STRTERI                      ((uint32_t)(0x1UL << MXC_F_I2C_INT_FL0_STRTERI_POS)) /**< INT_FL0_STRTERI Mask */

 #define MXC_F_I2C_INT_FL0_STOPERI_POS                  14 /**< INT_FL0_STOPERI Position */
 #define MXC_F_I2C_INT_FL0_STOPERI                      ((uint32_t)(0x1UL << MXC_F_I2C_INT_FL0_STOPERI_POS)) /**< INT_FL0_STOPERI Mask */

 #define MXC_F_I2C_INT_FL0_TXLOI_POS                    15 /**< INT_FL0_TXLOI Position */
 #define MXC_F_I2C_INT_FL0_TXLOI                        ((uint32_t)(0x1UL << MXC_F_I2C_INT_FL0_TXLOI_POS)) /**< INT_FL0_TXLOI Mask */

 #define MXC_F_I2C_INT_FL0_RDAMI_POS                    22 /**< INT_FL0_RDAMI Position */
 #define MXC_F_I2C_INT_FL0_RDAMI                        ((uint32_t)(0x1UL << MXC_F_I2C_INT_FL0_RDAMI_POS)) /**< INT_FL0_RDAMI Mask */

 #define MXC_F_I2C_INT_FL0_WRAMI_POS                    23 /**< INT_FL0_WRAMI Position */
 #define MXC_F_I2C_INT_FL0_WRAMI                        ((uint32_t)(0x1UL << MXC_F_I2C_INT_FL0_WRAMI_POS)) /**< INT_FL0_WRAMI Mask */

/**@} end of group I2C_INT_FL0_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_INT_EN0 I2C_INT_EN0
 * @brief    Interrupt Enable Register.
 * @{
 */
 #define MXC_F_I2C_INT_EN0_DONEIE_POS                   0 /**< INT_EN0_DONEIE Position */
 #define MXC_F_I2C_INT_EN0_DONEIE                       ((uint32_t)(0x1UL << MXC_F_I2C_INT_EN0_DONEIE_POS)) /**< INT_EN0_DONEIE Mask */

 #define MXC_F_I2C_INT_EN0_IRXMIE_POS                   1 /**< INT_EN0_IRXMIE Position */
 #define MXC_F_I2C_INT_EN0_IRXMIE                       ((uint32_t)(0x1UL << MXC_F_I2C_INT_EN0_IRXMIE_POS)) /**< INT_EN0_IRXMIE Mask */

 #define MXC_F_I2C_INT_EN0_GCIE_POS                     2 /**< INT_EN0_GCIE Position */
 #define MXC_F_I2C_INT_EN0_GCIE                         ((uint32_t)(0x1UL << MXC_F_I2C_INT_EN0_GCIE_POS)) /**< INT_EN0_GCIE Mask */

 #define MXC_F_I2C_INT_EN0_AMIE_POS                     3 /**< INT_EN0_AMIE Position */
 #define MXC_F_I2C_INT_EN0_AMIE                         ((uint32_t)(0x1UL << MXC_F_I2C_INT_EN0_AMIE_POS)) /**< INT_EN0_AMIE Mask */

 #define MXC_F_I2C_INT_EN0_RXTHIE_POS                   4 /**< INT_EN0_RXTHIE Position */
 #define MXC_F_I2C_INT_EN0_RXTHIE                       ((uint32_t)(0x1UL << MXC_F_I2C_INT_EN0_RXTHIE_POS)) /**< INT_EN0_RXTHIE Mask */

 #define MXC_F_I2C_INT_EN0_TXTHIE_POS                   5 /**< INT_EN0_TXTHIE Position */
 #define MXC_F_I2C_INT_EN0_TXTHIE                       ((uint32_t)(0x1UL << MXC_F_I2C_INT_EN0_TXTHIE_POS)) /**< INT_EN0_TXTHIE Mask */

 #define MXC_F_I2C_INT_EN0_STOPIE_POS                   6 /**< INT_EN0_STOPIE Position */
 #define MXC_F_I2C_INT_EN0_STOPIE                       ((uint32_t)(0x1UL << MXC_F_I2C_INT_EN0_STOPIE_POS)) /**< INT_EN0_STOPIE Mask */

 #define MXC_F_I2C_INT_EN0_ADRACKIE_POS                 7 /**< INT_EN0_ADRACKIE Position */
 #define MXC_F_I2C_INT_EN0_ADRACKIE                     ((uint32_t)(0x1UL << MXC_F_I2C_INT_EN0_ADRACKIE_POS)) /**< INT_EN0_ADRACKIE Mask */

 #define MXC_F_I2C_INT_EN0_ARBERIE_POS                  8 /**< INT_EN0_ARBERIE Position */
 #define MXC_F_I2C_INT_EN0_ARBERIE                      ((uint32_t)(0x1UL << MXC_F_I2C_INT_EN0_ARBERIE_POS)) /**< INT_EN0_ARBERIE Mask */

 #define MXC_F_I2C_INT_EN0_TOERIE_POS                   9 /**< INT_EN0_TOERIE Position */
 #define MXC_F_I2C_INT_EN0_TOERIE                       ((uint32_t)(0x1UL << MXC_F_I2C_INT_EN0_TOERIE_POS)) /**< INT_EN0_TOERIE Mask */

 #define MXC_F_I2C_INT_EN0_ADRERIE_POS                  10 /**< INT_EN0_ADRERIE Position */
 #define MXC_F_I2C_INT_EN0_ADRERIE                      ((uint32_t)(0x1UL << MXC_F_I2C_INT_EN0_ADRERIE_POS)) /**< INT_EN0_ADRERIE Mask */

 #define MXC_F_I2C_INT_EN0_DATERIE_POS                  11 /**< INT_EN0_DATERIE Position */
 #define MXC_F_I2C_INT_EN0_DATERIE                      ((uint32_t)(0x1UL << MXC_F_I2C_INT_EN0_DATERIE_POS)) /**< INT_EN0_DATERIE Mask */

 #define MXC_F_I2C_INT_EN0_DNRERIE_POS                  12 /**< INT_EN0_DNRERIE Position */
 #define MXC_F_I2C_INT_EN0_DNRERIE                      ((uint32_t)(0x1UL << MXC_F_I2C_INT_EN0_DNRERIE_POS)) /**< INT_EN0_DNRERIE Mask */

 #define MXC_F_I2C_INT_EN0_STRTERIE_POS                 13 /**< INT_EN0_STRTERIE Position */
 #define MXC_F_I2C_INT_EN0_STRTERIE                     ((uint32_t)(0x1UL << MXC_F_I2C_INT_EN0_STRTERIE_POS)) /**< INT_EN0_STRTERIE Mask */

 #define MXC_F_I2C_INT_EN0_STOPERIE_POS                 14 /**< INT_EN0_STOPERIE Position */
 #define MXC_F_I2C_INT_EN0_STOPERIE                     ((uint32_t)(0x1UL << MXC_F_I2C_INT_EN0_STOPERIE_POS)) /**< INT_EN0_STOPERIE Mask */

 #define MXC_F_I2C_INT_EN0_TXLOIE_POS                   15 /**< INT_EN0_TXLOIE Position */
 #define MXC_F_I2C_INT_EN0_TXLOIE                       ((uint32_t)(0x1UL << MXC_F_I2C_INT_EN0_TXLOIE_POS)) /**< INT_EN0_TXLOIE Mask */

 #define MXC_F_I2C_INT_EN0_RDAMIE_POS                   22 /**< INT_EN0_RDAMIE Position */
 #define MXC_F_I2C_INT_EN0_RDAMIE                       ((uint32_t)(0x1UL << MXC_F_I2C_INT_EN0_RDAMIE_POS)) /**< INT_EN0_RDAMIE Mask */

 #define MXC_F_I2C_INT_EN0_WRAMIE_POS                   23 /**< INT_EN0_WRAMIE Position */
 #define MXC_F_I2C_INT_EN0_WRAMIE                       ((uint32_t)(0x1UL << MXC_F_I2C_INT_EN0_WRAMIE_POS)) /**< INT_EN0_WRAMIE Mask */

/**@} end of group I2C_INT_EN0_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_INT_FL1 I2C_INT_FL1
 * @brief    Interrupt Status Register 1.
 * @{
 */
 #define MXC_F_I2C_INT_FL1_RXOFI_POS                    0 /**< INT_FL1_RXOFI Position */
 #define MXC_F_I2C_INT_FL1_RXOFI                        ((uint32_t)(0x1UL << MXC_F_I2C_INT_FL1_RXOFI_POS)) /**< INT_FL1_RXOFI Mask */

 #define MXC_F_I2C_INT_FL1_TXUFI_POS                    1 /**< INT_FL1_TXUFI Position */
 #define MXC_F_I2C_INT_FL1_TXUFI                        ((uint32_t)(0x1UL << MXC_F_I2C_INT_FL1_TXUFI_POS)) /**< INT_FL1_TXUFI Mask */

 #define MXC_F_I2C_INT_FL1_STARTI_POS                   2 /**< INT_FL1_STARTI Position */
 #define MXC_F_I2C_INT_FL1_STARTI                       ((uint32_t)(0x1UL << MXC_F_I2C_INT_FL1_STARTI_POS)) /**< INT_FL1_STARTI Mask */

/**@} end of group I2C_INT_FL1_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_INT_EN1 I2C_INT_EN1
 * @brief    Interrupt Staus Register 1.
 * @{
 */
 #define MXC_F_I2C_INT_EN1_RXOFIE_POS                   0 /**< INT_EN1_RXOFIE Position */
 #define MXC_F_I2C_INT_EN1_RXOFIE                       ((uint32_t)(0x1UL << MXC_F_I2C_INT_EN1_RXOFIE_POS)) /**< INT_EN1_RXOFIE Mask */

 #define MXC_F_I2C_INT_EN1_TXUFIE_POS                   1 /**< INT_EN1_TXUFIE Position */
 #define MXC_F_I2C_INT_EN1_TXUFIE                       ((uint32_t)(0x1UL << MXC_F_I2C_INT_EN1_TXUFIE_POS)) /**< INT_EN1_TXUFIE Mask */

 #define MXC_F_I2C_INT_EN1_STARTIE_POS                  2 /**< INT_EN1_STARTIE Position */
 #define MXC_F_I2C_INT_EN1_STARTIE                      ((uint32_t)(0x1UL << MXC_F_I2C_INT_EN1_STARTIE_POS)) /**< INT_EN1_STARTIE Mask */

/**@} end of group I2C_INT_EN1_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_FIFO_LEN I2C_FIFO_LEN
 * @brief    FIFO Configuration Register.
 * @{
 */
 #define MXC_F_I2C_FIFO_LEN_RXLEN_POS                   0 /**< FIFO_LEN_RXLEN Position */
 #define MXC_F_I2C_FIFO_LEN_RXLEN                       ((uint32_t)(0xFFUL << MXC_F_I2C_FIFO_LEN_RXLEN_POS)) /**< FIFO_LEN_RXLEN Mask */

 #define MXC_F_I2C_FIFO_LEN_TXLEN_POS                   8 /**< FIFO_LEN_TXLEN Position */
 #define MXC_F_I2C_FIFO_LEN_TXLEN                       ((uint32_t)(0xFFUL << MXC_F_I2C_FIFO_LEN_TXLEN_POS)) /**< FIFO_LEN_TXLEN Mask */

/**@} end of group I2C_FIFO_LEN_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_RX_CTRL0 I2C_RX_CTRL0
 * @brief    Receive Control Register 0.
 * @{
 */
 #define MXC_F_I2C_RX_CTRL0_DNR_POS                     0 /**< RX_CTRL0_DNR Position */
 #define MXC_F_I2C_RX_CTRL0_DNR                         ((uint32_t)(0x1UL << MXC_F_I2C_RX_CTRL0_DNR_POS)) /**< RX_CTRL0_DNR Mask */

 #define MXC_F_I2C_RX_CTRL0_RXFSH_POS                   7 /**< RX_CTRL0_RXFSH Position */
 #define MXC_F_I2C_RX_CTRL0_RXFSH                       ((uint32_t)(0x1UL << MXC_F_I2C_RX_CTRL0_RXFSH_POS)) /**< RX_CTRL0_RXFSH Mask */

 #define MXC_F_I2C_RX_CTRL0_RXTH_POS                    8 /**< RX_CTRL0_RXTH Position */
 #define MXC_F_I2C_RX_CTRL0_RXTH                        ((uint32_t)(0xFUL << MXC_F_I2C_RX_CTRL0_RXTH_POS)) /**< RX_CTRL0_RXTH Mask */

/**@} end of group I2C_RX_CTRL0_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_RX_CTRL1 I2C_RX_CTRL1
 * @brief    Receive Control Register 1.
 * @{
 */
 #define MXC_F_I2C_RX_CTRL1_RXCNT_POS                   0 /**< RX_CTRL1_RXCNT Position */
 #define MXC_F_I2C_RX_CTRL1_RXCNT                       ((uint32_t)(0xFFUL << MXC_F_I2C_RX_CTRL1_RXCNT_POS)) /**< RX_CTRL1_RXCNT Mask */

 #define MXC_F_I2C_RX_CTRL1_RXFIFO_POS                  8 /**< RX_CTRL1_RXFIFO Position */
 #define MXC_F_I2C_RX_CTRL1_RXFIFO                      ((uint32_t)(0xFUL << MXC_F_I2C_RX_CTRL1_RXFIFO_POS)) /**< RX_CTRL1_RXFIFO Mask */

/**@} end of group I2C_RX_CTRL1_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_TX_CTRL0 I2C_TX_CTRL0
 * @brief    Transmit Control Register 0.
 * @{
 */
 #define MXC_F_I2C_TX_CTRL0_TXPRELD_POS                 0 /**< TX_CTRL0_TXPRELD Position */
 #define MXC_F_I2C_TX_CTRL0_TXPRELD                     ((uint32_t)(0x1UL << MXC_F_I2C_TX_CTRL0_TXPRELD_POS)) /**< TX_CTRL0_TXPRELD Mask */

 #define MXC_F_I2C_TX_CTRL0_TXFIFORDY_POS               1 /**< TX_CTRL0_TXFIFORDY Position */
 #define MXC_F_I2C_TX_CTRL0_TXFIFORDY                   ((uint32_t)(0x1UL << MXC_F_I2C_TX_CTRL0_TXFIFORDY_POS)) /**< TX_CTRL0_TXFIFORDY Mask */

 #define MXC_F_I2C_TX_CTRL0_GCAMTXAFDIS_POS             2 /**< TX_CTRL0_GCAMTXAFDIS Position */
 #define MXC_F_I2C_TX_CTRL0_GCAMTXAFDIS                 ((uint32_t)(0x1UL << MXC_F_I2C_TX_CTRL0_GCAMTXAFDIS_POS)) /**< TX_CTRL0_GCAMTXAFDIS Mask */

 #define MXC_F_I2C_TX_CTRL0_SAMWTXAFDIS_POS             3 /**< TX_CTRL0_SAMWTXAFDIS Position */
 #define MXC_F_I2C_TX_CTRL0_SAMWTXAFDIS                 ((uint32_t)(0x1UL << MXC_F_I2C_TX_CTRL0_SAMWTXAFDIS_POS)) /**< TX_CTRL0_SAMWTXAFDIS Mask */

 #define MXC_F_I2C_TX_CTRL0_SAMRTXAFDIS_POS             4 /**< TX_CTRL0_SAMRTXAFDIS Position */
 #define MXC_F_I2C_TX_CTRL0_SAMRTXAFDIS                 ((uint32_t)(0x1UL << MXC_F_I2C_TX_CTRL0_SAMRTXAFDIS_POS)) /**< TX_CTRL0_SAMRTXAFDIS Mask */

 #define MXC_F_I2C_TX_CTRL0_RNACKTXAFDIS_POS            5 /**< TX_CTRL0_RNACKTXAFDIS Position */
 #define MXC_F_I2C_TX_CTRL0_RNACKTXAFDIS                ((uint32_t)(0x1UL << MXC_F_I2C_TX_CTRL0_RNACKTXAFDIS_POS)) /**< TX_CTRL0_RNACKTXAFDIS Mask */

 #define MXC_F_I2C_TX_CTRL0_TXFSH_POS                   7 /**< TX_CTRL0_TXFSH Position */
 #define MXC_F_I2C_TX_CTRL0_TXFSH                       ((uint32_t)(0x1UL << MXC_F_I2C_TX_CTRL0_TXFSH_POS)) /**< TX_CTRL0_TXFSH Mask */

 #define MXC_F_I2C_TX_CTRL0_TXTH_POS                    8 /**< TX_CTRL0_TXTH Position */
 #define MXC_F_I2C_TX_CTRL0_TXTH                        ((uint32_t)(0xFUL << MXC_F_I2C_TX_CTRL0_TXTH_POS)) /**< TX_CTRL0_TXTH Mask */

/**@} end of group I2C_TX_CTRL0_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_TX_CTRL1 I2C_TX_CTRL1
 * @brief    Transmit Control Register 1.
 * @{
 */
 #define MXC_F_I2C_TX_CTRL1_TXRDY_POS                   0 /**< TX_CTRL1_TXRDY Position */
 #define MXC_F_I2C_TX_CTRL1_TXRDY                       ((uint32_t)(0x1UL << MXC_F_I2C_TX_CTRL1_TXRDY_POS)) /**< TX_CTRL1_TXRDY Mask */

 #define MXC_F_I2C_TX_CTRL1_TX_LAST_POS                 1 /**< TX_CTRL1_TX_LAST Position */
 #define MXC_F_I2C_TX_CTRL1_TX_LAST                     ((uint32_t)(0x1UL << MXC_F_I2C_TX_CTRL1_TX_LAST_POS)) /**< TX_CTRL1_TX_LAST Mask */

 #define MXC_F_I2C_TX_CTRL1_TXFIFO_POS                  8 /**< TX_CTRL1_TXFIFO Position */
 #define MXC_F_I2C_TX_CTRL1_TXFIFO                      ((uint32_t)(0xFUL << MXC_F_I2C_TX_CTRL1_TXFIFO_POS)) /**< TX_CTRL1_TXFIFO Mask */

/**@} end of group I2C_TX_CTRL1_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_FIFO I2C_FIFO
 * @brief    Data Register.
 * @{
 */
 #define MXC_F_I2C_FIFO_DATA_POS                        0 /**< FIFO_DATA Position */
 #define MXC_F_I2C_FIFO_DATA                            ((uint32_t)(0xFFUL << MXC_F_I2C_FIFO_DATA_POS)) /**< FIFO_DATA Mask */

/**@} end of group I2C_FIFO_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_MSTR_MODE I2C_MSTR_MODE
 * @brief    Master Control Register.
 * @{
 */
 #define MXC_F_I2C_MSTR_MODE_START_POS                  0 /**< MSTR_MODE_START Position */
 #define MXC_F_I2C_MSTR_MODE_START                      ((uint32_t)(0x1UL << MXC_F_I2C_MSTR_MODE_START_POS)) /**< MSTR_MODE_START Mask */

 #define MXC_F_I2C_MSTR_MODE_RESTART_POS                1 /**< MSTR_MODE_RESTART Position */
 #define MXC_F_I2C_MSTR_MODE_RESTART                    ((uint32_t)(0x1UL << MXC_F_I2C_MSTR_MODE_RESTART_POS)) /**< MSTR_MODE_RESTART Mask */

 #define MXC_F_I2C_MSTR_MODE_STOP_POS                   2 /**< MSTR_MODE_STOP Position */
 #define MXC_F_I2C_MSTR_MODE_STOP                       ((uint32_t)(0x1UL << MXC_F_I2C_MSTR_MODE_STOP_POS)) /**< MSTR_MODE_STOP Mask */

 #define MXC_F_I2C_MSTR_MODE_SEA_POS                    7 /**< MSTR_MODE_SEA Position */
 #define MXC_F_I2C_MSTR_MODE_SEA                        ((uint32_t)(0x1UL << MXC_F_I2C_MSTR_MODE_SEA_POS)) /**< MSTR_MODE_SEA Mask */

 #define MXC_F_I2C_MSTR_MODE_MCODE_POS                  8 /**< MSTR_MODE_MCODE Position */
 #define MXC_F_I2C_MSTR_MODE_MCODE                      ((uint32_t)(0x7UL << MXC_F_I2C_MSTR_MODE_MCODE_POS)) /**< MSTR_MODE_MCODE Mask */

 #define MXC_F_I2C_MSTR_MODE_SCL_SPEED_UP_POS           11 /**< MSTR_MODE_SCL_SPEED_UP Position */
 #define MXC_F_I2C_MSTR_MODE_SCL_SPEED_UP               ((uint32_t)(0x1UL << MXC_F_I2C_MSTR_MODE_SCL_SPEED_UP_POS)) /**< MSTR_MODE_SCL_SPEED_UP Mask */

/**@} end of group I2C_MSTR_MODE_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_CLK_LO I2C_CLK_LO
 * @brief    Clock Low Register.
 * @{
 */
 #define MXC_F_I2C_CLK_LO_SCL_LO_POS                    0 /**< CLK_LO_SCL_LO Position */
 #define MXC_F_I2C_CLK_LO_SCL_LO                        ((uint32_t)(0x1FFUL << MXC_F_I2C_CLK_LO_SCL_LO_POS)) /**< CLK_LO_SCL_LO Mask */

/**@} end of group I2C_CLK_LO_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_CLK_HI I2C_CLK_HI
 * @brief    Clock high Register.
 * @{
 */
 #define MXC_F_I2C_CLK_HI_SCL_HI_POS                    0 /**< CLK_HI_SCL_HI Position */
 #define MXC_F_I2C_CLK_HI_SCL_HI                        ((uint32_t)(0x1FFUL << MXC_F_I2C_CLK_HI_SCL_HI_POS)) /**< CLK_HI_SCL_HI Mask */

/**@} end of group I2C_CLK_HI_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_HS_CLK I2C_HS_CLK
 * @brief    HS-Mode Clock Control Register
 * @{
 */
 #define MXC_F_I2C_HS_CLK_HS_CLK_LO_POS                 0 /**< HS_CLK_HS_CLK_LO Position */
 #define MXC_F_I2C_HS_CLK_HS_CLK_LO                     ((uint32_t)(0xFFUL << MXC_F_I2C_HS_CLK_HS_CLK_LO_POS)) /**< HS_CLK_HS_CLK_LO Mask */

 #define MXC_F_I2C_HS_CLK_HS_CLK_HI_POS                 8 /**< HS_CLK_HS_CLK_HI Position */
 #define MXC_F_I2C_HS_CLK_HS_CLK_HI                     ((uint32_t)(0xFFUL << MXC_F_I2C_HS_CLK_HS_CLK_HI_POS)) /**< HS_CLK_HS_CLK_HI Mask */

/**@} end of group I2C_HS_CLK_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_TIMEOUT I2C_TIMEOUT
 * @brief    Timeout Register
 * @{
 */
 #define MXC_F_I2C_TIMEOUT_TO_POS                       0 /**< TIMEOUT_TO Position */
 #define MXC_F_I2C_TIMEOUT_TO                           ((uint32_t)(0xFFFFUL << MXC_F_I2C_TIMEOUT_TO_POS)) /**< TIMEOUT_TO Mask */

/**@} end of group I2C_TIMEOUT_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_DMA I2C_DMA
 * @brief    DMA Register.
 * @{
 */
 #define MXC_F_I2C_DMA_TXEN_POS                         0 /**< DMA_TXEN Position */
 #define MXC_F_I2C_DMA_TXEN                             ((uint32_t)(0x1UL << MXC_F_I2C_DMA_TXEN_POS)) /**< DMA_TXEN Mask */

 #define MXC_F_I2C_DMA_RXEN_POS                         1 /**< DMA_RXEN Position */
 #define MXC_F_I2C_DMA_RXEN                             ((uint32_t)(0x1UL << MXC_F_I2C_DMA_RXEN_POS)) /**< DMA_RXEN Mask */

/**@} end of group I2C_DMA_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_SLV_ADDR I2C_SLV_ADDR
 * @brief    Slave Address Register.
 * @{
 */
 #define MXC_F_I2C_SLV_ADDR_SLA_POS                     0 /**< SLV_ADDR_SLA Position */
 #define MXC_F_I2C_SLV_ADDR_SLA                         ((uint32_t)(0x3FFUL << MXC_F_I2C_SLV_ADDR_SLA_POS)) /**< SLV_ADDR_SLA Mask */

 #define MXC_F_I2C_SLV_ADDR_EA_POS                      15 /**< SLV_ADDR_EA Position */
 #define MXC_F_I2C_SLV_ADDR_EA                          ((uint32_t)(0x1UL << MXC_F_I2C_SLV_ADDR_EA_POS)) /**< SLV_ADDR_EA Mask */

/**@} end of group I2C_SLV_ADDR_Register */

#ifdef __cplusplus
}
#endif

#endif /* _I2C_REGS_H_ */
