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

#ifndef _MAXCAM_DEFINTIIONS_H_
#define _MAXCAM_DEFINTIIONS_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "stdint.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define QSPI_SPEED                10000000UL
#define QSPI_START_SYMBOL         0xAABBCCDD

#define LCD_WIDTH           240
#define LCD_HEIGHT          240
#define IMAGE_SIZE          (LCD_WIDTH*LCD_HEIGHT*2)
#define RESULT_MAX_SIZE     32


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------
typedef enum {
    QSPI_TYPE_NO_DATA = 0,
    QSPI_TYPE_RESPONSE_VIDEO_DATA ,
    QSPI_TYPE_RESPONSE_VIDEO_RESULT,
    QSPI_TYPE_RESPONSE_AUDIO_RESULT
} teQspiDataType;

typedef struct __attribute__((packed)) {
    uint32_t start_symbol;
    uint32_t data_len;
    uint8_t data_type;
} qspi_header_t;


#endif /* _MAXCAM_DEFINTIIONS_H_ */
