/*******************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
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


/*****************************    Includes   *********************************/
#include <MAX32665.h>
#include <mxc_sys.h>

#include "max32666_image_capture.h"
#include "max32666_bitmap.h"
#include "max32666_sdcard.h"
#include "maxrefdes178_definitions.h"
#include "max32666_data.h"
#include "max32666_debug.h"
#include "max32666_timer_led_button.h"

/*****************************     Macros    *********************************/
#define S_MODULE_NAME   "imgcap"

#define IMAGE_PREFIX	"img" // each image in sd will start with this prefix

/*****************************    Typedef   *********************************/
typedef struct {
	char onetime;  //1:on , 0:off
	char periodic; //1:on , 0:off
	char store_as_raw;   //1:yes , 0:no
	char store_as_bitmap;//1:yes , 0:no
	//
	unsigned int interval;
	unsigned int nb_of_sample;
	unsigned int currentIndex;
	unsigned int lastIndex;
} image_capture_config_t;

static int g_sd_ready = FALSE;

/*****************************    Variables  *********************************/
static image_capture_config_t g_img_capture_conf;

/*
   Partial write to SDCard is too slow
   To increase performance instead of line to line write
   all file will be written by one write operation
*/
static uint16_t bitmap_data[64 + LCD_WIDTH*LCD_HEIGHT];

/***************************  Static Functions *******************************/
static int is_image_exist(int index)
{
	int ret = TRUE;//

	// use bitmap_data[] buffer to increase performance.
	snprintf((char *)bitmap_data, 128, "%s%u", IMAGE_PREFIX, index);

	if (sdcard_file_exist((char *)bitmap_data) == FALSE) {// 0 means not exist
		// check bitmap file too
		snprintf((char *)bitmap_data, 128, "%s%u.bmp", IMAGE_PREFIX, index);
		if (sdcard_file_exist((char *)bitmap_data) == FALSE) {// 0 means not exist
			ret = FALSE;// false means not exist
		}
	}

	return ret;
}

static int binary_search(int l, int r)
{
    if (r > (l+1) ) {
        int mid = (l + r) / 2;

        if (is_image_exist(mid) == FALSE) {
        	// file not found search [l:mid-1]
            return binary_search(l, mid);
        }

        // file found search [mid+1:r]
        return binary_search(mid, r);
    } else {
    	if (is_image_exist(r) == FALSE) {
    		if (r>0) {
        		r--;
    		}
    	}
    }

    return r;
}

static int check_sd(void)
{
	int ret = 0;

	ret = sdcard_init();
	if (ret == E_NO_ERROR) {

		g_sd_ready = TRUE;

		config_map_t config[] = {
			{"interval", 		0, 0},
			{"number_samples", 	0, 0},
			{"raw", 	0, 0},
			{"bitmap", 	0, 0},
		};

		ret = sdcard_load_config_file("_config.txt", config, sizeof(config)/sizeof(config_map_t), '=');
		if (ret != E_NO_ERROR) {
			PR_INFO("sdcard reading _config.txt file failed %d", ret);
		} else {
			if (config[0].found) g_img_capture_conf.interval        = config[0].val;
			if (config[1].found) g_img_capture_conf.nb_of_sample    = config[1].val;
			if (config[2].found) g_img_capture_conf.store_as_raw    = config[2].val;
			if (config[3].found) g_img_capture_conf.store_as_bitmap = config[3].val;

		}
		/* Find last index in SD, the file shall be sequential
		 * Setting bigger range will cause a delay during setup, so max nubmer configured for 32GB
		 * Each raw image is 115200KB, 32GB/115200KB =~ 275K sample
		 */
		g_img_capture_conf.currentIndex = binary_search(0, 275000);// 275K * 115200 ~= 32GB

		g_img_capture_conf.lastIndex = g_img_capture_conf.currentIndex + g_img_capture_conf.nb_of_sample;
		PR_INFO("Last Image Index in SD: %d\n", g_img_capture_conf.currentIndex);
	}

	return ret;
}

static int store_raw_img(uint8_t *img, unsigned int len)
{
	int ret = 0;
	char fileName[64];

	snprintf(fileName, sizeof(fileName), "%s%u", IMAGE_PREFIX, g_img_capture_conf.currentIndex);

	if (sdcard_file_exist(fileName) == TRUE) {
		return 0; // if file exist pass write, erasing file not good approach
	}

	ret = sdcard_write(fileName, img, len);

	return ret;
}

static int store_bitmap_img(uint8_t *rgb565, unsigned int len)
{
	int ret = 0;
	char fileName[64];
	uint32_t headerLen = sizeof(bitmap_data);

	snprintf(fileName, sizeof(fileName), "%s%u.bmp", IMAGE_PREFIX, g_img_capture_conf.currentIndex);
	if (sdcard_file_exist(fileName) == TRUE) {
		return 0; // if file exist pass write, erasing file not good approach
	}

	BMP_RGB565_create_header(LCD_WIDTH, LCD_HEIGHT, (uint8_t *)bitmap_data, &headerLen);

	if (ret == 0) {
		int x, y, k;
		int line;
		int bytes_per_row = LCD_WIDTH*2;

		k = headerLen/2; // continue after end of header
		for (y=LCD_HEIGHT-1; y >=0; y--) {
			line = bytes_per_row*y;
			for (x=0; x < LCD_WIDTH*2; x+=4) {
				// pixel1
				bitmap_data[k++] = (rgb565[line + x + 0] << 8) + rgb565[line + x + 1];
				// pixel2
				bitmap_data[k++] = (rgb565[line + x + 2] << 8) + rgb565[line + x + 3];
			}
		}
		// write line to file
		ret = sdcard_write(fileName, (uint8_t *)bitmap_data, k*2);
	}

	return ret;
}

