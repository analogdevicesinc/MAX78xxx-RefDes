/*
 * Copyright (C) Maxim Integrated Products, Inc., All rights Reserved.
 *
 * This software is protected by copyright laws of the United States and
 * of foreign countries. This material may also be protected by patent laws
 * and technology transfer regulations of the United States and of foreign
 * countries. This software is furnished under a license agreement and/or a
 * nondisclosure agreement and may only be used or reproduced in accordance
 * with the terms of those agreements. Dissemination of this information to
 * any party or parties not specified in the license agreement and/or
 * nondisclosure agreement is expressly prohibited.
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 */

package com.maximintegrated.bluetooth.bt

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
import com.maximintegrated.bluetooth.util.cancelBluetoothDeviceDiscovery
import com.maximintegrated.bluetooth.util.startBluetoothDeviceDiscovery
import com.maximintegrated.bluetooth.util.verifyPermissions

class BluetoothScannerDialog : AppCompatDialogFragment() {

    companion object {
        private const val TAG = "BleScannerDialog"
        const val REQUEST_LOCATION_PERMISSION = 1

        fun show(activity: AppCompatActivity) {
            val fragment = BluetoothScannerDialog()
            fragment.show(activity.supportFragmentManager, TAG)
        }
    }

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
            .title(R.string.bluetooth_title)
            .customView(R.layout.dialog_bluetooth_scanner, false)
            .positiveText(R.string.bluetooth_scan_stop)
            .onPositive { _, _ ->
                if (scannerDelegate.isDiscovering) {
                    cancelBluetoothDeviceDiscovery()
                } else {
                    startBluetoothDeviceDiscovery()
                }
            }
            .negativeText(R.string.bluetooth_scan_cancel)
            .onNegative { _, _ ->
                cancelBluetoothDeviceDiscovery()
                dismiss()
            }
            .cancelable(false)
            .autoDismiss(false)
            .build()

        scannerDelegate = BluetoothScannerDelegate(
            this,
            ViewModelProviders.of(this).get(BluetoothScannerViewModel::class.java)
        ).apply {
            deviceClickListener = this@BluetoothScannerDialog.deviceClickListener
            scanStateChangeHandler = this@BluetoothScannerDialog::updateDialogButtons
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