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
#include <max32665.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <flc.h>
#include <icc.h>
#include <ff.h>
#include <sdhc_lib.h>
#include <mxc_delay.h>
#include "max32666_bl.h"
#include "max32666_debug.h"
#include "max32666_loader.h"
#include "max32666_fonts.h"
#include "max32666_lcd.h"
#include "crc32.h"

#define S_MODULE_NAME   "bl"
extern FIL file;
extern TCHAR *FF_ERRORS[ERROR_MAX_LEN];
TCHAR message[MESSAGE_MAX_LEN];

static uint8_t page_plain[PAGE_PAYLOAD_SIZE] __attribute__ ((aligned (4)));
#if defined(SECURE_BOOTLOADER)
static uint8_t page_cipher[FLC_PAGE_SIZE + CHECKBYTE_16] __attribute__ ((aligned (4)));
#endif

int flc_init(void)
{
    return MXC_FLC_Init();
}

int flc_uninit(void)
{
    MXC_FLC_ClearFlags(MXC_F_FLC_INTR_DONE | MXC_F_FLC_INTR_AF);
    return 0;
}

int flc_erase_page(uint32_t address)
{
    int res;
    __disable_irq();
    MXC_ICC_Disable();
    res = MXC_FLC_PageErase(address);
    MXC_ICC_Enable();
    __enable_irq();
    return res;
}

int flc_prog_page(uint32_t address, uint32_t size, uint8_t *buffer)
{
    uint32_t dest_addr;
    int i = 0;
    uint32_t *buffer32 = (uint32_t *)buffer;

    __disable_irq();
    MXC_ICC_Disable();
    for (dest_addr = address; dest_addr < (address + size); dest_addr += 4) {
        // Write a word
        if (MXC_FLC_Write32(dest_addr, buffer32[i]) != E_NO_ERROR) {
            break;
        }

        i++;
    }
    MXC_ICC_Enable();
    __enable_irq();

    volatile uint8_t *ptr, *data;
    data = buffer;

    for (ptr = (uint8_t *)address; ptr < (uint8_t *)(address + size); ptr++, data++) {
        if (*ptr != *data) {
            return E_UNKNOWN;
        }
    }

    return 0;
}

static int erase_boot_mem_page(unsigned char delete_mark)
{
	int ret;
	uint8_t bootmem_buf[(unsigned long)&_boot_mem_len];

	memcpy(bootmem_buf, (void *)&_boot_mem_start, (unsigned long)&_boot_mem_len);

    app_header_t *header = (app_header_t *)bootmem_buf;
	if (delete_mark) {
	   header->valid_mark = 0;
	}

	ret = flc_erase_page((unsigned long)&_boot_mem_start);
	if (ret)
	{
		return -1;
	}

	ret = flc_prog_page((unsigned long)&_boot_mem_start, (unsigned long)&_boot_mem_len, bootmem_buf);
	if (ret) {
		return -1;
	}

	return 0;
}


int bl_master_erase()
{
	int ret;
	int startPage;
	int currentPage;
	int lastPage;
	int memLoc;
	unsigned long flash_size, app_start_loc;

	flc_uninit();
	flc_init();
	app_start_loc = (unsigned long)&_app_start;

	flash_size = (unsigned long)&_app_end - (unsigned long)&_app_start;

	startPage = ((app_start_loc - MXC_FLASH_MEM_BASE) >> FLC_PAGE_BIT_SIZE);
	lastPage = (flash_size >> FLC_PAGE_BIT_SIZE) + startPage;


	for (currentPage = startPage; currentPage < lastPage; currentPage++) {
		memLoc = MXC_FLASH_MEM_BASE + (currentPage * FLC_PAGE_SIZE);

		ret = flc_erase_page(memLoc);
		if (ret)
		{
			flc_uninit();
			return ret;
		}
	}

	erase_boot_mem_page(1);
	flc_uninit();
	return 0;
}

//Return 0 if valid
//Return others if not valid
char check_if_app_is_valid(unsigned char valid_check_flag)
{
	unsigned long flash_size = (unsigned long)&_app_end - (unsigned long)&_app_start;
	//Does app have a valid isr vector table?
	if ((uint32_t)_app_isr[1] == UNINITIALIZED_MEM) {
		return -1;
	}

	if(valid_check_flag) {
		//Get application crc from app header/footer
		app_header_t *header = (app_header_t *)&_boot_mem_start;

		if ((header->length == UNINITIALIZED_MEM) || (header->length > flash_size)) {
			return -2;
		} else if (header->valid_mark != MARK_VALID_MAGIC_VAL) {
			return -3;
		} else if (_boot_mode == UNINITIALIZED_MEM) {
			return -4;
		}
	}

	return 0;
}

//Return 0 if valid
//Return others if not valid
char check_app_crc(unsigned char crc_check_flag)
{
	unsigned long app_start_loc = (unsigned long)&_app_start;
	if(crc_check_flag) {
		//Get application crc from app header/footer
		app_header_t *header = (app_header_t *)&_boot_mem_start;
	    MXC_Delay(MXC_DELAY_MSEC(10)); // magic delay
		uint32_t app_crc = calcCrc32((uint8_t*)app_start_loc, header->length);
		if (app_crc == header->CRC32) {
			return 0;
		} else {
			return -1;
		}
	}
	return 0;
}

