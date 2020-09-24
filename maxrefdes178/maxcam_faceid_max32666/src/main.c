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
#include "tmr_utils.h"

#include "max20303.h"
#include "qspi.h"
#include "lcd.h"
#include "lcd_data.h"
#include "faceid_definitions.h"
#include "version.h"

#include "i2c.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
uint16_t resultColor;


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static void rgb_setChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color);
static void rgb_setString(uint16_t x, uint16_t y, const char *str, FontDef font, uint16_t color);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
static void rgb_setChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color)
{
    uint32_t i, b, j;

    for (i = 0; i < font.height; i++) {
        b = font.data[(ch - 32) * font.height + i];
        for (j = 0; j < font.width; j++) {
            if ((b << j) & 0x8000) {
                qspi_image_buff[(((i+y)*240) + (j+x))*2] = color >> 8;
                qspi_image_buff[(((i+y)*240) + (j+x))*2 + 1] = color & 0xFF;
            }
        }
    }
}

static void rgb_setString(uint16_t x, uint16_t y, const char *str, FontDef font, uint16_t color)
{
    while (*str) {
        if (x + font.width >= 240) {
            x = 0;
            y += font.height;
            if (y + font.height >= 240) {
                break;
            }

            if (*str == ' ') {
                // skip spaces in the beginning of the new line
                str++;
                continue;
            }
        }
        rgb_setChar(x, y, *str, font, color);
        x += font.width;
        str++;
    }
}

int main(void)
{
    uint8_t counter = 0;
    int qspi_return = 0;

    printf("\n\nmaxcam_faceid_max32666 v%d.%d.%d\n", S_VERSION_MAJOR, S_VERSION_MINOR, S_VERSION_BUILD);

    printf("init started\n");

    if (MAX20303_initialize(1) != E_NO_ERROR) {
        printf("pmic init failed\n");
        while(1);
    }

    /* Switch USB-TYpe-C Debug Connection to MAX78000-Image */
    const uint8_t cmdData[1] = {0xff};
    I2C_MasterWrite(MXC_I2C0_BUS0, 0xd8, cmdData, 1, 0);



    if (qspi_init(1) != E_NO_ERROR) {
        printf("qspi init failed\n");
        while(1);
    }

    if (lcd_init() != E_NO_ERROR) {
        printf("lcd init failed\n");
        while(1);
    }

    printf("init completed\n");

    MAX20303_led_green(MAX20303_LED_OUTPUT_ON);

    lcd_drawImage(0, 0, 240, 240, image_data_rsz_maxim_logo);

    while (1) {
        qspi_return = qspi_worker();
        if (qspi_return > E_NO_ERROR) {
            switch(qspi_return)
            {
                case IMAGE_RECEIVED:
                    rgb_setString(20, 210, resultString, Font_16x26, resultColor);
                    lcd_drawImage(0, 0, 240, 240, qspi_image_buff);
                    break;
                case RESULT_RECEIVED:
                    if (strcmp(resultString, "Unknown") == 0) {
                        resultColor = RED;
                    } else if(strcmp(resultString, "Adjust Face") == 0) {
                        resultColor = BLUE;
                    } else {
                        resultColor = GREEN;
                    }
                    break;
                default:
                    break;
            }
        }
        MAX20303_led_blue(counter%2);
        counter++;
    }
}
