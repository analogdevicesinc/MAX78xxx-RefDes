package com.maximintegrated.bluetooth.livedata

import android.Manifest
import android.bluetooth.BluetoothAdapter
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.os.Handler
import androidx.lifecycle.MutableLiveData
import com.maximintegrated.bluetooth.common.ExtendedBluetoothDevice
import com.maximintegrated.bluetooth.extension.hasPermission
import com.maximintegrated.bluetooth.util.isBluetoothEnabled
import no.nordicsemi.android.support.v18.scanner.BluetoothLeScannerCompat
import no.nordicsemi.android.support.v18.scanner.ScanCallback
import no.nordicsemi.android.support.v18.scanner.ScanResult
import no.nordicsemi.android.support.v18.scanner.ScanSettings

class BleAvailableDevicesLiveData(context: Context) :
    BroadcastLiveData<List<ExtendedBluetoothDevice>>(context) {

    companion object {
        private const val BLE_DISCOVERY_TIME_MS = 25000L
        const val BLE_SCAN_START =
            "com.maximintegrated.bluetooth.livedata.BLE_SCAN_START"
        const val BLE_SCAN_STOP =
            "com.maximintegrated.bluetooth.livedata.BLE_SCAN_STOP"

        fun startScan(context: Context) {
            context.sendBroadcast(Intent(BLE_SCAN_START))
        }

        fun stopScan(context: Context) {
            context.sendBroadcast(Intent(BLE_SCAN_STOP))
        }
    }

    override val intentFilter = IntentFilter().apply {
        addAction(BLE_SCAN_START)
        addAction(BLE_SCAN_STOP)
        addAction(BluetoothAdapter.ACTION_STATE_CHANGED)
    }

    override val broadcastReceiver = object : BroadcastReceiver() {
        override fun onReceive(ctx: Context, intent: Intent?) {
            when (intent?.action) {
                BLE_SCAN_START -> startBleScan()
                BLE_SCAN_STOP -> stopBleScan()
                BluetoothAdapter.ACTION_STATE_CHANGED -> {
                    val bluetoothState = intent.getIntExtra(
                        BluetoothAdapter.EXTRA_STATE,
                        BluetoothAdapter.ERROR
                    )
                    when (bluetoothState) {
                        BluetoothAdapter.STATE_OFF -> clearDevices()
                        BluetoothAdapter.STATE_ON -> startBleScan()
                    }
                }
            }
        }
    }

    private val availableDevices = mutableListOf<ExtendedBluetoothDevice>()
    private val bleScanner = BluetoothLeScannerCompat.getScanner()
    private val handler = Handler()

    val discoveryState = MutableLiveData<DiscoveryState>()

    private val bleScanCallback = object : ScanCallback() {
        override fun onScanResult(callbackType: Int, result: ScanResult) {
            super.onScanResult(callbackType, result)

            val foundDevice = availableDevices.firstOrNull { it.address == result.device.address }
            if (foundDevice == null) {
                availableDevices.add(
                    ExtendedBluetoothDevice(
                        result.device,
                        result.rssi,
                        result.scanRecord?.deviceName
                    )
                )
                value = availableDevices
            } else {
                availableDevices[availableDevices.indexOf(foundDevice)] =
                    ExtendedBluetoothDevice(
                        result.device,
                        result.rssi,
                        result.scanRecord?.deviceName
                    )
                value = availableDevices
            }
        }
    }

    override fun onActive() {
        super.onActive()
        if (isBluetoothEnabled && context.hasPermission(Manifest.permission.ACCESS_FINE_LOCATION)) {
            startBleScan()
            handler.postDelayed({
                stopBleScan()
            }, BLE_DISCOVERY_TIME_MS)
        }
    }

    override fun onInactive() {
        super.onInactive()
        stopBleScan()
    }

    private fun startBleScan() {
        clearDevices()

        val scanSettings = ScanSettings.Builder()
            .setScanMode(ScanSettings.SCAN_MODE_BALANCED)
            .build()
        if(isBluetoothEnabled){
            bleScanner.stopScan(bleScanCallback)
            bleScanner.startScan(null, scanSettings, bleScanCallback)
            discoveryState.value = DiscoveryState.DISCOVERING
        }
    }

    private fun stopBleScan() {
        if (isBluetoothEnabled) {
            bleScanner.stopScan(bleScanCallback)
        }
        discoveryState.value = DiscoveryState.NOT_DISCOVERING
    }

    private fun clearDevices() {
        availableDevices.clear()
        value = emptyList()
    }
}