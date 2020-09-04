package com.maximintegrated.bluetooth.util

import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice

fun enableBluetooth() = BluetoothAdapter.getDefaultAdapter()?.enable() ?: false

fun startBluetoothDeviceDiscovery() = BluetoothAdapter.getDefaultAdapter()?.startDiscovery() ?: false

fun cancelBluetoothDeviceDiscovery() = BluetoothAdapter.getDefaultAdapter()?.cancelDiscovery() ?: false

val isBluetoothEnabled get() = BluetoothAdapter.getDefaultAdapter()?.isEnabled ?: false

val isBluetoothDiscovering get() = BluetoothAdapter.getDefaultAdapter()?.isDiscovering ?: false

val bluetoothState get() = BluetoothAdapter.getDefaultAdapter()?.state ?: BluetoothAdapter.STATE_OFF

fun getPairedBluetoothDevices(): List<BluetoothDevice> {
    return BluetoothAdapter.getDefaultAdapter()
            ?.bondedDevices
            ?.sortedWith(compareBy({ it.name }, { it.address }))
            ?: emptyList()
}

