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
#ifndef INCLUDE_MAX32666_BL_H_
#define INCLUDE_MAX32666_BL_H_

#define FLC_PAGE_BIT_SIZE		13
#define FLC_PAGE_SIZE			(1 << FLC_PAGE_BIT_SIZE)
#define CHECKBYTE_16			16
#define USN_SIZE				24
#define PAGE_PAYLOAD_SIZE		(FLC_PAGE_SIZE + CHECKBYTE_16)
#define MARK_VALID_MAGIC_VAL    0x4D41524B //'MARK'
#define UNINITIALIZED_MEM		0xFFFFFFFF
#define ERROR_MAX_LEN      		20
#define MESSAGE_MAX_LEN      	8208

typedef struct {
	uint32_t CRC32;
	uint32_t length;
	uint32_t valid_mark;
	uint32_t boot_mode;
	/*
	Please note that when app_header_t is modified,
	bootloader linker files also need to be modifed.
	Becase Linker exposes boot_mode varible in the flash memory.
	*/
} app_header_t;
extern void *_app_isr[];
extern int _app_start;
extern int _app_end;
extern int _boot_mode;
extern int _boot_mem_len;
extern int _boot_mem_start;
//int selfprogrammer_flash_image(const char *image);
int bl_load_from_sdcard(const char* filename);
int bl_master_erase();
#endif /* INCLUDE_MAX32666_BL_H_ */
