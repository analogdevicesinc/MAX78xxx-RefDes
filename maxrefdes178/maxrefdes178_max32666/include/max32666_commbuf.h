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

#ifndef _MAX32666_COMMBUF_H_
#define _MAX32666_COMMBUF_H_


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
typedef enum {
    COMMBUF_STATUS_OK = 0,
    COMMBUF_STATUS_EMPTY,
    COMMBUF_STATUS_FULL
} commbuf_status_e;


//-----------------------------------------------------------------------------
// Function declarations
//-----------------------------------------------------------------------------
int commbuf_init(void);
int commbuf_trigger_int(void);
commbuf_status_e commbuf_pop_rx_ble(packet_container_t *ble_packet_container);
commbuf_status_e commbuf_push_rx_ble(packet_container_t *ble_packet_container);
commbuf_status_e commbuf_pop_tx_ble(packet_container_t *ble_packet_container);
commbuf_status_e commbuf_push_tx_ble(packet_container_t *ble_packet_container);
//commbuf_status_e commbuf_pop_rx_usb(packet_container_t *usb_packet_container);
//commbuf_status_e commbuf_push_rx_usb(packet_container_t *usb_packet_container);
//commbuf_status_e commbuf_pop_tx_usb(packet_container_t *usb_packet_container);
//commbuf_status_e commbuf_push_tx_usb(packet_container_t *usb_packet_container);

#endif /* _MAX32666_COMMBUF_H_ */
