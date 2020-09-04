package com.maximintegrated.bluetooth.ble

import android.Manifest
import android.bluetooth.BluetoothDevice
import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.Menu
import android.view.MenuItem
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.lifecycle.ViewModelProviders
import com.maximintegrated.bluetooth.R
import com.maximintegrated.bluetooth.common.BluetoothScannerDelegate
import com.maximintegrated.bluetooth.devicelist.OnBluetoothDeviceClickListener
import com.maximintegrated.bluetooth.extension.hasPermission
import com.maximintegrated.bluetooth.extension.hasPermissions
import com.maximintegrated.bluetooth.livedata.BleAvailableDevicesLiveData
import com.maximintegrated.bluetooth.util.verifyPermissions
import kotlinx.android.synthetic.main.activity_bluetooth_scanner.*

open class BleScannerActivity : AppCompatActivity(), OnBluetoothDeviceClickListener {

    companion object {
        const val REQUEST_LOCATION_PERMISSION = 1
        const val REQUEST_STORAGE_PERMISSION = 2

        fun start(context: Context) {
            val intent = Intent(context, BleScannerActivity::class.java)
            context.startActivity(intent)
        }
    }

    private lateinit var scannerDelegate: BluetoothScannerDelegate

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_bluetooth_scanner)

        val supportedDevices = resources.getStringArray(R.array.supported_devices_prefix)

        scannerDelegate = BluetoothScannerDelegate(
            this,
            ViewModelProviders.of(this).get(BleScannerViewModel::class.java), supportedDevices
        ).apply {
            deviceClickListener = this@BleScannerActivity
            scanStateChangeHandler = this@BleScannerActivity::invalidateOptionsMenu
        }

        with(devicesRecyclerView) {
            setHasFixedSize(true)
            adapter = scannerDelegate.deviceListAdapter
        }
    }

    override fun onResume() {
        super.onResume()

        if (!hasPermission(Manifest.permission.ACCESS_FINE_LOCATION)) {
            scannerDelegate.showLocationPermissionMessage(View.OnClickListener {
                ActivityCompat.requestPermissions(
                    this,
                    arrayOf(Manifest.permission.ACCESS_FINE_LOCATION), REQUEST_LOCATION_PERMISSION
                )
            })
        } else if (!hasPermissions(
                Manifest.permission.READ_EXTERNAL_STORAGE,
                Manifest.permission.WRITE_EXTERNAL_STORAGE
            )
        ) {
            scannerDelegate.showStoragePermissionMessage(View.OnClickListener {
                ActivityCompat.requestPermissions(
                    this,
                    arrayOf(
                        Manifest.permission.READ_EXTERNAL_STORAGE,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE
                    ),
                    REQUEST_STORAGE_PERMISSION
                )
            })
        }
    }

    override fun onRequestPermissionsResult(
        requestCode: Int, permissions: Array<String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)

        if (requestCode == REQUEST_LOCATION_PERMISSION && verifyPermissions(grantResults)) {
            // permission was granted
            scannerDelegate.hideLocationPermissionMessage()
            BleAvailableDevicesLiveData.startScan(this)
        }
        if (requestCode == REQUEST_STORAGE_PERMISSION && verifyPermissions(grantResults)) {
            // permission was granted
            scannerDelegate.hideStoragePermissionMessage()
        }
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        menuInflater.inflate(R.menu.activity_bluetooth_scanner, menu)
        return true
    }

    override fun onPrepareOptionsMenu(menu: Menu): Boolean {
        menu.findItem(R.id.action_scan_start).isVisible = scannerDelegate.isScanStartVisible
        menu.findItem(R.id.action_scan_stop).isVisible = scannerDelegate.isScanStopVisible
        if(!scannerDelegate.showingAnyErrorModel){
            scannerWarningCardView.visibility = View.VISIBLE
        }
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem) = when (item.itemId) {
        R.id.action_scan_start -> {
            BleAvailableDevicesLiveData.startScan(this)
            true
        }
        R.id.action_scan_stop -> {
            BleAvailableDevicesLiveData.stopScan(this)
            true
        }
        else -> super.onOptionsItemSelected(item)
    }

    override fun onBluetoothDeviceClicked(bluetoothDevice: BluetoothDevice) {
        // will be overridden
    }
}
