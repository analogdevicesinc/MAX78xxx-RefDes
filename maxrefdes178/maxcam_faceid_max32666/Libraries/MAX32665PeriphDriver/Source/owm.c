/* *****************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2018-08-29 01:03:02 +0300 (Wed, 29 Aug 2018) $
 * $Revision: 37424 $
 *
 **************************************************************************** */

/* **** Includes **** */
#include <string.h>
#include "mxc_assert.h"
#include "mxc_sys.h"
#include "owm.h"

/* **** Definitions **** */
#define OWM_CLK_FREQ    1000000 //1-Wire requires 1MHz clock

/* **** Globals **** */
int LastDiscrepancy;
int LastDeviceFlag;

/* **** Functions **** */
static uint8_t CalculateCRC8(uint8_t* data, int len);
static uint8_t update_crc8(uint8_t crc, uint8_t value);

/* ************************************************************************* */
int OWM_Init(const owm_cfg_t *cfg, const sys_cfg_owm_t* sys_cfg)
{
    int err = 0;
    uint32_t owm_clk, clk_div = 0;
    uint32_t ext_pu_en = 0;
    
    if (cfg == NULL) {
        return E_NULL_PTR;
    }
    
    // Set system level configurations
    if ((err = SYS_OWM_Init(sys_cfg)) != E_NO_ERROR) {
        return err;
    }
    
    // Configure clk divisor to get 1MHz OWM clk
    owm_clk = SYS_OWM_GetFreq();
    
    if (owm_clk == 0) {
        SYS_OWM_Shutdown();
        return E_UNINITIALIZED;
    }
    
    // Return error if clk doesn't divide evenly to 1MHz
    if (owm_clk % OWM_CLK_FREQ) {
        SYS_OWM_Shutdown();
        return E_NOT_SUPPORTED;
    }
    
    clk_div = (owm_clk / (OWM_CLK_FREQ));
    
    // Can not support lower frequencies
    if (clk_div == 0) {
        SYS_OWM_Shutdown();
        return E_NOT_SUPPORTED;
    }
    
    // Select the PU mode and polarity based on cfg input
    switch (cfg->ext_pu_mode) {
        case OWM_EXT_PU_ACT_HIGH:
            ext_pu_en = 1; // EXT_PULLUP_MODE_USED;
            break;
        case OWM_EXT_PU_ACT_LOW:
            ext_pu_en = 1; // EXT_PULLUP_MODE_USED;
            break;
        case OWM_EXT_PU_UNUSED:
            ext_pu_en = 0; // EXT_PULLUP_MODE_UNUSED;
            break;
        default:
            SYS_OWM_Shutdown();
            return E_BAD_PARAM;
    }
    
    // Set clk divisor
    MXC_OWM->clk_div_1us = (clk_div << MXC_F_OWM_CLK_DIV_1US_DIVISOR_POS) &
                           MXC_F_OWM_CLK_DIV_1US_DIVISOR;
                           
    // Set configuration
    MXC_OWM->cfg = (((cfg->int_pu_en << MXC_F_OWM_CFG_INT_PULLUP_ENABLE_POS) & MXC_F_OWM_CFG_INT_PULLUP_ENABLE) |
                    ((ext_pu_en << MXC_F_OWM_CFG_EXT_PULLUP_ENABLE_POS) & MXC_F_OWM_CFG_EXT_PULLUP_ENABLE) |
                    ((cfg->long_line_mode << MXC_F_OWM_CFG_LONG_LINE_MODE_POS) & MXC_F_OWM_CFG_LONG_LINE_MODE));
               
    // If external pullup is enabled, set the mode                    
    if(ext_pu_en) {
        MXC_SETFIELD(MXC_OWM->cfg, MXC_F_OWM_CFG_EXT_PULLUP_MODE, cfg->ext_pu_mode);
    }

    // Clear all interrupt flags
    MXC_OWM->intfl = MXC_OWM->intfl;
    
    return E_NO_ERROR;
}

