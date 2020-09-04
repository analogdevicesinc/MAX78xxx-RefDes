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
    val deviceAddress: String,
    val batteryLevel: Int? = null,
    val isCharging: Boolean = false
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