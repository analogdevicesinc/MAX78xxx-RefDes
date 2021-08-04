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

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>

#include "maxrefdes178_ble_packet.h"
#include "maxrefdes178_definitions.h"
#include "maxrefdes178_version.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define MIN(x, y) (((x) < (y)) ? (x) : (y))


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
static uint8_t my_next_seq = 0;
static uint8_t peer_expected_seq = 0;


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
int ble_packet_helper_send_embeddings(const uint32_t embeddings_size, const uint8_t *embeddings);
int get_current_max_packet_size(void);
int send_packet(const uint16_t packet_size, const uint8_t *packet);
int ble_receive(const uint16_t packet_size, const uint8_t *packet);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
// gcc maxrefdes178_ble_packet_helper.c maxrefdes178_ble_packet.c
int main(void)
{
    uint8_t packet[BLE_MAX_PACKET_SIZE] = {0};
    uint16_t packet_size = 0;

    // These should be called initially to get version and usn
    packet_size = ble_packet_form_command_packet(BLE_COMMAND_GET_VERSION_CMD, my_next_seq++, 0, 0, NULL, packet);
    send_packet(packet_size , packet);

    packet_size = ble_packet_form_command_packet(BLE_COMMAND_GET_SERIAL_NUM_CMD, my_next_seq++, 0, 0, NULL, packet);
    send_packet(packet_size , packet);


    // These will be button in app
    packet_size = ble_packet_form_command_packet(BLE_COMMAND_ENABLE_LCD_CMD, my_next_seq++, 0, 0, NULL, packet);
    send_packet(packet_size , packet);

    packet_size = ble_packet_form_command_packet(BLE_COMMAND_DISABLE_LCD_CMD, my_next_seq++, 0, 0, NULL, packet);
    send_packet(packet_size , packet);

    packet_size = ble_packet_form_command_packet(BLE_COMMAND_ENABLE_LCD_STATISCTICS_CMD, my_next_seq++, 0, 0, NULL, packet);
    send_packet(packet_size , packet);

    packet_size = ble_packet_form_command_packet(BLE_COMMAND_DISABLE_LCD_STATISCTICS_CMD, my_next_seq++, 0, 0, NULL, packet);
    send_packet(packet_size , packet);

    packet_size = ble_packet_form_command_packet(BLE_COMMAND_ENABLE_LCD_PROBABILITY_CMD, my_next_seq++, 0, 0, NULL, packet);
    send_packet(packet_size , packet);

    packet_size = ble_packet_form_command_packet(BLE_COMMAND_DISABLE_LCD_PROBABILITY_CMD, my_next_seq++, 0, 0, NULL, packet);
    send_packet(packet_size , packet);

    packet_size = ble_packet_form_command_packet(BLE_COMMAND_ENABLE_SEND_STATISTICS_CMD, my_next_seq++, 0, 0, NULL, packet);
    send_packet(packet_size , packet);

    packet_size = ble_packet_form_command_packet(BLE_COMMAND_DISABLE_SEND_STATISTICS_CMD, my_next_seq++, 0, 0, NULL, packet);
    send_packet(packet_size , packet);

    packet_size = ble_packet_form_command_packet(BLE_COMMAND_ENABLE_SEND_CLASSIFICATION_CMD, my_next_seq++, 0, 0, NULL, packet);
    send_packet(packet_size , packet);

    packet_size = ble_packet_form_command_packet(BLE_COMMAND_DISABLE_SEND_CLASSIFICATION_CMD, my_next_seq++, 0, 0, NULL, packet);
    send_packet(packet_size , packet);

    packet_size = ble_packet_form_command_packet(BLE_COMMAND_ENABLE_MAX78000_VIDEO_CNN_CMD, my_next_seq++, 0, 0, NULL, packet);
    send_packet(packet_size , packet);

    packet_size = ble_packet_form_command_packet(BLE_COMMAND_DISABLE_MAX78000_VIDEO_CNN_CMD, my_next_seq++, 0, 0, NULL, packet);
    send_packet(packet_size , packet);

    packet_size = ble_packet_form_command_packet(BLE_COMMAND_ENABLE_MAX78000_VIDEO_FLASH_LED_CMD, my_next_seq++, 0, 0, NULL, packet);
    send_packet(packet_size , packet);

    packet_size = ble_packet_form_command_packet(BLE_COMMAND_DISABLE_MAX78000_VIDEO_FLASH_LED_CMD, my_next_seq++, 0, 0, NULL, packet);
    send_packet(packet_size , packet);


    // Generate and send FaceID embeddings
    uint8_t embeddings[500];
    for (int i = 0; i < sizeof(embeddings); i++) {
        embeddings[i] = i & 0xff;
    }
    ble_packet_helper_send_embeddings(sizeof(embeddings), embeddings);


    // Receive
    uint8_t update_embeddings_response[] = { 0x00, BLE_COMMAND_FACEID_EMBED_UPDATE_RES, 0x01, 0x00, 0x00, 0x00, 0x00 };
    ble_receive(sizeof(update_embeddings_response), update_embeddings_response);

    uint8_t version_response[] = { 0x00, BLE_COMMAND_GET_VERSION_RES, 0x12, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
                                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                   0x00, 0x00 };
    ble_receive(sizeof(version_response), version_response);

    uint8_t statistics_response[] = { 0x1E, BLE_COMMAND_GET_STATISTICS_RES, 0x2D, 0x00, 0x00, 0x00, 0xB8, 0x82, 0x01, 0x00, 0x28,
                                      0xA0, 0x00, 0x00, 0x08, 0x52, 0x00, 0x00, 0xE8, 0x74, 0x02, 0x00,
                                      0x0C, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                      0x00, 0x00, 0x00, 0x00, 0x00, 0xFD, 0xC1, 0xC6, 0x40, 0x00, 0x00,
                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    ble_receive(sizeof(statistics_response), statistics_response);

    return 0;
}

int ble_packet_helper_send_embeddings(const uint32_t embeddings_size, const uint8_t *embeddings)
{
    uint8_t packet[BLE_MAX_PACKET_SIZE] = {0};
    uint16_t packet_size = 0;
    int command_packet_payload_size = get_current_max_packet_size() - sizeof(ble_command_packet_header_t);
    int payload_packet_payload_size = get_current_max_packet_size() - sizeof(ble_payload_packet_header_t);
    int remaining_size = (int) embeddings_size;
    const uint8_t *embeddings_ptr = embeddings;

    packet_size = ble_packet_form_command_packet(BLE_COMMAND_FACEID_EMBED_UPDATE_CMD, my_next_seq++, embeddings_size,
            MIN(embeddings_size, command_packet_payload_size) , embeddings_ptr, packet);
    send_packet(packet_size, packet);

    embeddings_ptr += command_packet_payload_size;
    remaining_size -= command_packet_payload_size;

    while (remaining_size > 0) {
        packet_size = ble_packet_form_payload_packet(my_next_seq++, MIN(payload_packet_payload_size, remaining_size),
                embeddings_ptr, packet);
        send_packet(packet_size, packet);

        embeddings_ptr += payload_packet_payload_size;
        remaining_size -= payload_packet_payload_size;
    }

    return 0;
}

int get_current_max_packet_size(void)
{
    return 150;
}

int send_packet(const uint16_t packet_size, const uint8_t *packet)
{
    printf("Send packet size: %d\n", packet_size);
    for (int i = 0; i < packet_size; i++) {
        printf("%02hhX ", packet[i]);
    }
    printf("\n");

    return 0;
}

int ble_receive(const uint16_t packet_size, const uint8_t *packet)
{
    uint8_t *packet_payload = NULL;
    uint16_t packet_payload_size = 0;
    uint32_t total_payload_size = 0;

    device_version_t *device_version = NULL;
    device_serial_num_t *device_serial_num = NULL;
    device_statistics_t *device_statistics = NULL;

    if (packet_size < sizeof(ble_packet_info_t)) {
        printf("illegal packet size %d\n", packet_size);
        return -1;
    }

    if (peer_expected_seq != ble_packet_parse_packet_seq(packet)) {
        // Error incorrect sequence number
//        return;
    }
    peer_expected_seq++;

    if (ble_packet_parse_packet_type(packet) == BLE_PACKET_TYPE_COMMAND) {
        if (packet_size < sizeof(ble_command_packet_header_t)) {
            printf("illegal command packet size %d\n", packet_size);
            return -1;
        }
        packet_payload_size = packet_size - sizeof(ble_command_packet_header_t);
        total_payload_size = ble_packet_parse_command_packet_total_payload_size(packet);
        packet_payload = ble_packet_parse_command_packet_payload(packet);

        switch(ble_packet_parse_command_packet_command(packet)) {
        case BLE_COMMAND_GET_VERSION_RES:
            device_version = (device_version_t *) packet_payload;

            printf("MAX32666 version %d.%d.%d\n",
                    device_version->max32666.major, device_version->max32666.minor, device_version->max32666.build);
            printf("MAX7800 Video version %d.%d.%d\n",
                    device_version->max78000_video.major, device_version->max78000_video.minor, device_version->max78000_video.build);
            printf("MAX7800 Audio version %d.%d.%d\n",
                    device_version->max78000_audio.major, device_version->max78000_audio.minor, device_version->max78000_audio.build);
            break;
        case BLE_COMMAND_GET_SERIAL_NUM_RES:
            device_serial_num = (device_serial_num_t *) packet_payload;

            printf("MAX32666 USN\n");
            for (int i = 0; i < sizeof(device_serial_num->max32666); i++) {
                printf("%02X", device_serial_num->max32666[i]);
            }
            printf("\n");
            break;
        case BLE_COMMAND_FACEID_EMBED_UPDATE_RES:
            if (packet_payload[0] == FACEID_EMBED_UPDATE_STATUS_SUCCESS) {
                printf("FaceID embeddings update is successful\n");
            } else {
                printf("FaceID embeddings update is failed\n");
            }
            break;
        case BLE_COMMAND_GET_STATISTICS_RES:
            device_statistics = (device_statistics_t *) packet_payload;

            printf("Battery %d\n", device_statistics->battery_level);
            printf("LCD FPS %f\n", device_statistics->lcd_fps);

            printf("Video power %d\n", device_statistics->max78000_video_power_uw);
            printf("Video CNN time %d\n", device_statistics->max78000_video.cnn_duration_us);
            printf("Video capture time %d\n", device_statistics->max78000_video.capture_duration_us);
            printf("Video communication time %d\n", device_statistics->max78000_video.communication_duration_us);

            printf("Audio power %d\n", device_statistics->max78000_audio_power_uw);
            printf("Audio CNN time %d\n", device_statistics->max78000_audio.cnn_duration_us);

            break;
        case BLE_COMMAND_GET_MAX78000_VIDEO_CLASSIFICATION_RES:
            break;
        case BLE_COMMAND_GET_MAX78000_AUDIO_CLASSIFICATION_RES:
            break;
        default:
            break;
        }

    } else { // BLE_PACKET_TYPE_PAYLOAD
        if (packet_size < sizeof(ble_payload_packet_header_t)) {
            printf("illegal payload packet size %d\n", packet_size);
            return -1;
        }
        packet_payload_size = packet_size - sizeof(ble_payload_packet_header_t);
        packet_payload = ble_packet_parse_payload_packet_payload(packet);

        // TODO
    }

    return 0;
}