/* ************************************************************************* */
void OWM_Shutdown(void)
{
    // Disable and clear interrupts
    MXC_OWM->inten = 0;
    MXC_OWM->intfl = MXC_OWM->intfl;
    SYS_OWM_Shutdown();
}

/* ************************************************************************* */
int OWM_Reset(void)
{
    MXC_OWM->intfl = MXC_F_OWM_INTFL_OW_RESET_DONE;                 // Clear the reset flag
    MXC_OWM->ctrl_stat |= MXC_F_OWM_CTRL_STAT_START_OW_RESET;       // Generate a reset pulse
    while ((MXC_OWM->intfl & MXC_F_OWM_INTFL_OW_RESET_DONE) == 0);  // Wait for reset time slot to complete
            
    return (!!(MXC_OWM->ctrl_stat & MXC_F_OWM_CTRL_STAT_PRESENCE_DETECT)); // Return presence pulse detect status
}

/* ************************************************************************* */
int OWM_TouchByte(uint8_t data)
{
    MXC_OWM->cfg &= ~MXC_F_OWM_CFG_SINGLE_BIT_MODE;                             // Set to 8 bit mode
    MXC_OWM->intfl = (MXC_F_OWM_INTFL_TX_DATA_EMPTY | MXC_F_OWM_INTEN_LINE_SHORT |
                      MXC_F_OWM_INTFL_RX_DATA_READY);                           // Clear the flags
    MXC_OWM->data = (data << MXC_F_OWM_DATA_TX_RX_POS) & MXC_F_OWM_DATA_TX_RX;  // Write data
    while ((MXC_OWM->intfl & MXC_F_OWM_INTFL_TX_DATA_EMPTY) == 0);              // Wait for data to be sent
    while ((MXC_OWM->intfl & MXC_F_OWM_INTFL_RX_DATA_READY) == 0);              // Wait for data to be read
   
    // Check error flag
    if (MXC_OWM->intfl & MXC_F_OWM_INTEN_LINE_SHORT) {
        return E_COMM_ERR; // Wire was low before transaction
    }                
    return (MXC_OWM->data >> MXC_F_OWM_DATA_TX_RX_POS) & 0xFF; // Return the data read
}

/* ************************************************************************* */
int OWM_WriteByte(uint8_t data)
{
    // Send one byte of data and verify the data sent = data parameter
    return (OWM_TouchByte(data) == data) ? E_NO_ERROR : E_COMM_ERR;
}

/* ************************************************************************* */
int OWM_ReadByte(void)
{
    // Read one byte of data
    return OWM_TouchByte(0xFF);
}

/* ************************************************************************* */
int OWM_TouchBit(uint8_t bit)
{
    MXC_OWM->cfg |= MXC_F_OWM_CFG_SINGLE_BIT_MODE;                               // Set to 1 bit mode
    MXC_OWM->intfl = (MXC_F_OWM_INTFL_TX_DATA_EMPTY | MXC_F_OWM_INTEN_LINE_SHORT |
                      MXC_F_OWM_INTFL_RX_DATA_READY);                            // Clear the flags
    MXC_OWM->data = (bit << MXC_F_OWM_DATA_TX_RX_POS) & MXC_F_OWM_DATA_TX_RX;    // Write data
    while ((MXC_OWM->intfl & MXC_F_OWM_INTFL_TX_DATA_EMPTY) == 0);               // Wait for data to be sent
    while ((MXC_OWM->intfl & MXC_F_OWM_INTFL_RX_DATA_READY) == 0);               // Wait for data to be read
            
    // Check error flag
    if (MXC_OWM->intfl & MXC_F_OWM_INTEN_LINE_SHORT) {
        return E_COMM_ERR; // Wire was low before transaction
    }     
    return (MXC_OWM->data >> MXC_F_OWM_DATA_TX_RX_POS) & 0x1; // Return the bit read
}

