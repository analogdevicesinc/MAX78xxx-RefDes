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
#include <gpio.h>
#include <mxc_delay.h>

#include "max32666_debug.h"
#include "max32666_lcd.h"
#include "max32666_max20303.h"
#include "max32666_spi_dma.h"
#include "maxrefdes178_definitions.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME  "lcd"

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

#define ST7789_ROTATION LCD_ROTATION

#if ST7789_ROTATION == 0
    #define X_SHIFT 0
    #define Y_SHIFT 80
#elif ST7789_ROTATION == 1
    #define X_SHIFT 80
    #define Y_SHIFT 0
#elif ST7789_ROTATION == 2
    #define X_SHIFT 0
    #define Y_SHIFT 0
#elif ST7789_ROTATION == 3
    #define X_SHIFT 0
    #define Y_SHIFT 0
#endif


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
static const mxc_gpio_cfg_t lcd_dc_pin    = MAX32666_LCD_DC_PIN;
static const mxc_gpio_cfg_t lcd_reset_pin = MAX32666_LCD_RESET_PIN;
static const mxc_gpio_cfg_t lcd_cs_pin    = MAX32666_LCD_CS_PIN;



//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static void lcd_reset(void);
static void lcd_sendCommand(uint8_t command);
static void lcd_sendSmallData(uint8_t data);
static void lcd_sendData(uint8_t *data, uint32_t dataLen);
static void lcd_configure(void);
static void lcd_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
static void spi_init(void);
static void spi_assert_cs(void);
static void spi_deassert_cs(void);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------

// DMA IRQ Handlers
void MAX32666_LCD_DMA_IRQ_HAND()
{
    spi_dma_int_handler(MAX32666_LCD_DMA_CHANNEL, MAX32666_LCD_SPI);
}

static void spi_init(void)
{
    // Initialize SPI peripheral
    spi_dma_master_init(MAX32666_LCD_SPI, MAX32666_LCD_MAP, MAX32666_LCD_SPI_SPEED, 0);

    NVIC_EnableIRQ(MAX32666_LCD_DMA_IRQ);
}

static void spi_assert_cs(void)
{
    GPIO_CLR(lcd_cs_pin);
}

static void spi_deassert_cs(void)
{
    GPIO_SET(lcd_cs_pin);
}

static void lcd_sendCommand(uint8_t command)
{
    GPIO_CLR(lcd_dc_pin);
    spi_dma(MAX32666_LCD_DMA_CHANNEL, MAX32666_LCD_SPI, &command, NULL, 1, MAX32666_LCD_DMA_REQSEL_SPITX, NULL);
    spi_dma_wait(MAX32666_LCD_DMA_CHANNEL, MAX32666_LCD_SPI);
}

static void lcd_sendSmallData(uint8_t data)
{
    GPIO_SET(lcd_dc_pin);
    spi_dma(MAX32666_LCD_DMA_CHANNEL, MAX32666_LCD_SPI, &data, NULL, 1, MAX32666_LCD_DMA_REQSEL_SPITX, NULL);
    spi_dma_wait(MAX32666_LCD_DMA_CHANNEL, MAX32666_LCD_SPI);
}

static void lcd_sendData(uint8_t *data, uint32_t dataLen)
{
    GPIO_SET(lcd_dc_pin);
    spi_dma(MAX32666_LCD_DMA_CHANNEL, MAX32666_LCD_SPI, data, NULL, dataLen, MAX32666_LCD_DMA_REQSEL_SPITX, NULL);
    spi_dma_wait(MAX32666_LCD_DMA_CHANNEL, MAX32666_LCD_SPI);
}

static void lcd_reset(void)
{
    GPIO_CLR(lcd_reset_pin);
    MXC_Delay(MXC_DELAY_MSEC(25));
    GPIO_SET(lcd_reset_pin);
    MXC_Delay(MXC_DELAY_MSEC(25));
}

void lcd_backlight(int on)
{
    if (on) {
        max20303_boost(1, 0x10);
    } else {
        max20303_boost(0, 0x10);
    }
}

