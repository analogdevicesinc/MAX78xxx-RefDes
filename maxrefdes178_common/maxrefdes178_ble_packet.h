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

#ifndef _MAXREFDES178_BLE_PACKET_H_
#define _MAXREFDES178_BLE_PACKET_H_


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "maxrefdes178_definitions.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function declarations
//-----------------------------------------------------------------------------
int ble_packet_get_version(char *version);

uint16_t ble_packet_get_max_mtu_size(void);

uint16_t ble_packet_get_max_packet_size(void);

uint16_t ble_packet_form_command_packet(const uint8_t ble_command, const uint8_t seq,
        const uint32_t total_payload_size, const uint16_t payload_size, const uint8_t *payload, uint8_t *packet);

uint16_t ble_packet_form_payload_packet(const uint8_t seq, const uint16_t payload_size,
        const uint8_t *payload, uint8_t *packet);

uint8_t ble_packet_parse_packet_type(const uint8_t *packet);

uint8_t ble_packet_parse_packet_seq(const uint8_t *packet);

uint8_t ble_packet_parse_command_packet_command(const uint8_t *packet);

uint32_t ble_packet_parse_command_packet_total_payload_size(const uint8_t *packet);

uint8_t * ble_packet_parse_command_packet_payload(const uint8_t *packet);

uint8_t * ble_packet_parse_payload_packet_payload(const uint8_t *packet);


#endif /* _MAXREFDES178_BLE_PACKET_H_ */
