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
#include <string.h>

#include "max32666_ble_queue.h"
#include "max32666_debug.h"
#include "maxrefdes178_definitions.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "queue"


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------
// Circular buffer queue implementation, overwrite is not permitted
typedef struct {
    ble_packet_container_t container_array[MAX32666_BLE_QUEUE_SIZE];
    uint32_t head;
    uint32_t tail;
} ble_queue_t;


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
//static const mxc_gpio_cfg_t core0_int_pin = MAX32666_CORE0_INT_PIN;
//static const mxc_gpio_cfg_t core1_int_pin = MAX32666_CORE1_INT_PIN;

static volatile ble_queue_t ble_queue_rx;
static volatile ble_queue_t ble_queue_tx;


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static int ble_queue_enq(volatile ble_queue_t *ble_queue, ble_packet_container_t *ble_packet_container);
static int ble_queue_deq(volatile ble_queue_t *ble_queue, ble_packet_container_t *ble_packet_container);
//static int ble_queue_trigger_interrupt(void)


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
//void core1_to_core0_int_handler(void *cbdata)
//{
//
//}

static int ble_queue_enq(volatile ble_queue_t *ble_queue, ble_packet_container_t *ble_packet_container)
{
    uint32_t next;

    while(MXC_SEMA_GetSema(MAX32666_SEMAPHORE_BLE_QUEUE) == E_BUSY) {};

    next = (ble_queue->head + 1) % MAX32666_BLE_QUEUE_SIZE;

    if (next == ble_queue->tail) {
        MXC_SEMA_FreeSema(MAX32666_SEMAPHORE_BLE_QUEUE);
        return E_OVERFLOW;
    }

    memcpy((uint8_t *) &(ble_queue->container_array[ble_queue->head]), ble_packet_container, sizeof(ble_packet_container_t));
    ble_queue->head = next;

    MXC_SEMA_FreeSema(MAX32666_SEMAPHORE_BLE_QUEUE);

    return E_SUCCESS;
}

static int ble_queue_deq(volatile ble_queue_t *ble_queue, ble_packet_container_t *ble_packet_container)
{
    uint32_t next;

    while(MXC_SEMA_GetSema(MAX32666_SEMAPHORE_BLE_QUEUE) == E_BUSY) {};

    if (ble_queue->head == ble_queue->tail) {
        MXC_SEMA_FreeSema(MAX32666_SEMAPHORE_BLE_QUEUE);
        return E_UNDERFLOW;
    }

    next = (ble_queue->tail + 1) % MAX32666_BLE_QUEUE_SIZE;

    memcpy(ble_packet_container, (uint8_t *) &(ble_queue->container_array[ble_queue->tail]), sizeof(ble_packet_container_t));
    ble_queue->tail = next;

    MXC_SEMA_FreeSema(MAX32666_SEMAPHORE_BLE_QUEUE);

    return E_SUCCESS;
}

int ble_queue_deq_rx(ble_packet_container_t *ble_packet_container)
{
    return ble_queue_deq(&ble_queue_rx, ble_packet_container);
}

int ble_queue_enq_rx(ble_packet_container_t *ble_packet_container)
{
    return ble_queue_enq(&ble_queue_rx, ble_packet_container);
}

int ble_queue_deq_tx(ble_packet_container_t *ble_packet_container)
{
    return ble_queue_deq(&ble_queue_tx, ble_packet_container);
}

int ble_queue_enq_tx(ble_packet_container_t *ble_packet_container)
{
    return ble_queue_enq(&ble_queue_tx, ble_packet_container);
}

int ble_queue_flush(void)
{
    while(MXC_SEMA_GetSema(MAX32666_SEMAPHORE_BLE_QUEUE) == E_BUSY) {};

    ble_queue_rx.head = 0;
    ble_queue_rx.tail = 0;
    ble_queue_tx.head = 0;
    ble_queue_tx.tail = 0;

    MXC_SEMA_FreeSema(MAX32666_SEMAPHORE_BLE_QUEUE);

    return E_SUCCESS;
}

//static int ble_queue_trigger_interrupt(void)
//{
//    // Only Core1 can call this function
//    if (SCB->VTOR != (unsigned long)&__isr_vector_core1) {
//        return E_NOT_SUPPORTED;
//    }
//    GPIO_CLR(core1_int_pin);
//    GPIO_SET(core1_int_pin);
//
//    return E_SUCCESS;
//}

int ble_queue_init(void)
{
//    MXC_GPIO_Config(&core0_int_pin);
//    MXC_GPIO_RegisterCallback(&core0_int_pin, core1_to_core0_int_handler, NULL);
//    MXC_GPIO_IntConfig(&core0_int_pin, MAX32666_CORE0_INT_MODE);
//    MXC_GPIO_EnableInt(core0_int_pin.port, core0_int_pin.mask);
//    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(MXC_GPIO_GET_IDX(core0_int_pin.port)));
//
//    GPIO_SET(core1_int_pin);
//    MXC_GPIO_Config(&core1_int_pin);

    ble_queue_flush();

    return E_SUCCESS;
}
