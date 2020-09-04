package com.maximintegrated.bluetooth.livedata

import android.bluetooth.BluetoothAdapter
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import com.maximintegrated.bluetooth.util.isBluetoothDiscovering

class BtDiscoveryStateLiveData(context: Context) : BroadcastLiveData<DiscoveryState>(context) {

    override val intentFilter = IntentFilter().apply {
        addAction(BluetoothAdapter.ACTION_DISCOVERY_STARTED)
        addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED)
    }

    override val broadcastReceiver = object : BroadcastReceiver() {
        override fun onReceive(ctx: Context, intent: Intent) {
            when (intent.action) {
                BluetoothAdapter.ACTION_DISCOVERY_STARTED -> value = DiscoveryState.DISCOVERING
                BluetoothAdapter.ACTION_DISCOVERY_FINISHED -> value = DiscoveryState.NOT_DISCOVERING
            }
        }
    }

    override fun onActive() {
        super.onActive()
        value = if (isBluetoothDiscovering) {
            DiscoveryState.DISCOVERING
        } else {
            DiscoveryState.NOT_DISCOVERING
        }
    }
}