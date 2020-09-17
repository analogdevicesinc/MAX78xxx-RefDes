/**
 * @file    simo.h
 * @brief   SIMO function prototypes and data types.
 */

/* ****************************************************************************
 * Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2018-08-29 01:03:02 +0300 (Wed, 29 Aug 2018) $
 * $Revision: 37424 $
 *
 *************************************************************************** */

/* Define to prevent redundant inclusion */
#ifndef _SIMO_H_
#define _SIMO_H_

/* **** Includes **** */
#include "mxc_config.h"
#include "simo_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup simo
 * @ingroup periphlibs
 * @{
 */

/* **** Definitions **** */
#define VREGO_LOW_RANGE_BASE	500
#define VREGO_HIGH_RANGE_BASE	600

/* **** Function Prototypes **** */
void SIMO_setVregO_A(uint32_t voltage);
void SIMO_setVregO_B(uint32_t voltage);
void SIMO_setVregO_C(uint32_t voltage);
void SIMO_setVregO_D(uint32_t voltage);

void SIMO_setIpkA(uint32_t peak_current);
void SIMO_setIpkB(uint32_t peak_current);
void SIMO_setIpkC(uint32_t peak_current);
void SIMO_setIpkD(uint32_t peak_current);

void SIMO_setMaxTon(uint32_t ontime);

void SIMO_setAlertThresholdA(uint32_t threshold);
void SIMO_setAlertThresholdB(uint32_t threshold);
void SIMO_setAlertThresholdC(uint32_t threshold);
void SIMO_setAlertThresholdD(uint32_t threshold);

void SIMO_setZeroCrossCalA(uint32_t zerocross);
void SIMO_setZeroCrossCalB(uint32_t zerocross);
void SIMO_setZeroCrossCalC(uint32_t zerocross);
void SIMO_setZeroCrossCalD(uint32_t zerocross);

uint32_t SIMO_getOutReadyA(void);
uint32_t SIMO_getOutReadyB(void);
uint32_t SIMO_getOutReadyC(void);
uint32_t SIMO_getOutReadyD(void);

/**@} end of group simo */

#ifdef __cplusplus
}
#endif

#endif /* _SIMO_H_ */
