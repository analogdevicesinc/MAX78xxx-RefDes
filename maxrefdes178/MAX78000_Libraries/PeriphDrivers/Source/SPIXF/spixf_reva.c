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
 *************************************************************************** */

/****** Includes *******/
#include <stddef.h>
#include <stdint.h>
#include "mxc_device.h"
#include "mxc_assert.h"
#include "mxc_lock.h"
#include "mxc_sys.h"
#include "spixf.h"
#include "spixfc_fifo_regs.h"
#include "spixfc_regs.h"
#include "spixfm_regs.h"
#include "spixf_reva.h"


/***** Definitions *****/
#define MXC_SPIXF_HEADER_DEASS_SS       1
#define MAX_SCLK                        0x10
#define MXC_SPIXF_HEADER_TX_DIR         1
#define NOT_HEADER_DATA                 0xF000      // 0xF makes sure it is not a header
#define MXC_SPIXF_MAX_BYTE_LEN          32
#define MXC_SPIXF_MAX_PAGE_LEN          32
#define MXC_SPIXF_NUM_BYTES_PER_PAGE    32

/* Bit positions for the 16-BIT Header. */
#define MXC_SPIXF_HEADER_DIRECTION_POS  0
#define MXC_SPIXF_HEADER_UNITS_POS      2
#define MXC_SPIXF_HEADER_SIZE_POS       4
#define MXC_SPIXF_HEADER_WIDTH_POS      9
#define MXC_SPIXF_HEADER_DEASS_SS_POS   13

uint8_t mxc_spixf_busy(void);
static int SPIXFC_ReadRXFIFO(mxc_spixfc_fifo_regs_t* fifo, uint8_t* data, int len);
static void SPIXFC_TransHandler(mxc_spixf_req_t* req);

/******* Globals *******/
typedef struct {
    mxc_spixf_req_t* req;
    unsigned int head_rem;
} spixf_req_head_t;
static spixf_req_head_t states;

/****** Functions ******/
uint8_t mxc_spixf_busy(void)
{
    if (MXC_SPIXF_RevA_GetSSDriveOutput() == 1) {
        return 1;
    }
    
    return 0;
}

int MXC_SPIXF_RevA_Init(uint32_t cmdval, uint32_t frequency)
{
    states.req = NULL;
    states.head_rem = 0;
    MXC_SPIXFC->gen_ctrl = 0;
    MXC_SPIXF_RevA_RXFIFOEnable();
    MXC_SPIXF_RevA_TXFIFOEnable();
    MXC_SPIXF_RevA_SCKFeedbackEnable();
    MXC_SPIXF_RevA_SetSPIFrequency(frequency);
    MXC_SPIXF_RevA_SetSSActiveTime(MXC_SPIXF_SYS_CLOCKS_0);
    MXC_SPIXF_RevA_SetSSInactiveTime(MXC_SPIXF_SYS_CLOCKS_1);
    MXC_SPIXF_RevA_SetCmdValue(cmdval);
    MXC_SPIXF_RevA_SetCmdWidth(MXC_SPIXF_SINGLE_SDIO);
    MXC_SPIXF_RevA_SetAddrWidth(MXC_SPIXF_SINGLE_SDIO);
    MXC_SPIXF_RevA_SetDataWidth(MXC_SPIXF_SINGLE_SDIO);
    MXC_SPIXF_RevA_Set3ByteAddr();
    MXC_SPIXF_RevA_SetMode(MXC_SPIXF_MODE_0);
    MXC_SPIXF_RevA_SetPageSize(MXC_SPIXF_32B);
    MXC_SPIXF_RevA_SetSSPolActiveLow();
    return E_NO_ERROR;
}

void MXC_SPIXF_RevA_Shutdown(void)
{
    mxc_spixf_req_t* temp_req;
    MXC_SPIXF_RevA_DisableInt(0x3F);
    MXC_SPIXF_RevA_ClearFlags(MXC_SPIXFC->int_fl);
    MXC_SPIXF_RevA_Disable();
    MXC_SPIXF_RevA_TXFIFODisable();
    MXC_SPIXF_RevA_RXFIFODisable();
    
    // Call all of the pending callbacks for this SPIXFC
    if (states.req != NULL) {
    
        // Save the request
        temp_req = states.req;
        
        // Unlock this SPIXFC
        MXC_FreeLock((uint32_t*) &states.req);
        
        // Callback if not NULL
        if (temp_req->callback != NULL) {
            temp_req->callback(temp_req, E_SHUTDOWN);
        }
    }
}

void MXC_SPIXF_RevA_IOCtrl(mxc_spixf_ds_t sclk_ds, mxc_spixf_ds_t ss_ds, mxc_spixf_ds_t sdio_ds, mxc_spixf_pup_t pupdctrl)
{
    MXC_SPIXFM->io_ctrl = 0;
    
    if (sclk_ds) {
        MXC_SPIXF_RevA_SetIoctrlSCLKDriveHigh();
    }
    else {
        MXC_SPIXF_RevA_SetIoctrlSCLKDriveLow();
    }
    
    if (ss_ds) {
        MXC_SPIXF_RevA_SetIoctrlSSDriveHigh();
    }
    else {
        MXC_SPIXF_RevA_SetIoctrlSSDriveLow();
    }
    
    if (sdio_ds) {
        MXC_SPIXF_RevA_SetIoctrlSDIODriveHigh();
    }
    
    MXC_SPIXF_RevA_SetPuPdCtrl(pupdctrl);
}

int MXC_SPIXF_RevA_Clocks(uint32_t len, uint8_t deass)
{
    mxc_spixfc_fifo_regs_t* fifo;
    uint16_t header = MXC_SPIXF_HEADER_TX_DIR;
    uint32_t num = len;
    
    // Make sure the SPIXFC has been initialized
    MXC_ASSERT(MXC_SPIXF_RevA_IsEnabled());
    
    if (len == 0) {
        return E_NO_ERROR;
    }
    
    // Lock this SPIXFC
    if (MXC_GetLock((uint32_t*) &states.req, 1) != E_NO_ERROR) {
        return E_BUSY;
    }
    
    // Wait for any previous data to transmit
    while (MXC_SPIXFC->fifo_ctrl & MXC_F_SPIXFC_FIFO_CTRL_TX_FIFO_CNT);
    
    // Disable the feedback clock
    MXC_SPIXF_RevA_SCKFeedbackDisable();
    
    // Get the TX and RX FIFO for this SPIXFC
    fifo = MXC_SPIXFC_FIFO;
    
    // Send the headers to transmit the clocks
    while (len > 32) {
        fifo->tx_16 = header;
        fifo->tx_16 = NOT_HEADER_DATA;
        fifo->tx_16 = NOT_HEADER_DATA;
        len -= 32;
    }
    
    if (len) {
        if (len < 32) {
            header |= (len << MXC_SPIXF_HEADER_SIZE_POS);
        }
        
        header |= (deass << MXC_SPIXF_HEADER_DEASS_SS_POS);
        
        fifo->tx_16 = header;
        
        if (len > 16) {
            fifo->tx_16 = NOT_HEADER_DATA;
        }
        
        fifo->tx_16 = NOT_HEADER_DATA;
    }
    
    // Wait for all of the data to transmit
    while (MXC_SPIXFC->fifo_ctrl & MXC_F_SPIXFC_FIFO_CTRL_TX_FIFO_CNT);
    
    // Enable the feedback clock
    MXC_SPIXF_RevA_SCKFeedbackEnable();
    
    // Unlock this SPIXFC
    MXC_FreeLock((uint32_t*) &states.req);
    
    return num;
}

