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

#ifndef _MAX78000_SPI_DMA_H_
#define _MAX78000_SPI_DMA_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <spi.h>
#include "maxrefdes178_definitions.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------
typedef enum {
    QSPI_STATE_IDLE = 0,

    QSPI_STATE_RX_STARTED,
    QSPI_STATE_RX_HEADER_CS_ASSERTED,
    QSPI_STATE_RX_WAITING_DATA_TO_RECEIVE,
    QSPI_STATE_RX_DATA_CS_ASSERTED,
    QSPI_STATE_RX_COMPLETED,

    QSPI_STATE_TX_STARTED,
    QSPI_STATE_TX_HEADER_CS_ASSERTED,
    QSPI_STATE_TX_WAITING_DATA_TO_SEND,
    QSPI_STATE_TX_DATA_CS_ASSERTED,
    QSPI_STATE_TX_COMPLETED,

    QSPI_STATE_LAST
} qspi_state_e;


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
extern volatile qspi_packet_header_t g_qspi_packet_header_rx;
extern volatile qspi_state_e g_qspi_state;


//-----------------------------------------------------------------------------
// Function declarations
//-----------------------------------------------------------------------------
int qspi_dma_send_packet(uint8_t *data, uint32_t data_size, uint8_t packet_type);
int qspi_dma_set_rx_data(uint8_t *data, uint32_t data_size);
int qspi_dma_trigger(void);
int qspi_dma_wait(qspi_state_e qspi_state);
int qspi_dma_slave_init(void);


#endif /* _MAX78000_SPI_DMA_H_ */
