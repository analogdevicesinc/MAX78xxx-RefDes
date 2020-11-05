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

#define S_MODULE_NAME   "spi_dma"


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <dma.h>
#include <spi.h>
#include <stdint.h>
#include <stdio.h>

#include "maxcam_debug.h"
#include "spi_dma.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define SPI_DMA_COUNTER_MAX  0xffff
#define SPI_TIMEOUT_CNT      100000000


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
static volatile uint8_t dma_done_flag[MXC_DMA_CHANNELS] = {0};


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
void spi_dma_int_handler(uint8_t ch, mxc_spi17y_regs_t *spi)
{
    if (MXC_DMA->intr & (0x1 << ch)) {
        if (MXC_DMA->ch[ch].st & (MXC_F_DMA_ST_TO_ST | MXC_F_DMA_ST_BUS_ERR)) {
            PR_ERROR("dma error %08x", MXC_DMA->ch[ch].st);
        }

        if (MXC_DMA->ch[ch].st & MXC_F_DMA_ST_RLD_ST) {
            // reload occurred, start the SPI transaction
            spi->ctrl0 |= (MXC_F_SPI17Y_CTRL0_EN | MXC_F_SPI17Y_CTRL0_START);
        } else {
            if (MXC_DMA->ch[ch].cnt) {
                PR_WARN("dma is not empty %d", MXC_DMA->ch[ch].cnt);
            }

            // SPI DMA reload bug workaround, SPI stays active even after DMA is completed
            if (MXC_DMA->ch[ch].cnt_rld) {
                // Stop DMA
                MXC_DMA->ch[ch].cfg &= ~MXC_F_DMA_CFG_CHEN;

                // Stop SPI
                spi->ctrl0 &= ~MXC_F_SPI17Y_CTRL0_EN;

                // Disable SPI DMA, flush FIFO
                spi->dma = (MXC_F_SPI17Y_DMA_TX_FIFO_CLEAR | MXC_F_SPI17Y_DMA_RX_FIFO_CLEAR);
            }

            dma_done_flag[ch] = 1;
        }

        // Clear DMA int flags
        MXC_DMA->ch[ch].st =  MXC_DMA->ch[ch].st;
    }
}


void spi_dma_master_init(mxc_spi17y_regs_t *spi, spi_type spi_id, sys_cfg_spi_t spi_pins, uint32_t speed, uint8_t quad)
{
    SPI_Shutdown(spi_id);

    // Configure the peripheral
    if (SPI_Init(spi_id, 0, speed, spi_pins) != 0) {
        PR_ERROR("Error configuring spi");
    }

    // Switch SPI to master mode, else the state of the SS pin could cause the SPI periph to appear 'BUSY';
    spi->ctrl0 |= MXC_F_SPI17Y_CTRL0_MASTER;

    // Initialize the CTRL2 register
    if (quad) {
        spi->ctrl2 = (MXC_S_SPI17Y_CTRL2_DATA_WIDTH_QUAD) | (8 << MXC_F_SPI17Y_CTRL2_NUMBITS_POS);
    } else {
        spi->ctrl2 = (MXC_S_SPI17Y_CTRL2_DATA_WIDTH_MONO) | (8 << MXC_F_SPI17Y_CTRL2_NUMBITS_POS);
    }

    spi->ss_time = ((4 << MXC_F_SPI17Y_SS_TIME_PRE_POS) |
                    (8 << MXC_F_SPI17Y_SS_TIME_POST_POS) |
                    (16 << MXC_F_SPI17Y_SS_TIME_INACT_POS));
}

void spi_dma_rx(uint8_t ch, mxc_spi17y_regs_t *spi, uint8_t *data, uint32_t len, dma_reqsel_t reqsel)
{
    // Initialize the CTRL1 register
    spi->ctrl1 = 0;

    // Set how many to characters to send, or when in quad mode how many characters to receive
    if (len > SPI_DMA_COUNTER_MAX) {
        spi->ctrl1 |= (SPI_DMA_COUNTER_MAX << MXC_F_SPI17Y_CTRL1_RX_NUM_CHAR_POS);
    } else {
        spi->ctrl1 |= (len << MXC_F_SPI17Y_CTRL1_RX_NUM_CHAR_POS);
    }

    // RX FIFO Enabled, clear TX and RX FIFO
    spi->dma = (MXC_F_SPI17Y_DMA_RX_DMA_EN |
                MXC_F_SPI17Y_DMA_TX_FIFO_CLEAR |
                MXC_F_SPI17Y_DMA_RX_FIFO_CLEAR);

    // Set SPI DMA TX and RX Thresholds
    spi->dma |= (0 << MXC_F_SPI17Y_DMA_TX_FIFO_LEVEL_POS);

    // Enable SPI DMA
    spi->dma |= MXC_F_SPI17Y_DMA_RX_FIFO_EN;

    // Setup DMA
    dma_done_flag[ch] = 0;

    // Clear DMA int flags
    MXC_DMA->ch[ch].st =  MXC_DMA->ch[ch].st;

    // Prepare DMA to fill TX FIFO.
    MXC_DMA->ch[ch].cfg = (MXC_F_DMA_CFG_DSTINC |
                           reqsel |
                           MXC_S_DMA_CFG_SRCWD_BYTE |
                           MXC_S_DMA_CFG_DSTWD_BYTE |
                           MXC_F_DMA_CFG_CHDIEN |
                           MXC_F_DMA_CFG_CTZIEN);

    // Set DMA source, destination, counter
    MXC_DMA->ch[ch].cnt = len;
    MXC_DMA->ch[ch].cnt_rld = 0;
    MXC_DMA->ch[ch].src = 0;
    MXC_DMA->ch[ch].dst = (unsigned int) data;

    // if too big, set reload registers
    if (len > SPI_DMA_COUNTER_MAX) {
        MXC_DMA->ch[ch].cnt = SPI_DMA_COUNTER_MAX;
        MXC_DMA->ch[ch].src_rld = 0;
        MXC_DMA->ch[ch].dst_rld = (unsigned int) (data + SPI_DMA_COUNTER_MAX);
        MXC_DMA->ch[ch].cnt_rld = len - SPI_DMA_COUNTER_MAX;
    }

    // Enable DMA int
    MXC_DMA->cn |= (1 << ch);

    // Enable DMA
    if (len > SPI_DMA_COUNTER_MAX) {
        MXC_DMA->ch[ch].cfg |= (MXC_F_DMA_CFG_CHEN | MXC_F_DMA_CFG_RLDEN);
    } else {
        MXC_DMA->ch[ch].cfg |= MXC_F_DMA_CFG_CHEN;
    }

    // Start the SPI transaction
    spi->ctrl0 |= (MXC_F_SPI17Y_CTRL0_EN | MXC_F_SPI17Y_CTRL0_START);
}