/* ************************************************************************* */
int OWM_WriteBit(uint8_t bit)
{
    // Send a bit and verify the bit sent = bit parameter
    return (OWM_TouchBit(bit) == bit) ? E_NO_ERROR : E_COMM_ERR;
}

/* ************************************************************************* */
int OWM_ReadBit(void)
{
    // Read a bit
    return OWM_TouchBit(1);
}

/* ************************************************************************* */
int OWM_Write(uint8_t* data, int len)
{
    int num = 0;
    
    MXC_OWM->cfg &= ~MXC_F_OWM_CFG_SINGLE_BIT_MODE; // Set to 8 bit mode
    
    while (num < len) {  // Loop for number of bytes to write
        
        if(OWM_WriteByte(data[num]) != E_NO_ERROR) {
            return  E_COMM_ERR;
        }
        
        num++; // Keep track of how many bytes written
    }
    
    return num; // Return number of bytes written
}

/* ************************************************************************* */
int OWM_Read(uint8_t* data, int len)
{
    int num = 0;
    
    MXC_OWM->cfg &= ~MXC_F_OWM_CFG_SINGLE_BIT_MODE; // Set to 8 bit mode
    
    while (num < len) { // Loop for number of bytes to read
        // Store read data into buffer
        data[num] = OWM_ReadByte();
        
        num++; // Keep track of how many bytes read
    }
    
    return num; // Return number of bytes read
}

/* ************************************************************************* */
int OWM_ReadROM(uint8_t* ROMCode)
{
    int num_read = 0;
    
    // Send reset and wait for presence pulse
    if (OWM_Reset()) {
        // Send Read ROM command code
        if (OWM_WriteByte(READ_ROM_COMMAND) == E_NO_ERROR) {
            // Read 8 bytes and store in buffer
            num_read = OWM_Read(ROMCode, 8);
            
            // Check the number of bytes read
            if (num_read != 8) {
                return E_COMM_ERR;
            }
        } else {
            // Write failed
            return E_COMM_ERR;
        }
    } else {
        // No presence pulse
        return E_COMM_ERR;
    }
    
    return E_NO_ERROR;
}

/* ************************************************************************* */
int OWM_MatchROM(uint8_t* ROMCode)
{
    int num_wrote = 0;
    
    // Send reset and wait for presence pulse
    if (OWM_Reset()) {
        // Send match ROM command code
        if (OWM_WriteByte(MATCH_ROM_COMMAND) == E_NO_ERROR) {
            // Write 8 bytes in ROMCode buffer
            num_wrote = OWM_Write(ROMCode, 8);
            
            // Check the number of bytes written
            if (num_wrote != 8) {
                return E_COMM_ERR;
            }
        } else {
            // Write failed
            return E_COMM_ERR;
        }
    } else {
        // No presence pulse
        return E_COMM_ERR;
    }
    
    return E_NO_ERROR;
}

/* ************************************************************************* */
int OWM_ODMatchROM(uint8_t* ROMCode)
{
    int num_wrote = 0;
    
    // Set to standard speed
    MXC_OWM->cfg &= ~(MXC_F_OWM_CFG_OVERDRIVE);
    
    // Send reset and wait for presence pulse
    if (OWM_Reset()) {
        // Send Overdrive match ROM command code
        if (OWM_WriteByte(OD_MATCH_ROM_COMMAND) == E_NO_ERROR) {
            // Set overdrive
            MXC_OWM->cfg |= MXC_F_OWM_CFG_OVERDRIVE;
            
            // Write 8 bytes in ROMCode buffer
            num_wrote = OWM_Write(ROMCode, 8);
            
            // Check the number of bytes written
            if (num_wrote != 8) {
                return E_COMM_ERR;
            }
        } else {
            // Write failed
            return E_COMM_ERR;
        }
    } else {
        // No presence pulse
        return E_COMM_ERR;
    }
    
    return E_NO_ERROR;
}

