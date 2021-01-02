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

#ifndef _MAX32666_DATA_H_
#define _MAX32666_DATA_H_


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdint.h>
#include <mxc_sys.h>

#include "maxrefdes178_definitions.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------
typedef struct {
    uint8_t enable_ble;
    uint8_t enable_lcd;
    uint8_t enable_max78000_audio;       // TODO
    uint8_t enable_max78000_audio_cnn;   // TODO
    uint8_t enable_max78000_video;       // TODO
    uint8_t enable_max78000_video_cnn;
    uint8_t enable_max78000_video_flash_led;        // TODO
    uint8_t enable_max78000_video_and_audio_power;  // TODO
    uint8_t enable_show_statistics_lcd;  // TODO
    uint8_t enable_show_probabilty_lcd;  // TODO
    uint8_t enable_send_statistics;      // TODO
    uint8_t enable_send_classification;  // TODO
} device_settings_t;

typedef struct {
    device_serial_num_t device_serial_num; // TODO
    device_version_t device_version;
    uint8_t ble_mac[6];  // written by core1
} device_info_t;

typedef struct {
    device_statistics_t statistics;      // TODO
    classification_result_t classification_video;
    classification_result_t classification_audio;
    faceid_embed_update_status_e faceid_embed_update_status_e;  // TODO

    uint8_t ble_status_changed;  // written by both core0 and core1
    uint8_t ble_connected;  // written by core1
    uint8_t ble_connected_peer_mac[6];  // written by core1
    uint8_t ble_expected_rx_seq;  // written by core1
    uint8_t ble_next_tx_seq;  // written by core1
} device_status_t;

typedef struct {
    uint8_t buffer[LCD_DATA_SIZE];
    char subtitle[LCD_SUBTITLE_SIZE];
    char toptitle[LCD_TOPTITLE_SIZE];
    char notification[LCD_NOTIFICATION_SIZE];
    uint16_t subtitle_color;
    uint16_t toptitle_color;
    uint16_t notification_color;
    uint16_t frame_color;
} lcd_data_t;

typedef struct {
    uint32_t audio_result_received;
    uint32_t notification_received;
    uint32_t screen_drew;
    uint32_t statistics_sent;
} timestamps_t;


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
extern lcd_data_t lcd_data;
extern device_status_t device_status;
extern device_settings_t device_settings;
extern device_info_t device_info;
extern timestamps_t timestamps;


//-----------------------------------------------------------------------------
// Function declarations
//-----------------------------------------------------------------------------



#endif /* _MAX32666_DATA_H_ */
