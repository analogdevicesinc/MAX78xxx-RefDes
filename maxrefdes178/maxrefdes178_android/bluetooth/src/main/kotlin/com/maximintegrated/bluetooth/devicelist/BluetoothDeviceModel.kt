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
import android.widget.ImageView
import android.widget.TextView
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyHolder
import com.airbnb.epoxy.EpoxyModelClass
import com.airbnb.epoxy.EpoxyModelWithHolder
import com.maximintegrated.bluetooth.R
import com.maximintegrated.bluetooth.common.ExtendedBluetoothDevice
import com.maximintegrated.bluetooth.devicelist.BluetoothDeviceModel.BluetoothDeviceHolder
import com.maximintegrated.bluetooth.extension.gone
import com.maximintegrated.bluetooth.extension.visible

@EpoxyModelClass
abstract class BluetoothDeviceModel : EpoxyModelWithHolder<BluetoothDeviceHolder>(),
    View.OnClickListener {

    @EpoxyAttribute
    lateinit var device: ExtendedBluetoothDevice

    @EpoxyAttribute(hash = false)
    var deviceClickListener: OnBluetoothDeviceClickListener? = null

    override fun getDefaultLayout() = R.layout.model_bluetooth_device

    override fun bind(holder: BluetoothDeviceHolder) {
        holder.rootView.setOnClickListener(this)

        holder.apply {
            deviceIconView.setImageResource(device.icon)

            // use device address as name if device name is blank
            if (device.name.isNullOrBlank()) {
                deviceNameView.text = device.address
                deviceAddressView.gone()
            } else {
                deviceNameView.text = device.name
                deviceAddressView.text = device.address
                deviceAddressView.visible()
            }

            device.rssi?.let {
                val rssiPercent = when {
                    it <= -100 -> 0
                    it >= -50 -> 100
                    else -> 2 * (it + 100)
                }
                signalStrengthView.setImageLevel(rssiPercent)
                signalStrengthView.visibility = View.VISIBLE
                rssiValueView.text =
                        rssiValueView.context.getString(R.string.bluetooth_rssi_value, it)

            }
        }
    }

    override fun onClick(view: View) {
        deviceClickListener?.onBluetoothDeviceClicked(device.device)
    }

    class BluetoothDeviceHolder : EpoxyHolder() {
        lateinit var rootView: View
        lateinit var deviceIconView: ImageView
        lateinit var deviceNameView: TextView
        lateinit var deviceAddressView: TextView
        lateinit var signalStrengthView: ImageView
        lateinit var rssiValueView: TextView

        override fun bindView(view: View) {
            rootView = view

            deviceIconView = view.findViewById(R.id.device_icon)
            deviceNameView = view.findViewById(R.id.device_name)
            deviceAddressView = view.findViewById(R.id.device_address)
            signalStrengthView = view.findViewById(R.id.signal_strength_icon)
            rssiValueView = view.findViewById(R.id.rssi_value)
        }
    }
}

interface OnBluetoothDeviceClickListener {
    fun onBluetoothDeviceClicked(bluetoothDevice: BluetoothDevice?)
}

