/* *****************************************************************************
 * Copyright (C) 2015 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2018-08-29 22:57:14 +0300 (Wed, 29 Aug 2018) $
 * $Revision: 37501 $
 *
 **************************************************************************** */

/* **** Includes **** */
#include <stddef.h>
#include <stdio.h>
#include "mxc_config.h"
#include "mxc_assert.h"
#include "mxc_lock.h"
#include "spixfc.h"
#include "spixf_fifo_regs.h"

/* **** Definitions **** */
#define SPIXFC_MAX_BYTE_LEN          32
#define SPIXFC_MAX_PAGE_LEN          32
#define SPIXFC_NUM_BYTES_PER_PAGE    32

/* **** Globals **** */

// Saves the state of the non-blocking requests
typedef struct {
    spixfc_req_t *req;
    unsigned int head_rem;
} spixfc_req_head_t;

static spixfc_req_head_t states;

/* **** Functions **** */

static int SPIXFC_ReadRXFIFO(mxc_spixfc_regs_t *spixfc, mxc_spixf_fifo_regs_t *fifo,
                            uint8_t *data, int len);

static void SPIXFC_TransHandler(mxc_spixfc_regs_t *spixfc, spixfc_req_t *req);


/* ************************************************************************** */
int SPIXFC_Init(mxc_spixfc_regs_t *spixfc, const spixfc_cfg_t *cfg, const sys_cfg_spixfc_t* sys_cfg)
{
    int err;
    uint32_t spixfc_clk, clocks;
    
    // Check the input parameters
    MXC_ASSERT(cfg != NULL);
    MXC_ASSERT(cfg->baud > 0);
    
    // Set system level configurations
    if ((err = SYS_SPIXFC_Init(spixfc, sys_cfg)) != E_NO_ERROR) {
        return err;
    }
    
    // Initialize state pointers
    
    states.req = NULL;
    states.head_rem = 0;
    
    // Drain the FIFOs, enable SPIXFC, enable SCK Feedback
    spixfc->gen_ctrl = 0;
    
    spixfc->gen_ctrl = (MXC_F_SPIXFC_GEN_CTRL_TX_FIFO_EN |
                       MXC_F_SPIXFC_GEN_CTRL_RX_FIFO_EN);
                       
    spixfc->gen_ctrl |= MXC_F_SPIXFC_GEN_CTRL_SCLK_FB;
    
    // Configure the mode and baud
    spixfc_clk = SYS_SPIXFC_GetFreq(spixfc);
    MXC_ASSERT(spixfc_clk > 0);
    
    // Make sure that we can generate this frequency
    clocks = (spixfc_clk / (2*cfg->baud));
    MXC_ASSERT((clocks > 0) && (clocks <= MAX_SCLK));
    
    // 0 => 16 in the 4 bit field for HI_CLK and LO_CLK
    if (clocks == MAX_SCLK) {
        clocks = 0;
    }
    
    // Set mode, baud, and page size
    spixfc->config = ((cfg->mode << MXC_F_SPIXFC_CONFIG_MODE_POS) |
                     ((clocks << MXC_F_SPIXFC_CONFIG_HI_CLK_POS) & MXC_F_SPIXFC_CONFIG_HI_CLK) |
                     ((clocks << MXC_F_SPIXFC_CONFIG_LO_CLK_POS) & MXC_F_SPIXFC_CONFIG_LO_CLK) |
                     MXC_S_SPIXFC_CONFIG_PAGE_32B);
                     
    // Configure the SSEL polarity
    spixfc->ss_pol |= (cfg->ssel_pol << MXC_F_SPIXFC_SS_POL_SS_POLARITY_POS) & MXC_F_SPIXFC_SS_POL_SS_POLARITY;
    
    spixfc->gen_ctrl |= MXC_F_SPIXFC_GEN_CTRL_ENABLE;
    
    return E_NO_ERROR;
}

