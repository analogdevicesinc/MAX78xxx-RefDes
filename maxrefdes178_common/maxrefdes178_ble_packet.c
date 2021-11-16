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


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
int ble_packet_get_version(char *version)
{
    return snprintf(version, sizeof(version) - 1, "v%d.%d.%d",
            S_VERSION_MAJOR, S_VERSION_MINOR, S_VERSION_BUILD);
}

uint16_t ble_packet_get_max_mtu_size(void)
{
    return BLE_MAX_MTU_REQUEST_SIZE;
}

uint16_t ble_packet_get_max_packet_size(void)
{
    return BLE_MAX_PACKET_SIZE;
}

uint16_t ble_packet_form_command_packet(const uint8_t ble_command, const uint8_t seq,
        const uint32_t total_payload_size, const uint16_t payload_size, const uint8_t *payload, uint8_t *packet)
{
    if (payload_size > BLE_MAX_PACKET_SIZE) {
        return 0;
    }

    ble_command_packet_t *ble_command_packet = (ble_command_packet_t *) packet;

    ble_command_packet->header.packet_info.type = BLE_PACKET_TYPE_COMMAND;
    ble_command_packet->header.packet_info.seq = seq & BLE_PACKET_SEQ_MASK;
    ble_command_packet->header.command = ble_command;
    ble_command_packet->header.total_payload_size = total_payload_size;
    memcpy(ble_command_packet->payload, payload, payload_size);

    return payload_size + sizeof(ble_command_packet_header_t);
}

uint16_t ble_packet_form_payload_packet(const uint8_t seq, const uint16_t payload_size,
        const uint8_t *payload, uint8_t *packet)
{
    if (payload_size > BLE_MAX_PACKET_SIZE) {
        return 0;
    }

    ble_payload_packet_t *ble_payload_packet = (ble_payload_packet_t *) packet;

    ble_payload_packet->header.packet_info.type = BLE_PACKET_TYPE_PAYLOAD;
    ble_payload_packet->header.packet_info.seq = seq & BLE_PACKET_SEQ_MASK;
    memcpy(ble_payload_packet->payload, payload, payload_size);

    return payload_size + sizeof(ble_payload_packet_header_t);
}

uint8_t ble_packet_parse_packet_type(const uint8_t *packet)
{
    ble_packet_info_t *ble_packet_info = (ble_packet_info_t *) packet;

    return ble_packet_info->type;
}

uint8_t ble_packet_parse_packet_seq(const uint8_t *packet)
{
    ble_packet_info_t *ble_packet_info = (ble_packet_info_t *) packet;

    return ble_packet_info->seq;
}

uint8_t ble_packet_parse_command_packet_command(const uint8_t *packet)
{
    ble_command_packet_t *ble_command_packet = (ble_command_packet_t *) packet;

    return ble_command_packet->header.command;
}

uint32_t ble_packet_parse_command_packet_total_payload_size(const uint8_t *packet)
{
    ble_command_packet_t *ble_command_packet = (ble_command_packet_t *) packet;

    return ble_command_packet->header.total_payload_size;
}

uint8_t * ble_packet_parse_command_packet_payload(const uint8_t *packet)
{
    ble_command_packet_t *ble_command_packet = (ble_command_packet_t *) packet;

    return ble_command_packet->payload;
}

uint8_t * ble_packet_parse_payload_packet_payload(const uint8_t *packet)
{
    ble_payload_packet_t *ble_payload_packet = (ble_payload_packet_t *) packet;

    return ble_payload_packet->payload;
}
