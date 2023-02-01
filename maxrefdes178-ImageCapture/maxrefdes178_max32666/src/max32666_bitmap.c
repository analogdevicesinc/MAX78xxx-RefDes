/*
* MIT License
* 
* Copyright (c) 2020 y2kblog
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

/*
 * It is customized version of https://github.com/y2kblog/bitmap-rgb565
 */

/*****************************    Includes   *********************************/
#include "max32666_bitmap.h"
 
/*****************************     Macros    *********************************/
#define BMP_RGB565_FILE_HEADER_SIZE	14	// = 0x0E
#define BMP_RGB565_INFO_HEADER_SIZE	40	// = 0x28
#define BMP_RGB565_BIT_FIELD_SIZE	16
static const uint32_t AllHeaderOffset = BMP_RGB565_FILE_HEADER_SIZE + BMP_RGB565_INFO_HEADER_SIZE + BMP_RGB565_BIT_FIELD_SIZE;

/*****************************    Typedef   *********************************/

/*****************************    Variables  *********************************/

/***************************  Static Functions *******************************/
// Calculate the number of bytes used to store a single image row.
// This is always rounded up to the next multiple of 4.
static uint32_t BMP_RGB565_getBytesPerRow(uint32_t width)
{
    if (width & 0x00000001)
        return ((width + 1) << 1);
    else
        return  (width << 1);
}

/**************************************************************
    Writes a little-endian unsigned short int to the file.
    Returns non-zero on success.
**************************************************************/
static void BMP_RGB565_write_uint32_t(uint32_t Src, uint8_t *pDst)
{
    *(pDst + 3) = (uint8_t)( ( Src & 0xff000000 ) >> 24 );
    *(pDst + 2) = (uint8_t)( ( Src & 0x00ff0000 ) >> 16 );
    *(pDst + 1) = (uint8_t)( ( Src & 0x0000ff00 ) >> 8  );
    *pDst       = (uint8_t)  ( Src & 0x000000ff )        ;
}

/**************************************************************
	Writes a little-endian unsigned short int to the file.
	Returns non-zero on success.
**************************************************************/
static void BMP_RGB565_write_uint16_t(uint16_t Src, uint8_t *pDst)
{
    *(pDst + 1) = (uint8_t)( ( Src & 0xff00 ) >> 8 );
	*pDst       = (uint8_t)  ( Src & 0x00ff )       ;
}

/***************************  Public Functions *******************************/
int BMP_RGB565_create_header(uint32_t width, uint32_t height, uint8_t *buf,  uint32_t *maxLen)
{
    uint32_t bytes_per_row = BMP_RGB565_getBytesPerRow(width);
    uint32_t image_size = bytes_per_row * height;
    uint32_t data_size = AllHeaderOffset + image_size;

	if (*maxLen < AllHeaderOffset) {
		return -1;
	}
	*maxLen = AllHeaderOffset;

    // Set header's default values
    uint8_t *tmp = buf;
    *(tmp  +  0) = 'B';                                        // 'B' : Magic number
    *(tmp  +  1) = 'M';                                        // 'M' : Magic number
    BMP_RGB565_write_uint32_t(data_size        , tmp + 0x02);  // File Size
    BMP_RGB565_write_uint16_t(0                , tmp + 0x06);  // Reserved1
    BMP_RGB565_write_uint16_t(0                , tmp + 0x08);  // Reserved2
    BMP_RGB565_write_uint32_t(AllHeaderOffset  , tmp + 0x0A);  // Offset
    tmp += BMP_RGB565_FILE_HEADER_SIZE;    // Next

    // Info header
    BMP_RGB565_write_uint32_t( BMP_RGB565_INFO_HEADER_SIZE + BMP_RGB565_BIT_FIELD_SIZE, tmp + 0x00);   // HeaderSize
    BMP_RGB565_write_uint32_t( width           , tmp + 0x04);  // width  (*** Signed value ***)
    BMP_RGB565_write_uint32_t( height          , tmp + 0x08);  // height (*** Signed value ***)
    BMP_RGB565_write_uint16_t( 1               , tmp + 0x0C);  // planes
    BMP_RGB565_write_uint16_t( 16              , tmp + 0x0E);  // Bit count
    BMP_RGB565_write_uint32_t( 3               , tmp + 0x10);  // Bit compression
    BMP_RGB565_write_uint32_t( image_size      , tmp + 0x14);  // Image size
    BMP_RGB565_write_uint32_t( 0               , tmp + 0x18);  // X pixels per meter
    BMP_RGB565_write_uint32_t( 0               , tmp + 0x1C);  // Y pixels per meter
    BMP_RGB565_write_uint32_t( 0               , tmp + 0x20);  // Color index
    BMP_RGB565_write_uint32_t( 0               , tmp + 0x24);  // Important index
    tmp += BMP_RGB565_INFO_HEADER_SIZE;    // Next

    // Bit field
    BMP_RGB565_write_uint32_t( 0x0000F800      , tmp + 0x00);  // red
    BMP_RGB565_write_uint32_t( 0x000007E0      , tmp + 0x04);  // green
    BMP_RGB565_write_uint32_t( 0x0000001F      , tmp + 0x08);  // blue
    BMP_RGB565_write_uint32_t( 0x00000000      , tmp + 0x0C);  // reserved

    return 0;
}