/* ************************************************************************** */
void SPIXFC_Shutdown(mxc_spixfc_regs_t *spixfc)
{

    spixfc_req_t *temp_req;
    
    // Disable and clear interrupts
    spixfc->inten = 0;
    spixfc->intfl = spixfc->intfl;
    
    // Disable SPIXFC and FIFOS
    spixfc->gen_ctrl &= ~(MXC_F_SPIXFC_GEN_CTRL_ENABLE | MXC_F_SPIXFC_GEN_CTRL_TX_FIFO_EN |
                         MXC_F_SPIXFC_GEN_CTRL_RX_FIFO_EN);
                         
    // Call all of the pending callbacks for this SPIXFC
    if (states.req != NULL) {
    
        // Save the request
        temp_req = states.req;
        
        // Unlock this SPIXFC
        mxc_free_lock((uint32_t*)&states.req);
        
        // Callback if not NULL
        if (temp_req->callback != NULL) {
            temp_req->callback(temp_req, E_SHUTDOWN);
        }
    }
    
    SYS_SPIXFC_Shutdown(spixfc);
}

/* ************************************************************************** */
int SPIXFC_Clocks(mxc_spixfc_regs_t *spixfc, uint32_t len, uint8_t ssel, uint8_t deass)
{

    mxc_spixf_fifo_regs_t *fifo;
    uint16_t header = SPIXFC_HEADER_TX_DIR;
    uint32_t num = len;
    
    // Make sure the SPIXFC has been initialized
    MXC_ASSERT(spixfc->gen_ctrl & MXC_F_SPIXFC_GEN_CTRL_ENABLE);
    
    if (len == 0) {
        return E_NO_ERROR;
    }
    
    // Lock this SPIXFC
    if (mxc_get_lock((uint32_t*)&states.req, 1) != E_NO_ERROR) {
        return E_BUSY;
    }
    
    // Wait for any previous data to transmit
    while (spixfc->fifo_ctrl & MXC_F_SPIXFC_FIFO_CTRL_TX_FIFO_CNT) {
    }
    
    // Disable the feedback clock
    spixfc->gen_ctrl &= ~MXC_F_SPIXFC_GEN_CTRL_SCLK_FB;
    
    // Set which slave select we are using
    spixfc->config = ((spixfc->config& ~MXC_F_SPIXFC_CONFIG_SSEL) |
                     (ssel << MXC_F_SPIXFC_CONFIG_SSEL_POS));
                     
    // Get the TX and RX FIFO for this SPIXFC
    fifo = MXC_SPIXF_FIFO;
    
    // Send the headers to transmit the clocks
    while (len > 32) {
        fifo->tx_16 = header;
        fifo->tx_16 = NOT_HEADER_DATA;
        fifo->tx_16 = NOT_HEADER_DATA;
        len -= 32;
    }
    
    if (len) {
        if (len < 32) {
            header |= (len << SPIXFC_HEADER_SIZE_POS);
        }
        header |= (deass << SPIXFC_HEADER_DEASS_SS_POS);
        
        fifo->tx_16 = header;
        
        if (len > 16) {
            fifo->tx_16 = NOT_HEADER_DATA;
        }
        fifo->tx_16 = NOT_HEADER_DATA;
    }
    
    // Wait for all of the data to transmit
    while (spixfc->fifo_ctrl & MXC_F_SPIXFC_FIFO_CTRL_TX_FIFO_CNT) {
        
    }
    
    // Enable the feedback clock
    spixfc->gen_ctrl |= MXC_F_SPIXFC_GEN_CTRL_SCLK_FB;
    
    // Unlock this SPIXFC
    mxc_free_lock((uint32_t*)&states.req);
    
    return num;
}

