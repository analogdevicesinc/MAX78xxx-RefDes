/*******************************************************************************
* Copyright (C) 2020-2021 Maxim Integrated Products, Inc., All Rights Reserved.
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

/*******************************      INCLUDES    ****************************/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ff.h>
#include <sdhc_lib.h>
#include <mxc_delay.h>

#include "max32666_loader.h"
#include "max32666_debug.h"
#include "max32666_bl.h"
#include "max32666_fonts.h"
#include "max32666_lcd.h"


/*******************************      DEFINES     ****************************/
#define S_MODULE_NAME   "loader"
#ifndef MIN
	#define MIN(x,y)	( (x<y) ? x: y )
#endif

#ifndef MAX
	#define MAX(x,y)	( (x>y) ? x: y )
#endif

/******************************* Type Definitions ****************************/


/******************************* 	Variables 	  ****************************/
static bl_conf_struct_t g_plt_funcs;
static char page_plain[PAGE_PAYLOAD_SIZE] __attribute__ ((aligned (4)));
extern FIL file;
extern TCHAR *FF_ERRORS[ERROR_MAX_LEN];

/******************************* Static Functions ****************************/
//static void hexdump(const char* title, unsigned char* buf, unsigned int len)
//{
//    unsigned int i;
//
//    if (title) {
//    	PR_INFO("%s", title);
//    }
//
//    /* Print buffer bytes */
//    for (i = 0; i < len; i++) {
//        if (!(i % 16)) {
//        	PR_INFO("");
//        }
//        PR_INFO("%02X ", buf[i]);
//    }
//
//    PR_INFO("");
//}

static int send_rcv(unsigned char *tx, int txLen, unsigned char *rx, int rxLen, int delay_ms, int delay_step_ms)
{
	int ret = 0;
	int i;

	if (delay_step_ms == 0) {
		delay_step_ms = 100; // 100ms default
	}

	for (i=0; i<2; i++) {
		ret = g_plt_funcs.write(tx, 2);
		if (ret == 0) {
			if(txLen>2)
			{
			    MXC_Delay(15);
				ret = g_plt_funcs.write(tx+2, txLen-2);
				if (ret == 0) {
					break;
				}
			}
			else
			{
				break;
			}
		}

		g_plt_funcs.delay_ms(100);
	}

	if (ret == 0) {
        int nb_of_retry = (delay_ms / delay_step_ms) + 1;
        for (i = 0; i < nb_of_retry; i++) {
        	if (delay_ms > 0) {
        		g_plt_funcs.delay_ms(delay_step_ms);
        	}

        	// read
        	MXC_Delay(15);
        	ret = g_plt_funcs.read(rx, rxLen);

			if ( (ret == 0) && (rx[0] != BL_RET_ERR_TRY_AGAIN) ) {
				break;
			}
        }

        // Convert BL return value
    	if (rx[0] == BL_RET_SUCCESS) {
    		ret = 0; // zero means success
    	} else if (rx[0] == 0) {
    		ret = -1;
    	} else {
    		ret = rx[0]; // first byte is BL response
    	}
	}

	return ret;
}

static int update_bl_cfg (unsigned char item, unsigned char cmd)
{
	int ret = 0;
	unsigned char req[4];
	unsigned char rsp[1] = {0xFF, };

	req[0] = BLCmdConfigWrite_MAIN_CMD;
	req[1] = BLCmdConfigWrite_ENTRY_CONFIG;
	req[2] = item;
	req[3] = cmd;

    ret = send_rcv(req, sizeof(req), rsp, 1, 0, 0);

	return ret;
}

/******************************* Public Functions ****************************/
int loader_init(bl_conf_struct_t *plt_funcs)
{
	int ret = 0;

	g_plt_funcs = *plt_funcs;

	return ret;
}

int loader_update_interface(comm_read_t read_func,  comm_write_t write_func)
{
	int ret = 0;

	g_plt_funcs.read  	  = read_func;
	g_plt_funcs.write 	  = write_func;

	return ret;
}

