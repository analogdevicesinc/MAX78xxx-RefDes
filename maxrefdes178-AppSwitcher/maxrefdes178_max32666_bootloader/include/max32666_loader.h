/*
 ******************************************************************************
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
 ******************************************************************************
 */

#ifndef _MAX32666_LOADER_H_
#define _MAX32666_LOADER_H_

/*******************************      INCLUDES    ****************************/
#include "max32666_loader_cmd.h"

/*******************************      DEFINES     ****************************/
#define GPIO_IDX_BL0	0	// RESET PIN
#define GPIO_IDX_BL1	1	// MFIO  PIN

/******************************* Type Definitions ****************************/
typedef int (*comm_read_t)(unsigned char *dst, unsigned int len);
typedef int (*comm_write_t)(unsigned char *src, unsigned int len);

typedef struct {
	comm_read_t  read;
	comm_write_t write;
	void (*gpio_set)(unsigned int idx, int state);
	void (*delay_ms)(unsigned int ms);
} bl_conf_struct_t;

typedef struct {
	unsigned char  magic[4];
	unsigned int   formatVersion;
	unsigned char  target[16];
	unsigned char  enc_type[16];
	unsigned char  nonce[11];
	unsigned char  resv0;
	unsigned char  auth[16];
	unsigned short numPages;
	unsigned short pageSize;
	unsigned char  crcSize;
	unsigned char  resv1[3];
} MsblHeader_t;

/******************************* Public Functions ****************************/
int loader_init(bl_conf_struct_t *plt_funcs);
int loader_update_interface(comm_read_t read_func,  comm_write_t write_func);
//
int loader_hard_reset_then_enter_bl_mode(void);
int loader_enter_bl_mode(void);
int loader_exit_bl_mode(void);
int loader_get_partnumber(char *buf, unsigned int maxLen);
int loader_get_version(char *buf, unsigned int maxLen);
int loader_get_usn(char *buf, unsigned int maxLen);
int loader_get_page_size(unsigned int *page_size);
int loader_erase_app(void);
int loader_set_num_pages(int page_num);
int loader_set_iv(unsigned char *iv);
int loader_set_auth(unsigned char *auth);
int loader_write_page(const char *page, unsigned int page_len);
int loader_flash_image(const char* filename, int video_audio);


// Bootloader configuration section
int loader_get_target_configure(const char *target_bl_version, void *bl_cfg_struct);
int loader_update_cfg_enter_bl_check(int enable);
int loader_update_cfg_set_ebl_pin(int port, int pin);
int loader_update_cfg_set_ebl_pin_polarity(int polarity);
int loader_update_cfg_set_valid_mark_check(int enable);
int loader_update_cfg_enable_uart(int enable);
int loader_update_cfg_enable_i2c(int enable);
int loader_update_cfg_enable_spi(int enable);
int loader_update_cfg_set_i2c_addr(const char *target_bl_version, unsigned char addr);
int loader_update_cfg_set_crc_check(int enable);
int loader_update_cfg_lock_swd(int enable);
int loader_update_cfg_set_bl_exit_mode(BLExitMode_t mode);
int loader_update_cfg_set_bl_exit_timeout(unsigned short timeout);
int loader_flash_bl_cfg(void);

#endif // _MAX32666_LOADER_H_
