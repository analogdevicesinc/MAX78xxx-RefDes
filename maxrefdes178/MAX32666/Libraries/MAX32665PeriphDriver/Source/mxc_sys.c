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
 * $Date: 2020-06-29 17:56:17 +0300 (Mon, 29 Jun 2020) $
 * $Revision: 53984 $
 *
 *************************************************************************** */

/**
 * @file mxc_sys.c
 * @brief      System layer driver.
 * @details    This driver is used to control the system layer of the device.
 */

/* **** Includes **** */
#include <stddef.h>
#include <string.h>
#include "mxc_config.h"
#include "mxc_assert.h"
#include "mxc_sys.h"
#include "mxc_delay.h"
#include "gpio.h"
#include "flc.h"
#include "cipher.h"
#include "mxc_pins.h"
#include "gcr_regs.h"
#include "tmr_regs.h"
#include "flc_regs.h"
#include "icc_regs.h"
#include "fcr_regs.h"
#include "tmr_utils.h"
#include "sdhc_regs.h"
#include "usbhs_regs.h"
#include "mcr_regs.h"
#include "htmr_regs.h"

/**
 * @ingroup mxc_sys
 * @{
 */

/* **** Definitions **** */
#define SYS_CLOCK_TIMEOUT       MSEC(1)
#define MXC_I2C0  MXC_I2C0_BUS0
#define MXC_I2C1  MXC_I2C1_BUS0
#define MXC_I2C2  MXC_I2C2_BUS0

/* **** Globals **** */
static sys_cfg_spixr_t* spixr_cfg;

/* **** Functions **** */
static int SYS_Clock_Timeout(uint32_t ready);

/* ************************************************************************** */
int SYS_GetUSN(uint8_t *usn, uint8_t *checksum)
{
    uint32_t *infoblock = (uint32_t*)MXC_INFO0_MEM_BASE;

    // Read the USN from the info block
    FLC_UnlockInfoBlock(MXC_INFO0_MEM_BASE);

    usn[0]  = (infoblock[0] & 0x007F8000) >> 15;
    usn[1]  = (infoblock[0] & 0x7F800000) >> 23;
    usn[2]  = (infoblock[1] & 0x0000007F) << 1;
    usn[2] |= (infoblock[0] & 0x80000000) >> 31;
    usn[3]  = (infoblock[1] & 0x00007F80) >> 7;
    usn[4]  = (infoblock[1] & 0x007F8000) >> 15;
    usn[5]  = (infoblock[1] & 0x7F800000) >> 23;
    usn[6]  = (infoblock[2] & 0x007F8000) >> 15;
    usn[7]  = (infoblock[2] & 0x7F800000) >> 23;
    usn[8]  = (infoblock[3] & 0x0000007F) << 1;
    usn[8] |= (infoblock[2] & 0x80000000) >> 31;
    usn[9]  = (infoblock[3] & 0x00007F80) >> 7;
    usn[10] = (infoblock[3] & 0x007F8000) >> 15;

    // Compute the checksum
    if(checksum != NULL) {
        uint8_t info_checksum[2];
        char key[MXC_AES_KEY_128_LEN];

        // Initialize the remainder of the USN and key
        memset(&usn[11], 0, (AES_DATA_LEN - 11));  
        memset(key, 0, MXC_AES_KEY_128_LEN);

        // Read the checksum from the info block
        info_checksum[0] = ((infoblock[3] & 0x7F800000) >> 23);
        info_checksum[1] = ((infoblock[4] & 0x007F8000) >> 15);

        TPU_Cipher_Reset();
        TPU_Cipher_Config(TPU_MODE_ECB, TPU_CIPHER_AES128);
        TPU_AES_Encrypt((const char *)usn, NULL, (const char *)key, 
            TPU_CIPHER_AES128, MXC_V_TPU_CIPHER_CTRL_MODE_ECB, AES_DATA_LEN, 
            (char*)checksum);
        TPU_Shutdown();

        // Verify the checksum
        if((checksum[1] != info_checksum[0]) || 
            (checksum[0] != info_checksum[1])) {

            FLC_LockInfoBlock(MXC_INFO0_MEM_BASE);
            return E_UNKNOWN;
        }   
    }

    // Add the info block checksum to the USN
    usn[11] = ((infoblock[3] & 0x7F800000) >> 23);
    usn[12] = ((infoblock[4] & 0x007F8000) >> 15);

    FLC_LockInfoBlock(MXC_INFO0_MEM_BASE);

    return E_NO_ERROR;
}

/* ************************************************************************** */
int SYS_IsClockEnabled(sys_periph_clock_t clock)
{
    /* The sys_periph_clock_t enum uses enum values that are the offset by 32 for the perckcn1 register. */
    if (clock > 31) {
        clock -= 32;
        return !(MXC_GCR->perckcn1 & (0x1 << clock));
    } else {
        return !(MXC_GCR->perckcn0 & (0x1 << clock));
    }
}