int loader_hard_reset_then_enter_bl_mode(void)
{
	int ret = 0;

	g_plt_funcs.gpio_set(GPIO_IDX_BL0, 0);
	g_plt_funcs.delay_ms(10);
	g_plt_funcs.gpio_set(GPIO_IDX_BL0, 1);
	g_plt_funcs.delay_ms(10);

	ret = loader_enter_bl_mode();

	return ret;
}

int loader_enter_bl_mode(void)
{
	int ret = 0;
	unsigned char req[ ] = {BLCmdDevSetMode_MAIN_CMD, BLCmdDevSetMode_SET_MODE, 0x08};
	unsigned char rsp[1] = {0xFF, };

    ret = send_rcv(req, sizeof(req), rsp, 1, 0, 0);

	return ret;
}

int loader_exit_bl_mode(void)
{
	int ret = 0;
	unsigned char req[ ] = {BLCmdDevSetMode_MAIN_CMD, BLCmdDevSetMode_SET_MODE, 0x00};
	unsigned char rsp[1] = {0xFF, };

    ret = send_rcv(req, sizeof(req), rsp, 1, 0, 0);

	return ret;
}

int loader_get_partnumber(char *buf, unsigned int maxLen)
{
	int ret = 0;
	unsigned char req[ ] = {BLCmdDeviceInfo_MAIN_CMD, BLCmdDeviceInfo_GET_PLATFORM_TYPE};
	unsigned char rsp[2] = {0xFF, };

    ret = send_rcv(req, sizeof(req), rsp, 2, 0, 0);

    if (ret == 0) {
    	switch(rsp[1]) {
			case 1: memcpy(buf, "MAX32660", MIN(strlen("MAX32660")+1, maxLen)); break;
			case 3: memcpy(buf, "MAX32670", MIN(strlen("MAX32670")+1, maxLen)); break;
			case 5: memcpy(buf, "MAX78000", MIN(strlen("MAX78000")+1, maxLen)); break;
			case 6: memcpy(buf, "MAX32655", MIN(strlen("MAX32655")+1, maxLen)); break;
			default:
				memcpy(buf, "UNKNOWN", MIN(strlen("UNKNOWN")+1, maxLen));
				break;
    	}
    }

	return ret;
}

int loader_get_version(char *buf, unsigned int maxLen)
{
	int ret = 0;
	unsigned char req[ ] = {BLCmdInfo_MAIN_CMD, BLCmdInfo_GET_VERSION};
	unsigned char rsp[4] = {0xFF, };

    ret = send_rcv(req, sizeof(req), rsp, 4, 0, 0);

    if (ret == 0) {
    	snprintf(buf, maxLen, "v%d.%d.%d", rsp[1], rsp[2], rsp[3]);
    }

	return ret;
}

int loader_get_usn(char *buf, unsigned int maxLen)
{
	int ret = 0;
	unsigned char req[ ]  = {BLCmdInfo_MAIN_CMD, BLCmdInfo_GET_USN};
	unsigned char rsp[25] = {0xFF, };

    ret = send_rcv(req, sizeof(req), rsp, 24, 0, 0);

    if (ret == 0) {
    	memcpy(buf, &rsp[1], MIN(24, maxLen));
    }

	return ret;
}

int loader_get_page_size(unsigned int *page_size)
{
	int ret = 0;
	unsigned char req[ ] = {BLCmdInfo_MAIN_CMD, BLCmdInfo_GET_PAGE_SIZE};
	unsigned char rsp[3] = {0xFF, };

    ret = send_rcv(req, sizeof(req), rsp, 3, 0, 0);

    if (ret == 0) {
    	*page_size =  (rsp[1]<<8) | rsp[2];
    }

	return ret;
}

int loader_erase_app(void)
{
	int ret = 0;
	unsigned char req[ ] = {BLCmdFlash_MAIN_CMD,  BLCmdFlash_ERASE_APP_MEMORY};
	unsigned char rsp[1] = {0xFF, };

    ret = send_rcv(req, sizeof(req), rsp, 1, 4000, 1000);

	return ret;
}