int MXC_SPIXF_RevA_Transaction(mxc_spixf_req_t* req)
{
    int remain, temp_read;
    uint32_t pages, bytes;
    uint8_t read, write;
    mxc_spixfc_fifo_regs_t* fifo;
    uint16_t header;
    
    // Check the input parameters
    MXC_ASSERT(req != NULL);
    MXC_ASSERT((req->rx_data != NULL) || (req->tx_data != NULL));
    
    // Make sure the SPIXFC has been initialized
    MXC_ASSERT(MXC_SPIXF_RevA_IsEnabled());
    
    if (req->len == 0) {
        return E_NO_ERROR;
    }
    
    // Lock this SPIXFC
    if (MXC_GetLock((uint32_t*) &states.req, 1) != E_NO_ERROR) {
        return E_BUSY;
    }
    
    // Clear the number of bytes counter
    req->read_num = 0;
    req->write_num = 0;
    states.head_rem = 0;
    
    // Figure out if we're reading and/or writing
    read = (req->rx_data != NULL);
    write = (req->tx_data != NULL);
    
    // Get the TX and RX FIFO for this SPIXFC
    fifo = MXC_SPIXFC_FIFO;
    
    remain = req->len;
    
    while (remain) {
        // Set the transaction configuration in the header
        header = ((write << MXC_SPIXF_HEADER_DIRECTION_POS)  |
                  (read << (MXC_SPIXF_HEADER_DIRECTION_POS + 1)) |
                  (req->width << MXC_SPIXF_HEADER_WIDTH_POS));
                  
        if (remain >= MXC_SPIXF_MAX_BYTE_LEN) {
            // Send a 32 byte header
            if (remain == MXC_SPIXF_MAX_BYTE_LEN) {
                // 0 maps to 32 in the header,...
                header |= ((MXC_SPIXF_HEADER_UNITS_BYTES << MXC_SPIXF_HEADER_UNITS_POS) |
                           (req->deass << MXC_SPIXF_HEADER_DEASS_SS_POS));
                           
                bytes = MXC_SPIXF_MAX_BYTE_LEN;
                
            }
            else {
                // Send in increments of 32 byte pages
                header |= (MXC_SPIXF_HEADER_UNITS_PAGES << MXC_SPIXF_HEADER_UNITS_POS);
                pages = remain / MXC_SPIXF_NUM_BYTES_PER_PAGE;
                
                if (pages >= MXC_SPIXF_MAX_PAGE_LEN) {
                    // No need to set num pages field in header since 0 maps to MXC_SPIXF_MAX_PAGE_LEN in the header
                    // There are 32 bytes per page
                    bytes = MXC_SPIXF_NUM_BYTES_PER_PAGE * MXC_SPIXF_MAX_PAGE_LEN;
                }
                else {
                    header |= (pages << MXC_SPIXF_HEADER_SIZE_POS);
                    bytes = pages * MXC_SPIXF_NUM_BYTES_PER_PAGE;
                }
                
                // Check if this is the last header we will send
                if ((remain - bytes) == 0) {
                    header |= (req->deass << MXC_SPIXF_HEADER_DEASS_SS_POS);
                }
            }
            
            fifo->tx_16 = header;
            
            // Save the number of bytes we need to write to the FIFO
            states.head_rem = bytes;
            remain -= bytes;
        }
        else {
            // Send final header with the number of bytes remaining and if
            // we want to de-assert the SS at the end of the transaction
            header |= ((MXC_SPIXF_HEADER_UNITS_BYTES << MXC_SPIXF_HEADER_UNITS_POS) |
                       (remain << MXC_SPIXF_HEADER_SIZE_POS) |
                       (req->deass << MXC_SPIXF_HEADER_DEASS_SS_POS));
                       
            fifo->tx_16 = header;
            
            states.head_rem = remain;
            remain = 0;
        }
        
        /* *** */
        while (states.head_rem) {
            if (write) {
            
                // Wait for there to be room in the TXFIFO
                while (((MXC_SPIXFC->fifo_ctrl & MXC_F_SPIXFC_FIFO_CTRL_TX_FIFO_CNT)
                        >> MXC_F_SPIXFC_FIFO_CTRL_TX_FIFO_CNT_POS) > (MXC_CFG_SPIXFC_FIFO_DEPTH - 2))  {
                        
                }
                
                if (states.head_rem > 1) {
                    fifo->tx_16 = ((req->tx_data[req->write_num + 1] << 8) | (req->tx_data[req->write_num]));
                    req->write_num += 2;
                }
                else {
                    fifo->tx_16 = (NOT_HEADER_DATA | req->tx_data[req->write_num]);
                    req->write_num++;
                }
            }
            
            if (read) {
                // Read two bytes
                if (states.head_rem > 1) {
                
                    temp_read = 0;
                    
                    do {
                        temp_read = SPIXFC_ReadRXFIFO(fifo, &req->rx_data[req->read_num], 1);
                    }
                    while (temp_read != 1);
                    
                    req->read_num += 1;
                }
                
                do {
                    temp_read = SPIXFC_ReadRXFIFO(fifo, &req->rx_data[req->read_num], 1);
                }
                while (temp_read != 1);
                
                req->read_num += 1;
            }
            
            if (states.head_rem > 1) {
                states.head_rem -= 1;
            }
            
            states.head_rem -= 1;
        }
    } // end of while(remain)
    
    // Unlock this SPIXFC
    MXC_FreeLock((uint32_t*) &states.req);
    
    if (write) {
        return req->write_num;
    }
    
    return req->read_num;
}

