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

package com.maximintegrated.maxcamandroid.view

import android.bluetooth.BluetoothAdapter
import android.content.Context
import android.util.AttributeSet
import android.view.View
import android.widget.LinearLayout
import android.widget.TextView
import androidx.appcompat.widget.Toolbar
import androidx.core.view.isVisible
import com.maximintegrated.maxcamandroid.R
import eo.view.bluetoothstate.BluetoothState
import eo.view.bluetoothstate.BluetoothStateView
import kotlinx.android.synthetic.main.connection.view.*

data class BleConnectionInfo(
    val connectionStateCode: Int,
    val deviceName: String?,
    val deviceAddress: String
) {
    val bluetoothState: BluetoothState.State
        get() = when (connectionStateCode) {
            BluetoothAdapter.STATE_CONNECTING -> BluetoothState.State.CONNECTING
            BluetoothAdapter.STATE_CONNECTED -> BluetoothState.State.CONNECTED
            else -> BluetoothState.State.ON
        }
}


class BleConnectionToolbar @JvmOverloads constructor(
    context: Context, attrs: AttributeSet? = null
) : Toolbar(context, attrs) {

    var connectionInfo: BleConnectionInfo? = null
        set(value) {
            field = value

            if (value != null) {
                showBleConnectionInfo(value)
            } else {
                //showSearchDevice()
            }
        }

    var pageTitle: String = ""
        set(value) {
            field = value
            toolbarTitle.text = value
        }

    var subtitle: String = ""
        set(value) {
            field = value
            toolbarSubtitle.text = value
            if (value == "") {
                toolbarSubtitle.visibility = View.GONE
            } else if (toolbarSubtitle.visibility != View.VISIBLE) {
                toolbarSubtitle.visibility = View.VISIBLE
            }
        }

    private val bluetoothStateView: BluetoothStateView
    private val toolbarTitle: TextView

    init {
        LinearLayout.inflate(context, R.layout.connection, this)

        bluetoothStateView = findViewById(R.id.bluetooth_state_view)
        toolbarTitle = findViewById(R.id.toolbarTitle)
    }

    private fun showBleConnectionInfo(connectionInfo: BleConnectionInfo) {
        bluetoothStateView.state = connectionInfo.bluetoothState
        bluetoothStateView.isVisible = true
    }
}