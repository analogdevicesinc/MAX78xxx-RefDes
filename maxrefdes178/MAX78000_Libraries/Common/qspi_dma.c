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

#define S_MODULE_NAME   "qspi_dma"


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdint.h>
#include <mxc.h>
#include "board.h"
#include "spi.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define SPI_DMA_COUNTER_MAX  0xffff
#define QSPI_TIMEOUT_CNT     100000000


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
volatile uint8_t DMA_DONE_FLAG = 0;


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
void qspi_dma_slave_init(mxc_spi_regs_t *qspi, mxc_spi_pins_t qspi_pins)
{
    if (MXC_SPI_Init(qspi, 0, 1, 0, 0, 0, qspi_pins) != E_NO_ERROR) {
        printf("SPI INITIALIZATION ERROR\n");
    }

    // Set data size
    MXC_SETFIELD(qspi->ctrl2, MXC_F_SPI_CTRL2_NUMBITS, 8 << MXC_F_SPI_CTRL2_NUMBITS_POS);

    if (MXC_SPI_SetWidth(qspi, SPI_WIDTH_QUAD) != E_NO_ERROR) {
        printf("SPI SET WIDTH ERROR\n");
    }
}

void qspi_dma_slave_tx(mxc_spi_regs_t *qspi, uint8_t ch, uint8_t *txData, uint32_t txLen, mxc_gpio_cfg_t *qspi_int)
{
    // Setup SPI
    qspi->ctrl1 = 0;

    // Number of characters to transmit from TX FIFO.
    if (txLen > SPI_DMA_COUNTER_MAX) {
        MXC_SETFIELD(qspi->ctrl1, MXC_F_SPI_CTRL1_TX_NUM_CHAR, SPI_DMA_COUNTER_MAX << MXC_F_SPI_CTRL1_TX_NUM_CHAR_POS);
    } else {
        MXC_SETFIELD(qspi->ctrl1, MXC_F_SPI_CTRL1_TX_NUM_CHAR, txLen << MXC_F_SPI_CTRL1_TX_NUM_CHAR_POS);
    }

    // TX FIFO Enabled, clear TX and RX FIFO
    qspi->dma = ((MXC_F_SPI_DMA_TX_FIFO_EN) |
                 (MXC_F_SPI_DMA_TX_FLUSH) |
                 (MXC_F_SPI_DMA_RX_FLUSH));


    // Set SPI DMA TX and RX Thresholds
    MXC_SETFIELD(qspi->dma, MXC_F_SPI_DMA_TX_THD_VAL, 4 << MXC_F_SPI_DMA_TX_THD_VAL_POS);
    MXC_SETFIELD(qspi->dma, MXC_F_SPI_DMA_RX_THD_VAL, 0 << MXC_F_SPI_DMA_RX_THD_VAL_POS);

    // Enable SPI DMA
    qspi->dma |= MXC_F_SPI_DMA_DMA_TX_EN;

    // Setup DMA
    DMA_DONE_FLAG = 0;

    // Clear DMA int flags
    MXC_DMA->ch[ch].status = MXC_DMA->ch[ch].status;

    // Enable SRC increment, set request, set source and destination width, enable channel, Count-To-Zero int enable
    MXC_DMA->ch[ch].ctrl = ((MXC_F_DMA_CTRL_SRCINC) |
                            (MXC_S_DMA_CTRL_REQUEST_SPI0TX) |
                            (MXC_S_DMA_CTRL_SRCWD_WORD << MXC_F_DMA_CTRL_SRCWD_POS) |
                            (MXC_S_DMA_CTRL_SRCWD_WORD << MXC_F_DMA_CTRL_DSTWD_POS) |
                            (MXC_F_DMA_CTRL_CTZ_IE));

    // Set DMA source, destination, counter
    MXC_DMA->ch[ch].cnt = txLen;
    MXC_DMA->ch[ch].src = (unsigned int) txData;
    MXC_DMA->ch[ch].dst = 0;

    if (txLen > SPI_DMA_COUNTER_MAX) {
        MXC_DMA->ch[ch].cnt = 0xffff;
        MXC_DMA->ch[ch].srcrld = (unsigned int) (txData + 0xffff);
        MXC_DMA->ch[ch].dstrld = 0;
        MXC_DMA->ch[ch].cntrld = txLen - SPI_DMA_COUNTER_MAX;
    }

    // Enable DMA int
    MXC_DMA->inten |= (1 << ch);

    // Enable DMA
    if (txLen > SPI_DMA_COUNTER_MAX) {
        MXC_DMA->ch[ch].ctrl |= (MXC_F_DMA_CTRL_EN | MXC_F_DMA_CTRL_RLDEN);
    } else {
        MXC_DMA->ch[ch].ctrl |= MXC_F_DMA_CTRL_EN;
    }

    // Enable SPI
    qspi->ctrl0 |= (MXC_F_SPI_CTRL0_EN);

    // Send interrupt to master
    qspi_int->port->out_clr = qspi_int->mask;
    qspi_int->port->out_set = qspi_int->mask;
}

int qspi_dma_slave_tx_wait(void)
{
    uint32_t cnt = QSPI_TIMEOUT_CNT;

    while(!DMA_DONE_FLAG && cnt) {
        cnt--;
    }

    if (cnt == 0) {
        printf("timeout\n");
        return 1;
    }

    return 0;
}

void qspi_dma_slave_int_handler(mxc_spi_regs_t *qspi, uint8_t ch)
{
    if (MXC_DMA->intfl & (0x1 << ch)) {
        if (MXC_DMA->ch[ch].status & (MXC_F_DMA_STATUS_TO_IF | MXC_F_DMA_STATUS_BUS_ERR)) {
            printf("dma error %d", MXC_DMA->ch[ch].status);
        }

        if (MXC_DMA->ch[ch].status & MXC_F_DMA_STATUS_RLD_IF) {
            // reload interrupt, do nothing
        } else {
            if (MXC_DMA->ch[ch].cnt) {
                printf("dma is not empty %d", MXC_DMA->ch[ch].cnt);
            }

            // Stop DMA
            MXC_DMA->ch[ch].ctrl &= ~MXC_F_DMA_CTRL_EN;

            // Stop SPI
            qspi->ctrl0 &= ~MXC_F_SPI_CTRL0_EN;

            // Disable SPI DMA, flush FIFO
            qspi->dma = (MXC_F_SPI_DMA_TX_FLUSH) | (MXC_F_SPI_DMA_RX_FLUSH);

            DMA_DONE_FLAG = 1;
        }

        // Clear DMA int flags
        MXC_DMA->ch[ch].status = MXC_DMA->ch[ch].status;
    }
}