/* ************************************************************************** */
int SPIXFC_Trans(mxc_spixfc_regs_t *spixfc, spixfc_req_t *req)
{
    int remain, temp_read;
    uint32_t pages, bytes;
    uint8_t read, write;
    mxc_spixf_fifo_regs_t *fifo;
    uint16_t header;
    
    
    // Check the input parameters
    MXC_ASSERT(req != NULL);
    MXC_ASSERT((req->rx_data != NULL) || (req->tx_data != NULL));
    
    // Make sure the SPIXFC has been initialized
    
    MXC_ASSERT(spixfc->gen_ctrl & MXC_F_SPIXFC_GEN_CTRL_ENABLE);
    
    if (req->len == 0) {
        return E_NO_ERROR;
    }
    
    // Lock this SPIXFC
    if (mxc_get_lock((uint32_t*)&states.req, 1) != E_NO_ERROR) {
        return E_BUSY;
    }
    
    // Set which slave select we are using
    
    spixfc->config = ((spixfc->config & ~MXC_F_SPIXFC_CONFIG_SSEL) |
                     (req->ssel << MXC_F_SPIXFC_CONFIG_SSEL_POS));
                     
    // Clear the number of bytes counter
    req->read_num = 0;
    req->write_num = 0;
    states.head_rem = 0;
    
    // Figure out if we're reading and/or writing
    read = (req->rx_data != NULL);
    write = (req->tx_data != NULL);
    
    // Get the TX and RX FIFO for this SPIXFC
    fifo = MXC_SPIXF_FIFO;
    
    remain = req->len;
    while (remain) {
    
        // Set the transaction configuration in the header
        header = ((write << SPIXFC_HEADER_DIRECTION_POS)  |
                  (read << (SPIXFC_HEADER_DIRECTION_POS+1)) |
                  (req->width << SPIXFC_HEADER_WIDTH_POS));
                  
        if (remain >= SPIXFC_MAX_BYTE_LEN) {
            // Send a 32 byte header
            if (remain == SPIXFC_MAX_BYTE_LEN) {
                // 0 maps to 32 in the header,...
                header |= ((SPIXFC_HEADER_UNITS_BYTES << SPIXFC_HEADER_UNITS_POS) |
                           (req->deass << SPIXFC_HEADER_DEASS_SS_POS));
                           
                bytes = SPIXFC_MAX_BYTE_LEN;
                
            } else {
                // Send in increments of 32 byte pages
                header |= (SPIXFC_HEADER_UNITS_PAGES << SPIXFC_HEADER_UNITS_POS);
                pages = remain / SPIXFC_NUM_BYTES_PER_PAGE;
                
                if (pages >= SPIXFC_MAX_PAGE_LEN) {
                    // No need to set num pages field in header since 0 maps to SPIXFC_MAX_PAGE_LEN in the header
                    // There are 32 bytes per page
                    bytes = SPIXFC_NUM_BYTES_PER_PAGE * SPIXFC_MAX_PAGE_LEN;
                } else {
                    header |= (pages << SPIXFC_HEADER_SIZE_POS);
                    bytes = pages * SPIXFC_NUM_BYTES_PER_PAGE;
                }
                
                // Check if this is the last header we will send
                if ((remain - bytes) == 0) {
                    header |= (req->deass << SPIXFC_HEADER_DEASS_SS_POS);
                }
            }
            
            fifo->tx_16 = header;
            
            // Save the number of bytes we need to write to the FIFO
            states.head_rem = bytes;
            remain -= bytes;
            
        } else {
            // Send final header with the number of bytes remaining and if
            // we want to de-assert the SS at the end of the transaction
            header |= ((SPIXFC_HEADER_UNITS_BYTES << SPIXFC_HEADER_UNITS_POS) |
                       (remain << SPIXFC_HEADER_SIZE_POS) |
                       (req->deass << SPIXFC_HEADER_DEASS_SS_POS));
                       
            fifo->tx_16 = header;
            
            states.head_rem = remain;
            remain = 0;
        }
        
        /* *** */
        
        while (states.head_rem) {
            if (write) {
            
                // Wait for there to be room in the TXFIFO
                while (((spixfc->fifo_ctrl & MXC_F_SPIXFC_FIFO_CTRL_TX_FIFO_CNT)
                        >> MXC_F_SPIXFC_FIFO_CTRL_TX_FIFO_CNT_POS) > (MXC_CFG_SPIXFC_FIFO_DEPTH - 2))  {
                            
                }
                        
                if (states.head_rem > 1) {
                    fifo->tx_16 = ((req->tx_data[req->write_num+1] << 8) | (req->tx_data[req->write_num]));
                    req->write_num += 2;
                } else {
                    fifo->tx_16 = (NOT_HEADER_DATA | req->tx_data[req->write_num]);
                    req->write_num++;
                }
                
            }
            if (read) {
            
                // Read two bytes
                if (states.head_rem > 1) {
                
                    temp_read = 0;
                    do {
                        temp_read = SPIXFC_ReadRXFIFO(spixfc, fifo, &req->rx_data[req->read_num], 1);
                    } while (temp_read != 1);
                    req->read_num += 1;
                }
                
                do {
                    temp_read = SPIXFC_ReadRXFIFO(spixfc, fifo, &req->rx_data[req->read_num], 1);
                } while (temp_read != 1);
                req->read_num += 1;
            }
            
            if (states.head_rem > 1) {
                states.head_rem -= 1;
            }
            
            states.head_rem -= 1;
        }
    }
    
    // Unlock this SPIXFC
    mxc_free_lock((uint32_t*)&states.req);
    
    if (write) {
        return req->write_num;
    }
    
    return req->read_num;
}