int MXC_SPIXF_RevA_TransactionAsync(mxc_spixf_req_t* req)
{
    // Check the input parameters
    MXC_ASSERT(req != NULL);
    MXC_ASSERT((req->rx_data != NULL) || (req->tx_data != NULL));
    
    // Make sure the SPIXFC has been initialized
    MXC_ASSERT(MXC_SPIXF_RevA_IsEnabled());
    
    if (req->len == 0) {
        return E_NO_ERROR;
    }
    
    // Attempt to register this write request
    if (MXC_GetLock((uint32_t*) &states.req, (uint32_t) req) != E_NO_ERROR) {
        return E_BUSY;
    }
    
    // Clear the number of bytes counter
    req->read_num = 0;
    req->write_num = 0;
    states.head_rem = 0;
    
    // Start the write
    SPIXFC_TransHandler(req);
    
    return E_NO_ERROR;
}

int MXC_SPIXF_RevA_AbortAsync(mxc_spixf_req_t* req)
{
    mxc_spixf_req_t* temp_req;
    
    // Check the input parameters
    if (req == NULL) {
        return E_BAD_PARAM;
    }
    
    // Find the request, set to NULL
    if (req == states.req) {
    
        MXC_SPIXFC->int_en = 0;
        MXC_SPIXFC->int_fl = MXC_SPIXFC->int_fl;
        
        // Save the request
        temp_req = states.req;
        
        // Unlock this SPIXFC
        MXC_FreeLock((uint32_t*) &states.req);
        
        // Callback if not NULL
        if (temp_req->callback != NULL) {
            temp_req->callback(temp_req, E_ABORT);
        }
        
        return E_NO_ERROR;
    }
    
    return E_BAD_PARAM;
}

void MXC_SPIXF_RevA_Handler(void)
{
    uint32_t flags;
    
    // Clear the interrupt flags
    MXC_SPIXFC->int_en = 0;
    flags = MXC_SPIXFC->int_fl;
    MXC_SPIXFC->int_fl = flags;
    
    // Figure out if this SPIXFC has an active request
    if ((states.req != NULL) && (flags)) {
        SPIXFC_TransHandler(states.req);
    }
}

__attribute__((noinline))  static int SPIXFC_ReadRXFIFO(mxc_spixfc_fifo_regs_t* fifo,
        uint8_t* data, int len)
{
    int num = 0;
    
    // Get data from the RXFIFO
    while ((MXC_SPIXFC->fifo_ctrl & MXC_F_SPIXFC_FIFO_CTRL_RX_FIFO_CNT) && (len - num)) {
        // Save data from the RXFIFO
        data[num] = fifo->rx_8;
        num++;
    }
    
    return num;
}

static void SPIXFC_TransHandler(mxc_spixf_req_t* req)
{
    int remain;
    uint8_t read, write;
    uint16_t header;
    uint32_t pages, bytes, inten;
    unsigned int bytes_read;
    mxc_spixfc_fifo_regs_t* fifo;
    mxc_spixf_req_t* temp_req;
    
    inten = 0;
    
    // Get the FIFOS for this spi
    fifo = MXC_SPIXFC_FIFO;
    
    // Figure out if we're reading
    if (req->rx_data != NULL) {
        read = 1;
    }
    else {
        read = 0;
    }
    
    // Figure out if we're writing
    if (req->tx_data != NULL) {
        write = 1;
    }
    else {
        write = 0;
    }
    
    // Read byte from the FIFO if we are reading
    if (read) {
    
        // Read all of the data in the RXFIFO, or until we don't need anymore
        bytes_read = SPIXFC_ReadRXFIFO(fifo, &req->rx_data[req->read_num],
                                       (req->len - req->read_num));
                                       
        req->read_num += bytes_read;
        
        // Adjust head_rem if we are only reading
        if (!write && (states.head_rem > 0)) {
            states.head_rem -= bytes_read;
        }
        
        // Figure out how many byte we have left to read
        if (states.head_rem > 0) {
            remain = states.head_rem;
        }
        else {
            remain = req->len - req->read_num;
        }
        
        if (remain) { // setting up int levels and RX interrupt flag...
        
            // Set the RX interrupts
            if (remain > MXC_CFG_SPIXFC_FIFO_DEPTH) {    // FIFO Almost FULL level = 16-2 = 14;
                MXC_SPIXFC->fifo_ctrl = ((MXC_SPIXFC->fifo_ctrl & ~MXC_F_SPIXFC_FIFO_CTRL_RX_FIFO_AF_LVL) |
                                         ((MXC_CFG_SPIXFC_FIFO_DEPTH - 2) << MXC_F_SPIXFC_FIFO_CTRL_RX_FIFO_AF_LVL_POS));
                                         
            }
            else {                                      // FIFO Almost Full level = remain-1;
                MXC_SPIXFC->fifo_ctrl = ((MXC_SPIXFC->fifo_ctrl & ~MXC_F_SPIXFC_FIFO_CTRL_RX_FIFO_AF_LVL) |
                                         ((remain - 1) << MXC_F_SPIXFC_FIFO_CTRL_RX_FIFO_AF_LVL_POS));
            }
            
            inten |= MXC_F_SPIXFC_INT_EN_RX_FIFO_AF;
        }
    }
    
    // Figure out how many bytes we have left to send headers for
    if (write) {
        remain = req->len - req->write_num;
    }
    else {
        remain = req->len - req->read_num;
    }
    
    // See if we need to send a new header
    if (states.head_rem <= 0 && remain) {
    
    
        // Set the transaction configuration in the header
        header = ((write << MXC_SPIXF_HEADER_DIRECTION_POS)  |
                  (read << (MXC_SPIXF_HEADER_DIRECTION_POS + 1)) |
                  (req->width << MXC_SPIXF_HEADER_WIDTH_POS));
                  
        if (remain >= MXC_SPIXF_MAX_BYTE_LEN) {
        
            // Send a 32 byte header
            if (remain == MXC_SPIXF_MAX_BYTE_LEN) {
                header |= ((MXC_SPIXF_HEADER_UNITS_BYTES << MXC_SPIXF_HEADER_UNITS_POS) |
                           (req->deass << MXC_SPIXF_HEADER_DEASS_SS_POS));
                // Save the number of bytes we need to write to the FIFO
                bytes = MXC_SPIXF_MAX_BYTE_LEN;
            }
            else {
                // Send in increments of 32 byte pages
                header |= (MXC_SPIXF_HEADER_UNITS_PAGES << MXC_SPIXF_HEADER_UNITS_POS);
                pages = remain / MXC_SPIXF_NUM_BYTES_PER_PAGE;
                
                if (pages >= MXC_SPIXF_MAX_PAGE_LEN) {
                    // 0 maps to 32 in the header
                    bytes = MXC_SPIXF_NUM_BYTES_PER_PAGE * MXC_SPIXF_MAX_PAGE_LEN;
                }
                else {
                    header |= (pages << MXC_SPIXF_HEADER_SIZE_POS);
                    bytes = pages * MXC_SPIXF_NUM_BYTES_PER_PAGE;
                }
                
                // Check if this is the last header we will send
                if ((remain - bytes) == 0) {
                    header |= (req->deass << MXC_SPIXF_HEADER_DEASS_SS_POS);
                }
            }
            
            fifo->tx_16 = header;
            
            // Save the number of bytes we need to write to the FIFO
            states.head_rem = bytes;
            
        }
        else {
            // Send final header with the number of bytes remaining and if
            // we want to de-assert the SS at the end of the transaction
            header |= ((MXC_SPIXF_HEADER_UNITS_BYTES << MXC_SPIXF_HEADER_UNITS_POS) |
                       (remain << MXC_SPIXF_HEADER_SIZE_POS) |
                       (req->deass << MXC_SPIXF_HEADER_DEASS_SS_POS));
            fifo->tx_16 = header;
            states.head_rem = remain;
        }
    }
    
    // Put data into the FIFO if we are writing
    remain = req->len - req->write_num;
    
    if (write && states.head_rem) {  // from above... the num of bytes...
    
        // Fill the FIFO
        while ((((MXC_SPIXFC->fifo_ctrl & MXC_F_SPIXFC_FIFO_CTRL_TX_FIFO_CNT) >>
                 MXC_F_SPIXFC_FIFO_CTRL_TX_FIFO_CNT_POS) < (MXC_CFG_SPIXFC_FIFO_DEPTH - 2)) && (states.head_rem))  {
                 
            if (states.head_rem > 1) {
            
                // Write 2 byte at a time
                fifo->tx_16 = ((req->tx_data[req->write_num + 1] << 8) |
                               (req->tx_data[req->write_num]));
                               
                req->write_num += 2;
                states.head_rem -= 2;
            }
            else {
            
                // Write the last byte
                fifo->tx_16 = (NOT_HEADER_DATA | req->tx_data[req->write_num]);
                
                req->write_num += 1;
                states.head_rem -= 1;
            }
        }
        
        remain = req->len - req->write_num;
        
        // Set the TX interrupts
        if (remain) {
            // Set the TX FIFO almost empty interrupt if we have to refill
            MXC_SPIXFC->fifo_ctrl = ((MXC_SPIXFC->fifo_ctrl & ~MXC_F_SPIXFC_FIFO_CTRL_TX_FIFO_AE_LVL) |
                                     ((MXC_CFG_SPIXFC_FIFO_DEPTH - 2) << MXC_F_SPIXFC_FIFO_CTRL_TX_FIFO_AE_LVL_POS));
                                     
            inten |= MXC_F_SPIXFC_INT_EN_TX_FIFO_AE;
        }
        else if (req->wait_tx) {
            // Set the TX Ready interrupt if we're waiting for the FIFO to empty
            inten |= MXC_F_SPIXFC_INT_FL_TX_READY;
            req->wait_tx = 0;
            
            // Wait for transaction to completely finish by just returning and waiting for next interrupt.
            MXC_SPIXFC->int_en = inten;
            return;
        }
    }
    
    // Check to see if we've finished reading and writing
    if (((read && (req->read_num == req->len)) || !read) &&
            (((req->write_num == req->len) && !req->wait_tx) || !write)) {
            
        // clear interrupts
        MXC_SPIXFC->int_fl = MXC_SPIXFC->int_fl;
        
        // Save the request
        temp_req = states.req;
        
        // Unlock this SPIXFC
        MXC_FreeLock((uint32_t*) &states.req);
        
        // Callback if not NULL
        if (temp_req->callback != NULL) {
            temp_req->callback(temp_req, E_NO_ERROR);
        }
    }
    
    // Enable the SPIXFC interrupts
    MXC_SPIXFC->int_en = inten;
}