int loader_set_num_pages(int page_num)
{
	int ret = 0;
	unsigned char req[4];
	unsigned char rsp[1] = {0xFF, };

    req[0] = BLCmdFlash_MAIN_CMD;
    req[1] = BLCmdFlash_SET_NUM_PAGES;
    req[2] = (page_num>>8) & 0xff;
    req[3] = (page_num>>0) & 0xff;

    ret = send_rcv(req, sizeof(req), rsp, 1, 0, 0);

	return ret;
}

int loader_set_iv(unsigned char *iv)
{
	int ret = 0;
	unsigned char req[2+AES_IV_SIZE];
	unsigned char rsp[1] = {0xFF, };

    req[0] = BLCmdFlash_MAIN_CMD;
    req[1] = BLCmdFlash_SET_IV;
    memcpy(&req[2], iv, AES_IV_SIZE);

    ret = send_rcv(req, sizeof(req), rsp, 1, 0, 0);

	return ret;
}

int loader_set_auth(unsigned char *auth)
{
	int ret = 0;
	unsigned char req[2+AES_AUTH_SIZE];
	unsigned char rsp[1] = {0xFF, };

    req[0] = BLCmdFlash_MAIN_CMD;
    req[1] = BLCmdFlash_SET_AUTH;
    memcpy(&req[2], auth, AES_AUTH_SIZE);

    ret = send_rcv(req, sizeof(req), rsp, 1, 0, 0);

	return ret;
}

int loader_set_key(unsigned char *key)
{
	int ret = 0;
	unsigned char req[2+AES_KEY_LOAD_SIZE];
	unsigned char rsp[1] = {0xFF, };

    req[0] = BLCmdFlash_MAIN_CMD;
    req[1] = BLCmdFlash_SET_KEY;
    memcpy(&req[2], key, AES_KEY_LOAD_SIZE);//valid key len, 32 byte key padded with 0, valid aad len 32 byte aad padded with 0

    ret = send_rcv(req, sizeof(req), rsp, 1, 0, 0);

	return ret;
}

int loader_write_page(const char *page, unsigned int page_len)
{
	int ret = 0;
	static unsigned char req[8*1024+32];// use static buffer
	unsigned char rsp[1] = {0xFF, };

	if (page_len > (sizeof(req)-2) ) {
		return -1;
	}

    req[0] = BLCmdFlash_MAIN_CMD;
    req[1] = BLCmdFlash_WRITE_PAGE;
    memcpy(&req[2], page, page_len);

    ret = send_rcv(req, page_len+2, rsp, 1, 1200, 300);

	return ret;
}

