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
#include <string.h>

#include "max32666_ble_command.h"
#include "max32666_ble_queue.h"
#include "max32666_data.h"
#include "max32666_debug.h"
#include "maxrefdes178_definitions.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "comm"


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------
typedef enum {
    BLE_COMMAND_STATE_IDLE = 0,
    BLE_COMMAND_STATE_RX_STARTED,
    BLE_COMMAND_STATE_RX_PAYLOAD_RECEIVED,
    BLE_COMMAND_STATE_TX_STARTED,
    BLE_COMMAND_STATE_TX_PAYLOAD_SENT,

    BLE_COMMAND_STATE_LAST
} ble_command_state_e;


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
static ble_packet_container_t tmp_ble_packet_container;

struct {
    uint8_t command;
    ble_command_state_e command_state;
    uint32_t received_payload_size;
    uint32_t total_payload_size;
    uint8_t total_payload_buffer[MAX32666_BLE_COMMAND_BUFFER_SIZE];
} ble_command_buffer;


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static int ble_command_form_single_packet(ble_command_e ble_command, uint32_t payload_size, uint8_t *payload);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
// TODO: handle small MTU, multiple packet cases
static int ble_command_form_single_packet(ble_command_e ble_command, uint32_t payload_size, uint8_t *payload)
{
    if (payload_size > sizeof(tmp_ble_packet_container.packet.command_packet.payload)) {
        PR_ERROR("invalid command %d payload size %d", ble_command, payload_size);
        return E_BAD_PARAM;
    }
    tmp_ble_packet_container.packet.command_packet.header.packet_info.type = BLE_PACKET_TYPE_COMMAND;
    tmp_ble_packet_container.packet.command_packet.header.packet_info.seq = device_status.ble_next_tx_seq;
    tmp_ble_packet_container.packet.command_packet.header.command = ble_command;
    tmp_ble_packet_container.packet.command_packet.header.total_payload_size = payload_size;
    memcpy(&tmp_ble_packet_container.packet.command_packet.payload, payload, payload_size);
    tmp_ble_packet_container.size = payload_size + sizeof(ble_command_packet_header_t);

    return E_SUCCESS;
}

int ble_command_send_audio_classification(void)
{
    int ret;

    if (ble_command_buffer.command_state != BLE_COMMAND_STATE_IDLE) {
        PR_ERROR("command state is not idle");
        return E_BAD_STATE;
    }

    ret = ble_command_form_single_packet(BLE_COMMAND_GET_MAX78000_AUDIO_CLASSIFICATION_RES,
            sizeof(device_status.classification_audio), (uint8_t *) &device_status.classification_audio);
    if (ret != E_SUCCESS) {
        PR_ERROR("form_single_packet_command failed %d", ret);
        return ret;
    }

    ret = ble_queue_enq_tx(&tmp_ble_packet_container);
    if (ret != E_SUCCESS) {
        PR_ERROR("ble_queue_enq_tx failed %d", ret);
        return ret;
    }

    return ret;
}

int ble_command_send_video_classification(void)
{
    int ret;

    if (ble_command_buffer.command_state != BLE_COMMAND_STATE_IDLE) {
        PR_ERROR("command state is not idle");
        return E_BAD_STATE;
    }

    ret = ble_command_form_single_packet(BLE_COMMAND_GET_MAX78000_VIDEO_CLASSIFICATION_RES,
            sizeof(device_status.classification_video), (uint8_t *) &device_status.classification_video);
    if (ret != E_SUCCESS) {
        PR_ERROR("form_single_packet_command failed %d", ret);
        return ret;
    }

    ret = ble_queue_enq_tx(&tmp_ble_packet_container);
    if (ret != E_SUCCESS) {
        PR_ERROR("ble_queue_enq_tx failed %d", ret);
        return ret;
    }

    return E_SUCCESS;
}

