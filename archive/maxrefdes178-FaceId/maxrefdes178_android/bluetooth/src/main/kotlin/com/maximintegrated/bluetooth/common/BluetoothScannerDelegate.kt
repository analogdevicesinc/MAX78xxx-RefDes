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

package com.maximintegrated.bluetooth.common

import android.view.View
import androidx.lifecycle.LifecycleOwner
import androidx.lifecycle.Observer
import androidx.recyclerview.widget.RecyclerView
import com.maximintegrated.bluetooth.R
import com.maximintegrated.bluetooth.devicelist.BluetoothDeviceListController
import com.maximintegrated.bluetooth.livedata.AdapterState
import com.maximintegrated.bluetooth.livedata.DiscoveryState
import com.maximintegrated.bluetooth.util.enableBluetooth
import com.maximintegrated.bluetooth.view.ErrorModel

class BluetoothScannerDelegate(
    lifecycleOwner: LifecycleOwner,
    viewModel: BaseBluetoothViewModel,
    private val deviceNamePrefixList: Array<String>? = null
) {
    private val deviceListController = BluetoothDeviceListController()

    private val bluetoothOffErrorModel = ErrorModel(
        R.drawable.ic_bluetooth_off,
        R.string.bluetooth_off_error_message,
        R.string.bluetooth_off_error_action
    )

    private val locationPermissionErrorModel = ErrorModel(
        R.drawable.ic_location,
        R.string.bluetooth_location_permission_message,
        R.string.bluetooth_location_permission_action
    )

    private val storagePermissionErrorModel = ErrorModel(
        R.drawable.ic_storage,
        R.string.bluetooth_storage_permission_message,
        R.string.bluetooth_storage_permission_action
    )

    private var isBluetoothOn = false
        set(value) {
            field = value
            showBluetoothOffErrorIfNeeded()
            scanStateChangeHandler?.invoke()
        }

    var isDiscovering
        get() = deviceListController.isDiscovering
        private set(value) {
            deviceListController.isDiscovering = value
            scanStateChangeHandler?.invoke()
        }

    val isShowingLocationPermissionMessage
        get() = deviceListController.errorModel == locationPermissionErrorModel

    val isShowingStoragePermissionMessage
        get() = deviceListController.errorModel == storagePermissionErrorModel

    val isScanStartVisible
        get() = !isShowingLocationPermissionMessage && !isShowingStoragePermissionMessage && isBluetoothOn && !isDiscovering

    val isScanStopVisible
        get() = !isShowingLocationPermissionMessage && !isShowingStoragePermissionMessage && isBluetoothOn && isDiscovering

    var deviceClickListener
        get() = deviceListController.deviceClickListener
        set(value) {
            deviceListController.deviceClickListener = value
        }

    val deviceListAdapter: RecyclerView.Adapter<*> get() = deviceListController.adapter

    var scanStateChangeHandler: (() -> Unit)? = null

    val showingAnyErrorModel
        get() = deviceListController.errorModel != null

    init {
        viewModel.adapterState
            .observe(lifecycleOwner, Observer {
                isBluetoothOn = (it == AdapterState.ON)
            })

        viewModel.discoveryState
            .observe(lifecycleOwner, Observer {
                isDiscovering = (it == DiscoveryState.DISCOVERING)
            })

        viewModel.pairedDevices
            .observe(lifecycleOwner, Observer {
                deviceListController.pairedDevices = if (deviceNamePrefixList == null) {
                    it
                } else {
                    it.filter { device ->
                        val deviceName = device.name ?: ""
                       deviceNameCheck(deviceName)
                    }
                }
            })

        viewModel.availableDevices
            .observe(lifecycleOwner, Observer {
                deviceListController.availableDevices = if (deviceNamePrefixList == null) {
                    it
                } else {
                    it.filter { device ->
                        val deviceName = device.name ?: ""
                        deviceNameCheck(deviceName)
                    }
                }
            })
    }

    private fun deviceNameCheck(deviceName: String): Boolean{
        var found = false
        for(name in deviceNamePrefixList!!){
            if(deviceName.startsWith(name, true)){
                found = true
                break
            }
        }
        return found;
    }

    fun showLocationPermissionMessage(actionClickListener: View.OnClickListener) {
        deviceListController.errorButtonClickListener = actionClickListener
        deviceListController.errorModel = locationPermissionErrorModel
        scanStateChangeHandler?.invoke()
    }

    fun hideLocationPermissionMessage() {
        deviceListController.errorButtonClickListener = null
        deviceListController.errorModel = null
        scanStateChangeHandler?.invoke()
    }

    fun showStoragePermissionMessage(actionClickListener: View.OnClickListener) {
        deviceListController.errorButtonClickListener = actionClickListener
        deviceListController.errorModel = storagePermissionErrorModel
        scanStateChangeHandler?.invoke()
    }

    fun hideStoragePermissionMessage() {
        deviceListController.errorButtonClickListener = null
        deviceListController.errorModel = null
        scanStateChangeHandler?.invoke()
        showBluetoothOffErrorIfNeeded()
    }

    private fun showBluetoothOffErrorIfNeeded() {
        // ignore if location permission message is shown
        if (isShowingStoragePermissionMessage) return

        if (isBluetoothOn) {
            deviceListController.errorButtonClickListener = null
            deviceListController.errorModel = null
        } else {
            deviceListController.errorButtonClickListener =
                View.OnClickListener { enableBluetooth() }
            deviceListController.errorModel = bluetoothOffErrorModel
        }
    }
}