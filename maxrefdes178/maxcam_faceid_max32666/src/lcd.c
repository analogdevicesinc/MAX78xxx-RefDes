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
#include "spi.h"
#include "dma.h"
#include "gpio.h"
#include "tmr_utils.h"

#include "max20303.h"
#include "lcd.h"
#include "lcd_data.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define SPI_NAME          SPI1
#define MXC_SPI           MXC_SPI1
#define DMA_REQSEL_SPIRX  DMA_REQSEL_SPI1RX
#define DMA_REQSEL_SPITX  DMA_REQSEL_SPI1TX
#define SPI_SPEED         16000000

#define ST_CMD_DELAY   0x80

#define ST7789_NOP     0x00
#define ST7789_SWRESET 0x01

#define ST7789_NOP     0x00
#define ST7789_SWRESET 0x01
#define ST7789_RDDID   0x04
#define ST7789_RDDST   0x09

#define ST7789_SLPIN   0x10
#define ST7789_SLPOUT  0x11
#define ST7789_PTLON   0x12
#define ST7789_NORON   0x13

#define ST7789_INVOFF  0x20
#define ST7789_INVON   0x21
#define ST7789_DISPOFF 0x28
#define ST7789_DISPON  0x29
#define ST7789_CASET   0x2A
#define ST7789_RASET   0x2B
#define ST7789_RAMWR   0x2C
#define ST7789_RAMRD   0x2E

#define ST7789_PTLAR   0x30
#define ST7789_COLMOD  0x3A
#define ST7789_MADCTL  0x36

#define ST7789_MADCTL_MY  0x80
#define ST7789_MADCTL_MX  0x40
#define ST7789_MADCTL_MV  0x20
#define ST7789_MADCTL_ML  0x10
#define ST7789_MADCTL_RGB 0x00

#define ST7789_RDID1   0xDA
#define ST7789_RDID2   0xDB
#define ST7789_RDID3   0xDC
#define ST7789_RDID4   0xDD

#define ST7789_TFTWIDTH     240
#define ST7789_TFTHEIGHT    240

#define ST7789_240x240_XSTART 0
#define ST7789_240x240_YSTART 0


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
//static volatile int  spi_flag;
static const gpio_cfg_t lcd_dc_pin     = {PORT_1, PIN_14  , GPIO_FUNC_OUT, GPIO_PAD_PULL_UP};
static const gpio_cfg_t lcd_reset_pin  = {PORT_1, PIN_15 , GPIO_FUNC_OUT, GPIO_PAD_PULL_UP};
static const gpio_cfg_t ssel_pin       = {PORT_0, PIN_22 , GPIO_FUNC_OUT, GPIO_PAD_PULL_UP};
static volatile int in_dma_complete  = 1;
static volatile int out_dma_complete = 1;


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static void lcd_reset();
static void lcd_backlight(unsigned char onoff);
static void lcd_sendCommand(uint8_t command);
static void lcd_sendData(uint8_t data);
static void lcd_configure();
static void lcd_setAddrWindow(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1);
static void lcd_logoUpdateMaxim();
static void lcd_logoUpdateRobert();
static void spi_init();
static void spi_sendPacket(uint8_t* out, uint8_t* in, unsigned int len);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------

// DMA IRQ Handlers - there needs to be one handler for each channel used.  This example uses 2.
void DMA0_IRQHandler()
{
    DMA_Handler(0);
}

void DMA1_IRQHandler()
{
    DMA_Handler(1);
}

// A callback function that will be called when the RX DMA completes.
void in_callback(int ch, int reason)
{
    in_dma_complete = 1;
}

// A callback function that will be called when the TX DMA completes.
void out_callback(int ch, int reason)
{
    out_dma_complete = 1;
}

