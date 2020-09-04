package com.maximintegrated.bluetooth.livedata

import android.bluetooth.BluetoothAdapter
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import com.maximintegrated.bluetooth.util.bluetoothState

enum class AdapterState {
    OFF,
    ON,
    TURNING_OFF,
    TURNING_ON
}

class AdapterStateLiveData(context: Context) : BroadcastLiveData<AdapterState>(context) {

    override val intentFilter = IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED)

    override val broadcastReceiver = object : BroadcastReceiver() {
        override fun onReceive(ctx: Context, intent: Intent) {
            if (intent.action == BluetoothAdapter.ACTION_STATE_CHANGED) {
                val bluetoothState = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE,
                        BluetoothAdapter.ERROR)
                setBluetoothState(bluetoothState)
            }
        }
    }

    override fun onActive() {
        super.onActive()
        setBluetoothState(bluetoothState)
    }

    private fun setBluetoothState(state: Int) {
        when (state) {
            BluetoothAdapter.STATE_OFF -> value = AdapterState.OFF
            BluetoothAdapter.STATE_ON -> value = AdapterState.ON
            BluetoothAdapter.STATE_TURNING_OFF -> value = AdapterState.TURNING_OFF
            BluetoothAdapter.STATE_TURNING_ON -> value = AdapterState.TURNING_ON
        }
    }
}
