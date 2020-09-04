package com.maximintegrated.maxcamandroid

import android.bluetooth.BluetoothDevice
import com.maximintegrated.bluetooth.ble.BleScannerActivity

class ScannerActivity : BleScannerActivity() {

    override fun onBluetoothDeviceClicked(bluetoothDevice: BluetoothDevice) {
        MainActivity.start(this, bluetoothDevice)
    }

}