package com.maximintegrated.bluetooth.livedata

import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import com.maximintegrated.bluetooth.util.getPairedBluetoothDevices


class PairedDevicesLiveData(context: Context) : BroadcastLiveData<List<BluetoothDevice>>(context) {

    override val intentFilter = IntentFilter().apply {
        addAction(BluetoothAdapter.ACTION_STATE_CHANGED)
        addAction(BluetoothDevice.ACTION_BOND_STATE_CHANGED)
    }

    override val broadcastReceiver = object : BroadcastReceiver() {
        override fun onReceive(ctx: Context, intent: Intent) {
            when (intent.action) {
                BluetoothAdapter.ACTION_STATE_CHANGED -> {
                    val bluetoothState = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE,
                            BluetoothAdapter.ERROR)
                    if (bluetoothState == BluetoothAdapter.STATE_ON ||
                            bluetoothState == BluetoothAdapter.STATE_OFF) {
                        value = getPairedBluetoothDevices()
                    }
                }

                BluetoothDevice.ACTION_BOND_STATE_CHANGED -> {
                    value = getPairedBluetoothDevices()
                }
            }
        }
    }

    override fun onActive() {
        super.onActive()
        value = getPairedBluetoothDevices()
    }
}