int loader_flash_image(const char* filename, int video_audio)
{
	int ret;
	int i;
	unsigned int bytes_read;
	MsblHeader_t header;
	int checksum_size = 16; // checksum value at the end of page
    PR_INFO("Attempting to read back file...");
    if((ret = f_open(&file, filename, FA_READ)) != FR_OK){
        PR_ERROR("Error opening file: %s", FF_ERRORS[ret]);
        f_mount(NULL, "", 0);
        return ret;
    }
	//memcpy(&header, image, sizeof(MsblHeader_t));
	f_read(&file, &header, sizeof(MsblHeader_t), &bytes_read);

    PR_INFO("MSBL Info");
	PR_INFO("------------------------------------------------");
	PR_INFO("%-15s: %c%c%c%c", 	"magic", header.magic[0], header.magic[1], header.magic[2], header.magic[3]);
	PR_INFO("%-15s: %d", 		"formatVersion", header.formatVersion);
	PR_INFO("%-15s: %s", 		"target", header.target);
	PR_INFO("%-15s: %s", 		"EncType", header.enc_type);
	PR_INFO("%-15s: %d", 		"numPages", header.numPages);
	PR_INFO("%-15s: %d", 		"pageSize", header.pageSize);
	PR_INFO("%-15s: %d", 		"crcSize", header.crcSize);
	PR_INFO("%-15s: %d", 		"Header Size", sizeof(header));
	PR_INFO("%-15s: %d", 		"resv0", header.resv0);
	//
//	hexdump("nonce", header.nonce, 	AES_IV_SIZE);
//	hexdump("auth",  header.auth,   AES_AUTH_SIZE);
//	hexdump("resv1", header.resv1,  3);

	ret = loader_hard_reset_then_enter_bl_mode();
    if (ret) {
        return ret;
    }

	ret = loader_set_num_pages(header.numPages);
    if (ret) {
    	PR_ERROR("Error! bl_set_num_pages");
        return ret;
    }
//
//	ret = bl_set_iv(header.nonce);
//    if (ret) {
//    	PR_ERROR("Error! bl_set_iv");
//        return ret;
//    }
//
//	ret = bl_set_auth(header.auth);
//    if (ret) {
//    	PR_ERROR("Error! bl_set_auth");
//        return ret;
//    }

	ret = loader_erase_app();
    if (ret) {
        PR_ERROR("Error! bl_erase_app");
        return ret;
    }

    int page_len = header.pageSize + checksum_size;
    for (i=0; i < header.numPages; i++) {
    	f_read(&file, page_plain, page_len, &bytes_read);
		ret = loader_write_page( page_plain, page_len);
		if (ret) {
			PR_INFO("Flashing page %d/%d  [FAILED] err:%d", i+1, header.numPages, ret);
			return ret;
		}
		PR_INFO("Flashing page %d/%d  [SUCCESS]", i+1, header.numPages);

		if (video_audio) { // MAX78000 Video
		    sprintf(line_str, "MAX78000 Video FW %d/%d", i+1, header.numPages);
		    fonts_putStringOver(1, 80, line_str, &Font_7x10, BLACK, 0, 0, lcd_buff);
		    lcd_drawImage(lcd_buff);
		} else { // MAX78000 Audio
		    sprintf(line_str, "MAX78000 Audio FW %d/%d", i+1, header.numPages);
		    fonts_putStringOver(1, 60, line_str, &Font_7x10, BLACK, 0, 0, lcd_buff);
		    lcd_drawImage(lcd_buff);
		}
    }

    loader_exit_bl_mode();

	return ret;
}

int loader_get_target_configure(const char *target_bl_version, void *bl_cfg_struct)
{
	int ret = 0;
	unsigned char req[3];
	unsigned char rsp[9] = {0xFF, };

    req[0] = BLCmdConfigRead_MAIN_CMD;
    req[1] = BLCmdConfigRead_ALL_CFG;
    req[2] = 0x00; // dummy byte

    if (strcmp(target_bl_version, "v3.4.1") <= 0) {
		ret = send_rcv(req, sizeof(req), rsp, 5, 0, 0);

		if (ret == 0) {
			//
			((boot_config_t_before_v342 *)bl_cfg_struct)->v[0] = rsp[4];
			((boot_config_t_before_v342 *)bl_cfg_struct)->v[1] = rsp[3];
			((boot_config_t_before_v342 *)bl_cfg_struct)->v[2] = rsp[2];
			((boot_config_t_before_v342 *)bl_cfg_struct)->v[3] = rsp[1];
		}
    } else {
    	ret = send_rcv(req, sizeof(req), rsp, 9, 0, 0);

		if (ret == 0) {
			//
			((boot_config_t *)bl_cfg_struct)->v[0] = rsp[8];
			((boot_config_t *)bl_cfg_struct)->v[1] = rsp[7];
			((boot_config_t *)bl_cfg_struct)->v[2] = rsp[6];
			((boot_config_t *)bl_cfg_struct)->v[3] = rsp[5];
			((boot_config_t *)bl_cfg_struct)->v[4] = rsp[4];
			((boot_config_t *)bl_cfg_struct)->v[5] = rsp[3];
			((boot_config_t *)bl_cfg_struct)->v[6] = rsp[2];
			((boot_config_t *)bl_cfg_struct)->v[7] = rsp[1];
		}
    }

	return ret;
}

int loader_update_cfg_enter_bl_check(int enable)
{
    return update_bl_cfg(BL_CFG_EBL_CHECK,  enable?1:0);
}

