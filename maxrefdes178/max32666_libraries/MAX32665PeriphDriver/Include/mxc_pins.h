/**
 * @file mxc_pins.h
 * @brief      This file contains constant pin configurations for the peripherals.
 */

/* *****************************************************************************
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
 **************************************************************************** */


#ifndef _MXC_PINS_H_
#define _MXC_PINS_H_

#include "gpio.h"

/***** Global Variables *****/

// Predefined GPIO Configurations
extern const gpio_cfg_t gpio_cfg_tmr0;
extern const gpio_cfg_t gpio_cfg_tmr1;
extern const gpio_cfg_t gpio_cfg_tmr2;
extern const gpio_cfg_t gpio_cfg_tmr3;
extern const gpio_cfg_t gpio_cfg_tmr4;
extern const gpio_cfg_t gpio_cfg_tmr5;
extern const gpio_cfg_t gpio_cfg_uart0a;
extern const gpio_cfg_t gpio_cfg_uart0a_flow;
extern const gpio_cfg_t gpio_cfg_uart1a;
extern const gpio_cfg_t gpio_cfg_uart1a_flow;
extern const gpio_cfg_t gpio_cfg_uart2a;
extern const gpio_cfg_t gpio_cfg_uart2a_flow;
extern const gpio_cfg_t gpio_cfg_uart0b;
extern const gpio_cfg_t gpio_cfg_uart0b_flow;
extern const gpio_cfg_t gpio_cfg_uart1b;
extern const gpio_cfg_t gpio_cfg_uart1b_flow;
extern const gpio_cfg_t gpio_cfg_uart2b;
extern const gpio_cfg_t gpio_cfg_uart2b_flow;
extern const gpio_cfg_t gpio_cfg_i2c0;
extern const gpio_cfg_t gpio_cfg_i2c1;
extern const gpio_cfg_t gpio_cfg_i2c2;
extern const gpio_cfg_t gpio_cfg_spi17y0a_tri;
extern const gpio_cfg_t gpio_cfg_spi17y0a_dual;
extern const gpio_cfg_t gpio_cfg_spi17y0a;
extern const gpio_cfg_t gpio_cfg_spi17y0b_tri;
extern const gpio_cfg_t gpio_cfg_spi17y0b_dual;
extern const gpio_cfg_t gpio_cfg_spi17y0b;
extern const gpio_cfg_t gpio_cfg_spi17y0_ss0a;
extern const gpio_cfg_t gpio_cfg_spi17y0_ss0b;
extern const gpio_cfg_t gpio_cfg_spi17y0_ss1;
extern const gpio_cfg_t gpio_cfg_spi17y0_ss2;
extern const gpio_cfg_t gpio_cfg_spi17y1_tri;
extern const gpio_cfg_t gpio_cfg_spi17y1_dual;
extern const gpio_cfg_t gpio_cfg_spi17y1;
extern const gpio_cfg_t gpio_cfg_spi17y1_ss0;
extern const gpio_cfg_t gpio_cfg_spi17y1_ss1;
extern const gpio_cfg_t gpio_cfg_spi17y1_ss2;
extern const gpio_cfg_t gpio_cfg_spi17y2_tri;
extern const gpio_cfg_t gpio_cfg_spi17y2_dual;
extern const gpio_cfg_t gpio_cfg_spi17y2;
extern const gpio_cfg_t gpio_cfg_spi17y2_ss0;
extern const gpio_cfg_t gpio_cfg_spi17y2_ss1;
extern const gpio_cfg_t gpio_cfg_spi17y2_ss2;
extern const gpio_cfg_t gpio_cfg_pt0;
extern const gpio_cfg_t gpio_cfg_pt1;
extern const gpio_cfg_t gpio_cfg_pt2;
extern const gpio_cfg_t gpio_cfg_pt3;
extern const gpio_cfg_t gpio_cfg_pt4;
extern const gpio_cfg_t gpio_cfg_pt5;
extern const gpio_cfg_t gpio_cfg_pt6;
extern const gpio_cfg_t gpio_cfg_pt7;
extern const gpio_cfg_t gpio_cfg_pt8;
extern const gpio_cfg_t gpio_cfg_pt9;
extern const gpio_cfg_t gpio_cfg_pt10;
extern const gpio_cfg_t gpio_cfg_pt11;
extern const gpio_cfg_t gpio_cfg_pt12;
extern const gpio_cfg_t gpio_cfg_pt13;
extern const gpio_cfg_t gpio_cfg_pt14;
extern const gpio_cfg_t gpio_cfg_pt15;
extern const gpio_cfg_t gpio_cfg_rtcsqw;
extern const gpio_cfg_t gpio_cfg_spixfc;
extern const gpio_cfg_t gpio_cfg_spim0;
extern const gpio_cfg_t gpio_cfg_spim1;
extern const gpio_cfg_t gpio_cfg_spim2;
extern const gpio_cfg_t gpio_cfg_spis;
extern const gpio_cfg_t gpio_cfg_spixr;

extern const gpio_cfg_t gpio_cfg_sdhc;
extern const gpio_cfg_t gpio_cfg_owm;

extern const gpio_cfg_t gpio_cfg_adc0;
extern const gpio_cfg_t gpio_cfg_adc1;
extern const gpio_cfg_t gpio_cfg_adc2;
extern const gpio_cfg_t gpio_cfg_adc3;
extern const gpio_cfg_t gpio_cfg_adc4;
extern const gpio_cfg_t gpio_cfg_adc5;
extern const gpio_cfg_t gpio_cfg_adc6;
extern const gpio_cfg_t gpio_cfg_adc7;

extern const gpio_cfg_t gpio_cfg_sdma;

#endif /* _MXC_PINS_H_ */

