package com.maximintegrated.bluetooth.bt

import android.app.Application
import com.maximintegrated.bluetooth.common.BaseBluetoothViewModel
import com.maximintegrated.bluetooth.livedata.BtAvailableDevicesLiveData
import com.maximintegrated.bluetooth.livedata.BtDiscoveryStateLiveData

class BluetoothScannerViewModel(app: Application) : BaseBluetoothViewModel(app) {
    override val discoveryState = BtDiscoveryStateLiveData(app)
    override val availableDevices = BtAvailableDevicesLiveData(app)
}