int loader_update_cfg_set_ebl_pin(int port, int pin)
{
    return update_bl_cfg(BL_CFG_EBL_PIN, (port<<5) | pin);
}

int loader_update_cfg_set_ebl_pin_polarity(int polarity)
{
    return update_bl_cfg(BL_CFG_EBL_POL, polarity?1:0);
}

int loader_update_cfg_set_valid_mark_check(int enable)
{
    return update_bl_cfg(BL_CFG_VALID_MARK_CHK, enable?1:0);
}

int loader_update_cfg_enable_uart(int enable)
{
    return update_bl_cfg(BL_CFG_UART_ENABLE, enable?1:0);
}

int loader_update_cfg_enable_i2c(int enable)
{
    return update_bl_cfg(BL_CFG_I2C_ENABLE, enable?1:0);
}

int loader_update_cfg_enable_spi(int enable)
{
    return update_bl_cfg(BL_CFG_SPI_ENABLE, enable?1:0);
}

int loader_update_cfg_set_i2c_addr(const char *target_bl_version, unsigned char addr)
{
	int ret = -1; // on default error

    if (strcmp(target_bl_version, "v3.4.1") <= 0) {
             if (addr == 0x58) ret = update_bl_cfg(BL_CFG_I2C_SLAVE_ADDR, 0);
        else if (addr == 0x5A) ret = update_bl_cfg(BL_CFG_I2C_SLAVE_ADDR, 1);
        else if (addr == 0x5C) ret = update_bl_cfg(BL_CFG_I2C_SLAVE_ADDR, 2);
        else if (addr == 0xAA) ret = update_bl_cfg(BL_CFG_I2C_SLAVE_ADDR, 3);
        else {
            PR_INFO("%s version bootloader does not support 0x%X I2C addr", target_bl_version, addr);
        }
    } else {
    	ret = update_bl_cfg(BL_CFG_I2C_SLAVE_ADDR, addr);
    }

    return ret;
}

int loader_update_cfg_set_crc_check(int enable)
{
    return update_bl_cfg(BL_CFG_CRC_CHECK, enable?1:0);
}

int loader_update_cfg_lock_swd(int enable)
{
    return update_bl_cfg(BL_CFG_LOCK_SWD, enable?1:0);
}

int loader_update_cfg_set_bl_exit_mode(BLExitMode_t mode)
{
	int ret = 0;
	unsigned char req[4];
	unsigned char rsp[1] = {0xFF, };

    req[0] = BLCmdConfigWrite_MAIN_CMD;
    req[1] = BLCmdConfigWrite_EXIT_CONFIG;
    req[2] = 0x00; // means exit mode
    req[3] = mode; //

	ret = send_rcv(req, sizeof(req), rsp, 1, 0, 0);

	return ret;
}

int loader_update_cfg_set_bl_exit_timeout(unsigned short timeout)
{
	int ret = 0;
	unsigned char req[4];
	unsigned char rsp[1] = {0xFF, };
	int i;

    req[0] = BLCmdConfigWrite_MAIN_CMD;
    req[1] = BLCmdConfigWrite_EXIT_CONFIG;
    req[2] = BL_EXIT_TIMEOUT;

    // Log2(timeout) then round
    for (i = 15; i > 0; i--) {
    	if (timeout & (1<<i)) { // Does MSB bit set?

    		// Check Round
			if ( ( timeout & ~(1<<i) ) >= (1<<i)/2 ) {
			    req[3] = i+1; // add plus one
			} else {
			    req[3] = i;
			}
			break;
    	}
    }

	ret = send_rcv(req, sizeof(req), rsp, 1, 0, 0);

	return ret;
}

int loader_flash_bl_cfg(void)
{
	int ret = 0;
	unsigned char req[ ] = {BLCmdConfigWrite_MAIN_CMD, BLCmdConfigWrite_SAVE_SETTINGS};
	unsigned char rsp[1] = {0xFF, };

	ret = send_rcv(req, sizeof(req), rsp, 1, 1000, 0);

	return ret;
}
