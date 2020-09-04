package com.maximintegrated.bluetooth.ble

import android.app.Application
import androidx.lifecycle.LiveData
import com.maximintegrated.bluetooth.common.BaseBluetoothViewModel
import com.maximintegrated.bluetooth.livedata.BleAvailableDevicesLiveData
import com.maximintegrated.bluetooth.livedata.DiscoveryState

class BleScannerViewModel(app: Application) : BaseBluetoothViewModel(app) {
    override val availableDevices = BleAvailableDevicesLiveData(app)
    override val discoveryState: LiveData<DiscoveryState> get() = availableDevices.discoveryState
}