/* ************************************************************************** */
void SYS_ClockDisable(sys_periph_clock_t clock)
{
    /* The sys_periph_clock_t enum uses enum values that are the offset by 32 for the perckcn1 register. */
    if (clock > 31) {
        clock -= 32;
        MXC_GCR->perckcn1 |= (0x1 << clock);
    } else {
        MXC_GCR->perckcn0 |= (0x1 << clock);
    }
}

/* ************************************************************************** */
void SYS_ClockEnable(sys_periph_clock_t clock)
{
    /* The sys_periph_clock_t enum uses enum values that are the offset by 32 for the perckcn1 register. */
    if (clock > 31) {
        clock -= 32;
        MXC_GCR->perckcn1 &= ~(0x1 << clock);
    } else {
        MXC_GCR->perckcn0 &= ~(0x1 << clock);
    }
}
/* ************************************************************************** */
void SYS_RTCClockEnable(sys_cfg_rtc_t *sys_cfg)
{
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_X32K_EN;
}

/* ************************************************************************** */
int SYS_RTCClockDisable(void)
{
    /* Check that the RTC is not the system clock source */
    if ((MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CLKSEL) != MXC_S_GCR_CLKCN_CLKSEL_XTAL32K) {
        MXC_GCR->clkcn &= ~MXC_F_GCR_CLKCN_X32K_EN;
        return E_NO_ERROR;
    } else {
        return E_BAD_STATE;
    }
}

/******************************************************************************/
int SYS_ClockSourceEnable(sys_system_clock_t clock)
{
    switch(clock) {
        case SYS_CLOCK_HIRC96:
            MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC96M_EN;
            return SYS_Clock_Timeout(MXC_F_GCR_CLKCN_HIRC96M_RDY);
        case SYS_CLOCK_HIRC8:
            MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC8M_EN;
            return SYS_Clock_Timeout(MXC_F_GCR_CLKCN_HIRC8M_RDY);
        case SYS_CLOCK_HIRC:
            MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC_EN;
            return SYS_Clock_Timeout(MXC_F_GCR_CLKCN_HIRC_RDY);
        case SYS_CLOCK_LIRC8K:
            // The 8k clock is always enabled
            return SYS_Clock_Timeout(MXC_F_GCR_CLKCN_LIRC8K_RDY);
        case SYS_CLOCK_XTAL32M:
            MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_X32M_EN;
            return SYS_Clock_Timeout(MXC_F_GCR_CLKCN_X32M_RDY);
        case SYS_CLOCK_XTAL32K:
            MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_X32K_EN;
            return SYS_Clock_Timeout(MXC_F_GCR_CLKCN_X32K_RDY);
        default:
            return E_BAD_PARAM;
    }
}

/******************************************************************************/
int SYS_ClockSourceDisable(sys_system_clock_t clock)
{
    uint32_t current_clock;

    current_clock = MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CLKSEL;

    // Don't turn off the clock we're running on
    if(clock == current_clock) {
        return E_BAD_PARAM;
    }

    switch(clock) {
        case SYS_CLOCK_HIRC96:
            MXC_GCR->clkcn &= ~MXC_F_GCR_CLKCN_HIRC96M_EN;
            break;
        case SYS_CLOCK_HIRC8:
            MXC_GCR->clkcn &= ~MXC_F_GCR_CLKCN_HIRC8M_EN;
            break;
        case SYS_CLOCK_HIRC:
            MXC_GCR->clkcn &= ~MXC_F_GCR_CLKCN_HIRC_EN;
            break;
        case SYS_CLOCK_LIRC8K:
            // The 8k clock is always enabled
            break;
        case SYS_CLOCK_XTAL32M:
            MXC_GCR->clkcn &= ~MXC_F_GCR_CLKCN_X32M_EN;
            break;
        case SYS_CLOCK_XTAL32K:
            MXC_GCR->clkcn &= ~MXC_F_GCR_CLKCN_X32K_EN;
            break;
        default:
            return E_BAD_PARAM;
    }

    return E_NO_ERROR;
}

/* ************************************************************************** */
static int SYS_Clock_Timeout(uint32_t ready)
{
    // Start timeout, wait for ready
    mxc_delay_start(SYS_CLOCK_TIMEOUT);
    do {
        if (MXC_GCR->clkcn & ready) {
            mxc_delay_stop();
            return E_NO_ERROR;
        }
    } while (mxc_delay_check() == E_BUSY);
    
    return E_TIME_OUT;
}

