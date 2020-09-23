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
#include "faceid_definitions.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define QSPI              SPI0
#define QSPI_REGS         MXC_SPI17Y0
#define QSPI_IRQ          SPI0_IRQn


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
uint8_t       qspi_image_buff[IMAGE_SIZE];
char          resultString[RESULT_MAX_SIZE];
static qspi_header_t qspi_header;
static volatile int  qspi_flag;

gpio_cfg_t ai85_image_cs = {PORT_0, PIN_8, GPIO_FUNC_OUT, GPIO_PAD_NONE};


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
void SPI0_IRQHandler(void)
{
    SPI_Handler(QSPI);
}

void qspi_callback(void *req, int error)
{
    qspi_flag = error;
}

int qspi_init()
{
    sys_cfg_spi_t qspi_slave_cfg;

    qspi_slave_cfg.map = MAP_B;
    qspi_slave_cfg.ss0 = Enable;
    qspi_slave_cfg.ss1 = Disable;
    qspi_slave_cfg.ss2 = Disable;
    qspi_slave_cfg.num_io = 4;
    // Setup the interrupt
    NVIC_EnableIRQ(QSPI_IRQ);

    GPIO_Config(&ai85_image_cs);
    GPIO_OutClr(&ai85_image_cs);

    SPI_Shutdown(QSPI);

    // Configure the peripheral
    if (SPI_Init(QSPI, 0, QSPI_SPEED, qspi_slave_cfg) != 0) {
        printf("Error configuring QSPI\n");
    }

    return E_NO_ERROR;
}

int qspi_worker(void)
{
    spi_req_t qspi_req = {0};

    memset(&qspi_header, 0, sizeof(qspi_header_t));

    SPI_Clear_fifo(QSPI);

    printf("Waiting for QSPI transaction\n");

    qspi_req.tx_data = NULL;
    qspi_req.rx_data = (uint8_t *) &qspi_header;
    qspi_req.len = sizeof(qspi_header_t);
    qspi_req.bits = 8;
    qspi_req.width = SPI17Y_WIDTH_4;
    qspi_req.ssel = 0;
    qspi_req.deass = 0;
    qspi_req.ssel_pol = SPI17Y_POL_LOW;
    qspi_req.tx_num = 0;
    qspi_req.rx_num = 0;
    qspi_req.callback = qspi_callback;
    qspi_flag = 1;
    SPI_SlaveTransAsync(QSPI, &qspi_req);
    while (qspi_flag == 1);

    if (qspi_header.start_symbol != QSPI_START_SYMBOL) {
        printf("Invalid QSPI start byte 0x%08hhX\n", qspi_header.start_symbol);
        return E_BAD_STATE;
    }

    if ((qspi_header.data_len == 0) || (qspi_header.data_len > IMAGE_SIZE)) {
        printf("Invalid QSPI data len %u\n", qspi_header.data_len);
        return E_BAD_PARAM;
    }

    switch (qspi_header.command)
    {
        case QSPI_COMMAND_IMAGE:
            goto ImageReceive;
            break;
        case QSPI_COMMAND_RESULT:
            goto ResultReceive;
            break;
        default :
            return E_COMM_ERR;
    }

    ImageReceive:

    qspi_req.tx_data = NULL;
    qspi_req.rx_data = (void *)qspi_image_buff;
    qspi_req.len = qspi_header.data_len/2;
    qspi_req.bits = 8;
    qspi_req.width = SPI17Y_WIDTH_4;
    qspi_req.ssel = 0;
    qspi_req.deass = 0;
    qspi_req.ssel_pol = SPI17Y_POL_LOW;
    qspi_req.tx_num = 0;
    qspi_req.rx_num = 0;
    qspi_req.callback = qspi_callback;
    qspi_flag = 1;
    SPI_SlaveTransAsync(QSPI, &qspi_req);
    while (qspi_flag == 1);

    qspi_req.tx_data = NULL;
    qspi_req.rx_data = (void *)&(qspi_image_buff[qspi_header.data_len/2]);
    qspi_req.len = qspi_header.data_len - qspi_header.data_len/2;
    qspi_req.bits = 8;
    qspi_req.width = SPI17Y_WIDTH_4;
    qspi_req.ssel = 0;
    qspi_req.deass = 0;
    qspi_req.ssel_pol = SPI17Y_POL_LOW;
    qspi_req.tx_num = 0;
    qspi_req.rx_num = 0;
    qspi_req.callback = qspi_callback;
    qspi_flag = 1;
    SPI_SlaveTransAsync(QSPI, &qspi_req);
    while (qspi_flag == 1);

    printf("QSPI transaction completed %u\n", qspi_req.rx_num*2);

    return IMAGE_RECEIVED;

    ResultReceive:

    memset(resultString, 0, sizeof(resultString));

    qspi_req.tx_data = NULL;
    qspi_req.rx_data = (void *)resultString;
    qspi_req.len = qspi_header.data_len/2;
    qspi_req.bits = 8;
    qspi_req.width = SPI17Y_WIDTH_4;
    qspi_req.ssel = 0;
    qspi_req.deass = 0;
    qspi_req.ssel_pol = SPI17Y_POL_LOW;
    qspi_req.tx_num = 0;
    qspi_req.rx_num = 0;
    qspi_req.callback = qspi_callback;
    qspi_flag = 1;
    SPI_SlaveTransAsync(QSPI, &qspi_req);
    while (qspi_flag == 1);

    printf("QSPI transaction completed %u\n", qspi_req.rx_num);

    printf("Result : %s\n", resultString);

    return RESULT_RECEIVED;

}
