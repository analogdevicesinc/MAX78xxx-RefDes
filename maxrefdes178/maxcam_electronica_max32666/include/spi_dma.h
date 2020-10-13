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
#ifndef SPI_DMA_H_
#define SPI_DMA_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "spi.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
void spi_dma_int_handler(uint8_t ch, mxc_spi17y_regs_t *spi);
void spi_dma_master_init(mxc_spi17y_regs_t *spi, spi_type spi_id, sys_cfg_spi_t spi_pins, uint32_t speed, uint8_t quad);
void spi_dma_rx(uint8_t ch, mxc_spi17y_regs_t *spi, uint8_t *data, uint32_t len, dma_reqsel_t reqsel);
void spi_dma_tx(uint8_t ch, mxc_spi17y_regs_t *spi, uint8_t *data, uint32_t len, dma_reqsel_t reqsel);
int spi_dma_wait(uint8_t ch, mxc_spi17y_regs_t *spi);

#endif /* SPI_DMA_H_ */