/* ************************************************************************** */
int SYS_Clock_Select(sys_system_clock_t clock, mxc_tmr_regs_t* tmr)
{
    uint32_t current_clock;
    
    // Save the current system clock
    current_clock = MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CLKSEL;
    
    switch (clock) {
        case SYS_CLOCK_HIRC96:
            // Enable HIRC96 clock
            if(!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_HIRC96M_EN)) {

                MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC96M_EN;
                        
                // Check if HIRC96 clock is ready
                if (SYS_Clock_Timeout(MXC_F_GCR_CLKCN_HIRC96M_RDY) != E_NO_ERROR) {
                    return E_TIME_OUT;
                }
            }
            
            // Set HIRC96 clock as System Clock
            MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_CLKSEL, MXC_S_GCR_CLKCN_CLKSEL_HIRC96);
            
            break;
        case SYS_CLOCK_HIRC8:
            // Enable HIRC8 clock
            if(!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_HIRC8M_EN)) {
                MXC_GCR->clkcn |=MXC_F_GCR_CLKCN_HIRC8M_EN;
                
                // Check if HIRC8 clock is ready
                if (SYS_Clock_Timeout(MXC_F_GCR_CLKCN_HIRC8M_RDY) != E_NO_ERROR) {
                    return E_TIME_OUT;
                }
            }
            
            // Set HIRC8 clock as System Clock
            MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_CLKSEL, MXC_S_GCR_CLKCN_CLKSEL_HIRC8);
            
            break;
        case SYS_CLOCK_HIRC:
            // Enable HIRC clock
            if(!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_HIRC_EN)) {
                MXC_GCR->clkcn |=MXC_F_GCR_CLKCN_HIRC_EN;
                
                // Check if HIRC clock is ready
                if (SYS_Clock_Timeout(MXC_F_GCR_CLKCN_HIRC_RDY) != E_NO_ERROR) {
                    return E_TIME_OUT;
                }
            }
            
            // Set HIRC clock as System Clock
            MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_CLKSEL, MXC_S_GCR_CLKCN_CLKSEL_HIRC);
            
            break;
        case SYS_CLOCK_XTAL32M:
            // Enable XTAL32M clock
            if(!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_X32M_EN)) {
                MXC_GCR->clkcn |=MXC_F_GCR_CLKCN_X32M_EN;
                
                // Check if XTAL32M clock is ready
                if (SYS_Clock_Timeout(MXC_F_GCR_CLKCN_X32M_RDY) != E_NO_ERROR) {
                    return E_TIME_OUT;
                }
            }
            
            // Set XTAL32M clock as System Clock
            MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_CLKSEL, MXC_S_GCR_CLKCN_CLKSEL_XTAL32M);
            
            break;
        case SYS_CLOCK_LIRC8K:            
            // Set LIRC8 clock as System Clock
            MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_CLKSEL, MXC_S_GCR_CLKCN_CLKSEL_LIRC8);
            
            break;
        case SYS_CLOCK_XTAL32K:
            // Enable XTAL32K clock
            if(!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_X32K_EN)) {
                MXC_GCR->clkcn |=MXC_F_GCR_CLKCN_X32K_EN;
                
                // Check if XTAL32K clock is ready
                if (SYS_Clock_Timeout(MXC_F_GCR_CLKCN_X32K_RDY) != E_NO_ERROR) {
                    return E_TIME_OUT;
                }
            }
            
            // Set XTAL32K clock as System Clock
            MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_CLKSEL, MXC_S_GCR_CLKCN_CLKSEL_XTAL32K);
            
            break;
        default:
            return E_BAD_PARAM;
    }
    
    // Wait for system clock to be ready
    if (SYS_Clock_Timeout(MXC_F_GCR_CLKCN_CKRDY) != E_NO_ERROR) {
    
        // Restore the old system clock if timeout
        MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_CLKSEL, current_clock);
        
        return E_TIME_OUT;
    }

    // Update the system core clock
    SystemCoreClockUpdate();
    
    return E_NO_ERROR;
}

/* ************************************************************************** */
int SYS_UART_Init(mxc_uart_regs_t *uart, const sys_cfg_uart_t* sys_cfg)
{
    if (sys_cfg == NULL) {
        return E_BAD_PARAM;
    }
    // Configure GPIO for UART
    if (uart == MXC_UART0) {
        SYS_ClockEnable(SYS_PERIPH_CLOCK_UART0);
        if (sys_cfg->map == MAP_A) {
        
            GPIO_Config(&gpio_cfg_uart0a);
            
            if (sys_cfg->flow == Enable) {
                GPIO_Config(&gpio_cfg_uart0a_flow);
            }
        } else {
        
            GPIO_Config(&gpio_cfg_uart0b);
            
            if (sys_cfg->flow == Enable) {
                GPIO_Config(&gpio_cfg_uart0b_flow);
            }
            
        }
    } else if (uart == MXC_UART1) {
        SYS_ClockEnable(SYS_PERIPH_CLOCK_UART1);
        if (sys_cfg->map == MAP_A) {
        
            GPIO_Config(&gpio_cfg_uart1a);
            
            if (sys_cfg->flow == Enable) {
                GPIO_Config(&gpio_cfg_uart1a_flow);
            }
        } else {
        
            GPIO_Config(&gpio_cfg_uart1b);
            
            if (sys_cfg->flow == Enable) {
                GPIO_Config(&gpio_cfg_uart1b_flow);
            }
            
        }
    } else if (uart == MXC_UART2) {
        SYS_ClockEnable(SYS_PERIPH_CLOCK_UART2);
        if (sys_cfg->map == MAP_A) {
        
            GPIO_Config(&gpio_cfg_uart2a);
            
            if (sys_cfg->flow == Enable) {
                GPIO_Config(&gpio_cfg_uart2a_flow);
            }
        } else {
        
            GPIO_Config(&gpio_cfg_uart2b);
            
            if (sys_cfg->flow == Enable) {
                GPIO_Config(&gpio_cfg_uart2b_flow);
            }
            
        }
    } else {
        return E_NO_DEVICE;
    }
    
    return E_NO_ERROR;
}
/* ************************************************************************** */
int SYS_UART_Shutdown(mxc_uart_regs_t *uart)
{

    // Configure GPIO for UART
    if (uart == MXC_UART0) {
        SYS_ClockDisable(SYS_PERIPH_CLOCK_UART0);
    } else if (uart == MXC_UART1) {
        SYS_ClockDisable(SYS_PERIPH_CLOCK_UART1);
    } else if (uart == MXC_UART2) {
        SYS_ClockDisable(SYS_PERIPH_CLOCK_UART2);
    } else {
        return E_NO_DEVICE;
    }
    
    return E_NO_ERROR;
}

