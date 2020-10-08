#ifndef _FONT_H_
#define _FONT_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "stdint.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define GRAY 0X8430
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define BROWN 0XBC40
#define BRRED 0XFC07
#define DARKBLUE 0X01CF
#define LIGHTBLUE 0X7D7C
#define GRAYBLUE 0X5458
#define LIGHTGREEN 0X841F
#define LGRAY 0XC618
#define LGRAYBLUE 0XA651
#define LBBLUE 0X2B12


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------
typedef struct {
    const uint8_t width;
    const uint8_t height;
    const uint16_t *data;
} FontDef;


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
extern const FontDef Font_7x10;
extern const FontDef Font_11x18;
extern const FontDef Font_16x26;


//-----------------------------------------------------------------------------
// Function declarations
//-----------------------------------------------------------------------------
void fonts_putString(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *str, FontDef font, uint16_t color, uint8_t bg, uint16_t bgcolor, uint8_t *buff);
void fonts_putSubtitle(uint16_t w, uint16_t h, const char *str, FontDef font, uint16_t color, uint8_t *buff);
void fonts_putToptitle(uint16_t w, uint16_t h, const char *str, FontDef font, uint16_t color, uint8_t *buff);

#endif /* _FONT_H_ */
