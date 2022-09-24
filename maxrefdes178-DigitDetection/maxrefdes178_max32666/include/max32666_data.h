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
typedef enum {
    INACTIVITY_STATE_ACTIVE = 0,
    INACTIVITY_STATE_INACTIVE_SHORT,
    INACTIVITY_STATE_INACTIVE_LONG,

    INACTIVITY_STATE_LAST
} inactivity_state_e;

typedef struct {
    volatile uint8_t enable_ble;  // shared with core1
    uint8_t enable_lcd;
    uint8_t enable_max78000_audio;
    uint8_t enable_max78000_audio_cnn;   // TODO
    uint8_t enable_max78000_video;
    uint8_t enable_max78000_video_cnn;
    uint8_t enable_max78000_video_flash_led;
    uint8_t enable_max78000_video_vflip;
    uint8_t enable_max78000_video_and_audio_power;  // TODO
    uint8_t enable_lcd_statistics;
    uint8_t enable_lcd_probabilty;
    uint8_t enable_ble_send_statistics;      // TODO
    uint8_t enable_ble_send_classification;  // TODO
    uint8_t enable_inactivity;
    lcd_rotation_e lcd_rotation;
} device_settings_t;

typedef struct {
    device_serial_num_t device_serial_num;
    device_version_t device_version;
    volatile uint8_t ble_mac[6];  // written by core1
    char max32666_demo_name[DEMO_STRING_SIZE];
    char max78000_video_demo_name[DEMO_STRING_SIZE];
    char max78000_audio_demo_name[DEMO_STRING_SIZE];
} device_info_t;

typedef struct {
    device_statistics_t statistics;
    classification_result_t classification_video;
    classification_result_t classification_audio;
    uint8_t faceid_embed_update_status;
    char faceid_embed_subject_names[FACEID_MAX_SUBJECT * (FACEID_MAX_SUBJECT_NAME_SIZE + 1)];
    uint16_t faceid_embed_subject_names_size;

    uint8_t fuel_gauge_working;
    float vcell;
    uint8_t usb_chgin;

    inactivity_state_e inactivity_state;

    volatile uint8_t ble_running_status_changed;  // written by both core0 and core1
    volatile uint8_t ble_connected_status_changed;  // written by both core0 and core1
    volatile uint8_t ble_connected;  // written by core1
    volatile uint8_t ble_connected_peer_mac[6];  // written by core1
    volatile uint16_t ble_max_packet_size; // written by core1
    volatile uint8_t ble_expected_rx_seq;  // written by core1
    volatile uint8_t ble_next_tx_seq;  // written by core1
} device_status_t;

typedef struct {
    uint8_t buffer[LCD_DATA_SIZE];
    uint8_t ml_data8[200];
    char notification[LCD_NOTIFICATION_MAX_SIZE];
    uint16_t notification_color;
    volatile uint8_t refresh_screen;
} lcd_data_t;

typedef struct {
    uint32_t audio_result_received;
    uint32_t video_data_received;
    uint32_t notification_received;
    uint32_t faceid_subject_names_received;
    uint32_t screen_drew;
    uint32_t statistics_sent;
    uint32_t pmic_check;
    uint32_t led;
    uint32_t powmon;
    uint32_t activity_detected;
} timestamps_t;


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
extern lcd_data_t lcd_data;
extern device_status_t device_status;
extern volatile device_settings_t device_settings;
extern device_info_t device_info;
extern timestamps_t timestamps;


//-----------------------------------------------------------------------------
// Function declarations
//-----------------------------------------------------------------------------



#endif /* _MAX32666_DATA_H_ */
