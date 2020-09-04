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