int ble_command_send_statistics(void)
{
    int ret;

    if (ble_command_buffer.command_state != BLE_COMMAND_STATE_IDLE) {
        PR_ERROR("command state is not idle");
        return E_BAD_STATE;
    }

    ret = ble_command_form_single_packet(BLE_COMMAND_GET_STATISTICS_RES,
            sizeof(device_status.statistics), (uint8_t *) &device_status.statistics);
    if (ret != E_SUCCESS) {
        PR_ERROR("form_single_packet_command failed %d", ret);
        return ret;
    }

    ret = ble_queue_enq_tx(&tmp_ble_packet_container);
    if (ret != E_SUCCESS) {
        PR_ERROR("ble_queue_enq_tx failed ret", ret);
        return ret;
    }

    return E_SUCCESS;
}

int ble_command_worker(void)
{
    int ret;

    if (ble_queue_deq_rx(&tmp_ble_packet_container) != E_SUCCESS) {
        // No new packet on ble rx queue
        return E_NO_ERROR;
    }

    // Check sequence number of packet
    if (device_status.ble_expected_rx_seq != tmp_ble_packet_container.packet.packet_info.seq) {
        PR_ERROR("Incorrect seq expected %d received %d", device_status.ble_expected_rx_seq,
                tmp_ble_packet_container.packet.packet_info.seq);
//        return E_BAD_STATE;
    }

    PR_INFO("BLE RX packet size %d", tmp_ble_packet_container.size);
    if (tmp_ble_packet_container.packet.packet_info.type == BLE_PACKET_TYPE_COMMAND) {
        PR_INFO("Command %d", tmp_ble_packet_container.packet.command_packet.header.command);
        PR_INFO("Total payload size %d", tmp_ble_packet_container.packet.command_packet.header.total_payload_size);
        PR_INFO("Payload: ");
        for (int i = 0; i < tmp_ble_packet_container.size - sizeof(ble_command_packet_header_t); i++) {
            PR("%02hhX ", tmp_ble_packet_container.packet.command_packet.payload[i]);
        }
        PR("\n");

        // Check abort command
        if (tmp_ble_packet_container.packet.command_packet.header.command == BLE_COMMAND_ABORT_CMD) {
            PR_INFO("reset command state");
            ble_command_buffer.command_state = BLE_COMMAND_STATE_IDLE;
            return E_SUCCESS;
        }

        // Check state
        if (ble_command_buffer.command_state != BLE_COMMAND_STATE_IDLE) {
            PR_ERROR("command packet is not expected");

            // Send invalid command response
            ret = ble_command_form_single_packet(BLE_COMMAND_INVALID_RES, 0, NULL);
            if (ret != E_SUCCESS) {
                PR_ERROR("form_single_packet_command failed %d", ret);
                return ret;
            }
            ret = ble_queue_enq_tx(&tmp_ble_packet_container);
            if (ret != E_SUCCESS) {
                PR_ERROR("commbuf_push_tx_ble failed %d", ret);
                return ret;
            }

            return E_BAD_STATE;
        }

        // Start command state
        ble_command_buffer.command_state = BLE_COMMAND_STATE_RX_STARTED;
        ble_command_buffer.command = tmp_ble_packet_container.packet.command_packet.header.command;


        return E_SUCCESS;
    } else if (tmp_ble_packet_container.packet.packet_info.type == BLE_PACKET_TYPE_PAYLOAD) {
        PR_INFO("Payload: ");
        for (int i = 0; i < tmp_ble_packet_container.size - sizeof(ble_payload_packet_header_t); i++) {
            PR("%02hhX ", tmp_ble_packet_container.packet.payload_packet.payload[i]);
        }
        PR("\n");
        // TODO: handle multiple packet commands
        return E_SUCCESS;
    }

    return E_SUCCESS;
}



int ble_command_init(void)
{
    ble_command_buffer.command_state = BLE_COMMAND_STATE_IDLE;

    return E_NO_ERROR;
}
