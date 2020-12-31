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

#include "max32666_commbuf.h"
#include "max32666_commhandler.h"
#include "max32666_debug.h"
#include "maxrefdes178_definitions.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "commhand"


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
static packet_container_t ble_packet_container = {0};


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
int commhandler_send_audio_classification(void)
{

    return E_NO_ERROR;
}

int commhandler_send_video_classification(void)
{

    return E_NO_ERROR;
}

int commhandler_send_statistics(void)
{

    return E_NO_ERROR;
}

int commhandler_worker(void)
{
    if (commbuf_pop_rx_ble(&ble_packet_container) != COMMBUF_STATUS_OK) {
        return E_NO_ERROR;
    }

    PR_INFO("BLE RX packet size %d", ble_packet_container.size);
    if (ble_packet_container.packet.packet_info == PACKET_TYPE_COMMAND) {
        PR_INFO("Command %02X", ble_packet_container.packet.command_packet.header.command);
        PR_INFO("Command size %d", ble_packet_container.packet.command_packet.header.command_size);
        PR_INFO("Payload: ");
        for (int i = 0; i < ble_packet_container.size - sizeof(payload_packet_header_t); i++) {
            PR("0x%02hhX ", ble_packet_container.packet.command_packet.payload[i]);
        }
        PR("\n");
    } else if (ble_packet_container.packet.packet_info == PACKET_TYPE_PAYLOAD) {
        PR_INFO("Payload: ");
        for (int i = 0; i < ble_packet_container.size - sizeof(command_packet_header_t); i++) {
            PR("0x%02hhX ", ble_packet_container.packet.payload_packet.payload[i]);
        }
        PR("\n");
    }

    return E_NO_ERROR;
}

int commhandler_init(void)
{
    return E_NO_ERROR;
}

