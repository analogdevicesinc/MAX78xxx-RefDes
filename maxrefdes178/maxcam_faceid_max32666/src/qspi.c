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

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include "spi.h"

#include "qspi.h"
#include "definitions.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define QSPI              SPI0
#define QSPI_IRQ_HANDLER  SPI0_IRQHandler
#define QSPI_REGS         MXC_SPI17Y_GET_SPI17Y(QSPI)
#define QSPI_IRQ          MXC_SPI17Y_GET_IRQ(QSPI)
#define QSPI_SPEED        500000UL
#define QSPI_RX_BUFF_LEN  (1024 * 10)


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
static uint8_t       qspi_rx_buff[QSPI_RX_BUFF_LEN];
static qspi_header_t qspi_header;
static volatile int  qspi_header_flag;
static volatile int  qspi_data_flag;


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
void QSPI_IRQ_HANDLER(void)
{
    SPI_Handler(QSPI);
}

void qspi_haeder_callback(void *req, int error)
{
    qspi_header_flag = error;
}

void qspi_data_callback(void *req, int error)
{
    qspi_data_flag = error;
}

void qspi_init()
{
    sys_cfg_spi_t qspi_slave_cfg;

    qspi_slave_cfg.map = MAP_A;
    qspi_slave_cfg.ss0 = Enable;
    qspi_slave_cfg.ss1 = Disable;
    qspi_slave_cfg.ss2 = Disable;
    qspi_slave_cfg.num_io = 4;
    // Setup the interrupt
    NVIC_EnableIRQ(QSPI_IRQ);

    // Configure the peripheral
    if (SPI_Init(QSPI, 0, QSPI_SPEED, qspi_slave_cfg) != 0) {
        printf("Error configuring QSPI\n");
    }

//    SPI_Enable(SPI);
}

void qspi_worker()
{
    spi_req_t qspi_req = {0};

    memset(&qspi_header, 0, sizeof(qspi_header));
    memset(qspi_rx_buff, 0, sizeof(qspi_rx_buff));

    SPI_Clear_fifo(QSPI);

    printf("Waiting for QSPI transaction\n");

    qspi_req.tx_data = NULL;
    qspi_req.rx_data = (uint8_t *) &qspi_header;
    qspi_req.len = sizeof(qspi_header);
    qspi_req.bits = 8;
    qspi_req.width = SPI17Y_WIDTH_4;
    qspi_req.ssel = 0;
    qspi_req.deass = 0;
    qspi_req.ssel_pol = SPI17Y_POL_LOW;
    qspi_req.tx_num = 0;
    qspi_req.rx_num = 0;
    qspi_req.callback = qspi_haeder_callback;
    qspi_header_flag = 1;
    SPI_SlaveTransAsync(QSPI, &qspi_req);
    while (qspi_header_flag == 1);

    if (qspi_header.start_byte != QSPI_START_BYTE) {
        printf("Invalid QSPI start byte 0x%02hhX\n", qspi_header.start_byte);
    }

    if ((qspi_header.data_len == 0) || (qspi_header.data_len > QSPI_RX_BUFF_LEN)) {
        printf("Invalid QSPI data len %u\n", qspi_header.data_len);
    }

    qspi_req.tx_data = NULL;
    qspi_req.rx_data = qspi_rx_buff;
    qspi_req.len = qspi_header.data_len;
    qspi_req.bits = 8;
    qspi_req.width = SPI17Y_WIDTH_4;
    qspi_req.ssel = 0;
    qspi_req.deass = 0;
    qspi_req.ssel_pol = SPI17Y_POL_LOW;
    qspi_req.tx_num = 0;
    qspi_req.rx_num = 0;
    qspi_req.callback = qspi_data_callback;
    qspi_data_flag = 1;
    SPI_SlaveTransAsync(QSPI, &qspi_req);
    while (qspi_data_flag == 1);

    printf("QSPI transaction completed %u\n", qspi_req.rx_num);

    for (unsigned int i = 0; i < qspi_req.rx_num; i++) {
        printf("0x%02hhX ", qspi_rx_buff[i]);
    }
    printf("\n");
}
