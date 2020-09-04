package com.maximintegrated.bluetooth.common

import android.bluetooth.BluetoothClass
import android.bluetooth.BluetoothDevice
import com.maximintegrated.bluetooth.R

class ExtendedBluetoothDevice(val device: BluetoothDevice, val rssi: Int? = null, val name: String? = device.name) {
    val address: String get() = device.address

    val icon: Int
        get() = when (device.bluetoothClass?.majorDeviceClass) {
            BluetoothClass.Device.Major.COMPUTER -> R.drawable.ic_bluetooth_type_computer
            BluetoothClass.Device.Major.AUDIO_VIDEO -> R.drawable.ic_bluetooth_type_audio
            BluetoothClass.Device.Major.PHONE -> R.drawable.ic_bluetooth_type_phone
            else -> R.drawable.ic_bluetooth_type_unknown
        }
}