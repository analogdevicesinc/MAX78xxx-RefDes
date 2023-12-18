/*******************************************************************************
 * Copyright (C) 2020 Maxim Integrated Products, Inc., All Rights Reserved.
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
 ******************************************************************************/


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <gcr_regs.h>
#include <mxc_delay.h>
#include <mxc_errors.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "max32666_data.h"
#include "max32666_touch.h"
#include "max32666_debug.h"
#include "max32666_fonts.h"
#include "max32666_lcd.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------


/*********************************** VARIABLES *******************************/

char alphabet[26][1] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

static char lcd_string_buff[LCD_NOTIFICATION_MAX_SIZE] = {0};


void show_keyboard()
{
    char word;
    for (int i = 0; i < 26; i++)
    {
        if (i/7 == 0)
        {
            MXC_TS_AddButton( (i%7)*30 + 10 , 15, (i%7)*30 + 50, 60, i + 1);
            word = alphabet[i][0];
            snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "%c",word);
            fonts_putString((i%7)*30 + 15, 15, lcd_string_buff, &Font_16x26, WHITE, 0, 0, lcd_data.buffer);
        }
        else if (i/7 == 1)
        {
            MXC_TS_AddButton( (i%7)*30 + 15 , 60, (i%7)*30 + 50, 105, i + 1);
            word = alphabet[i][0];
            snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "%c",word);
            fonts_putString((i%7)*30 + 15, 60, lcd_string_buff, &Font_16x26, WHITE, 0, 0, lcd_data.buffer);
        }
        else if (i/7 == 2)
        {
            MXC_TS_AddButton( (i%7)*30 + 15 , 105, (i%7)*30 + 50, 145, i + 1);
            word = alphabet[i][0];
            snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "%c",word);
            fonts_putString((i%7)*30 + 15, 105, lcd_string_buff, &Font_16x26, WHITE, 0, 0, lcd_data.buffer);
        }
        else if (i/7 == 3)
        {
            MXC_TS_AddButton( (i%7)*30 + 15 , 145, (i%7)*30 + 50, 190, i + 1);
            word = alphabet[i][0];
            snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "%c",word);
            fonts_putString((i%7)*30 + 15, 145, lcd_string_buff, &Font_16x26, WHITE, 0, 0, lcd_data.buffer);
        }          
    }
    
    MXC_TS_AddButton(130,190,190,240,27);
    snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "BCK");
    fonts_putString(130, 190, lcd_string_buff, &Font_16x26, ORANGE, 0, 0, lcd_data.buffer);
    MXC_Delay(1000);
    MXC_TS_AddButton(190,190,240,240,28);
    snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "OK");
    fonts_putString(190, 190, lcd_string_buff, &Font_16x26, ORANGE, 0, 0, lcd_data.buffer);
    MXC_Delay(1000);
}

void get_name(embeddings_t *p)
{
    char word;
    memset(lcd_data.buffer, 0x00, sizeof(lcd_data.buffer));
    MXC_TS_RemoveAllButton();
    show_keyboard();
    lcd_drawImage(lcd_data.buffer);
    int key = 0;
    int len = 0;
    while(1){
        key = MXC_TS_GetKey();
        if (key == 0){
            continue;
        }
        else if (key == 28)
        {
            p->embeddings_name[len] = '\0'; // null terminator
            break;
        }
        else if (key == 27 && len > 0)
        {
            fonts_drawFilledRectangle(15+len*15, 190, 11, 18, BLACK, lcd_data.buffer);
            lcd_drawImage(lcd_data.buffer);            
            len--;
            p->embeddings_name[len] = '\0';

        }
        else if (len < 6) // 6th character is reserved for null terminator 
        {

            p->embeddings_name[len] = alphabet[key-1][0];
            len++;
            word = alphabet[key-1][0];
            snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "%c",word);
            fonts_putString(15+len*15, 190, lcd_string_buff, &Font_11x18, LIGHTBLUE, 0, 0, lcd_data.buffer);
            lcd_drawImage(lcd_data.buffer);
            MXC_Delay(1000);

        }
    }
}
