package com.maximintegrated.bluetooth.devicelist

import android.bluetooth.BluetoothDevice
import android.view.View
import com.airbnb.epoxy.AutoModel
import com.airbnb.epoxy.EpoxyController
import com.maximintegrated.bluetooth.common.ExtendedBluetoothDevice
import com.maximintegrated.bluetooth.view.ErrorModel

class BluetoothDeviceListController : EpoxyController() {
    @AutoModel lateinit var pairedDevicesHeader: PairedDeviceListHeaderModel_
    @AutoModel lateinit var availableDevicesHeader: AvailableDeviceListHeaderModel_
    @AutoModel lateinit var noAvailableDevicesMessage: NoAvailableDevicesMessageModel_
    @AutoModel lateinit var bluetoothError: BluetoothErrorModel_

    var pairedDevices: List<BluetoothDevice>? = null
        set(value) {
            field = value
            requestModelBuild()
        }

    var availableDevices: List<ExtendedBluetoothDevice>? = emptyList()
        set(value) {
            field = value
            requestModelBuild()
        }

    var isDiscovering = false
        set(value) {
            field = value
            requestModelBuild()
        }

    var errorModel: ErrorModel? = null
        set(value) {
            field = value
            requestModelBuild()
        }

    var deviceClickListener: OnBluetoothDeviceClickListener? = null
        set(value) {
            field = value
            requestModelBuild()
        }

    var errorButtonClickListener: View.OnClickListener? = null
        set(value) {
            field = value
            requestModelBuild()
        }

    override fun buildModels() {
        errorModel?.let {
            bluetoothError
                    .errorModel(it)
                    .errorButtonClickListener(errorButtonClickListener)
                    .addTo(this)
            // Do not build rest of the models if error occurred
            return
        }

        pairedDevices?.let { devices ->
            if (devices.isNotEmpty()) {
                add(pairedDevicesHeader)

                add(devices.map {
                    BluetoothDeviceModel_()
                            .id(it.address)
                            .device(ExtendedBluetoothDevice(it))
                            .deviceClickListener(deviceClickListener)
                })
            }
        }

        add(availableDevicesHeader.discovering(isDiscovering))

        availableDevices?.let { devices ->
            if (isDiscovering || devices.isNotEmpty()) {
                // add devices that are not paired as well
                add(devices
                        .filterNot { pairedDevices?.contains(it.device) ?: false }
                        .map {
                            BluetoothDeviceModel_()
                                    .id(it.address)
                                    .device(it)
                                    .deviceClickListener(deviceClickListener)
                        }
                )
            } else {
                add(noAvailableDevicesMessage)
            }
        }

    }
}