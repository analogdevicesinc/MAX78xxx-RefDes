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
 * $Date: 2019-06-01 00:00:07 +0300 (Sat, 01 Jun 2019) $
 * $Revision: 43638 $
 *
 ******************************************************************************/

#include <stdio.h>
#include "mxc_config.h"
#include "mxc_sys.h"
#include "mxc_assert.h"
#include "board.h"
#include "uart.h"
#include "gpio.h"
#include "mxc_pins.h"
#include "led.h"
#include "pb.h"
#include "spixfc.h"

/***** Global Variables *****/
mxc_uart_regs_t *ConsoleUART = MXC_UART_GET_UART(CONSOLE_UART);
extern uint32_t SystemCoreClock;

const gpio_cfg_t pb_pin[] = {
    {PORT_0, PIN_24, GPIO_FUNC_IN, GPIO_PAD_NONE},
    {PORT_0, PIN_25, GPIO_FUNC_IN, GPIO_PAD_NONE},
};
const unsigned int num_pbs = (sizeof(pb_pin) / sizeof(gpio_cfg_t));

const gpio_cfg_t led_pin[] = {
    {PORT_0, PIN_26, GPIO_FUNC_OUT, GPIO_PAD_NONE},
    {PORT_0, PIN_27, GPIO_FUNC_OUT, GPIO_PAD_NONE},
};
const unsigned int num_leds = (sizeof(led_pin) / sizeof(gpio_cfg_t));

/***** File Scope Variables *****/
const uart_cfg_t uart_cfg = {
    .parity = UART_PARITY_DISABLE,
    .size   = UART_DATA_SIZE_8_BITS,
    .stop   = UART_STOP_1,
    .flow   = UART_FLOW_CTRL_DIS,
    .pol    = UART_FLOW_POL_DIS,
    .baud   = CONSOLE_BAUD,
    .clksel = UART_CLKSEL_SYSTEM
};

const sys_cfg_uart_t uart_sys_cfg = {
    .map = MAP_B,
    .flow = Disable
};
const sys_cfg_i2c_t i2c_sys_cfg = NULL;     // There is no special system configuration parameters for I2C on MAX32650
const sys_cfg_spixfc_t spixfc_sys_cfg = NULL;   // There is no special system configuration parameters for SPIXC on MAX32650

const spixfc_cfg_t mx25_spixfc_cfg = {
    0, //mode
    0, //ssel_pol
    1000000 //baud
};

/******************************************************************************/
void mxc_assert(const char *expr, const char *file, int line)
{
    printf("MXC_ASSERT %s #%d: (%s)\n", file, line, expr);
    while (1);
}

/******************************************************************************/
// void SystemCoreClockUpdate(void)
// {
//     SystemCoreClock = CRYPTO_FREQ;
// }

/******************************************************************************/
int Board_Init(void)
{
    int err;

    // if ((err = MX25_Board_Init()) != E_NO_ERROR) {
    //     MXC_ASSERT_FAIL();
    //     return err;
    // }

    if ((err = Console_Init()) != E_NO_ERROR) {
        MXC_ASSERT_FAIL();
        return err;
    }
    if ((err = PB_Init()) != E_NO_ERROR) {
        MXC_ASSERT_FAIL();
        return err;
    }

    if ((err = LED_Init()) != E_NO_ERROR) {
        MXC_ASSERT_FAIL();
        return err;
    }

	
    return E_NO_ERROR;
}

/******************************************************************************/
int Console_Init(void)
{
    int err;

    if ((err = UART_Init(ConsoleUART, &uart_cfg, &uart_sys_cfg)) != E_NO_ERROR) {
        return err;
    }

    return E_NO_ERROR;
}

/******************************************************************************/
int Console_Shutdown(void)
{
    int err;
    
    if ((err = UART_Shutdown(ConsoleUART)) != E_NO_ERROR) {
        return err;
    }
    
    return E_NO_ERROR;
}

/******************************************************************************/
void NMI_Handler(void)
{
    __NOP();
}


/******************************************************************************/
int MX25_Board_Init(void)
{
  return SPIXFC_Init(MX25_SPI, &mx25_spixfc_cfg, &spixfc_sys_cfg);

}

/******************************************************************************/
int MX25_Board_Read(uint8_t* read, unsigned len, unsigned deassert, spixfc_width_t width)
{

    spixfc_req_t req = {MX25_SSEL,deassert,0,NULL,read, width,len,0,0,NULL};
	
    return SPIXFC_Trans(MX25_SPI, &req);
}

/******************************************************************************/
int MX25_Board_Write(const uint8_t* write, unsigned len, unsigned deassert, spixfc_width_t width)
{
	
    spixfc_req_t req = {MX25_SSEL,deassert,0,write,NULL, width,len,0,0,NULL};
	
    return SPIXFC_Trans(MX25_SPI, &req);
}

/******************************************************************************/
int MX25_Clock(unsigned len, unsigned deassert)
{
    return SPIXFC_Clocks(MX25_SPI, len, MX25_SSEL, deassert);
}
