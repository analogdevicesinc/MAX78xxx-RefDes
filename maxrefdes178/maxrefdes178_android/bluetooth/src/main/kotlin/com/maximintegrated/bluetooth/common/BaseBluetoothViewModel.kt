package com.maximintegrated.bluetooth.common

import android.app.Application
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.LiveData
import com.maximintegrated.bluetooth.livedata.AdapterStateLiveData
import com.maximintegrated.bluetooth.livedata.DiscoveryState
import com.maximintegrated.bluetooth.livedata.PairedDevicesLiveData

abstract class BaseBluetoothViewModel(app: Application) : AndroidViewModel(app) {
    val adapterState = AdapterStateLiveData(app)
    val pairedDevices = PairedDevicesLiveData(app)

    abstract val discoveryState: LiveData<DiscoveryState>
    abstract val availableDevices: LiveData<List<ExtendedBluetoothDevice>>
}