int MXC_SPIXF_RevA_ReadyForSleep(void)
{
    // Check to see if there are any ongoing transactions
    if ((states.req == NULL) &&
            !(MXC_SPIXFC->fifo_ctrl & MXC_F_SPIXFC_FIFO_CTRL_TX_FIFO_CNT)) {
            
        // Disable interrupts, clear flags
        MXC_SPIXFC->int_en = 0;
        MXC_SPIXFC->int_fl = MXC_SPIXFC->int_fl;
        
        return E_NO_ERROR;
    }
    
    return E_BUSY;
}

int MXC_SPIXF_RevA_EnableInt(uint32_t mask)
{
    mask &= (MXC_F_SPIXFC_INT_EN_TX_STALLED |  MXC_F_SPIXFC_INT_EN_RX_STALLED | MXC_F_SPIXFC_INT_EN_TX_READY |
             MXC_F_SPIXFC_INT_EN_RX_DONE | MXC_F_SPIXFC_INT_EN_TX_FIFO_AE | MXC_F_SPIXFC_INT_EN_RX_FIFO_AF);
             
    if (!mask) {
        /* No bits set? Wasn't something we can enable. */
        return E_BAD_PARAM;
    }
    
    MXC_SPIXFC->int_en |= mask;
    return E_NO_ERROR;
}

int MXC_SPIXF_RevA_DisableInt(uint32_t mask)
{
    mask &= (MXC_F_SPIXFC_INT_EN_TX_STALLED |  MXC_F_SPIXFC_INT_EN_RX_STALLED | MXC_F_SPIXFC_INT_EN_TX_READY |
             MXC_F_SPIXFC_INT_EN_RX_DONE | MXC_F_SPIXFC_INT_EN_TX_FIFO_AE | MXC_F_SPIXFC_INT_EN_RX_FIFO_AF);
             
    if (!mask) {
        /* No bits set? Wasn't something we can disable. */
        return E_BAD_PARAM;
    }
    
    MXC_SPIXFC->int_en &= (~mask);
    return E_NO_ERROR;
}

int MXC_SPIXF_RevA_ClearFlags(uint32_t flags)
{
    flags &= (MXC_F_SPIXFC_INT_FL_TX_STALLED |  MXC_F_SPIXFC_INT_FL_RX_STALLED | MXC_F_SPIXFC_INT_FL_TX_READY |
              MXC_F_SPIXFC_INT_FL_RX_DONE | MXC_F_SPIXFC_INT_FL_TX_FIFO_AE | MXC_F_SPIXFC_INT_FL_RX_FIFO_AF);
              
    if (!flags) {
        /* No bits set? Wasn't a flag we can clear. */
        return E_BAD_PARAM;
    }
    
    MXC_SPIXFC->int_fl = flags;
    return E_NO_ERROR;
}