/* ************************************************************************* */
int OWM_SkipROM(void)
{
    // Send reset and wait for presence pulse
    if (OWM_Reset()) {
        // Send skip ROM command code
        return OWM_WriteByte(SKIP_ROM_COMMAND);
    } else {
        // No presence pulse
        return E_COMM_ERR;
    }
}

/* ************************************************************************* */
int OWM_ODSkipROM(void)
{
    // Set to standard speed
    MXC_OWM->cfg &= ~(MXC_F_OWM_CFG_OVERDRIVE);
    
    // Send reset and wait for presence pulse
    if (OWM_Reset()) {
        // Send Overdrive skip ROM command code
        if (OWM_WriteByte(OD_SKIP_ROM_COMMAND) == E_NO_ERROR) {
            // Set overdrive speed
            MXC_OWM->cfg |= MXC_F_OWM_CFG_OVERDRIVE;
            
            return E_NO_ERROR;
        } else {
            // Write failed
            return E_COMM_ERR;
        }
    } else {
        // No presence pulse
        return E_COMM_ERR;
    }
}

/* ************************************************************************* */
int OWM_Resume(void)
{
    // Send reset and wait for presence pulse
    if (OWM_Reset()) {
        // Send resume command code
        return OWM_WriteByte(RESUME_COMMAND);
    } else {
        // No presence pulse
        return E_COMM_ERR;
    }
}

/* ************************************************************************* */
int OWM_SearchROM(int newSearch, uint8_t* ROMCode)
{
    int nibble_start_bit = 1;
    int rom_byte_number = 0;
    uint8_t rom_nibble_mask = 0x0F;
    uint8_t search_direction = 0;
    int readValue = 0;
    int sentBits = 0;
    int discrepancy = 0;
    int bit_position = 0;
    int discrepancy_mask = 0;
    int last_zero = 0;
    uint8_t crc8 = 0;
    int search_result = 0;
    
    // Clear ROM array
    memset(ROMCode, 0x0, 8);
    
    if (newSearch) {
        // Reset all global variables to start search from beginning
        LastDiscrepancy = 0;
        LastDeviceFlag = 0;
    }
    
    // Check if the last call was the last device
    if (LastDeviceFlag) {
        // Reset the search
        LastDiscrepancy = 0;
        LastDeviceFlag = 0;
        return search_result;
    }
    
    // Send reset and wait for presence pulse
    if (OWM_Reset()) {
        // Send the search command
        OWM_WriteByte(SEARCH_ROM_COMMAND);
        
        // Set search ROM accelerator bit
        MXC_OWM->ctrl_stat |= MXC_F_OWM_CTRL_STAT_SRA_MODE;
        
        // Loop through all ROM bytes 0-7 (this loops 2 times per byte)
        while (rom_byte_number < 8) {
            // Each loop finds the discrepancy bits and finds 4 bits (nibble) of the ROM
            
            // Set the search direction the same as last time for the nibble masked
            search_direction = ROMCode[rom_byte_number] & rom_nibble_mask;
            
            // If the upper nibble is the mask then shift bits to lower nibble
            if (rom_nibble_mask > 0x0F) {
                search_direction  = search_direction >> 4;
            }
            
            // Get the last discrepancy bit position relative to the nibble start bit
            bit_position = LastDiscrepancy - nibble_start_bit;
            
            // Check if last discrepancy is within this nibble
            if ((bit_position >= 0) && (bit_position < 4)) {
                // Last discrepancy is within this nibble
                // Set the bit of the last discrepancy bit
                search_direction |= (1 << (bit_position));
            }
            
            // Performs two read bits and a write bit for 4 bits of the ROM
            readValue = OWM_TouchByte(search_direction);
            // Get discrepancy flags
            discrepancy = readValue & 0xF;
            // Get the 4 bits sent to select the ROM
            sentBits = (readValue >> 4) & 0xF;
            
            // Store the bit location of the MSB discrepancy with sentbit = 0
            if (discrepancy) {
                // Initialize bit_position to MSB of nibble
                bit_position = 3;
                
                while (bit_position >= 0) {
                    // Get discrepancy flag of the current bit position
                    discrepancy_mask =  discrepancy & (1 << bit_position);
                    
                    // If there is a discrepancy and the sent bit is 0 save this bit position
                    if ((discrepancy_mask)  && !(sentBits & discrepancy_mask)) {
                        last_zero = nibble_start_bit + bit_position;
                        break;
                    }
                    
                    bit_position--;
                }
            }
            
            // Clear the nibble
            ROMCode[rom_byte_number] &= ~rom_nibble_mask;
            
            // Store the sentBits in the ROMCode
            if (rom_nibble_mask > 0x0F) {
                ROMCode[rom_byte_number] |= (sentBits << 4);
            } else {
                ROMCode[rom_byte_number] |= sentBits;
            }
            
            // Increment the nibble start bit and shift mask
            nibble_start_bit += 4;
            rom_nibble_mask <<= 4;
            
            // If the mask is 0 then go to new ROM byte rom_byte_number and reset mask
            if (rom_nibble_mask == 0) {
                rom_byte_number++;
                rom_nibble_mask = 0x0F;
            }
            
        } // End while(rom_byte_number < 8)
        
        // Clear search ROM accelerator
        MXC_OWM->ctrl_stat &= ~(MXC_F_OWM_CTRL_STAT_SRA_MODE);
        
        // Calculate CRC to verify ROM code is correct
        crc8 = CalculateCRC8(ROMCode, 7);
        
        // If the search was successful then
        if ((nibble_start_bit >= 65) && (crc8 == ROMCode[7])) {
            // Search successful so set LastDiscrepancy,LastDeviceFlag,search_result
            LastDiscrepancy = last_zero;
            
            // Check for last device
            if (LastDiscrepancy == 0) {
                LastDeviceFlag = 1;
            }
            
            search_result = 1;
        }
    } // End if (OWM_Reset)
    
    // If no device found then reset counters so next 'search' will be like a first
    if (!search_result || !ROMCode[0]) {
        LastDiscrepancy = 0;
        LastDeviceFlag = 0;
        search_result = 0;
    }
    
    return search_result;
}

