/*******************************************************************************
 * Copyright (C) 2020 Maxim Integrated Products, Inc., All Rights Reserved.
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


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <mxc_errors.h>
#include <stdio.h>

#include "max32666_debug.h"
#include "max32666_ext_sram.h"
#include "max32666_spi_dma.h"
#include "maxrefdes178_utility.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "extram"

#define SRAM_READ   0x03    // Read data from memory starting at selected address
#define SRAM_WRITE  0x02    // Write (program) data to memory starting at selected address
#define SRAM_EQIO   0x38    // Enable QUAD I/O access
#define SRAM_EDIO   0x3B    // Enable DUAL I/O access
#define SRAM_RSTQIO 0xFF    // Reset from QUAD and DUAL to SPI I/O access
#define SRAM_RDMR   0x05    // Read mode register
#define SRAM_WRMR   0x01    // Write mode register

#define SRAM_MODE   0x41


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
static const mxc_gpio_cfg_t sram_cs_pin    = MAX32666_SRAM_CS_PIN;
static const mxc_gpio_cfg_t sram_hold_pin  = MAX32666_SRAM_HOLD_PIN;


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
int ext_sram_init(void)
{
    int ret;
    uint8_t buff = 0;

    MXC_GPIO_Config(&sram_cs_pin);
    MXC_GPIO_Config(&sram_hold_pin);

    GPIO_SET(sram_cs_pin);  // cs disable
    GPIO_SET(sram_hold_pin);  // hold pin

    // Initialize SPI mode
    if ((ret = spi_dma_master_init(MAX32666_QSPI, MAX32666_QSPI_MAP, QSPI_SPEED, 0)) != E_NO_ERROR) {
        PR_ERROR("spi_dma_master_init fail %d", ret);
        return ret;
    }
    NVIC_EnableIRQ(MAX32666_QSPI_DMA_IRQ);

    // Enable QUAD I/O access
    GPIO_CLR(sram_cs_pin);  // cs enable
    buff = SRAM_EQIO;
    spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, &buff, NULL, 1, MAX32666_QSPI_DMA_REQSEL_SPITX, NULL);
    spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
    GPIO_SET(sram_cs_pin);  // cs disable

    // Initialize QSPI mode
    if ((ret = spi_dma_master_init(MAX32666_QSPI, MAX32666_QSPI_MAP, QSPI_SPEED, 1)) != E_NO_ERROR) {
        PR_ERROR("spi_dma_master_init fail %d", ret);
        return ret;
    }
    NVIC_EnableIRQ(MAX32666_QSPI_DMA_IRQ);

    // Write mode register
    GPIO_CLR(sram_cs_pin);  // cs enable
    buff = SRAM_WRMR;
    spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, &buff, NULL, 1, MAX32666_QSPI_DMA_REQSEL_SPITX, NULL);
    spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
    buff = SRAM_MODE;  // Hold function disabled, Burst Mode
    spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, &buff, NULL, 1, MAX32666_QSPI_DMA_REQSEL_SPITX, NULL);
    spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
    GPIO_SET(sram_cs_pin);  // cs disable

    // Read mode register
    GPIO_CLR(sram_cs_pin);  // cs enable
    buff = SRAM_RDMR;
    spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, &buff, NULL, 1, MAX32666_QSPI_DMA_REQSEL_SPITX, NULL);
    spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
    spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, NULL, &buff, 1, MAX32666_QSPI_DMA_REQSEL_SPIRX, NULL);
    spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
    GPIO_SET(sram_cs_pin);  // cs disable

    if (buff != SRAM_MODE) {
        PR_ERROR("write mode failed");
        return E_COMM_ERR;
    }

//    uint8_t test_write[100];
//    uint8_t test_read[100];
//    for (int i = 0; i < sizeof(test_write); i++) {
//        test_write[i] = i & 0xff;
//    }
//
//    ext_sram_write(0x1000, test_write, sizeof(test_write));
//    ext_sram_read(0x1000, test_read, sizeof(test_read));
//
//    for (int i = 0; i < sizeof(test_read); i++) {
//        if (test_read[i] != (i & 0xff)) {
//           PR_ERROR("%d 0x%x 0x%x", i, test_read[i], (i & 0xff));
//        }
//    }

    return E_NO_ERROR;
}

int ext_sram_read(uint32_t address, uint8_t *buf, uint32_t len)
{
    uint8_t command[5];

    command[0] = SRAM_READ;
    command[1] = (address & 0xFF0000) >> 16;
    command[2] = (address & 0x00FF00) >> 8;
    command[3] = (address & 0x0000FF);
    command[4] = 0;

    GPIO_CLR(sram_cs_pin);  // cs enable
    spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, command, NULL, sizeof(command), MAX32666_QSPI_DMA_REQSEL_SPITX, NULL);
    spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
    spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, NULL, buf, len, MAX32666_QSPI_DMA_REQSEL_SPIRX, NULL);
    spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
    GPIO_SET(sram_cs_pin);  // cs disable

    return E_NO_ERROR;
}

int ext_sram_write(uint32_t address, uint8_t *buf, uint32_t len)
{
    uint8_t command[4];

    command[0] = SRAM_WRITE;
    command[1] = (address & 0xFF0000)>>16;
    command[2] = (address & 0x00FF00)>>8;
    command[3] = (address & 0x0000FF);

    GPIO_CLR(sram_cs_pin);  // cs enable
    spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, command, NULL, sizeof(command), MAX32666_QSPI_DMA_REQSEL_SPITX, NULL);
    spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
    spi_dma(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI, buf, NULL, len, MAX32666_QSPI_DMA_REQSEL_SPITX, NULL);
    spi_dma_wait(MAX32666_QSPI_DMA_CHANNEL, MAX32666_QSPI);
    GPIO_SET(sram_cs_pin);  // cs disable

    return E_NO_ERROR;
}