int MXC_SPIXF_RevA_GetFlags(void)
{
    return MXC_SPIXFC->int_fl;
}

/* ************************************************ */
//Low level
/* ************************************************ */

int MXC_SPIXF_RevA_SetMode(mxc_spixf_mode_t mode)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SETFIELD(MXC_SPIXFM->cfg, MXC_F_SPIXFM_CFG_MODE, mode << MXC_F_SPIXFM_CFG_MODE_POS);
    MXC_SETFIELD(MXC_SPIXFC->cfg, MXC_F_SPIXFC_CFG_MODE, mode << MXC_F_SPIXFC_CFG_MODE_POS);
    return E_NO_ERROR;
}

mxc_spixf_mode_t MXC_SPIXF_RevA_GetMode(void)
{
    return ((MXC_SPIXFC->cfg & MXC_F_SPIXFC_CFG_MODE) >> MXC_F_SPIXFC_CFG_MODE_POS);
}

int MXC_SPIXF_RevA_SetSSPolActiveHigh(void)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFM->cfg &= ~MXC_F_SPIXFM_CFG_SSPOL;
    MXC_SPIXFC->ss_pol |= MXC_F_SPIXFC_SS_POL_SS_POLARITY;
    return E_NO_ERROR;
}

int MXC_SPIXF_RevA_SetSSPolActiveLow(void)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFM->cfg |= MXC_F_SPIXFM_CFG_SSPOL;
    MXC_SPIXFC->ss_pol &= (~MXC_F_SPIXFC_SS_POL_SS_POLARITY);
    return E_NO_ERROR;
}

int MXC_SPIXF_RevA_GetSSPolarity(void)
{
    return !!(MXC_SPIXFC->ss_pol & MXC_F_SPIXFC_SS_POL_SS_POLARITY);
}

int MXC_SPIXF_RevA_SetSPIFrequency(unsigned int hz)
{
    uint32_t clocks, hi_clk, lo_clk;
    
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    // Check if frequency is too high
    if (hz > PeripheralClock) {
        return E_BAD_PARAM;
    }
    
    // Make sure that we can generate this frequency
    clocks = (PeripheralClock / (2 * hz));
    hi_clk = clocks / 2;
    lo_clk = clocks / 2;
    
    if (clocks % 2) {
        hi_clk += 1;
    }
    
    if (hi_clk == 0) {
        MXC_SPIXFM->cfg |= MXC_F_SPIXFM_CFG_LO_CLK;
        MXC_SPIXFM->cfg |= MXC_F_SPIXFM_CFG_HI_CLK;
        MXC_SPIXFC->cfg |= MXC_F_SPIXFC_CFG_LO_CLK;
        MXC_SPIXFC->cfg |= MXC_F_SPIXFC_CFG_HI_CLK;
    }
    else if (hi_clk > 15) {
        MXC_SETFIELD(MXC_SPIXFM->cfg, MXC_F_SPIXFM_CFG_LO_CLK, 1 << MXC_F_SPIXFM_CFG_LO_CLK_POS);
        MXC_SETFIELD(MXC_SPIXFM->cfg, MXC_F_SPIXFM_CFG_HI_CLK, 1 << MXC_F_SPIXFM_CFG_HI_CLK_POS);
        MXC_SETFIELD(MXC_SPIXFC->cfg, MXC_F_SPIXFC_CFG_LO_CLK, 1 << MXC_F_SPIXFC_CFG_LO_CLK_POS);
        MXC_SETFIELD(MXC_SPIXFC->cfg, MXC_F_SPIXFC_CFG_HI_CLK, 1 << MXC_F_SPIXFC_CFG_HI_CLK_POS);
    }
    else {
        MXC_SETFIELD(MXC_SPIXFM->cfg, MXC_F_SPIXFM_CFG_LO_CLK, lo_clk << MXC_F_SPIXFM_CFG_LO_CLK_POS);
        MXC_SETFIELD(MXC_SPIXFM->cfg, MXC_F_SPIXFM_CFG_HI_CLK, hi_clk << MXC_F_SPIXFM_CFG_HI_CLK_POS);
        MXC_SETFIELD(MXC_SPIXFC->cfg, MXC_F_SPIXFC_CFG_LO_CLK, lo_clk << MXC_F_SPIXFC_CFG_LO_CLK_POS);
        MXC_SETFIELD(MXC_SPIXFC->cfg, MXC_F_SPIXFC_CFG_HI_CLK, hi_clk << MXC_F_SPIXFC_CFG_HI_CLK_POS);
    }
    
    return MXC_SPIXF_RevA_GetSPIFrequency();
}

uint32_t MXC_SPIXF_RevA_GetSPIFrequency(void)
{
    uint32_t clocks;
    clocks = ((MXC_SPIXFM->cfg & MXC_F_SPIXFM_CFG_HI_CLK) >> MXC_F_SPIXFM_CFG_HI_CLK_POS);
    
    // Avoid divide by zero
    if (clocks == 0) {
        clocks = 0x10;
    }
    
    return PeripheralClock / (2 * clocks);
}

int MXC_SPIXF_RevA_SetSSActiveTime(mxc_spixf_ssact_t ssact)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SETFIELD(MXC_SPIXFM->cfg, MXC_F_SPIXFM_CFG_SSACT, ssact << MXC_F_SPIXFM_CFG_SSACT_POS);
    MXC_SETFIELD(MXC_SPIXFC->cfg, MXC_F_SPIXFC_CFG_SSACT, ssact << MXC_F_SPIXFC_CFG_SSACT_POS);
    return E_NO_ERROR;
}

mxc_spixf_ssact_t MXC_SPIXF_RevA_GetSSActiveTime(void)
{
    return ((MXC_SPIXFC->cfg & MXC_F_SPIXFC_CFG_SSACT) >> MXC_F_SPIXFC_CFG_SSACT_POS);
}

int MXC_SPIXF_RevA_SetSSInactiveTime(mxc_spixf_ssiact_t ssiact)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SETFIELD(MXC_SPIXFM->cfg, MXC_F_SPIXFM_CFG_SSIACT, ssiact << MXC_F_SPIXFM_CFG_SSIACT_POS);
    MXC_SETFIELD(MXC_SPIXFC->cfg, MXC_F_SPIXFC_CFG_SSIACT, ssiact << MXC_F_SPIXFC_CFG_SSIACT_POS);
    return E_NO_ERROR;
}

mxc_spixf_ssiact_t MXC_SPIXF_RevA_GetSSInactiveTime(void)
{
    return ((MXC_SPIXFC->cfg & MXC_F_SPIXFC_CFG_SSIACT) >> MXC_F_SPIXFC_CFG_SSIACT_POS);
}