/* ************************************************************************** */
int SYS_I2C_Init(mxc_i2c_regs_t *i2c, const sys_cfg_i2c_t* sys_cfg)
{
    // Configure GPIO for I2C
    if (i2c == MXC_I2C0) {
        SYS_ClockEnable(SYS_PERIPH_CLOCK_I2C0);
        GPIO_Config(&gpio_cfg_i2c0);
    } else if (i2c == MXC_I2C1) {
        SYS_ClockEnable(SYS_PERIPH_CLOCK_I2C1);
        GPIO_Config(&gpio_cfg_i2c1);
    } else if (i2c == MXC_I2C2) {
        SYS_ClockEnable(SYS_PERIPH_CLOCK_I2C2);
        GPIO_Config(&gpio_cfg_i2c2);
    } else {
        return E_NO_DEVICE;
    }
    
    return E_NO_ERROR;
}
/* ************************************************************************** */
int SYS_I2C_Shutdown(mxc_i2c_regs_t *i2c)
{
    // Configure GPIO for I2C
    if (i2c == MXC_I2C0) {
        SYS_ClockDisable(SYS_PERIPH_CLOCK_I2C0);
    } else if (i2c == MXC_I2C1) {
        SYS_ClockDisable(SYS_PERIPH_CLOCK_I2C1);
    } else if (i2c == MXC_I2C2) {
        SYS_ClockDisable(SYS_PERIPH_CLOCK_I2C2);
    } else {
        return E_NO_DEVICE;
    }
    
    return E_NO_ERROR;
}

/* ************************************************************************** */
unsigned SYS_I2C_GetFreq(mxc_i2c_regs_t *i2c)
{
    return PeripheralClock;
}

/* ************************************************************************** */
int SYS_PT_Config(mxc_pt_regs_t *pt, const sys_cfg_pt_t *cfg)
{
    if(cfg->port != PORT_1 || cfg->func != GPIO_FUNC_ALT4)
    {
        return E_BAD_PARAM;
    }
    return GPIO_Config(cfg);
}

/* ************************************************************************** */
void SYS_PT_Init(const sys_cfg_ptg_t* sys_cfg)
{
    SYS_ClockEnable(SYS_PERIPH_CLOCK_PT);
}

/* ************************************************************************** */
void SYS_PT_Shutdown(void)
{
    if (!MXC_PTG_BUS0->enable && !MXC_PTG_BUS1->enable) {
        SYS_ClockDisable(SYS_PERIPH_CLOCK_PT);
    }
}

/* ************************************************************************** */
unsigned SYS_PT_GetFreq(void)
{
    return PeripheralClock;
}

/* ************************************************************************** */
unsigned SYS_TMR_GetFreq(mxc_tmr_regs_t *tmr)
{
    return PeripheralClock;
}

/* ************************************************************************** */
#ifdef __IAR_SYSTEMS_ICC__
    #pragma section=".flashprog"
#else
    __attribute__((section(".flashprog")))
#endif
void SYS_Flash_Operation(void)
{
    /* Flush all instruction caches */
    MXC_GCR->scon |= MXC_F_GCR_SCON_CCACHE_FLUSH;
    /* Wait for flush to complete */
    while(MXC_GCR->scon & MXC_F_GCR_SCON_CCACHE_FLUSH) {
    }
}

/* ************************************************************************** */
void SYS_Reset_Periph(sys_reset_t reset)
{
    /* The sys_reset_t enum uses enum values that are the offset by 32 for the rstr1 register. */
    if (reset > 31) {
        reset -= 32;
        MXC_GCR->rstr1 = (0x1 << reset);
    } else {
        MXC_GCR->rstr0 = (0x1 << reset);
    }
}
/* ************************************************************************** */

int SYS_SDHC_Init(const sys_cfg_sdhc_t* sys_cfg)
{
    mxc_gpio_regs_t *gpio = MXC_GPIO_GET_GPIO(gpio_cfg_sdhc.port);

    // Startup the HIRC96M clock if it's not on already
    if(!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_HIRC96M_EN)) {
        MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC96M_EN;
        if(SYS_Clock_Timeout(MXC_F_GCR_CLKCN_HIRC96M_RDY) != E_NO_ERROR){
            return E_TIME_OUT;
        }
    }

    SYS_ClockEnable(SYS_PERIPH_CLOCK_SDHC);
    
    gpio->vssel |= gpio_cfg_sdhc.mask;
    gpio->ds |= gpio_cfg_sdhc.mask;
    
    return GPIO_Config(&gpio_cfg_sdhc);
}

