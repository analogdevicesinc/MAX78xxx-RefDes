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

package com.maximintegrated.communication

import android.app.Application
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothProfile
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import timber.log.Timber

class MaxCamViewModel(application: Application) : AndroidViewModel(application),
    DeviceManagerCallbacks {

    var bluetoothDevice: BluetoothDevice? = null
        private set

    private val _connectionState = MutableLiveData<Int>()
    val connectionState: LiveData<Int>
        get() = _connectionState

    var previousConnectionState = -1

    private fun setConnectionState(state: Int) {
        previousConnectionState = connectionState.value ?: -1
        _connectionState.value = state
    }

    private val _receivedData = MutableLiveData<ByteArray>()
    val receivedData: LiveData<ByteArray>
        get() = _receivedData

    private val _batteryLevel = MutableLiveData<Int>()
    val batteryLevel: LiveData<Int>
        get() = _batteryLevel

    private val _isDeviceSupported = MutableLiveData<Boolean>()
    val isDeviceSupported: LiveData<Boolean>
        get() = _isDeviceSupported

    private val deviceManager = DeviceManager(application)

    val isConnected: Boolean
        get() = connectionState.value == BluetoothAdapter.STATE_CONNECTED

    private val _mtuSize = MutableLiveData<Int>()
    val mtuSize: LiveData<Int>
        get() = _mtuSize

    val packetSize: Int get() = _mtuSize.value!! - 3
    private val _writeStatus = MutableLiveData<Pair<Int, Int>>(Pair(0, 0))
    val writeStatus: LiveData<Pair<Int, Int>>
        get() = _writeStatus

    init {
        deviceManager.setGattCallbacks(this)
    }

    fun setMtu(mtu: Int) {
        _mtuSize.value = mtu
    }

    override fun onCleared() {
        super.onCleared()
        if (deviceManager.isConnected) {
            disconnect()
        }
    }

    fun connect(device: BluetoothDevice, requestedMtu: Int) {
        if (bluetoothDevice == null) {
            bluetoothDevice = device
            deviceManager.connect(device)
                .fail { device, status ->
                    Timber.e("BLE Connection Failed!! $device - $status")
                    reconnect()
                }
                .done {
                    deviceManager.requestMtu(requestedMtu)
                        .fail { device, status ->
                            Timber.e("BLE requestMtu Failed!! $device - $status")
                        }
                        .with { _, mtu ->
                            deviceManager.maxCamBleMtu = mtu
                        }.done {
                            _mtuSize.postValue(deviceManager.maxCamBleMtu)
                        }
                        .enqueue()
                }.enqueue()

        }
    }

    fun disconnect() {
        bluetoothDevice = null
        deviceManager.disconnect().enqueue()
    }

    fun reconnect() {
        deviceManager.disconnect().enqueue()

        bluetoothDevice?.let {
            deviceManager
                .connect(it)
                .retry(3, 100)
                .useAutoConnect(false)
                .enqueue()
        }
    }

    fun sendData(data: ByteArray) {
        deviceManager.sendData(data)
    }

    override fun onDeviceDisconnecting(device: BluetoothDevice) {
        setConnectionState(BluetoothProfile.STATE_DISCONNECTING)
        Timber.i("Disconnecting from device $device")
    }

    override fun onDeviceDisconnected(device: BluetoothDevice) {
        setConnectionState(BluetoothProfile.STATE_DISCONNECTED)
        Timber.i("Disconnected from device $device")
    }

    override fun onDeviceConnecting(device: BluetoothDevice) {
        setConnectionState(BluetoothProfile.STATE_CONNECTING)
        Timber.i("Connecting to device $device")
    }

    override fun onDeviceConnected(device: BluetoothDevice) {
        setConnectionState(BluetoothProfile.STATE_CONNECTED)
        Timber.i("Connected to device $device")
    }

    override fun onDeviceNotSupported(device: BluetoothDevice) {
        _isDeviceSupported.value = false
        Timber.e("Unsupported device $device")
    }

    override fun onBondingFailed(device: BluetoothDevice) {
        Timber.e("Bonding to device $device failed")
    }

    override fun onServicesDiscovered(device: BluetoothDevice, optionalServicesFound: Boolean) {
        Timber.i("Services of device $device discovered")
    }

    override fun onBondingRequired(device: BluetoothDevice) {
        Timber.d("Bonding to device $device is required")
    }

    override fun onLinkLossOccurred(device: BluetoothDevice) {
        Timber.e("Link to device $device is lost")
    }

    override fun onBonded(device: BluetoothDevice) {
        Timber.d("Bonded to device $device")
    }

    override fun onDeviceReady(device: BluetoothDevice) {
        _isDeviceSupported.value = true
        Timber.i("Device $device is ready")
    }

    override fun onError(device: BluetoothDevice, message: String, errorCode: Int) {
        Timber.e("An error occurred (Device: $device, ErrorCode: $errorCode, Message: $message)")
    }

    override fun onBatteryLevelChanged(device: BluetoothDevice, batteryLevel: Int) {
        _batteryLevel.value = batteryLevel
        Timber.i("Battery level changed ($device, $batteryLevel)")
    }

    override fun onDataReceived(device: BluetoothDevice, packet: ByteArray) {
        _receivedData.value = packet
        Timber.i("Data Received: ViewModel")
    }

    override fun onWriteCompleted(byteSent: Int, packetSize: Int) {
        _writeStatus.value = Pair(byteSent, packetSize)
    }
}