int MXC_SPIXF_RevA_SetCmdWidth(mxc_spixf_spiwidth_t width)
{

    MXC_ASSERT(width < MXC_SPIXF_INVALID);
    
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SETFIELD(MXC_SPIXFM->fetch_ctrl, MXC_F_SPIXFM_FETCH_CTRL_CMD_WIDTH, width << MXC_F_SPIXFM_FETCH_CTRL_CMD_WIDTH_POS);
    return E_NO_ERROR;
}

mxc_spixf_spiwidth_t MXC_SPIXF_RevA_GetCmdWidth(void)
{
    return ((MXC_SPIXFM->fetch_ctrl & MXC_F_SPIXFM_FETCH_CTRL_CMD_WIDTH) >> MXC_F_SPIXFM_FETCH_CTRL_CMD_WIDTH_POS);
}

int MXC_SPIXF_RevA_SetAddrWidth(mxc_spixf_spiwidth_t width)
{
    MXC_ASSERT(width < MXC_SPIXF_INVALID);
    
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SETFIELD(MXC_SPIXFM->fetch_ctrl, MXC_F_SPIXFM_FETCH_CTRL_ADDR_WIDTH, width << MXC_F_SPIXFM_FETCH_CTRL_ADDR_WIDTH_POS);
    return E_NO_ERROR;
}

mxc_spixf_spiwidth_t MXC_SPIXF_RevA_GetAddrWidth(void)
{
    return ((MXC_SPIXFM->fetch_ctrl & MXC_F_SPIXFM_FETCH_CTRL_ADDR_WIDTH) >> MXC_F_SPIXFM_FETCH_CTRL_ADDR_WIDTH_POS);
}

int MXC_SPIXF_RevA_SetDataWidth(mxc_spixf_spiwidth_t width)
{
    MXC_ASSERT(width < MXC_SPIXF_INVALID);
    
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SETFIELD(MXC_SPIXFM->fetch_ctrl, MXC_F_SPIXFM_FETCH_CTRL_DATA_WIDTH, width << MXC_F_SPIXFM_FETCH_CTRL_DATA_WIDTH_POS);
    return E_NO_ERROR;
}

mxc_spixf_spiwidth_t MXC_SPIXF_RevA_GetDataWidth(void)
{
    return ((MXC_SPIXFM->fetch_ctrl & MXC_F_SPIXFM_FETCH_CTRL_DATA_WIDTH) >> MXC_F_SPIXFM_FETCH_CTRL_DATA_WIDTH_POS);
}

int MXC_SPIXF_RevA_Set4ByteAddr(void)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFM->fetch_ctrl |= MXC_F_SPIXFM_FETCH_CTRL_FOUR_BYTE_ADDR;
    return E_NO_ERROR;
}

int MXC_SPIXF_RevA_Set3ByteAddr(void)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFM->fetch_ctrl &= (~MXC_F_SPIXFM_FETCH_CTRL_FOUR_BYTE_ADDR);
    return E_NO_ERROR;
}

unsigned int MXC_SPIXF_RevA_GetBytesPerAddr(void)
{
    if (!!(MXC_SPIXFM->fetch_ctrl & MXC_F_SPIXFM_FETCH_CTRL_FOUR_BYTE_ADDR)) {
        return 4;
    }
    else {
        return 3;
    }
}

int MXC_SPIXF_RevA_SetModeClk(uint8_t mdclk)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SETFIELD(MXC_SPIXFM->mode_ctrl, MXC_F_SPIXFM_MODE_CTRL_MDCLK, mdclk);
    return E_NO_ERROR;
}

uint8_t MXC_SPIXF_RevA_GetModeClk(void)
{
    return ((MXC_SPIXFM->mode_ctrl & MXC_F_SPIXFM_MODE_CTRL_MDCLK) >> MXC_F_SPIXFM_MODE_CTRL_MDCLK_POS);
}

int MXC_SPIXF_RevA_SetCmdModeEveryTrans(void)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFM->mode_ctrl |= MXC_F_SPIXFM_MODE_CTRL_NO_CMD;
    return E_NO_ERROR;
}

int MXC_SPIXF_RevA_SetCmdModeFirstTrans(void)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFM->mode_ctrl &= (~MXC_F_SPIXFM_MODE_CTRL_NO_CMD);
    return E_NO_ERROR;
}

mxc_spixf_cmd_t MXC_SPIXF_RevA_GetCmdMode(void)
{
    if (!!(MXC_SPIXFM->mode_ctrl & MXC_F_SPIXFM_MODE_CTRL_NO_CMD)) {
        return MXC_SPIXF_CMD_FIRST_TRANS;
    }
    
    return MXC_SPIXF_CMD_EVERY_TRANS;
}

int MXC_SPIXF_RevA_BBDataOutputEnable(uint8_t mask)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFC->gen_ctrl |= (mask << MXC_F_SPIXFC_GEN_CTRL_BB_DATA_OUT_EN_POS);
    return E_NO_ERROR;
}

int MXC_SPIXF_RevA_BBDataOutputDisable(uint8_t mask)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFC->gen_ctrl &= ~(mask << MXC_F_SPIXFC_GEN_CTRL_BB_DATA_OUT_EN_POS);
    return E_NO_ERROR;
}

uint8_t MXC_SPIXF_RevA_BBDataOutputIsEnabled(void)
{
    return ((MXC_SPIXFC->gen_ctrl & MXC_F_SPIXFC_GEN_CTRL_BB_DATA_OUT_EN) >> MXC_F_SPIXFC_GEN_CTRL_BB_DATA_OUT_EN_POS);
}

uint8_t MXC_SPIXF_RevA_GetBBDataOutputValue(void)
{
    return ((MXC_SPIXFC->gen_ctrl & MXC_F_SPIXFC_GEN_CTRL_BB_DATA) >> MXC_F_SPIXFC_GEN_CTRL_BB_DATA_POS);
}

uint8_t MXC_SPIXF_RevA_GetBBDataInputValue(void)
{
    return ((MXC_SPIXFC->gen_ctrl & MXC_F_SPIXFC_GEN_CTRL_SDIO_DATA_IN) >> MXC_F_SPIXFC_GEN_CTRL_SDIO_DATA_IN_POS);
}

int MXC_SPIXF_RevA_SetModeData(uint16_t data)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SETFIELD(MXC_SPIXFM->mode_data, MXC_F_SPIXFM_MODE_DATA_DATA, data << MXC_F_SPIXFM_MODE_DATA_DATA_POS);
    return E_NO_ERROR;
}