static void spi_init()
{
    // Initialize DMA peripheral
    DMA_Init();
    NVIC_EnableIRQ(DMA0_IRQn);
    NVIC_EnableIRQ(DMA1_IRQn);

    // Initialize SPI peripheral
    sys_cfg_spi_t master_cfg = {0};
    master_cfg.map = MAP_A;
    master_cfg.ss0 = Disable;
    master_cfg.ss1 = Disable;
    master_cfg.ss2 = Disable;

    SPI_Init(SPI_NAME, 0, SPI_SPEED, master_cfg);
    MXC_SPI->ctrl0 |= MXC_F_SPI17Y_CTRL0_MASTER; // Switch SPI to master mode, else the state of the SS pin could cause the SPI periph to appear 'BUSY';

    MXC_SPI->ss_time = (4 << MXC_F_SPI17Y_SS_TIME_PRE_POS) |
            (8 << MXC_F_SPI17Y_SS_TIME_POST_POS) |
            (16 << MXC_F_SPI17Y_SS_TIME_INACT_POS);
}

static void spi_sendPacket(uint8_t* out, uint8_t* in, unsigned int len)
{
    int in_ch = -1;
    int out_ch = -1;

    // Assert SSEL
    GPIO_OutClr(&ssel_pin);

    // Initialize the CTRL0 register
    MXC_SPI->ctrl0 = MXC_F_SPI17Y_CTRL0_MASTER |   // Enable master mode
            MXC_F_SPI17Y_CTRL0_EN;   // Enable the peripheral

    // Initialize the CTRL1 register
    MXC_SPI->ctrl1 = 0;
    if(out) {
        // Set how many to characters to send, or when in quad mode how many characters to receive
        MXC_SPI->ctrl1 |= (len << MXC_F_SPI17Y_CTRL1_TX_NUM_CHAR_POS);
    }

    if(in) {
        // Set how many characters to receive
        MXC_SPI->ctrl1 |= (len << MXC_F_SPI17Y_CTRL1_RX_NUM_CHAR_POS);
    }

    // Initialize the CTRL2 register
    MXC_SPI->ctrl2 = (MXC_S_SPI17Y_CTRL2_DATA_WIDTH_MONO) |
            (8 << MXC_F_SPI17Y_CTRL2_NUMBITS_POS);

    // Initialize the DMA register
    MXC_SPI->dma = 0;
    if(out) {
        MXC_SPI->dma |= MXC_F_SPI17Y_DMA_TX_DMA_EN |                   // Enable DMA for transmit
                MXC_F_SPI17Y_DMA_TX_FIFO_EN |                  // Enable the TX FIFO
                (31 << MXC_F_SPI17Y_DMA_TX_FIFO_LEVEL_POS);    // Set the threshold of when to add more data to TX FIFO
    }
    if(in) {
        MXC_SPI->dma |= MXC_F_SPI17Y_DMA_RX_DMA_EN |                   // Enable DMA for receive
                MXC_F_SPI17Y_DMA_RX_FIFO_EN |                  // Enable the RX FIFO
                (0 << MXC_F_SPI17Y_DMA_TX_FIFO_LEVEL_POS);     // Set the threshold of when to read data from RX FIFO
    }

    // Prepare DMA for unloading RX FIFO.
    if(in) {
        in_ch = DMA_AcquireChannel();
        DMA_ConfigChannel(in_ch, DMA_PRIO_LOW, DMA_REQSEL_SPIRX, 0,
                DMA_TIMEOUT_4_CLK, DMA_PRESCALE_DISABLE,
                DMA_WIDTH_BYTE, 0, DMA_WIDTH_BYTE, 1, 1, 1, 0);
        DMA_SetCallback(in_ch, in_callback);
        DMA_EnableInterrupt(in_ch);
        DMA_SetSrcDstCnt(in_ch, 0, in, len);
        in_dma_complete = 0;
        DMA_Start(in_ch);
    }
    else {
        in_dma_complete = 1;    // Not using the RX DMA, mark it as completed.
    }

    // Prepare DMA to fill TX FIFO.
    if(out) {
        out_ch = DMA_AcquireChannel();
        DMA_ConfigChannel(out_ch, DMA_PRIO_LOW, DMA_REQSEL_SPITX, 0,
                DMA_TIMEOUT_4_CLK, DMA_PRESCALE_DISABLE,
                DMA_WIDTH_BYTE, 1, DMA_WIDTH_BYTE, 0, 1, 1, 0);
        DMA_SetCallback(out_ch, out_callback);
        DMA_EnableInterrupt(out_ch);
        DMA_SetSrcDstCnt(out_ch, out, 0, len);
        out_dma_complete = 0;
        DMA_Start(out_ch);
    }
    else {
        out_dma_complete = 1;    // Not using the TX DMA, mark it as completed.
    }

    // Start the SPI transaction
    MXC_SPI->ctrl0 |= MXC_F_SPI17Y_CTRL0_START;

    // Wait here for all DMA transactions to complete.
    // This would normally be where this function would
    // return so the micro can go do other things.  The
    // callbacks will be triggered when the DMA completes
    // so the application knows its time to do more work.
    // For this simple example, we'll just block here.
    while(!in_dma_complete || !out_dma_complete);

    if(in) {
        DMA_ReleaseChannel(in_ch);
    }
    if(out) {
        DMA_ReleaseChannel(out_ch);
    }

    // DMA has completed, but that just means DMA has loaded all of its data to/from
    //  the FIFOs.  We now need to wait for the SPI transaction to fully complete.
    if(MXC_SPI->ctrl0 & MXC_F_SPI17Y_CTRL0_EN) {
        while(MXC_SPI->stat & MXC_F_SPI17Y_STAT_BUSY);
    }
}

