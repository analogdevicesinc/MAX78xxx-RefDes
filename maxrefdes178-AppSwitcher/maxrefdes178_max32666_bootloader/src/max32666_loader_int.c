/*******************************************************************************
* Copyright (C) 2020-2021 Maxim Integrated Products, Inc., All Rights Reserved.
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
******************************************************************************/

/*******************************      INCLUDES    ****************************/
#include <stdio.h>
#include <stdint.h>


#include "max32666_loader_int.h"

#include "mxc_delay.h"
#include "spi.h"


#include "nvic_table.h"

/*******************************      DEFINES     ****************************/

//
#define SPI             MXC_SPI0
#define SPI_IRQ         SPI0_IRQn
#define SPI_SPEED       8000000

//#define SPI_MASTERASYNC
#define SPI_MASTERSYNC

/******************************* Type Definitions ****************************/

/******************************* 	Variables 	  ****************************/
static unsigned char selected_ss = S_SS_VIDEO;
/******************************* Static Functions ****************************/
#ifdef SPI_MASTERASYNC
static volatile int g_spi_flag;

void SPI1_IRQHandler(void)
{
    MXC_SPI_AsyncHandler(SPI);
}

static void SPI_Callback(void *req, int error)
{
	//mxc_spi_req_t *req;
    g_spi_flag = error;
}
#endif

/******************************* Public Functions ****************************/

/*
 * 	SPI
 */
int loader_int_spi_init(void)
{
	int ret = 0;

    // Configure the peripheral
    MXC_SPI_Init(SPI, 1, 0, 1, 0, SPI_SPEED, MAP_B);
    MXC_GPIO_Config (&gpio_cfg_spi0_ss0b);
    MXC_GPIO_Config (&gpio_cfg_spi0_ss1);
    //
    MXC_SPI_SetDataSize(SPI, 8);
    MXC_SPI_SetWidth(SPI, SPI_WIDTH_STANDARD);

#ifdef SPI_MASTERASYNC
    MXC_NVIC_SetVector(SPI_IRQ, SPI1_IRQHandler);
    NVIC_EnableIRQ(SPI_IRQ);
#endif

	return ret;
}
void loader_int_set_active_ss(unsigned char ss)
{
	selected_ss = ss;
}

int loader_int_spi_write(unsigned char *src, unsigned int len)
{
	int ret = 0;
    mxc_spi_req_t req;

    //SPI Request
    req.spi = SPI;
    req.txData = src;
    req.rxData = NULL;
    req.txLen = len;
    req.rxLen = 0;
    req.ssIdx = selected_ss;
    req.ssDeassert = 1;
    req.txCnt = 0;
    req.rxCnt = 0;
    req.completeCB = NULL;

#ifdef SPI_MASTERASYNC
    req.completeCB = SPI_Callback;

    int counter = 20;
	g_spi_flag = 1;
	ret = MXC_SPI_MasterTransactionAsync(&req);
	while(g_spi_flag == 1)  {
		loader_int_delay_ms(1);
		if (--counter == 0) {
			break;
		}
	}
#else
	ret = MXC_SPI_MasterTransaction(&req);
#endif

	return ret;
}

int loader_int_spi_read(unsigned char *dst, unsigned int len)
{
	int ret = 0;
    mxc_spi_req_t req;

    //SPI Request
    req.spi = SPI;
    req.txData = NULL;
    req.rxData = dst;
    req.txLen = 0;
    req.rxLen = len;
    req.ssIdx = selected_ss;
    req.ssDeassert = 1;
    req.txCnt = 0;
    req.rxCnt = 0;
    req.completeCB = NULL;

#ifdef SPI_MASTERASYNC
    req.completeCB = SPI_Callback;

	int counter = 20;
    g_spi_flag = 1;
	ret = MXC_SPI_MasterTransactionAsync(&req);
	while(g_spi_flag == 1)  {
		loader_int_delay_ms(1);
		if (--counter == 0) {
			break;
		}
	}
#else
	ret = MXC_SPI_MasterTransaction(&req);
#endif

	return ret;
}

/*
 *  GPIO
 */
static const mxc_gpio_cfg_t bl_pins[] = {
	{MXC_GPIO0, MXC_GPIO_PIN_20, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIOH},
	{MXC_GPIO0, MXC_GPIO_PIN_21, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIOH},
};

int loader_int_gpio_init(void)
{
	MXC_GPIO_Config(&bl_pins[0]);
	MXC_GPIO_Config(&bl_pins[1]);
    return 0;
}

void loader_int_gpio_set(unsigned int idx, int state)
{
	if (state) {
		MXC_GPIO_OutSet(bl_pins[idx].port, bl_pins[idx].mask);
	} else {
		MXC_GPIO_OutClr(bl_pins[idx].port, bl_pins[idx].mask);
	}
}

int loader_int_gpio_get(unsigned int idx)
{
    return !MXC_GPIO_InGet(bl_pins[idx].port, bl_pins[idx].mask);
}

void loader_int_enable_bootloader_mode(void)
{
	loader_int_gpio_init();
	loader_int_gpio_set(0,0);
	loader_int_gpio_set(1,0);
}
void loader_int_disable_bootloader_mode(void)
{
	loader_int_gpio_init();
	loader_int_gpio_set(0,1);
	loader_int_gpio_set(1,1);
}

/*
 *	Delay
 */
void loader_int_delay_ms(unsigned int ms)
{
	MXC_Delay(ms * 1000UL);
}