/***************************  Public Functions *******************************/
int imgcap_init(void)
{
	int ret = 0;

	g_sd_ready = FALSE;
    g_img_capture_conf.interval = 0;
    g_img_capture_conf.nb_of_sample = 0;
    g_img_capture_conf.currentIndex = 0;
    g_img_capture_conf.lastIndex = 0;

    g_img_capture_conf.store_as_raw = 0;
    g_img_capture_conf.store_as_bitmap = 1; // on default on

    check_sd();

	return ret;
}

int imgcap_set_mode(imgcap_mode_t mode)
{
	int ret = 0;

	switch(mode) {
	case IMGCAP_MODE_ONESHOOT:
		if (g_img_capture_conf.periodic == 0) {
			g_img_capture_conf.onetime = 1;
		} else {
			ret = -1; // wait until sequence end
		}
		break;
	case IMGCAP_MODE_PERIODIC:
		if (g_img_capture_conf.periodic == 0) {
			// If periodic mode is off, check whether last cycle finalized or not
			if (g_img_capture_conf.currentIndex >= g_img_capture_conf.lastIndex) {
				// Update lastIndex for next cycle
				g_img_capture_conf.lastIndex += g_img_capture_conf.nb_of_sample;
			}
		}
		g_img_capture_conf.periodic = 1;
		break;
	default:
		break;
	}

	return ret;
}

unsigned int imgcap_get_interval(void)
{
	return g_img_capture_conf.interval;
}

unsigned int imgcap_get_nb_of_sample(void)
{
	return g_img_capture_conf.nb_of_sample;
}

unsigned int imgcap_get_currentIndex(void)
{
	return g_img_capture_conf.currentIndex;
}

unsigned int imgcap_get_lastIndex(void)
{
	return g_img_capture_conf.lastIndex;
}

imgcap_mode_t imgcap_get_mode(void)
{
	imgcap_mode_t mode = IMGCAP_MODE_NONE;

	if (g_img_capture_conf.periodic == 1) {
		mode = IMGCAP_MODE_PERIODIC;
	} else if (g_img_capture_conf.onetime == 1) {
		mode = IMGCAP_MODE_ONESHOOT;
	}

	return mode;
}

imgcap_sd_status_t imgcap_get_sd_status(void)
{
    imgcap_sd_status_t ret = IMGCAP_SDSTAT_READY; // sd exist and ready

    if (sdcard_card_inserted() == FALSE) {
        g_sd_ready = FALSE;
        return IMGCAP_SDSTAT_SD_NOT_INSERTED; // SD not inserted
    }

    if (g_sd_ready == FALSE) {
    	imgcap_init(); // to set default parameters and check sd card
        if (g_sd_ready == FALSE) {
            ret = IMGCAP_SDSTAT_SD_INIT_FAILED; // SD exist but init failed
        }
    }

    return ret;
}

int imgcap_tick(void)
{
	int ret = -1;
	static unsigned int last_time = 0;

	if (imgcap_get_sd_status() != IMGCAP_SDSTAT_READY) {
		return -1;
	}

	if (g_img_capture_conf.onetime) {
		g_img_capture_conf.onetime = 0;// clear flag
		g_img_capture_conf.lastIndex++;   // to do not decrease nb_of_sample
		g_img_capture_conf.currentIndex++; // write succeeded so increase counter.

		PR_INFO("Image %d saving", g_img_capture_conf.currentIndex);

		ret = 0;
		if (ret == 0) {
			if (g_img_capture_conf.store_as_raw) {
				ret = store_raw_img(lcd_data.buffer, LCD_DATA_SIZE);
			}
		}

		if (ret == 0) {
			if (g_img_capture_conf.store_as_bitmap) {
				ret = store_bitmap_img(lcd_data.buffer, LCD_DATA_SIZE);
			}
		}

		if (ret != 0) { // failed
			g_img_capture_conf.currentIndex--;
			g_img_capture_conf.lastIndex--;
		}
	} else if (g_img_capture_conf.periodic) {

		ret = 0;
		if ( (timer_ms_tick-last_time) > g_img_capture_conf.interval) {
			last_time = timer_ms_tick;
			if (g_img_capture_conf.currentIndex >= g_img_capture_conf.lastIndex) {
				g_img_capture_conf.periodic = 0;// disable flag
			} else {
				g_img_capture_conf.currentIndex++;
				PR_INFO("Image %d/%d saving, Time:%u", g_img_capture_conf.currentIndex, g_img_capture_conf.lastIndex, last_time);

				if (ret == 0) {
					if (g_img_capture_conf.store_as_raw) {
						ret = store_raw_img(lcd_data.buffer, LCD_DATA_SIZE);
					}
				}

				if (ret == 0) {
					if (g_img_capture_conf.store_as_bitmap) {
						ret = store_bitmap_img(lcd_data.buffer, LCD_DATA_SIZE);
					}
				}

				if (ret != 0) { // failed
					g_img_capture_conf.currentIndex--;
				}
			}
		}
	}

	return ret;
}