int bl_load_image(FIL *file)
{
	int ret;
	int startPage;
	int currentPage;
	int bootMemPage;
	int endPage;
	MsblHeader_t header;
	unsigned long app_start_loc = (unsigned long)&_app_start;
	unsigned int bytes_read;
	unsigned long memLoc;
	unsigned long flc_write_size;
	unsigned char is_app_valid = -1;
	unsigned char is_crc_ok = -1;

	f_read(file, &header, sizeof(MsblHeader_t), &bytes_read);

    MXC_Delay(MXC_DELAY_MSEC(10)); // magic delay

	//memcpy(&header, image, sizeof(MsblHeader_t));

#if defined(SECURE_BOOTLOADER)
	bl_security_set_aes_auth(header.auth,sizeof(header.auth));
	bl_security_set_aes_nonce(header.nonce,sizeof(header.nonce));
    int encrypted_img_size = header.numPages * (FLC_PAGE_SIZE + CHECKBYTE_16);
    ret = bl_security_init(encrypted_img_size);
    if (ret) {
		return -1;
    }
#endif

	ret = bl_master_erase();
    if (ret) {
        return ret;
    }

	startPage = ((app_start_loc - MXC_FLASH_MEM_BASE) >> FLC_PAGE_BIT_SIZE);
	bootMemPage = (uint32_t)&_boot_mem_start - MXC_FLASH_MEM_BASE;
	bootMemPage >>= FLC_PAGE_BIT_SIZE;
	currentPage = startPage;
	endPage = startPage + header.numPages - 1;
    while (currentPage < (startPage + header.numPages)) {


#if defined(SECURE_BOOTLOADER)
    	f_read(file, page_cipher, FLC_PAGE_SIZE + CHECKBYTE_16, &bytes_read);
    	bl_security_decrypt(page_plain, page_cipher, FLC_PAGE_SIZE + CHECKBYTE_16);

#else
    	f_read(file, page_plain, FLC_PAGE_SIZE + CHECKBYTE_16, &bytes_read);
#endif

		//memcpy(page_plain, (const uint8_t*)&image[sizeof(MsblHeader_t) + (currentPage - startPage)*page_len], FLC_PAGE_SIZE + CHECKBYTE_16);
		//Checksum
		if (crcVerifyMsg((const uint8_t*)page_plain, FLC_PAGE_SIZE + CHECK_BYTESIZE)) {
			return -1;
		}

		if (currentPage < endPage) { //currentPage < bootMemPage
			memLoc = (currentPage * FLC_PAGE_SIZE) + MXC_FLASH_MEM_BASE;
			flc_write_size = FLC_PAGE_SIZE;
			if(currentPage == bootMemPage) {
				flc_write_size = FLC_PAGE_SIZE - (unsigned long)&_boot_mem_len;
			}
		} else {
			app_header_t header;
#if defined(SECURE_BOOTLOADER)
			char authenticated = bl_security_decrypt_auth_valid();
			if (!authenticated) {
				bl_master_erase();
				return -1;
			}
#endif

			memcpy(&header, page_plain, sizeof(app_header_t));
			if (header.valid_mark) {
				bl_master_erase();
				return -1;
			}
			if (header.length == 0 || header.length > (_boot_mem_start - _app_start)) {
				bl_master_erase();
				return -2;
			}

			/* Back up page, and update boot memory arguments */
			memLoc = (bootMemPage * FLC_PAGE_SIZE) + MXC_FLASH_MEM_BASE;
			memcpy(page_plain, (void *)memLoc, FLC_PAGE_SIZE);
			header.valid_mark = MARK_VALID_MAGIC_VAL;
			uint32_t offset = (uint32_t)&_boot_mem_start & (FLC_PAGE_SIZE - 1);
			memcpy(&page_plain[offset], &header, sizeof(app_header_t));
			flc_write_size = FLC_PAGE_SIZE;
			flc_erase_page((uint32_t)memLoc);
		}


		ret = flc_prog_page(memLoc, flc_write_size, page_plain);
		if (ret) {
			return -3;
		}
		currentPage++;

        sprintf(line_str, "MAX32666 FW %d/%d", currentPage - startPage, header.numPages);
        fonts_putStringOver(1, 100, line_str, &Font_7x10, BLACK, 0, 0, lcd_buff);
        lcd_drawImage(lcd_buff);
	}

	flc_uninit();
	is_app_valid = check_if_app_is_valid(1);
	is_crc_ok = check_app_crc(1);

    MXC_Delay(MXC_DELAY_MSEC(10)); // magic delay

	if (is_crc_ok || is_app_valid) {
		bl_master_erase();
		return -1;
	}

	return 0;

}

int bl_load_from_sdcard(const char* filename)
{

	int err;
    PR_INFO("Attempting to read back file...");
    MXC_Delay(MXC_DELAY_MSEC(10)); // magic delay

    if((err = f_open(&file, filename, FA_READ)) != FR_OK){
        PR_ERROR("Error opening file: %s", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }
    if((err = bl_load_image(&file)) != FR_OK){
        PR_ERROR("Error programming MAX32666: %d", err);
        f_mount(NULL, "", 0);
        return err;
    }

    if((err = f_close(&file)) != FR_OK){
        PR_ERROR("Error closing file: %s", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }
    PR_INFO("File Closed!\n");

    return 0;
}