/* ************************************************************************** */
int SYS_SDHC_Shutdown(void)
{
    SYS_ClockDisable(SYS_PERIPH_CLOCK_SDHC);
    return E_NO_ERROR;
}

/******************************************************************************/
int SYS_SEMA_Init(const sys_cfg_sema_t* sys_cfg)
{
    SYS_ClockEnable(SYS_PERIPH_CLOCK_SMPHR);
    return E_NO_ERROR;
}

/******************************************************************************/
int SYS_SEMA_Shutdown(void)
{
    SYS_ClockDisable(SYS_PERIPH_CLOCK_SMPHR);
    return E_NO_ERROR;
}

/******************************************************************************/
int SYS_SPIXFC_Init(mxc_spixfc_regs_t *spixfc, const sys_cfg_spixfc_t* sys_cfg)
{
    SYS_ClockEnable(SYS_PERIPH_CLOCK_SPIXIP);  // SPIX
    SYS_ClockEnable(SYS_PERIPH_CLOCK_SPIXFC);   // SPIXFC
    SYS_ClockEnable(SYS_PERIPH_CLOCK_ICACHEXIP);

    
    if (spixfc == MXC_SPIXFC) {
        GPIO_Config(&gpio_cfg_spixfc);
    } else {
        return E_NO_DEVICE;
    }
    
    return E_NO_ERROR;
}

/******************************************************************************/
int SYS_SPIXFC_Shutdown(mxc_spixfc_regs_t *spixfc)
{
    SYS_ClockDisable(SYS_PERIPH_CLOCK_SPIXFC);   // SPIXFC
    SYS_ClockDisable(SYS_PERIPH_CLOCK_SPIXIP);  // SPIX
    
    return E_NO_ERROR;
}

/******************************************************************************/
int SYS_SPIXFC_GetFreq(mxc_spixfc_regs_t *spixfc)
{
    return PeripheralClock;
}

/******************************************************************************/
uint32_t SYS_OWM_Init(const sys_cfg_owm_t* sys_cfg)
{
    mxc_gpio_regs_t *gpio = MXC_GPIO_GET_GPIO(gpio_cfg_owm.port);

    SYS_ClockEnable(SYS_PERIPH_CLOCK_OWIRE);
    gpio->vssel |= gpio_cfg_owm.mask;         // 1-Wire pins need to be at 3.3V.
    return GPIO_Config(&gpio_cfg_owm);
}

/******************************************************************************/
void SYS_OWM_Shutdown(void)
{
    SYS_ClockDisable(SYS_PERIPH_CLOCK_OWIRE);
}

/******************************************************************************/
uint32_t SYS_OWM_GetFreq(void)
{
    return PeripheralClock;
}

/******************************************************************************/
int SYS_SPIXR_Init(const sys_cfg_spixr_t* sys_cfg)
{    
    if (sys_cfg->scache_flag == SYS_SCACHE_CLK) {
        SYS_SCACHE_Init(NULL);
    }
    
    if (sys_cfg->crypto_flag == SYS_CRYPTO_CLK) {
        SYS_TPU_Init(NULL);
    }
    
    SYS_ClockEnable(SYS_PERIPH_CLOCK_SPIXIPD);
    
    return GPIO_Config(&gpio_cfg_spixr);          //Configure GPIO for spixr
}


/******************************************************************************/

void SYS_SPIXR_Shutdown(void)
{
    if (spixr_cfg->scache_flag == SYS_SCACHE_CLK) {
        SYS_ClockDisable(SYS_PERIPH_CLOCK_SPIXIPD);
    }
    
    SYS_SCACHE_Shutdown();
}


/******************************************************************************/
void SYS_SCACHE_Init(const sys_cfg_scache_t* sys_cfg)
{
    SYS_ClockEnable(SYS_PERIPH_CLOCK_SCACHE);
}

/******************************************************************************/
void SYS_SCACHE_Shutdown(void)
{
    SYS_ClockDisable(SYS_PERIPH_CLOCK_SCACHE);
}