/* ************************************************************************** */
int SPIXFC_TransAsync(mxc_spixfc_regs_t *spixfc, spixfc_req_t *req)
{

    // Check the input parameters
    MXC_ASSERT(req != NULL);
    MXC_ASSERT((req->rx_data != NULL) || (req->tx_data != NULL));
    
    // Make sure the SPIXFC has been initialized
    MXC_ASSERT(spixfc->gen_ctrl & MXC_F_SPIXFC_GEN_CTRL_ENABLE);
    
    if (req->len == 0) {
        return E_NO_ERROR;
    }
    
    // Attempt to register this write request
    
    if (mxc_get_lock((uint32_t*)&states.req, (uint32_t)req) != E_NO_ERROR) {
        return E_BUSY;
    }
    
    // Set which slave select we are using
    
    spixfc->config = ((spixfc->config & ~MXC_F_SPIXFC_CONFIG_SSEL) |
                     (req->ssel << MXC_F_SPIXFC_CONFIG_SSEL_POS));
                     
    // Clear the number of bytes counter
    req->read_num = 0;
    req->write_num = 0;
    states.head_rem = 0;
    
    // Start the write
    SPIXFC_TransHandler(spixfc, req);
    
    return E_NO_ERROR;
}

/* ************************************************************************** */
int SPIXFC_AbortAsync(mxc_spixfc_regs_t *spixfc, spixfc_req_t *req)
{
    spixfc_req_t *temp_req;

    // Check the input parameters
    if (req == NULL) {
        return E_BAD_PARAM;
    }
    
    // Find the request, set to NULL
    
    if (req == states.req) {
    
        spixfc->inten = 0;
        spixfc->intfl = spixfc->intfl;
        
        // Save the request
        temp_req = states.req;
        
        // Unlock this SPIXFC
        mxc_free_lock((uint32_t*)&states.req);
        
        // Callback if not NULL
        if (temp_req->callback != NULL) {
            temp_req->callback(temp_req, E_ABORT);
        }
        
        return E_NO_ERROR;
    }
    
    
    return E_BAD_PARAM;
}

/* ************************************************************************** */
void SPIXFC_Handler(mxc_spixfc_regs_t *spixfc)
{

    uint32_t flags;
    
    // Clear the interrupt flags
    spixfc->inten = 0;
    flags = spixfc->intfl;
    spixfc->intfl = flags;
    
    
    // Figure out if this SPIXFC has an active request
    if ((states.req != NULL) && (flags)) {
        SPIXFC_TransHandler(spixfc, states.req);
    }
}

/* ************************************************************************** */
int SPIXFC_PrepForSleep(mxc_spixfc_regs_t *spixfc)
{
    // Check to see if there are any ongoing transactions
    if ((states.req == NULL) &&
            !(spixfc->fifo_ctrl & MXC_F_SPIXFC_FIFO_CTRL_TX_FIFO_CNT)) {
            
        // Disable interrupts, clear flags
        spixfc->inten = 0;
        spixfc->intfl = spixfc->intfl;
        
        return E_NO_ERROR;
    }
    
    return E_BUSY;
}