void spi_dma_tx(uint8_t ch, mxc_spi17y_regs_t *spi, uint8_t *data, uint32_t len, dma_reqsel_t reqsel)
{
    // Initialize the CTRL1 register
    spi->ctrl1 = 0;

    // Set how many to characters to send, or when in quad mode how many characters to receive
    if (len > SPI_DMA_COUNTER_MAX) {
        spi->ctrl1 |= (SPI_DMA_COUNTER_MAX << MXC_F_SPI17Y_CTRL1_TX_NUM_CHAR_POS);
    } else {
        spi->ctrl1 |= (len << MXC_F_SPI17Y_CTRL1_TX_NUM_CHAR_POS);
    }

    // RX FIFO Enabled, clear TX and RX FIFO
    spi->dma = (MXC_F_SPI17Y_DMA_TX_DMA_EN |
                MXC_F_SPI17Y_DMA_TX_FIFO_CLEAR |
                MXC_F_SPI17Y_DMA_RX_FIFO_CLEAR);

    // Set SPI DMA TX and RX Thresholds
    spi->dma |= (31 << MXC_F_SPI17Y_DMA_TX_FIFO_LEVEL_POS);

    // Enable SPI DMA
    spi->dma |= MXC_F_SPI17Y_DMA_TX_FIFO_EN;

    // Setup DMA
    dma_done_flag[ch] = 0;

    // Clear DMA int flags
    MXC_DMA->ch[ch].st =  MXC_DMA->ch[ch].st;

    // Prepare DMA to fill TX FIFO.
    MXC_DMA->ch[ch].cfg = (MXC_F_DMA_CFG_SRCINC |
                           reqsel |
                           MXC_S_DMA_CFG_SRCWD_BYTE |
                           MXC_S_DMA_CFG_DSTWD_BYTE |
                           MXC_F_DMA_CFG_CHDIEN |
                           MXC_F_DMA_CFG_CTZIEN);

    // Set DMA source, destination, counter
    MXC_DMA->ch[ch].cnt = len;
    MXC_DMA->ch[ch].cnt_rld = 0;
    MXC_DMA->ch[ch].src = (unsigned int) data;
    MXC_DMA->ch[ch].dst = 0;

    // if too big, set reload registers
    if (len > SPI_DMA_COUNTER_MAX) {
        MXC_DMA->ch[ch].cnt = SPI_DMA_COUNTER_MAX;
        MXC_DMA->ch[ch].src_rld = (unsigned int) (data + SPI_DMA_COUNTER_MAX);
        MXC_DMA->ch[ch].dst_rld = 0;
        MXC_DMA->ch[ch].cnt_rld = len - SPI_DMA_COUNTER_MAX;
    }

    // Enable DMA int
    MXC_DMA->cn |= (1 << ch);

    // Enable DMA
    if (len > SPI_DMA_COUNTER_MAX) {
        MXC_DMA->ch[ch].cfg |= (MXC_F_DMA_CFG_CHEN | MXC_F_DMA_CFG_RLDEN);
    } else {
        MXC_DMA->ch[ch].cfg |= MXC_F_DMA_CFG_CHEN;
    }

    // Start the SPI transaction
    spi->ctrl0 |= (MXC_F_SPI17Y_CTRL0_EN | MXC_F_SPI17Y_CTRL0_START);
}

int spi_dma_wait(uint8_t ch, mxc_spi17y_regs_t *spi)
{
    uint32_t cnt = SPI_TIMEOUT_CNT;

    while(!dma_done_flag[ch] && cnt) {
        cnt--;
    }

    if (spi->ctrl0 & MXC_F_SPI17Y_CTRL0_EN) {
        while((spi->stat & MXC_F_SPI17Y_STAT_BUSY) && cnt) {
            cnt--;
        }
    }

    if (cnt == 0) {
        PR_WARN("timeout");
        return 1;
    }

    return 0;
}