static void lcd_sendCommand(uint8_t command)
{
    uint8_t cmd_buffer[1];
    GPIO_OutClr(&lcd_dc_pin);
    cmd_buffer[0] = command;
    spi_sendPacket(cmd_buffer, NULL, 1);
}

static void lcd_sendData(uint8_t data)
{
    uint8_t cmd_buffer[1];
    GPIO_OutSet(&lcd_dc_pin);
    cmd_buffer[0] = data;
    spi_sendPacket(cmd_buffer, NULL, 1);
}

static void lcd_reset()
{
    GPIO_OutClr(&lcd_reset_pin);
    TMR_Delay(MXC_TMR0, MSEC(15), 0);  // Keep Reset at low for 15ms
    GPIO_OutSet(&lcd_reset_pin);
}

static void lcd_backlight(unsigned char onoff)
{
    if (onoff) {
        MAX20303_setboost( MAX20303_BOOST_OUTPUT_ON, 0x10 );
    } else {
        MAX20303_setboost( MAX20303_BOOST_OUTPUT_OFF, 0x10 );
    }
}

static void lcd_configure()
{
    unsigned int i, j;
    unsigned int numArgs;
    unsigned int ms;
    unsigned char numberOfInitCommands;
    unsigned char index = 0;

    static const unsigned char cmd_240x240[] = {                         // Initialization commands for 7789 screens
        10,                                     // 9 commands in list:

        ST7789_SWRESET,   ST_CMD_DELAY,         // 1: Software reset, no args, w/delay
        150,                                    // 150 ms delay

        ST7789_SLPOUT ,   ST_CMD_DELAY,         // 2: Out of sleep mode, no args, w/delay
        255,                                    // 255 = 500 ms delay

        ST7789_COLMOD , 1+ST_CMD_DELAY,         // 3: Set color mode, 1 arg + delay:
        0x55,                                   // 16-bit color
        10,                                     // 10 ms delay

        ST7789_MADCTL , 1,                      // 4: Memory access ctrl (directions), 1 arg:
        0x01,                                   // Row addr/col addr, bottom to top refresh

        ST7789_CASET  , 4,                      // 5: Column addr set, 4 args, no delay:
        0x00, ST7789_240x240_XSTART,            // XSTART = 0
        (ST7789_TFTWIDTH+ST7789_240x240_XSTART) >> 8,
        (ST7789_TFTWIDTH+ST7789_240x240_XSTART) & 0xFF,   // XEND = 240

        ST7789_RASET  , 4,                      // 6: Row addr set, 4 args, no delay:

        0x00, ST7789_240x240_YSTART,            // YSTART = 0
        (ST7789_TFTHEIGHT+ST7789_240x240_YSTART) >> 8,
        (ST7789_TFTHEIGHT+ST7789_240x240_YSTART) & 0xFF,    // YEND = 240

        ST7789_INVON ,   ST_CMD_DELAY,          // 7: Inversion ON
        10,

        ST7789_NORON  ,   ST_CMD_DELAY,         // 8: Normal display on, no args, w/delay
        10,                                     // 10 ms delay

        ST7789_DISPON ,   ST_CMD_DELAY,         // 9: Main screen turn on, no args, w/delay
        255
    };

    numberOfInitCommands = cmd_240x240[ index ] - 2;

    for ( i = 0; i < numberOfInitCommands; i++ )
    {
        index++;
        lcd_sendCommand( cmd_240x240[ index ] );
        index++;
        numArgs = cmd_240x240[ index ];
        ms      = numArgs & ST_CMD_DELAY;
        numArgs = cmd_240x240[ index ] & 0x7f;

        if (numArgs) {
            for ( j = 0 ; j < numArgs ; j++ )
            {
                index++;
                lcd_sendData( cmd_240x240[ index ] );
            }
        }
        if (ms) {
            index++;
            TMR_Delay(MXC_TMR0, MSEC(  cmd_240x240[ index ] ), 0);  // Wait for 120ms
        }
        GPIO_OutSet(&ssel_pin);
    }

    lcd_sendCommand( 0x13 );     // NORON
    lcd_sendCommand( 0x21 );     // Inversion ON
    lcd_sendCommand( 0x29 );     // Display ON
}

