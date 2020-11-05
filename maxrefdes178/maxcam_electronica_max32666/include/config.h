/*******************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All rights Reserved.
 *
 * This software is protected by copyright laws of the United States and
 * of foreign countries. This material may also be protected by patent laws
 * and technology transfer regulations of the United States and of foreign
 * countries. This software is furnished under a license agreement and/or a
 * nondisclosure agreement and may only be used or reproduced in accordance
 * with the terms of those agreements. Dissemination of this information to
 * any party or parties not specified in the license agreement and/or
 * nondisclosure agreement is expressly prohibited.
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
 *******************************************************************************
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
// LCD
#define LCD_SPI_ID                SPI1
#define LCD_SPI                   MXC_SPI1
#define LCD_DMA_REQSEL_SPITX      DMA_REQSEL_SPI1TX
#define LCD_SPI_SPEED             16000000

#define LCD_DMA_CHANNEL           0
#define LCD_DMA_CHANNEL_IRQ       DMA0_IRQn
#define LCD_DMA_CHANNEL_IRQ_HAND  DMA0_IRQHandler

// QSPI
#define QSPI_ID                   SPI0
#define QSPI                      MXC_SPI0
#define QSPI_DMA_REQSEL_SPITX     DMA_REQSEL_SPI0TX
#define QSPI_DMA_REQSEL_SPIRX     DMA_REQSEL_SPI0RX

#define QSPI_DMA_CHANNEL          1
#define QSPI_DMA_CHANNEL_IRQ      DMA1_IRQn
#define QSPI_DMA_CHANNEL_IRQ_HAND DMA1_IRQHandler


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function declarations
//-----------------------------------------------------------------------------

#endif /* _CONFIG_H_ */