/* ************************************************************************* */
void OWM_ClearFlags(uint32_t mask)
{
    MXC_OWM->intfl = mask;
}

/* ************************************************************************* */
unsigned OWM_GetFlags(void)
{
    return (MXC_OWM->intfl);
}

/* ************************************************************************* */
void OWM_SetExtPullup(int enable)
{
    if (enable) {
        MXC_OWM->cfg |= MXC_F_OWM_CFG_EXT_PULLUP_ENABLE;
    } else {
        MXC_OWM->cfg &= ~(MXC_F_OWM_CFG_EXT_PULLUP_ENABLE);
    }
}

/* ************************************************************************* */
void OWM_SetOverdrive(int enable)
{
    if (enable) {
        MXC_OWM->cfg |= MXC_F_OWM_CFG_OVERDRIVE;
    } else {
        MXC_OWM->cfg &= ~(MXC_F_OWM_CFG_OVERDRIVE);
    }
}

/* ************************************************************************* */
static uint8_t CalculateCRC8(uint8_t* data, int len)
{
    int i;
    uint8_t crc = 0;
    
    for (i = 0; i < len; i++) {
        crc = update_crc8(crc, data[i]);
    }
    
    return crc;
}


/* ************************************************************************* */
static uint8_t update_crc8(uint8_t crc, uint8_t val)
{
    uint8_t inc, tmp;
    
    for (inc = 0; inc < 8; inc++) {
        tmp = (uint8_t)(crc << 7); // Save X7 bit value
        crc >>= 1; // Shift crc
        if (((tmp >> 7) ^ (val & 0x01)) == 1) { // If X7 xor X8 (input data)
            crc ^= 0x8c; // XOR crc with X4 and X5, X1 = X7^X8
            crc |= 0x80; // Carry
        }
        val >>= 1;
    }
    
    return crc;
}