uint16_t MXC_SPIXF_RevA_GetModeData(void)
{
    return ((MXC_SPIXFM->mode_data & MXC_F_SPIXFM_MODE_DATA_DATA) >> MXC_F_SPIXFM_MODE_DATA_DATA_POS);
}

int MXC_SPIXF_RevA_SetSCKInverted(void)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFM->fb_ctrl |= MXC_F_SPIXFM_FB_CTRL_INVERT_EN;
    MXC_SPIXFC->gen_ctrl |= MXC_F_SPIXFC_GEN_CTRL_SCLK_FB_INVERT;
    return E_NO_ERROR;
}

int MXC_SPIXF_RevA_SetSCKNonInverted(void)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFM->fb_ctrl  &= (~MXC_F_SPIXFM_FB_CTRL_INVERT_EN);
    MXC_SPIXFC->gen_ctrl  &= (~MXC_F_SPIXFC_GEN_CTRL_SCLK_FB_INVERT);
    return E_NO_ERROR;
}

int MXC_SPIXF_RevA_GetSCKInverted(void)
{
    return !!(MXC_SPIXFM->fb_ctrl & MXC_F_SPIXFM_FB_CTRL_INVERT_EN);
}

int MXC_SPIXF_RevA_SCKFeedbackEnable(void)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFM->fb_ctrl |= MXC_F_SPIXFM_FB_CTRL_FB_EN;
    MXC_SPIXFC->gen_ctrl |= MXC_F_SPIXFC_GEN_CTRL_SCLK_FB;
    return E_NO_ERROR;
    
}

int MXC_SPIXF_RevA_SCKFeedbackDisable(void)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFM->fb_ctrl  &= (~MXC_F_SPIXFM_FB_CTRL_FB_EN);
    MXC_SPIXFC->gen_ctrl  &= (~MXC_F_SPIXFC_GEN_CTRL_SCLK_FB);
    return E_NO_ERROR;
}

int MXC_SPIXF_RevA_SCKFeedbackIsEnabled(void)
{
    return !!(MXC_SPIXFM->fb_ctrl & MXC_F_SPIXFM_FB_CTRL_FB_EN);
}

int MXC_SPIXF_RevA_SetSCKSampleDelay(uint8_t delay)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SETFIELD(MXC_SPIXFC->cfg, MXC_F_SPIXFC_CFG_IOSMPL, delay << MXC_F_SPIXFC_CFG_IOSMPL_POS);
    return E_NO_ERROR;
}

uint8_t MXC_SPIXF_RevA_GetSCKSampleDelay(void)
{
    return ((MXC_SPIXFC->cfg & MXC_F_SPIXFC_CFG_IOSMPL) >> MXC_F_SPIXFC_CFG_IOSMPL_POS);
}

int MXC_SPIXF_RevA_SetCmdValue(uint8_t cmdval)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SETFIELD(MXC_SPIXFM->fetch_ctrl, MXC_F_SPIXFM_FETCH_CTRL_CMDVAL, cmdval << MXC_F_SPIXFM_FETCH_CTRL_CMDVAL_POS);
    return E_NO_ERROR;
}

uint8_t MXC_SPIXF_RevA_GetCmdValue(void)
{
    return ((MXC_SPIXFM->fetch_ctrl & MXC_F_SPIXFM_FETCH_CTRL_CMDVAL) >> MXC_F_SPIXFM_FETCH_CTRL_CMDVAL_POS);
}

void MXC_SPIXF_RevA_SetPageSize(mxc_spixf_page_size_t size)
{
    MXC_SETFIELD(MXC_SPIXFC->cfg, MXC_F_SPIXFC_CFG_PAGE_SIZE, size << MXC_F_SPIXFC_CFG_PAGE_SIZE_POS);
}

mxc_spixf_page_size_t MXC_SPIXF_RevA_GetPageSize(void)
{
    return ((MXC_SPIXFC->cfg & MXC_F_SPIXFC_CFG_PAGE_SIZE) >> MXC_F_SPIXFC_CFG_PAGE_SIZE_POS);
}

int MXC_SPIXF_RevA_SimpleRXEnabled(void)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFC->gen_ctrl |= MXC_F_SPIXFC_GEN_CTRL_SIMPLE_RX;
    return E_NO_ERROR;
}

int MXC_SPIXF_RevA_SimpleRXDisable(void)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFC->gen_ctrl  &= (~MXC_F_SPIXFC_GEN_CTRL_SIMPLE_RX);
    return E_NO_ERROR;
}

int MXC_SPIXF_RevA_SimpleRXIsEnabled(void)
{
    return !!(MXC_SPIXFC->gen_ctrl & MXC_F_SPIXFC_GEN_CTRL_SIMPLE_RX);
}

int MXC_SPIXF_RevA_SimpleModeEnable(void)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFC->gen_ctrl |= MXC_F_SPIXFC_GEN_CTRL_SIMPLE;
    return E_NO_ERROR;
    
}

int MXC_SPIXF_RevA_SimpleModeDisable(void)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFC->gen_ctrl  &= (~MXC_F_SPIXFC_GEN_CTRL_SIMPLE);
    return E_NO_ERROR;
}

int MXC_SPIXF_RevA_SimpleModeIsEnabled(void)
{
    return !!(MXC_SPIXFC->gen_ctrl & MXC_F_SPIXFC_GEN_CTRL_SIMPLE);
}

int MXC_SPIXF_RevA_SampleOutputEnable(uint8_t mask)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFC->sp_ctrl |= (mask << MXC_F_SPIXFC_SP_CTRL_SDIO_OUT_EN_POS);
    return E_NO_ERROR;;
}

int MXC_SPIXF_RevA_SampleOutputDisable(uint8_t mask)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFC->sp_ctrl &= ~(mask << MXC_F_SPIXFC_SP_CTRL_SDIO_OUT_EN_POS);
    return E_NO_ERROR;
}

uint8_t MXC_SPIXF_RevA_SampleOutputIsEnabled(void)
{
    return ((MXC_SPIXFC->sp_ctrl & MXC_F_SPIXFC_SP_CTRL_SDIO_OUT_EN) >> MXC_F_SPIXFC_SP_CTRL_SDIO_OUT_EN_POS);
}

uint8_t MXC_SPIXF_RevA_GetSampleOutputValue(void)
{
    return ((MXC_SPIXFC->sp_ctrl & MXC_F_SPIXFC_SP_CTRL_SDIO_OUT) >> MXC_F_SPIXFC_SP_CTRL_SDIO_OUT_POS);
}