static void lcd_configure(void)
{
    spi_assert_cs();

    lcd_sendCommand(ST7789_COLMOD);     //  Set color mode
    lcd_sendSmallData(0x55);            //  RGB565 (16bit)

    lcd_sendCommand(0xB2);              //  Porch control
    {
        uint8_t data[] = {0x0C, 0x0C, 0x00, 0x33, 0x33};
        lcd_sendData(data, sizeof(data));
    }

    lcd_sendCommand(ST7789_MADCTL);    //  Display Rotation
#if ST7789_ROTATION == 0
    lcd_sendSmallData(ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB);
#elif ST7789_ROTATION == 1
    lcd_sendSmallData(ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
#elif ST7789_ROTATION == 2
    lcd_sendSmallData(ST7789_MADCTL_RGB);
#elif ST7789_ROTATION == 3
    lcd_sendSmallData(ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
#endif

    /* Internal LCD Voltage generator settings */
    lcd_sendCommand(0XB7);             //  Gate Control
    lcd_sendSmallData(0x35);           //  Default value

    lcd_sendCommand(0xBB);             //  VCOM setting
    lcd_sendSmallData(0x19);           //  0.725v (default 0.75v for 0x20)

    lcd_sendCommand(0xC0);             //  LCMCTRL
    lcd_sendSmallData(0x2C);           //  Default value

    lcd_sendCommand(0xC2);             //  VDV and VRH command Enable
    lcd_sendSmallData(0x01);           //  Default value

    lcd_sendCommand(0xC3);             //  VRH set
    lcd_sendSmallData(0x12);           //  +-4.45v (defalut +-4.1v for 0x0B)

    lcd_sendCommand(0xC4);             //  VDV set
    lcd_sendSmallData(0x20);           //  Default value

    lcd_sendCommand(0xC6);             //  Frame rate control in normal mode
    lcd_sendSmallData(0x0F);           //  Default value (60HZ)

    lcd_sendCommand(0xD0);             //  Power control
    lcd_sendSmallData(0xA4);           //  Default value
    lcd_sendSmallData(0xA1);           //  Default value

    lcd_sendCommand(0xE0);
    {
        uint8_t data[] = {0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23};
        lcd_sendData(data, sizeof(data));
    }

    lcd_sendCommand(0xE1);
    {
        uint8_t data[] = {0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23};
        lcd_sendData(data, sizeof(data));
    }

    lcd_sendCommand(ST7789_INVON);     //  Inversion ON

    lcd_sendCommand(ST7789_SLPOUT);    //  Out of sleep mode

    lcd_sendCommand(ST7789_NORON);     //  Normal Display on

    lcd_sendCommand(ST7789_DISPON);    //  Main screen turned on

    spi_deassert_cs();
}

/**
 * @brief Set address of DisplayWindow
 * @param xi&yi -> coordinates of window
 * @return none
 */
static void lcd_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    uint16_t x_start = x0 + X_SHIFT, x_end = x1 + X_SHIFT;
    uint16_t y_start = y0 + Y_SHIFT, y_end = y1 + Y_SHIFT;

    spi_assert_cs();
    lcd_sendCommand(ST7789_CASET);
    {
        uint8_t data[] = {x_start >> 8, x_start & 0xFF, x_end >> 8, x_end & 0xFF};
        lcd_sendData(data, sizeof(data));
    }

    lcd_sendCommand(ST7789_RASET);
    {
        uint8_t data[] = {y_start >> 8, y_start & 0xFF, y_end >> 8, y_end & 0xFF};
        lcd_sendData(data, sizeof(data));
    }

    lcd_sendCommand(ST7789_RAMWR);
    spi_deassert_cs();
}

/**
 * @brief Draw an Image on the screen
 * @param x&y -> start point of the Image
 * @param w&h -> width & height of the Image to Draw
 * @param data -> pointer of the Image array
 * @return none
 */
void lcd_drawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data)
{
    lcd_setAddrWindow(x, y, x + w - 1, y + h - 1);

    GPIO_SET(lcd_dc_pin);
    spi_assert_cs();

    spi_dma(MAX32666_LCD_DMA_CHANNEL, MAX32666_LCD_SPI, data, NULL, (w * h * LCD_BYTE_PER_PIXEL), MAX32666_LCD_DMA_REQSEL_SPITX, spi_deassert_cs);
}

int lcd_init(void)
{
    lcd_backlight(1);

    spi_deassert_cs();
    MXC_GPIO_Config(&lcd_cs_pin);

    GPIO_SET(lcd_dc_pin);
    MXC_GPIO_Config(&lcd_dc_pin);

    GPIO_SET(lcd_reset_pin);
    MXC_GPIO_Config(&lcd_reset_pin);

    lcd_reset();

    spi_init();

    lcd_configure();

    return E_NO_ERROR;
}
