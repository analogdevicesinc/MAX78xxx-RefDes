package com.maximintegrated.bluetooth.livedata

import android.content.BroadcastReceiver
import android.content.Context
import android.content.IntentFilter
import androidx.lifecycle.LiveData

abstract class BroadcastLiveData<T>(context: Context) : LiveData<T>() {

    internal abstract val intentFilter: IntentFilter
    internal abstract val broadcastReceiver: BroadcastReceiver

    internal val context: Context = context.applicationContext

    override fun onActive() {
        super.onActive()
        context.registerReceiver(broadcastReceiver, intentFilter)
    }

    override fun onInactive() {
        super.onInactive()
        context.unregisterReceiver(broadcastReceiver)
    }

}