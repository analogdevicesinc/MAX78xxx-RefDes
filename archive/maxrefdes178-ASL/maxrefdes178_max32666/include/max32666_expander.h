/*******************************************************************************
* Copyright (C) 2020-2021 Maxim Integrated Products, Inc., All rights Reserved.
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
* @file   max20303.h
* @brief  TODO
*/

#ifndef _MAX32666_EXPANDER_H_
#define _MAX32666_EXPANDER_H_


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define EXPANDER_OUTPUT_EXPANSION4_OUT    (0x01 << 0)  // O8
#define EXPANDER_OUTPUT_EXPANSION5_OUT    (0x01 << 1)  // O9
#define EXPANDER_OUTPUT_SLAVE_DEBUG_SEL   (0x01 << 2)  // O10
#define EXPANDER_OUTPUT_EXPANSION6_OUT    (0x01 << 3)  // O11
#define EXPANDER_OUTPUT_HDK1_TARGET_SEL   (0x01 << 4)  // O12
#define EXPANDER_OUTPUT_UART_TARGET_SEL   (0x01 << 5)  // O13
#define EXPANDER_OUTPUT_ENABLE_CODEC_OSC  (0x01 << 6)  // O14
#define EXPANDER_OUTPUT_RESET_LCD         (0x01 << 7)  // O15

#define EXPANDER_INPUT_INT_PMIC           (0x01 << 0)  // P0
#define EXPANDER_INPUT_INT_CODEC          (0x01 << 1)  // P1
#define EXPANDER_INPUT_ALERT_PMIC         (0x01 << 2)  // P2
#define EXPANDER_OUTPUT_RESET_TOUCH       (0x01 << 3)  // P3
#define EXPANDER_INPUT_INT_ACC            (0x01 << 4)  // P4
#define EXPANDER_IO_BOOTLOADER_INVOKE     (0x01 << 5)  // P5
#define EXPANDER_IO_EXPANSION3_IO         (0x01 << 6)  // P6
#define EXPANDER_INPUT_BUTTON_Y           (0x01 << 7)  // P7


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function declarations
//-----------------------------------------------------------------------------
int expander_init(void);
int expander_worker(void);
int expander_select_debugger(debugger_select_e debugger_select);
int expander_read_output(uint8_t *output);
int expander_set_output(uint8_t mask);
int expander_clear_output(uint8_t mask);
int expander_read_io(uint8_t *input);
int expander_set_io(uint8_t mask);
int expander_invoke_video_bootloader(void);
int expander_invoke_audio_bootloader(void);

#endif /* _MAX32666_EXPANDER_H_ */