/* ************************************************************************** */
int SYS_SPI17Y_Init(mxc_spi17y_regs_t *spi, const sys_cfg_spi17y_t* sys_cfg)
{
    if (sys_cfg == NULL) {
        return E_BAD_PARAM;
    }

    // Configure GPIO for spi
    if (spi == MXC_SPI17Y0) {
        if (sys_cfg->map == MAP_A) {
            if(sys_cfg->num_io == 1) {
                GPIO_Config(&gpio_cfg_spi17y0a_tri);
            } else if (sys_cfg->num_io == 2) {
                GPIO_Config(&gpio_cfg_spi17y0a_dual);
            } else {
                GPIO_Config(&gpio_cfg_spi17y0a);
            }
            
            if (sys_cfg->ss0 == Enable) {
                GPIO_Config(&gpio_cfg_spi17y0_ss0a);
            }
        } else if (sys_cfg->map == MAP_B) {
            if(sys_cfg->num_io == 1) {
                GPIO_Config(&gpio_cfg_spi17y0b_tri);
            } else if (sys_cfg->num_io == 2) {
                GPIO_Config(&gpio_cfg_spi17y0b_dual);
            } else {
                GPIO_Config(&gpio_cfg_spi17y0b);
            }
            
            
            if (sys_cfg->ss0 == Enable) {
                GPIO_Config(&gpio_cfg_spi17y0_ss0b);
            }
        } else {
            return E_BAD_PARAM;
        }

        if (sys_cfg->ss1 == Enable) {
            GPIO_Config(&gpio_cfg_spi17y0_ss1);
        }
        
        if (sys_cfg->ss2 == Enable) {
            GPIO_Config(&gpio_cfg_spi17y0_ss2);
        }

        SYS_ClockEnable(SYS_PERIPH_CLOCK_SPI0);
    } else if (spi == MXC_SPI17Y1) {
        if (sys_cfg->map == MAP_A) {
            if(sys_cfg->num_io == 1) {
                GPIO_Config(&gpio_cfg_spi17y1_tri);
            } else if (sys_cfg->num_io == 2) {
                GPIO_Config(&gpio_cfg_spi17y1_dual);
            } else {
                GPIO_Config(&gpio_cfg_spi17y1);
            }
        } else {
            return E_BAD_PARAM;
        }
        
        if (sys_cfg->ss0 == Enable) {
            GPIO_Config(&gpio_cfg_spi17y1_ss0);
        }
        
        if (sys_cfg->ss1 == Enable) {
            GPIO_Config(&gpio_cfg_spi17y1_ss1);
        }
        
        if (sys_cfg->ss2 == Enable) {
            GPIO_Config(&gpio_cfg_spi17y1_ss2);
        }

        SYS_ClockEnable(SYS_PERIPH_CLOCK_SPI1);
    } else if (spi == MXC_SPI17Y2) {
        if (sys_cfg->map == MAP_A) {
            if(sys_cfg->num_io == 1) {
                GPIO_Config(&gpio_cfg_spi17y2_tri);
            } else if (sys_cfg->num_io == 2) {
                GPIO_Config(&gpio_cfg_spi17y2_dual);
            } else {
                GPIO_Config(&gpio_cfg_spi17y2);
            }
            
            if (sys_cfg->ss0 == Enable) {
                GPIO_Config(&gpio_cfg_spi17y2_ss0);
            }
            
            if (sys_cfg->ss1 == Enable) {
                GPIO_Config(&gpio_cfg_spi17y2_ss1);
            }
            
            if (sys_cfg->ss2 == Enable) {
                GPIO_Config(&gpio_cfg_spi17y2_ss2);
            }
        } else {
            return E_BAD_PARAM;
        }

        SYS_ClockEnable(SYS_PERIPH_CLOCK_SPI2);
        
    } else {
        return E_NO_DEVICE;
    }
    
    return E_NO_ERROR;
}

/******************************************************************************/
int SYS_SPI17Y_Shutdown(mxc_spi17y_regs_t *spi)
{
    if (spi == MXC_SPI17Y0) {
        SYS_ClockDisable(SYS_PERIPH_CLOCK_SPI0);
    } else if (spi == MXC_SPI17Y1) {
        SYS_ClockDisable(SYS_PERIPH_CLOCK_SPI1);
    } else if (spi == MXC_SPI17Y2) {
        SYS_ClockDisable(SYS_PERIPH_CLOCK_SPI2);
    }
    return E_NO_ERROR;
}

/******************************************************************************/
void SYS_RTC_SqwavInit(const sys_cfg_rtc_t* sys_cfg)
{
    GPIO_Config(&gpio_cfg_rtcsqw);
    mxc_delay(1000);
    MXC_MCR->outen |= MXC_F_MCR_OUTEN_SQWOUT0EN;
}

/******************************************************************************/
int SYS_USBHS_Init(const sys_cfg_usbhs_t* sys_cfg)
{
    // Startup the HIRC96M clock if it's not on already
    if(!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_HIRC96M_EN)) {
        MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC96M_EN;
        if(SYS_Clock_Timeout(MXC_F_GCR_CLKCN_HIRC96M_RDY) != E_NO_ERROR){
            return E_TIME_OUT;
        }
    }

    SYS_ClockEnable(SYS_PERIPH_CLOCK_USB);
    
    return E_NO_ERROR;
}

/******************************************************************************/
int SYS_USBHS_Shutdown(void)
{
    // Don't shutdown the HIRC96M Clock, others may be using it

    SYS_ClockDisable(SYS_PERIPH_CLOCK_USB);
    
    return E_NO_ERROR;
}

/* ************************************************************************** */
void SYS_DMA_Init(void)
{
    SYS_ClockEnable(SYS_PERIPH_CLOCK_DMA);
}

/* ************************************************************************** */
void SYS_DMA_Shutdown(void)
{
    SYS_ClockDisable(SYS_PERIPH_CLOCK_DMA);
}

