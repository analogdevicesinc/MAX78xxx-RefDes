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
                val device = ExtendedBluetoothDevice(null, 0, "DEMO", "00:11:22:33:AA:BB")
                add(BluetoothDeviceModel_()
                    .id(device.address)
                    .device(device)
                    .deviceClickListener(deviceClickListener))
                //add(noAvailableDevicesMessage)
            }
        }

    }
}