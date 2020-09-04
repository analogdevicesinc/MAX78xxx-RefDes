package com.maximintegrated.bluetooth.ble

import android.Manifest
import android.app.Dialog
import android.content.Context
import android.os.Bundle
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.app.AppCompatDialogFragment
import androidx.lifecycle.ViewModelProviders
import androidx.recyclerview.widget.RecyclerView
import com.afollestad.materialdialogs.DialogAction
import com.afollestad.materialdialogs.MaterialDialog
import com.maximintegrated.bluetooth.R
import com.maximintegrated.bluetooth.common.BluetoothScannerDelegate
import com.maximintegrated.bluetooth.devicelist.OnBluetoothDeviceClickListener
import com.maximintegrated.bluetooth.extension.gone
import com.maximintegrated.bluetooth.extension.hasPermission
import com.maximintegrated.bluetooth.extension.visible
import com.maximintegrated.bluetooth.livedata.BleAvailableDevicesLiveData
import com.maximintegrated.bluetooth.util.verifyPermissions

class BleScannerDialog : AppCompatDialogFragment() {

    companion object {
        const val TAG = "BleScannerDialog"
        const val REQUEST_LOCATION_PERMISSION = 1

        const val ARG_DEVICE_NAME_PREFIX = "deviceNamePrefix"
        const val ARG_DIALOG_TITLE = "dialogTitle"

        fun newInstance(
            dialogTitle: String? = null,
            deviceNamePrefix: String? = null
        ): BleScannerDialog {
            val fragment = BleScannerDialog()
            fragment.arguments = Bundle().apply {
                putString(ARG_DIALOG_TITLE, dialogTitle)
                putString(ARG_DEVICE_NAME_PREFIX, deviceNamePrefix)
            }

            return fragment
        }
    }

    private val deviceNamePrefix: String?
        get() = arguments?.getString(ARG_DEVICE_NAME_PREFIX)

    private val dialogTitle: String
        get() = arguments?.getString(ARG_DIALOG_TITLE) ?: getString(R.string.bluetooth_title)


    private lateinit var deviceClickListener: OnBluetoothDeviceClickListener
    private lateinit var scannerDelegate: BluetoothScannerDelegate

    override fun onAttach(context: Context) {
        super.onAttach(context)

        deviceClickListener = context as? OnBluetoothDeviceClickListener
            ?: throw ClassCastException("$context must implement OnBluetoothDeviceClickListener")
    }

    override fun onResume() {
        super.onResume()

        if (context?.hasPermission(Manifest.permission.ACCESS_FINE_LOCATION) != true) {
            scannerDelegate.showLocationPermissionMessage(View.OnClickListener {
                requestPermissions(
                    arrayOf(Manifest.permission.ACCESS_FINE_LOCATION),
                    REQUEST_LOCATION_PERMISSION
                )
            })
        }
    }

    override fun onRequestPermissionsResult(
        requestCode: Int, permissions: Array<String>,
        grantResults: IntArray
    ) {
        if (requestCode == REQUEST_LOCATION_PERMISSION && verifyPermissions(grantResults)) {
            // permission was granted
            scannerDelegate.hideLocationPermissionMessage()
        }
    }

    override fun onCreateDialog(savedInstanceState: Bundle?): Dialog {
        val context = context ?: throw RuntimeException("Context cannot be null")

        val dialog = MaterialDialog.Builder(context)
            .title(dialogTitle)
            .customView(R.layout.dialog_bluetooth_scanner, false)
            .positiveText(R.string.bluetooth_scan_stop)
            .onPositive { _, _ ->
                if (scannerDelegate.isDiscovering) {
                    BleAvailableDevicesLiveData.stopScan(context)
                } else {
                    BleAvailableDevicesLiveData.startScan(context)
                }
            }
            .negativeText(R.string.bluetooth_scan_cancel)
            .onNegative { _, _ ->
                BleAvailableDevicesLiveData.stopScan(context)
                dismiss()
            }
            .cancelable(false)
            .autoDismiss(false)
            .build()

        scannerDelegate = BluetoothScannerDelegate(
            this,
            ViewModelProviders.of(this).get(BleScannerViewModel::class.java),
            deviceNamePrefix?.let { arrayOf(it) }
        ).apply {
            deviceClickListener = this@BleScannerDialog.deviceClickListener
            scanStateChangeHandler = this@BleScannerDialog::updateDialogButtons
        }

        dialog.customView?.findViewById<RecyclerView>(R.id.devices_recycler_view)?.apply {
            setHasFixedSize(true)
            adapter = scannerDelegate.deviceListAdapter
        }

        return dialog
    }

    fun updateDialogButtons() {
        val dialog = dialog as? MaterialDialog ?: return

        val positiveButton = dialog.getActionButton(DialogAction.POSITIVE)
        if (!scannerDelegate.isScanStartVisible && !scannerDelegate.isScanStopVisible) {
            positiveButton.gone()
        } else {
            positiveButton.visible()
            positiveButton.setText(
                if (scannerDelegate.isScanStartVisible) {
                    R.string.bluetooth_scan_start
                } else {
                    R.string.bluetooth_scan_stop
                }
            )
        }
    }
}