/* ************************************************************************** */
int SYS_TMR_Init(mxc_tmr_regs_t *tmr, const sys_cfg_tmr_t* sys_cfg)
{
    if (tmr == MXC_TMR0) {
        SYS_ClockEnable(SYS_PERIPH_CLOCK_T0);
    } else if (tmr == MXC_TMR1) {
        SYS_ClockEnable(SYS_PERIPH_CLOCK_T1);
    } else if (tmr == MXC_TMR2) {
        SYS_ClockEnable(SYS_PERIPH_CLOCK_T2);
    } else if (tmr == MXC_TMR3) {
        SYS_ClockEnable(SYS_PERIPH_CLOCK_T3);
    } else if (tmr == MXC_TMR4) {
        SYS_ClockEnable(SYS_PERIPH_CLOCK_T4);
    } else if (tmr == MXC_TMR5) {
        SYS_ClockEnable(SYS_PERIPH_CLOCK_T5);
    } else {
        return E_BAD_PARAM;
    }
    return E_NO_ERROR;
}

/* ************************************************************************** */
int SYS_TMR_Shutdown(mxc_tmr_regs_t *tmr)
{
    if (tmr == MXC_TMR0) {
        SYS_ClockDisable(SYS_PERIPH_CLOCK_T0);
    } else if (tmr == MXC_TMR1) {
        SYS_ClockDisable(SYS_PERIPH_CLOCK_T1);
    } else if (tmr == MXC_TMR2) {
        SYS_ClockDisable(SYS_PERIPH_CLOCK_T2);
    } else if (tmr == MXC_TMR3) {
        SYS_ClockDisable(SYS_PERIPH_CLOCK_T3);
    } else if (tmr == MXC_TMR4) {
        SYS_ClockDisable(SYS_PERIPH_CLOCK_T4);
    } else if (tmr == MXC_TMR5) {
        SYS_ClockDisable(SYS_PERIPH_CLOCK_T5);
    } else {
        return E_BAD_PARAM;
    }
    return E_NO_ERROR;
}

/* ************************************************************************** */
int SYS_TPU_Init(const sys_cfg_tpu_t* sys_cfg)
{
    /* The crypto clock needs to be turned on for crypto to work. */
    if ((MXC_GCR->clkcn & MXC_F_GCR_CLKCN_HIRC_EN) == 0) {
        MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC_EN;
        // Check if CRYPTO clock is ready
        if (SYS_Clock_Timeout(MXC_F_GCR_CLKCN_HIRC_RDY) != E_NO_ERROR) {
            return E_TIME_OUT;
        }
    }
    
    SYS_ClockEnable(SYS_PERIPH_CLOCK_TPU);
    
    return E_NO_ERROR;
}

/* ************************************************************************** */
int SYS_TPU_Shutdown(void)
{
    SYS_ClockDisable(SYS_PERIPH_CLOCK_TPU);
    
    // Don't shutdown the HIRC Clock, others may be using it

    return E_NO_ERROR;
}

/* ************************************************************************** */
int SYS_ADC_Init(const sys_cfg_adc_t* sys_cfg)
{
    SYS_ClockEnable(SYS_PERIPH_CLOCK_ADC);
    return E_NO_ERROR;
}

/* ************************************************************************** */
int SYS_ADC_Shutdown(void)
{
    SYS_ClockDisable(SYS_PERIPH_CLOCK_ADC);
    return E_NO_ERROR;
}

/* ************************************************************************** */
#ifdef __IAR_SYSTEMS_ICC__
    #pragma section=".flashprog"
#else
    __attribute__((section(".flashprog")))
#endif
int SYS_FLC_Init(const sys_cfg_flc_t* sys_cfg)
{
    return E_NO_ERROR;
}

/* ************************************************************************** */
#ifdef __IAR_SYSTEMS_ICC__
    #pragma section=".flashprog"
#else
    __attribute__((section(".flashprog")))
#endif
int SYS_FLC_GetByAddress(mxc_flc_regs_t **flc, uint32_t addr)
{

    if (addr < MXC_FLASH1_MEM_BASE && addr >= MXC_FLASH0_MEM_BASE) {
        *flc =  MXC_FLC0;
    } else if (addr >= MXC_FLASH1_MEM_BASE &&
               addr < (MXC_FLASH1_MEM_BASE + MXC_FLASH_MEM_SIZE)) {
        *flc = MXC_FLC1;
    } else if (addr >= MXC_INFO0_MEM_BASE &&
               addr < (MXC_INFO0_MEM_BASE + MXC_INFO_MEM_SIZE)) {
        *flc = MXC_FLC0;
    } else if (addr >= MXC_INFO1_MEM_BASE &&
               addr < (MXC_INFO1_MEM_BASE + MXC_INFO_MEM_SIZE)) {
        *flc = MXC_FLC1;
    } else {
        return E_BAD_PARAM;
    }
    return E_NO_ERROR;
}

/* ************************************************************************** */
#ifdef __IAR_SYSTEMS_ICC__
    #pragma section=".flashprog"
