/*
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
 */

package com.maximintegrated.bluetooth.livedata

import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import com.maximintegrated.bluetooth.common.ExtendedBluetoothDevice
import com.maximintegrated.bluetooth.util.cancelBluetoothDeviceDiscovery
import com.maximintegrated.bluetooth.util.startBluetoothDeviceDiscovery

class BtAvailableDevicesLiveData(context: Context) : BroadcastLiveData<List<ExtendedBluetoothDevice>>(context) {

    private val availableDevices = mutableListOf<ExtendedBluetoothDevice>()

    override val intentFilter = IntentFilter().apply {
        addAction(BluetoothAdapter.ACTION_STATE_CHANGED)
        addAction(BluetoothAdapter.ACTION_DISCOVERY_STARTED)
        addAction(BluetoothDevice.ACTION_FOUND)
    }

    override val broadcastReceiver = object : BroadcastReceiver() {
        override fun onReceive(ctx: Context, intent: Intent) {
            when (intent.action) {
                BluetoothAdapter.ACTION_STATE_CHANGED -> {
                    val bluetoothState = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE,
                            BluetoothAdapter.ERROR)
                    when (bluetoothState) {
                        BluetoothAdapter.STATE_OFF -> clearDevices()
                        BluetoothAdapter.STATE_ON -> startBluetoothDeviceDiscovery()
                    }
                }
                BluetoothAdapter.ACTION_DISCOVERY_STARTED -> {
                    clearDevices()
                }
                BluetoothDevice.ACTION_FOUND -> {
                    val bluetoothDevice = intent.getParcelableExtra<BluetoothDevice>(BluetoothDevice.EXTRA_DEVICE)
                    val rssi = if (intent.hasExtra(BluetoothDevice.EXTRA_RSSI)) {
                        intent.getShortExtra(BluetoothDevice.EXTRA_RSSI, 0).toInt()
                    } else {
                        null
                    }
                    addDevice(bluetoothDevice, rssi)
                }
            }
        }
    }

    override fun onActive() {
        super.onActive()
        startBluetoothDeviceDiscovery()
    }

    override fun onInactive() {
        super.onInactive()
        cancelBluetoothDeviceDiscovery()
    }

    private fun clearDevices() {
        availableDevices.clear()
        value = emptyList()
    }

    private fun addDevice(device: BluetoothDevice, rssi: Int?) {
        if (availableDevices.none { device == it.device }) {
            availableDevices.add(
                ExtendedBluetoothDevice(device, rssi)
            )
            value = availableDevices
        } else {
            availableDevices.firstOrNull { it.address == device.address }
                    ?.let { foundDevice ->
                        availableDevices[availableDevices.indexOf(foundDevice)] =
                                ExtendedBluetoothDevice(device, rssi)
                        value = availableDevices
                    }
        }
    }
}