/* ************************************************************************** */
__attribute__((noinline))  static int SPIXFC_ReadRXFIFO(mxc_spixfc_regs_t *spixfc,
        mxc_spixf_fifo_regs_t *fifo,
        uint8_t *data, int len)
{
    int num = 0;
    
    // Get data from the RXFIFO
    while ((spixfc->fifo_ctrl & MXC_F_SPIXFC_FIFO_CTRL_RX_FIFO_CNT) && (len - num)) {
        // Save data from the RXFIFO
        data[num] = fifo->rx_8;
        num++;
    }
    return num;
}

/* ************************************************************************** */
static void SPIXFC_TransHandler(mxc_spixfc_regs_t *spixfc, spixfc_req_t *req)
{
    int remain;
    uint8_t read, write;
    uint16_t header;
    uint32_t pages, bytes, inten;
    unsigned int bytes_read;
    mxc_spixf_fifo_regs_t *fifo;
    spixfc_req_t *temp_req;
    
    inten = 0;
    
    // Get the FIFOS for this UART
    fifo = MXC_SPIXF_FIFO;
    
    // Figure out if we're reading
    if (req->rx_data != NULL) {
        read = 1;
    } else {
        read = 0; 
    }
    
    // Figure out if we're writing
    if (req->tx_data != NULL) {
        write = 1;
    } else {
        write = 0;
    }
    
    // Read byte from the FIFO if we are reading
    if (read) {
    
        // Read all of the data in the RXFIFO, or until we don't need anymore
        bytes_read = SPIXFC_ReadRXFIFO(spixfc, fifo, &req->rx_data[req->read_num],
                                      (req->len - req->read_num));
                                      
        req->read_num += bytes_read;
        
        // Adjust head_rem if we are only reading
        if (!write && (states.head_rem > 0)) {
            states.head_rem -= bytes_read;
        }
        
        // Figure out how many byte we have left to read
        if (states.head_rem > 0) {
            remain = states.head_rem;
        } else {
            remain = req->len - req->read_num;
        }
        
        if (remain) { // setting up int levels and RX interrupt flag...
        
            // Set the RX interrupts
            if (remain > MXC_CFG_SPIXFC_FIFO_DEPTH) {    // FIFO Almost FULL level = 16-2 = 14;
                spixfc->fifo_ctrl = ((spixfc->fifo_ctrl & ~MXC_F_SPIXFC_FIFO_CTRL_RX_FIFO_AF_LVL) |
                                    ((MXC_CFG_SPIXFC_FIFO_DEPTH - 2) << MXC_F_SPIXFC_FIFO_CTRL_RX_FIFO_AF_LVL_POS));
                                    
            } else {                                    // FIFO Almost Full level = remain-1;
                spixfc->fifo_ctrl = ((spixfc->fifo_ctrl & ~MXC_F_SPIXFC_FIFO_CTRL_RX_FIFO_AF_LVL) |
                                    ((remain - 1) << MXC_F_SPIXFC_FIFO_CTRL_RX_FIFO_AF_LVL_POS));
            }
            
            inten |= MXC_F_SPIXFC_INTEN_RX_FIFO_AF;
        }
    }
    
    // Figure out how many bytes we have left to send headers for
    if (write) {
        remain = req->len - req->write_num;
    } else {
        remain = req->len - req->read_num;
    }
    
    // See if we need to send a new header
    if (states.head_rem <= 0 && remain) {
    
    
        // Set the transaction configuration in the header
        header = ((write << SPIXFC_HEADER_DIRECTION_POS)  |
                  (read << (SPIXFC_HEADER_DIRECTION_POS+1)) |
                  (req->width << SPIXFC_HEADER_WIDTH_POS));
                  
        if (remain >= SPIXFC_MAX_BYTE_LEN) {
        
            // Send a 32 byte header
            if (remain == SPIXFC_MAX_BYTE_LEN) {
                header |= ((SPIXFC_HEADER_UNITS_BYTES << SPIXFC_HEADER_UNITS_POS) |
                           (req->deass << SPIXFC_HEADER_DEASS_SS_POS));
                // Save the number of bytes we need to write to the FIFO
                bytes = SPIXFC_MAX_BYTE_LEN;
            } else {
                // Send in increments of 32 byte pages
                header |= (SPIXFC_HEADER_UNITS_PAGES << SPIXFC_HEADER_UNITS_POS);
                pages = remain / SPIXFC_NUM_BYTES_PER_PAGE;
                
                if (pages >= SPIXFC_MAX_PAGE_LEN) {
                    // 0 maps to 32 in the header
                    bytes = SPIXFC_NUM_BYTES_PER_PAGE * SPIXFC_MAX_PAGE_LEN;
                } else {
                    header |= (pages << SPIXFC_HEADER_SIZE_POS);
                    bytes = pages * SPIXFC_NUM_BYTES_PER_PAGE;
                }
                // Check if this is the last header we will send
                if ((remain - bytes) == 0) {
                    header |= (req->deass << SPIXFC_HEADER_DEASS_SS_POS);
                }
            }
            
            fifo->tx_16 = header;
            
            // Save the number of bytes we need to write to the FIFO
            states.head_rem = bytes;
            
        } else {
            // Send final header with the number of bytes remaining and if
            // we want to de-assert the SS at the end of the transaction
            header |= ((SPIXFC_HEADER_UNITS_BYTES << SPIXFC_HEADER_UNITS_POS) |
                       (remain << SPIXFC_HEADER_SIZE_POS) |
                       (req->deass << SPIXFC_HEADER_DEASS_SS_POS));
            fifo->tx_16 = header;
            states.head_rem = remain;
        }
    }
    
   
    // Put data into the FIFO if we are writing
    remain = req->len - req->write_num;
    if (write && states.head_rem) {  // from above... the num of bytes...
    
        // Fill the FIFO
        while ((((spixfc->fifo_ctrl & MXC_F_SPIXFC_FIFO_CTRL_TX_FIFO_CNT) >>
                 MXC_F_SPIXFC_FIFO_CTRL_TX_FIFO_CNT_POS) < (MXC_CFG_SPIXFC_FIFO_DEPTH - 2)) &&(states.head_rem))  {
                 
            if (states.head_rem > 1) {
            
                // Write 2 byte at a time
                fifo->tx_16 = ((req->tx_data[req->write_num+1] << 8) |
                               (req->tx_data[req->write_num]));
                               
                req->write_num += 2;
                states.head_rem -= 2;
            } else {
            
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
            spixfc->fifo_ctrl = ((spixfc->fifo_ctrl & ~MXC_F_SPIXFC_FIFO_CTRL_TX_FIFO_AE_LVL) |
                                ((MXC_CFG_SPIXFC_FIFO_DEPTH - 2) << MXC_F_SPIXFC_FIFO_CTRL_TX_FIFO_AE_LVL_POS));
                                
            inten |= MXC_F_SPIXFC_INTEN_TX_FIFO_AE;
            
        } else if (req->wait_tx) {
        
            // Set the TX Ready interrupt if we're waiting for the FIFO to empty
            inten |= MXC_F_SPIXFC_INTFL_TX_READY;
            req->wait_tx = 0;
            
            // Wait for transaction to completely finish by just returning and waiting for next interrupt.
            spixfc->inten = inten;    
            return;
        }
    }
    
    // Check to see if we've finished reading and writing
    if (((read && (req->read_num == req->len)) || !read) &&
            (((req->write_num == req->len) && !req->wait_tx) || !write)) {
            
        // clear interrupts
        spixfc->intfl = spixfc->intfl;
        
        // Save the request
        temp_req = states.req;
        
        // Unlock this SPIXFC
        mxc_free_lock((uint32_t*)&states.req);
        
        // Callback if not NULL
        if (temp_req->callback != NULL) {
            temp_req->callback(temp_req, E_NO_ERROR);
        }
    }
    
    // Enable the SPIXFC interrupts
    spixfc->inten = inten;    
}
