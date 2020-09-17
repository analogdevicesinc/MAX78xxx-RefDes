/**
 * @file mxc_pins.c
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

#include "gpio.h"
#include "mxc_config.h"

/***** Definitions *****/
//@TODO: Some of the alternate function definitions differ between WLP and CTBGA
/***** Global Variables *****/

const gpio_cfg_t gpio_cfg_tmr0 =    { PORT_0, PIN_0, GPIO_FUNC_ALT4, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_tmr1 =    { PORT_0, PIN_1, GPIO_FUNC_ALT4, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_tmr2 =    { PORT_0, PIN_2, GPIO_FUNC_ALT4, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_tmr3 =    { PORT_0, PIN_3, GPIO_FUNC_ALT4, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_tmr4 =    { PORT_0, PIN_4, GPIO_FUNC_ALT4, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_tmr5 =    { PORT_0, PIN_5, GPIO_FUNC_ALT4, GPIO_PAD_NONE };

const gpio_cfg_t gpio_cfg_uart0a =        { PORT_0, (PIN_9 | PIN_10), GPIO_FUNC_ALT3, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_uart0a_flow =   { PORT_0, (PIN_8 | PIN_11), GPIO_FUNC_ALT3, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_uart1a =        { PORT_0, (PIN_20 | PIN_21), GPIO_FUNC_ALT3, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_uart1a_flow =   { PORT_0, (PIN_22 | PIN_23), GPIO_FUNC_ALT3, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_uart2a =        { PORT_0, (PIN_1 | PIN_2), GPIO_FUNC_ALT3, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_uart2a_flow =   { PORT_0, (PIN_0 | PIN_3), GPIO_FUNC_ALT3, GPIO_PAD_NONE };

const gpio_cfg_t gpio_cfg_uart0b =        { PORT_1, (PIN_4 | PIN_5), GPIO_FUNC_ALT3, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_uart0b_flow =   { PORT_1, (PIN_6 | PIN_7), GPIO_FUNC_ALT3, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_uart1b =        { PORT_1, (PIN_12 | PIN_13), GPIO_FUNC_ALT3, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_uart1b_flow =   { PORT_1, (PIN_14 | PIN_15), GPIO_FUNC_ALT3, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_uart2b =        { PORT_0, (PIN_28 | PIN_29), GPIO_FUNC_ALT3, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_uart2b_flow =   { PORT_0, (PIN_30 | PIN_31), GPIO_FUNC_ALT3, GPIO_PAD_NONE };

const gpio_cfg_t gpio_cfg_i2c0 =   { PORT_0, (PIN_6 | PIN_7), GPIO_FUNC_ALT1, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_i2c1 =   { PORT_0, (PIN_14 | PIN_15), GPIO_FUNC_ALT1, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_i2c2 =   { PORT_1, (PIN_14 | PIN_15), GPIO_FUNC_ALT1, GPIO_PAD_NONE };

const gpio_cfg_t gpio_cfg_spi17y0a_tri =  { PORT_1, (PIN_9 | PIN_11), GPIO_FUNC_ALT1, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_spi17y0a_dual = { PORT_1, (PIN_9 | PIN_10 | PIN_11), GPIO_FUNC_ALT1, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_spi17y0a =      { PORT_1, (PIN_9 | PIN_10 | PIN_11 | PIN_12 | PIN_13), GPIO_FUNC_ALT1, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_spi17y0b_tri =  { PORT_0, (PIN_9 | PIN_11), GPIO_FUNC_ALT2, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_spi17y0b_dual = { PORT_0, (PIN_9 | PIN_10 | PIN_11), GPIO_FUNC_ALT2, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_spi17y0b =      { PORT_0, (PIN_9 | PIN_10 | PIN_11 | PIN_12 | PIN_13), GPIO_FUNC_ALT2, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_spi17y0_ss0a =  { PORT_1, (PIN_8), GPIO_FUNC_ALT1, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_spi17y0_ss0b =  { PORT_0, (PIN_8), GPIO_FUNC_ALT2, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_spi17y0_ss1 =   { PORT_0, (PIN_14), GPIO_FUNC_ALT2, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_spi17y0_ss2 =   { PORT_0, (PIN_15), GPIO_FUNC_ALT2, GPIO_PAD_NONE };

const gpio_cfg_t gpio_cfg_spi17y1_tri =   { PORT_0, (PIN_17 | PIN_19), GPIO_FUNC_ALT2, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_spi17y1_dual =  { PORT_0, (PIN_17 | PIN_18 | PIN_19), GPIO_FUNC_ALT2, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_spi17y1 =       { PORT_0, (PIN_17 | PIN_18 | PIN_19 | PIN_20 | PIN_21), GPIO_FUNC_ALT2, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_spi17y1_ss0 =   { PORT_0, (PIN_16), GPIO_FUNC_ALT2, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_spi17y1_ss1 =   { PORT_0, (PIN_22), GPIO_FUNC_ALT2, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_spi17y1_ss2 =   { PORT_0, (PIN_23), GPIO_FUNC_ALT2, GPIO_PAD_NONE };

const gpio_cfg_t gpio_cfg_spi17y2_tri =   { PORT_0, (PIN_25 | PIN_27), GPIO_FUNC_ALT2, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_spi17y2_dual =  { PORT_0, (PIN_25 | PIN_26 | PIN_27), GPIO_FUNC_ALT2, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_spi17y2 =       { PORT_0, (PIN_25 | PIN_26 | PIN_27 | PIN_28 | PIN_29), GPIO_FUNC_ALT2, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_spi17y2_ss0 =   { PORT_0, (PIN_24), GPIO_FUNC_ALT2, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_spi17y2_ss1 =   { PORT_0, (PIN_30), GPIO_FUNC_ALT2, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_spi17y2_ss2 =   { PORT_0, (PIN_31), GPIO_FUNC_ALT2, GPIO_PAD_NONE };

const gpio_cfg_t gpio_cfg_pt0 =     { PORT_1, PIN_0,  GPIO_FUNC_ALT4, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_pt1 =     { PORT_1, PIN_1,  GPIO_FUNC_ALT4, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_pt2 =     { PORT_1, PIN_2,  GPIO_FUNC_ALT4, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_pt3 =     { PORT_1, PIN_3,  GPIO_FUNC_ALT4, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_pt4 =     { PORT_1, PIN_4,  GPIO_FUNC_ALT4, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_pt5 =     { PORT_1, PIN_5,  GPIO_FUNC_ALT4, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_pt6 =     { PORT_1, PIN_6,  GPIO_FUNC_ALT4, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_pt7 =     { PORT_1, PIN_7,  GPIO_FUNC_ALT4, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_pt8 =     { PORT_1, PIN_8,  GPIO_FUNC_ALT4, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_pt9 =     { PORT_1, PIN_9,  GPIO_FUNC_ALT4, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_pt10 =    { PORT_1, PIN_10, GPIO_FUNC_ALT4, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_pt11 =    { PORT_1, PIN_11, GPIO_FUNC_ALT4, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_pt12 =    { PORT_1, PIN_12, GPIO_FUNC_ALT4, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_pt13 =    { PORT_1, PIN_13, GPIO_FUNC_ALT4, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_pt14 =    { PORT_1, PIN_14, GPIO_FUNC_ALT4, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_pt15 =    { PORT_1, PIN_15, GPIO_FUNC_ALT4, GPIO_PAD_NONE };

const gpio_cfg_t gpio_cfg_spixr =      { PORT_0, (PIN_8 | PIN_9 | PIN_10 | PIN_11 | PIN_12 | PIN_13), GPIO_FUNC_ALT1, GPIO_PAD_PULL_UP };

const gpio_cfg_t gpio_cfg_rtcsqw =    { PORT_0, PIN_19, GPIO_FUNC_OUT, GPIO_PAD_NONE };

const gpio_cfg_t gpio_cfg_spixfc  =    { PORT_0, (PIN_0 | PIN_1 | PIN_2 | PIN_3 | PIN_4 | PIN_5), GPIO_FUNC_ALT1, GPIO_PAD_NONE };

const gpio_cfg_t gpio_cfg_sdhc =   { PORT_1, (PIN_0 | PIN_1 |PIN_2 |PIN_3 |PIN_4 |PIN_5 |PIN_6 |PIN_7), GPIO_FUNC_ALT1, GPIO_PAD_NONE };

const gpio_cfg_t gpio_cfg_owm = { PORT_0, (PIN_4 | PIN_5), GPIO_FUNC_ALT3, GPIO_PAD_NONE };

const gpio_cfg_t gpio_cfg_adc0 = {PORT_0, PIN_16, GPIO_FUNC_ALT1, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_adc1 = {PORT_0, PIN_17, GPIO_FUNC_ALT1, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_adc2 = {PORT_0, PIN_18, GPIO_FUNC_ALT1, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_adc3 = {PORT_0, PIN_19, GPIO_FUNC_ALT1, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_adc4 = {PORT_0, PIN_20, GPIO_FUNC_ALT1, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_adc5 = {PORT_0, PIN_21, GPIO_FUNC_ALT1, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_adc6 = {PORT_0, PIN_22, GPIO_FUNC_ALT1, GPIO_PAD_NONE };
const gpio_cfg_t gpio_cfg_adc7 = {PORT_0, PIN_23, GPIO_FUNC_ALT1, GPIO_PAD_NONE };

const gpio_cfg_t gpio_swd_core1 = {PORT_0, (PIN_6 | PIN_7), GPIO_FUNC_ALT3, GPIO_PAD_NONE} ;

const gpio_cfg_t gpio_cfg_sdma = { PORT_1, (PIN_0 | PIN_1 | PIN_2 | PIN_3), GPIO_FUNC_ALT3, GPIO_PAD_NONE};