void MXC_SPIXF_RevA_SetIoctrlSDIODriveHigh()
{
    MXC_SPIXFM->io_ctrl |= MXC_F_SPIXFM_IO_CTRL_SDIO_DS;
}

void MXC_SPIXF_RevA_SetIoctrlSDIODriveLow()
{
    MXC_SPIXFM->io_ctrl &= (~MXC_F_SPIXFM_IO_CTRL_SDIO_DS);
}

uint8_t MXC_SPIXF_RevA_GetIoctrlSDIODrive(void)
{
    return !!(MXC_SPIXFM->io_ctrl & MXC_F_SPIXFM_IO_CTRL_SDIO_DS);
}

void MXC_SPIXF_RevA_SetIoctrlSCLKDriveHigh()
{
    MXC_SPIXFM->io_ctrl |= MXC_F_SPIXFM_IO_CTRL_SCLK_DS;
}

void MXC_SPIXF_RevA_SetIoctrlSCLKDriveLow()
{
    MXC_SPIXFM->io_ctrl &= (~MXC_F_SPIXFM_IO_CTRL_SCLK_DS);
}

uint8_t MXC_SPIXF_RevA_GetIoctrlSCLKDrive(void)
{
    return !!(MXC_SPIXFM->io_ctrl & MXC_F_SPIXFM_IO_CTRL_SCLK_DS);
}

void MXC_SPIXF_RevA_SetIoctrlSSDriveHigh()
{
    MXC_SPIXFM->io_ctrl |= MXC_F_SPIXFM_IO_CTRL_SS_DS;
}

void MXC_SPIXF_RevA_SetIoctrlSSDriveLow()
{
    MXC_SPIXFM->io_ctrl &= (~MXC_F_SPIXFM_IO_CTRL_SS_DS);
}

uint8_t MXC_SPIXF_RevA_GetIoctrlSSDrive(void)
{
    return !!(MXC_SPIXFM->io_ctrl & MXC_F_SPIXFM_IO_CTRL_SS_DS);
}

void MXC_SPIXF_RevA_SetPuPdCtrl(mxc_spixf_pup_t pupd)
{
    MXC_SPIXFM->io_ctrl |= pupd;
}

uint8_t MXC_SPIXF_RevA_GetPuPdCtrl(void)
{
    return !!(MXC_SPIXFM->io_ctrl & MXC_F_SPIXFM_IO_CTRL_PU_PD_CTRL);
}

void MXC_SPIXF_RevA_SetSCKDriveHigh()
{
    MXC_SPIXFC->gen_ctrl |= MXC_F_SPIXFC_GEN_CTRL_SCLK_DR;
}

void MXC_SPIXF_RevA_SetSCKDriveLow()
{
    MXC_SPIXFC->gen_ctrl &= (~MXC_F_SPIXFC_GEN_CTRL_SCLK_DR);
}

uint8_t MXC_SPIXF_RevA_GetSCKDrive(void)
{
    return !!(MXC_SPIXFC->gen_ctrl & MXC_F_SPIXFC_GEN_CTRL_SCLK_DR);
}

void MXC_SPIXF_RevA_SetSSDriveOutputHigh(void)
{
    MXC_SPIXFC->gen_ctrl |= MXC_F_SPIXFC_GEN_CTRL_SCLK_DR;
    
}

void MXC_SPIXF_RevA_SetSSDriveOutputLow(void)
{
    MXC_SPIXFC->gen_ctrl &= (~MXC_F_SPIXFC_GEN_CTRL_SCLK_DR);
}

uint8_t MXC_SPIXF_RevA_GetSSDriveOutput(void)
{
    return !!(MXC_SPIXFC->gen_ctrl & MXC_F_SPIXFC_GEN_CTRL_SCLK_DR);
}

int MXC_SPIXF_RevA_BitBangModeEnable(void)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFC->gen_ctrl |= MXC_F_SPIXFC_GEN_CTRL_BBMODE;
    return E_NO_ERROR;
}

int MXC_SPIXF_RevA_BitBangModeDisable(void)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFC->gen_ctrl &= (~MXC_F_SPIXFC_GEN_CTRL_BBMODE);
    return E_NO_ERROR;
    
}

int MXC_SPIXF_RevA_BitBangModeIsEnabled(void)
{
    return !!(MXC_SPIXFC->gen_ctrl & MXC_F_SPIXFC_GEN_CTRL_BBMODE);
}

int MXC_SPIXF_RevA_RXFIFOEnable(void)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFC->gen_ctrl |= MXC_F_SPIXFC_GEN_CTRL_RX_FIFO_EN;
    return E_NO_ERROR;
}

int MXC_SPIXF_RevA_RXFIFODisable(void)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFC->gen_ctrl &= (~MXC_F_SPIXFC_GEN_CTRL_RX_FIFO_EN);
    return E_NO_ERROR;
}

int MXC_SPIXF_RevA_RXFIFOIsEnabled(void)
{
    return !!(MXC_SPIXFC->gen_ctrl & MXC_F_SPIXFC_GEN_CTRL_RX_FIFO_EN);
}

int MXC_SPIXF_RevA_TXFIFOEnable(void)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFC->gen_ctrl |= MXC_F_SPIXFC_GEN_CTRL_TX_FIFO_EN;
    return E_NO_ERROR;
}

int MXC_SPIXF_RevA_TXFIFODisable(void)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFC->gen_ctrl &= (~MXC_F_SPIXFC_GEN_CTRL_TX_FIFO_EN);
    return E_NO_ERROR;
}

int MXC_SPIXF_RevA_TXFIFOIsEnabled(void)
{
    return !!(MXC_SPIXFC->gen_ctrl & MXC_F_SPIXFC_GEN_CTRL_TX_FIFO_EN);
}

int MXC_SPIXF_RevA_Enable(void)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFC->gen_ctrl |= MXC_F_SPIXFC_GEN_CTRL_ENABLE;
    return E_NO_ERROR;
}

int MXC_SPIXF_RevA_Disable(void)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFC->gen_ctrl &= (~MXC_F_SPIXFC_GEN_CTRL_ENABLE);
    return E_NO_ERROR;
}

int MXC_SPIXF_RevA_IsEnabled(void)
{
    return !!(MXC_SPIXFC->gen_ctrl & MXC_F_SPIXFC_GEN_CTRL_ENABLE);
}

int MXC_SPIXF_RevA_SetBusIdle(unsigned int busidle)
{
    if (mxc_spixf_busy()) {
        return E_BUSY;
    }
    
    MXC_SPIXFM->bus_idle = busidle;
    return E_NO_ERROR;
}

unsigned int MXC_SPIXF_RevA_GetBusIdle(void)
{

    return MXC_SPIXFM->bus_idle;
}
