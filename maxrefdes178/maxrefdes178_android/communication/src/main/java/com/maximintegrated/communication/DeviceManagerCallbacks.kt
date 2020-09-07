package com.maximintegrated.communication

import android.bluetooth.BluetoothDevice
import no.nordicsemi.android.ble.BleManagerCallbacks
import no.nordicsemi.android.ble.common.profile.battery.BatteryLevelCallback

interface MaxCamCallbacks {
    fun onDataReceived(device: BluetoothDevice, packet: ByteArray)
    fun onWriteCompleted(byteSent: Int, packetSize: Int)
}

interface DeviceManagerCallbacks : BleManagerCallbacks, BatteryLevelCallback, MaxCamCallbacks