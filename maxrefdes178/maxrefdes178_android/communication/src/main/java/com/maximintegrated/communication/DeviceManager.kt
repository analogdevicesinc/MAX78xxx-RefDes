package com.maximintegrated.communication

import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothGatt
import android.bluetooth.BluetoothGattCharacteristic
import android.content.Context
import no.nordicsemi.android.ble.BleManager
import no.nordicsemi.android.ble.ConnectionPriorityRequest
import no.nordicsemi.android.ble.MtuRequest
import no.nordicsemi.android.ble.callback.DataReceivedCallback
import no.nordicsemi.android.ble.common.callback.battery.BatteryLevelDataCallback
import no.nordicsemi.android.ble.data.Data
import timber.log.Timber
import java.util.*

class DeviceManager(context: Context) : BleManager<DeviceManagerCallbacks>(context) {

    companion object {
        // 180A Device Information
        private val UUID_DEVICE_INFORMATION_SERVICE = uuidFromShortCode16("180A")
        private val UUID_MANUFACTURER_NAME_CHARACTERISTIC = uuidFromShortCode16("2A29")
        private val UUID_MODEL_NUMBER_CHARACTERISTIC = uuidFromShortCode16("2A24")
        private val UUID_SERIAL_NUMBER_CHARACTERISTIC = uuidFromShortCode16("2A25")

        // 1802 Immediate Alert
        private val UUID_DEVICE_IMMEDIATE_ALERT_SERVICE = uuidFromShortCode16("1802")
        private val UUID_ALERT_LEVEL_CHARACTERISTIC = uuidFromShortCode16("2A06")

        private val UUID_BATTERY_SERVICE = uuidFromShortCode16("180F")
        private val UUID_BATTERY_LEVEL_CHARACTERISTIC = uuidFromShortCode16("2A19")

        private val UUID_MAX_CAM_SERVICE = UUID.fromString("395fdbca-be80-4541-a325-3ab02d871001")
        private val UUID_MAX_CAM_WRITE_CHARACTERISTIC =
            UUID.fromString("395fdbca-be80-4541-a325-3ab02d870001")
    }

    var maxCamBleMtu = 23

    private var writeCharacteristic: BluetoothGattCharacteristic? = null
    private var batteryLevelCharacteristic: BluetoothGattCharacteristic? = null

    private val batteryLevelDataCallback = object : BatteryLevelDataCallback() {
        override fun onBatteryLevelChanged(device: BluetoothDevice, batteryLevel: Int) {
            mCallbacks.onBatteryLevelChanged(device, batteryLevel)
        }

        override fun onInvalidDataReceived(device: BluetoothDevice, data: Data) {
            Timber.e("Invalid battery level data received: %s", data)
        }
    }

    private val dataReceivedCallback = DataReceivedCallback { device, data ->
        mCallbacks.onDataReceived(
            device,
            data.value ?: byteArrayOf()
        )
    }

    private val gattCallback = object : BleManagerGattCallback() {

        override fun onDeviceDisconnected() {
            writeCharacteristic = null
            batteryLevelCharacteristic = null
        }

        override fun isRequiredServiceSupported(gatt: BluetoothGatt): Boolean {
            val maxCamService = gatt.getService(UUID_MAX_CAM_SERVICE)
            writeCharacteristic =
                maxCamService?.getCharacteristic(UUID_MAX_CAM_WRITE_CHARACTERISTIC)

            val batteryService = gatt.getService(UUID_BATTERY_SERVICE)
            batteryLevelCharacteristic =
                batteryService?.getCharacteristic(UUID_BATTERY_LEVEL_CHARACTERISTIC)

            requestConnectionPriority(ConnectionPriorityRequest.CONNECTION_PRIORITY_HIGH).enqueue()

            return writeCharacteristic.hasWriteProperty
        }

        override fun initialize() {
            super.initialize()
            enableCharacteristicNotifications()
        }

    }

    override fun getGattCallback() = gattCallback

    fun readBatteryLevelCharacteristic() {
        if (isConnected) {
            readCharacteristic(batteryLevelCharacteristic)
                .with(batteryLevelDataCallback)
                .fail { device, status ->
                    Timber.e(
                        "Failed to read battery level characteristic (Device: %s, Status: %d)",
                        device,
                        status
                    )
                }
                .enqueue()
        }
    }

    fun enableCharacteristicNotifications() {
        if (isConnected) {
            setNotificationCallback(writeCharacteristic)
                .with(dataReceivedCallback)
            enableNotifications(writeCharacteristic)
                .done { device ->
                    Timber.i(
                        "Enabled notifications (Device: %s)",
                        device
                    )
                }
                .fail { device, status ->
                    Timber.e(
                        "Failed to enable notifications (Device: %s, Status: %d)",
                        device,
                        status
                    )
                }
                .enqueue()
        }
    }

    public override fun requestMtu(mtu: Int): MtuRequest {
        return super.requestMtu(mtu)
    }

    public fun sendData(data: ByteArray?) {
        if(!isConnected || data == null || data.isEmpty()) return
        var packetLength = maxCamBleMtu - 3
        var currentIndex = 0
        while(currentIndex < data.size){
            val messageLength = data.size - currentIndex
            packetLength = if(packetLength > messageLength) messageLength else packetLength
            val buffer = ByteArray(packetLength)
            for(i in 0 until packetLength){
                buffer[i] = data[currentIndex + i]
            }
            currentIndex += packetLength
            writeCharacteristic(writeCharacteristic, buffer).with{_, _ ->
                mCallbacks.onWriteCompleted(currentIndex, data.size)
            }.fail{ _, _ ->
                mCallbacks.onWriteCompleted(-1, data.size)
            }.enqueue()
        }
    }
}