static void lcd_setAddrWindow(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1)
{
    unsigned short xs = x0 + ST7789_240x240_XSTART, xe = x1 + ST7789_240x240_XSTART;
    unsigned short ys = y0 + ST7789_240x240_YSTART, ye = y1 + ST7789_240x240_YSTART;

    lcd_sendCommand(ST7789_CASET);
    lcd_sendData(xs >> 8);
    lcd_sendData(xs & 0xFF);
    lcd_sendData(xe >> 8);
    lcd_sendData(xe & 0xFF);
    GPIO_OutSet(&ssel_pin);

    lcd_sendCommand(ST7789_RASET);
    lcd_sendData(ys >> 8);
    lcd_sendData(ys & 0xFF);
    lcd_sendData(ye >> 8);
    lcd_sendData(ye & 0xFF);
    GPIO_OutSet(&ssel_pin);

    lcd_sendCommand(ST7789_RAMWR);
    GPIO_OutSet(&ssel_pin);
}

static void lcd_logoUpdateMaxim()
{
    lcd_setAddrWindow(0, 0, 240, 240);
    GPIO_OutSet(&lcd_dc_pin);
    spi_sendPacket( (uint8_t *) image_data_rsz_maxim_logo, NULL, 240*240);
    spi_sendPacket( ((uint8_t *) image_data_rsz_maxim_logo) + 57600, NULL, 240*240);
    GPIO_OutSet(&ssel_pin);
}

static void lcd_logoUpdateRobert()
{
    lcd_setAddrWindow(0, 0, 240, 240);
    GPIO_OutSet(&lcd_dc_pin);
    spi_sendPacket( (uint8_t *) image_data_robert, NULL, 240*240);
    spi_sendPacket( ((uint8_t *) image_data_robert) + 57600, NULL, 240*240);
    GPIO_OutSet(&ssel_pin);
}

void lcd_init()
{
    lcd_backlight(1);

    GPIO_OutSet(&ssel_pin);
    GPIO_Config(&ssel_pin);

    GPIO_OutSet(&lcd_dc_pin);
    GPIO_Config(&lcd_dc_pin);

    GPIO_OutSet(&lcd_reset_pin);
    GPIO_Config(&lcd_reset_pin);

    lcd_reset();

    spi_init();

    lcd_configure();
}

void lcd_worker()
{
    while(1) {
        lcd_logoUpdateMaxim();
        TMR_Delay(MXC_TMR0, MSEC(1000), 0);
        lcd_logoUpdateRobert();
        TMR_Delay(MXC_TMR0, MSEC(1000), 0);
    }
}
