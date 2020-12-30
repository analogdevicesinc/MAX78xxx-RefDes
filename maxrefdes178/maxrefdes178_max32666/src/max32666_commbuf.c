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
#include "max32666_debug.h"
#include "maxrefdes178_definitions.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "commbuf"


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------
typedef struct {
    packet_container_t container[MAX32666_COMMBUF_ARRAY_SIZE];;
    int head;
    int tail;
} commbuf_buffer_t;


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
//static const mxc_gpio_cfg_t core0_int_pin = MAX32666_CORE0_INT_PIN;
//static const mxc_gpio_cfg_t core1_int_pin = MAX32666_CORE1_INT_PIN;

static commbuf_buffer_t ble_rx_container;
static commbuf_buffer_t ble_tx_container;
//static commbuf_buffer_t usb_rx_container;
//static commbuf_buffer_t usb_tx_container;


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static commbuf_status_e push_buffer(commbuf_buffer_t *buffer, packet_container_t *packet_container);
static commbuf_status_e pop_buffer(commbuf_buffer_t *buffer, packet_container_t *packet_container);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
//void core1_to_core0_int(void *cbdata)
//{
//
//}

static commbuf_status_e push_buffer(commbuf_buffer_t *buffer, packet_container_t *packet_container)
{
    int next;

    while(MXC_SEMA_GetSema(MAX32666_COMMBUF_BUFFER) == E_BUSY) {};

    next = buffer->head + 1;
    if (next >= MAX32666_COMMBUF_ARRAY_SIZE) {
        next = 0;
    }

    if (next == buffer->tail) {
        MXC_SEMA_FreeSema(MAX32666_COMMBUF_BUFFER);
        return COMMBUF_STATUS_FULL;
    }

    memcpy(&(buffer->container[buffer->head]), packet_container, sizeof(packet_container_t));
    buffer->head = next;

    MXC_SEMA_FreeSema(MAX32666_COMMBUF_BUFFER);

    return COMMBUF_STATUS_OK;
}

static commbuf_status_e pop_buffer(commbuf_buffer_t *buffer, packet_container_t *packet_container)
{
    int next;

    while(MXC_SEMA_GetSema(MAX32666_COMMBUF_BUFFER) == E_BUSY) {};

    if (buffer->head == buffer->tail) {
        MXC_SEMA_FreeSema(MAX32666_COMMBUF_BUFFER);
        return COMMBUF_STATUS_EMPTY;
    }

    next = buffer->tail + 1;
    if(next >= MAX32666_COMMBUF_ARRAY_SIZE) {
        next = 0;
    }

    memcpy(packet_container, &(buffer->container[buffer->tail]), sizeof(packet_container_t));
    buffer->tail = next;

    MXC_SEMA_FreeSema(MAX32666_COMMBUF_BUFFER);

    return COMMBUF_STATUS_OK;
}

commbuf_status_e commbuf_pop_rx_ble(packet_container_t *ble_packet_container)
{
    return pop_buffer(&ble_rx_container, ble_packet_container);
}

commbuf_status_e commbuf_push_rx_ble(packet_container_t *ble_packet_container)
{
    return push_buffer(&ble_rx_container, ble_packet_container);
}

commbuf_status_e commbuf_pop_tx_ble(packet_container_t *ble_packet_container)
{
    return pop_buffer(&ble_tx_container, ble_packet_container);
}

commbuf_status_e commbuf_push_tx_ble(packet_container_t *ble_packet_container)
{
    return push_buffer(&ble_tx_container, ble_packet_container);
}

//commbuf_status_e commbuf_pop_rx_usb(packet_container_t *usb_packet_container)
//{
//    return pop_buffer(&usb_rx_container, usb_packet_container);
//}
//
//commbuf_status_e commbuf_push_rx_usb(packet_container_t *usb_packet_container)
//{
//    return push_buffer(&usb_rx_container, usb_packet_container);
//}
//
//commbuf_status_e commbuf_pop_tx_usb(packet_container_t *usb_packet_container)
//{
//    return pop_buffer(&usb_tx_container, usb_packet_container);
//}
//
//commbuf_status_e commbuf_push_tx_usb(packet_container_t *usb_packet_container)
//{
//    return push_buffer(&usb_tx_container, usb_packet_container);
//}

int commbuf_trigger_int(void)
{
//    // Only Core1 can call this function
//    if (SCB->VTOR != (unsigned long)&__isr_vector_core1) {
//        return E_NOT_SUPPORTED;
//    }
//    GPIO_CLR(core1_int_pin);
//    GPIO_SET(core1_int_pin);

    return E_NO_ERROR;
}

int commbuf_init(void)
{
//    MXC_GPIO_Config(&core0_int_pin);
//    MXC_GPIO_RegisterCallback(&core0_int_pin, core1_to_core0_int, NULL);
//    MXC_GPIO_IntConfig(&core0_int_pin, MAX32666_CORE0_INT_MODE);
//    MXC_GPIO_EnableInt(core0_int_pin.port, core0_int_pin.mask);
//    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(MXC_GPIO_GET_IDX(core0_int_pin.port)));
//
//    GPIO_SET(core1_int_pin);
//    MXC_GPIO_Config(&core1_int_pin);

    ble_rx_container.head = 0;
    ble_rx_container.tail = 0;
    ble_tx_container.head = 0;
    ble_tx_container.tail = 0;

    return E_NO_ERROR;
}