#else
    __attribute__((section(".flashprog")))
#endif
int SYS_FLC_GetPhysicalAddress(uint32_t addr, uint32_t *result)
{
    if (addr < MXC_FLASH1_MEM_BASE && addr >= MXC_FLASH0_MEM_BASE) {
        *result = addr & (MXC_FLASH_MEM_SIZE-1);
    } else if (addr >= MXC_FLASH1_MEM_BASE &&
               addr < (MXC_FLASH1_MEM_BASE + MXC_FLASH_MEM_SIZE)) {
        *result = (addr-MXC_FLASH_MEM_SIZE) & (MXC_FLASH_MEM_SIZE-1);
        
    } else if (addr >= MXC_INFO0_MEM_BASE &&
               addr < (MXC_INFO0_MEM_BASE + MXC_INFO_MEM_SIZE)) {
        *result = (addr & (MXC_INFO_MEM_SIZE-1)) + MXC_FLASH_MEM_SIZE;
    } else if (addr >= MXC_INFO1_MEM_BASE && addr < (MXC_INFO1_MEM_BASE + MXC_INFO_MEM_SIZE)) {
        *result = ((addr-MXC_INFO_MEM_SIZE) & (MXC_INFO_MEM_SIZE-1)) + MXC_FLASH_MEM_SIZE;
    } else {
    
        return E_BAD_PARAM;
    }
    return E_NO_ERROR;
}

/* ************************************************************************** */
#ifdef __IAR_SYSTEMS_ICC__
    #pragma section=".flashprog"
#else
    __attribute__((section(".flashprog")))
#endif
/* ************************************************************************** */
int SYS_FLC_Shutdown(void)
{
    return E_NO_ERROR;
}

/* ************************************************************************** */
int SYS_TRNG_Init(const sys_cfg_trng_t* sys_cfg)
{
    SYS_ClockEnable(SYS_PERIPH_CLOCK_TRNG);
    return E_NO_ERROR;
}

/* ************************************************************************** */
int SYS_TRNG_Shutdown(void)
{
    SYS_ClockDisable(SYS_PERIPH_CLOCK_TRNG);
    return E_NO_ERROR;
}

/* ************************************************************************** */
int SYS_HTMR_Init(mxc_htmr_regs_t *htmr)
{
    //Enable 8MHz clock and wait for ready 
    if ((MXC_GCR->clkcn & MXC_F_GCR_CLKCN_HIRC8M_EN) == 0) {
        MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC8M_EN;
        // Check if 8MHz clock is ready
        if (SYS_Clock_Timeout(MXC_F_GCR_CLKCN_HIRC8M_RDY) != E_NO_ERROR) {
            return E_TIME_OUT;
        }
    }
    
    if(htmr == MXC_HTMR0) {
        SYS_ClockEnable(SYS_PERIPH_CLOCK_HTMR0);
    } else if (htmr == MXC_HTMR1) {
        SYS_ClockEnable(SYS_PERIPH_CLOCK_HTMR1);
    } else {
        return E_BAD_PARAM;
    }
    return E_NO_ERROR;
}

/* ************************************************************************** */
int SYS_HTMR_Shutdown(mxc_htmr_regs_t *htmr)
{
    if(htmr == MXC_HTMR0) {
        SYS_ClockDisable(SYS_PERIPH_CLOCK_HTMR0);
    } else if (htmr == MXC_HTMR1) {
        SYS_ClockDisable(SYS_PERIPH_CLOCK_HTMR1);
    } else {
        return E_BAD_PARAM;
    }

    // Don't shutdown the 8MHz Clock, others may be using it

    return E_NO_ERROR;
}

/* ************************************************************************** */
int SYS_WDT_Init(mxc_wdt_regs_t* wdt, const sys_cfg_wdt_t* sys_cfg)
{
    if (wdt == MXC_WDT0){      
        SYS_ClockEnable(SYS_PERIPH_CLOCK_WDT0);
    } else if (wdt == MXC_WDT1) {  
        SYS_ClockEnable(SYS_PERIPH_CLOCK_WDT1);
    } else if (wdt == MXC_WDT2) {  
        SYS_ClockEnable(SYS_PERIPH_CLOCK_WDT2);
    } else {
        return E_BAD_PARAM;
    }
    return E_NO_ERROR;
}

/* ************************************************************************** */
int SYS_WDT_Shutdown(mxc_wdt_regs_t* wdt)
{
    if (wdt == MXC_WDT0){      
        SYS_ClockDisable(SYS_PERIPH_CLOCK_WDT0);
    } else if (wdt == MXC_WDT1) {  
        SYS_ClockDisable(SYS_PERIPH_CLOCK_WDT1);
    } else if (wdt == MXC_WDT2) {  
        SYS_ClockDisable(SYS_PERIPH_CLOCK_WDT2);
    } else {
        return E_BAD_PARAM;
    }
    return E_NO_ERROR;
}

/* ************************************************************************** */
uint32_t SYS_WUT_GetFreq(void)
{
    return XTAL32K_FREQ;
}

/**@} end of mxc_sys */

