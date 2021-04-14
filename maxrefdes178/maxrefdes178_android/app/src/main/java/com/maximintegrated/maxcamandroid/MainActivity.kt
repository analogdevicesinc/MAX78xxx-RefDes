package com.maximintegrated.maxcamandroid

import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.WindowManager
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.isVisible
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.ViewModelProviders
import androidx.lifecycle.observe
import com.maximintegrated.bluetooth.devicelist.OnBluetoothDeviceClickListener
import com.maximintegrated.communication.MaxCamViewModel
import com.maximintegrated.maxcamandroid.blePacket.BleDefinitions
import com.maximintegrated.maxcamandroid.blePacket.ble_command_e
import com.maximintegrated.maxcamandroid.blePacket.ble_command_packet_t
import com.maximintegrated.maxcamandroid.exts.addFragment
import com.maximintegrated.maxcamandroid.exts.getCurrentFragment
import com.maximintegrated.maxcamandroid.main.LandingPage
import com.maximintegrated.maxcamandroid.main.MainFragment
import com.maximintegrated.maxcamandroid.utils.ROOT_FRAGMENT
import com.maximintegrated.maxcamandroid.utils.startScannerActivity
import com.maximintegrated.maxcamandroid.view.BleConnectionInfo
import kotlinx.android.synthetic.main.activity_main.*
import kotlinx.android.synthetic.main.app_bar_layout.*
import timber.log.Timber

class MainActivity : AppCompatActivity(), OnBluetoothDeviceClickListener {

    private var bluetoothDevice: BluetoothDevice? = null

    private lateinit var mainViewModel: MainViewModel
    private lateinit var maxCamViewModel: MaxCamViewModel

    companion object {
        private const val KEY_BLUETOOTH_DEVICE =
            "com.maximintegrated.maxcamandroid.BLUETOOTH_DEVICE"

        fun start(context: Context, bluetoothDevice: BluetoothDevice?) {
            context.startActivity(
                Intent(context, MainActivity::class.java).apply {
                    flags = Intent.FLAG_ACTIVITY_NEW_TASK or Intent.FLAG_ACTIVITY_CLEAR_TASK
                    putExtra(KEY_BLUETOOTH_DEVICE, bluetoothDevice)
                })
        }
    }

    private val dataReceivedObserver = Observer<ByteArray> { data ->
        Timber.d("data size = ${data.size} data = ${data.contentToString()}")
        mainViewModel.onBleDataReceived(data)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        window.addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON)
        supportActionBar?.hide()
        appVersion.text = getString(R.string.app_version, BuildConfig.VERSION_NAME)
        bluetoothDevice = intent.getParcelableExtra(KEY_BLUETOOTH_DEVICE)

        /*
        val text = "${bluetoothDevice?.name} - ${maxCamNativeLibrary.version}"
        firmwareVersion.text = text

         */
        addFragment(fragment = MainFragment.newInstance(), backStackName = ROOT_FRAGMENT)
        mainViewModel = ViewModelProvider(
            this,
            ViewModelProvider.AndroidViewModelFactory(application)
        ).get(MainViewModel::class.java)
        maxCamViewModel = ViewModelProviders.of(this).get(MaxCamViewModel::class.java)
        bluetoothDevice?.let {
            maxCamViewModel.connect(it, BleDefinitions.BLE_MAX_MTU_REQUEST_SIZE)
        }
        maxCamViewModel.connectionState
            .observe(this) { connectionState ->
                Timber.i("Connection state is changed! $connectionState")
                if (connectionState == BluetoothAdapter.STATE_DISCONNECTED && (maxCamViewModel.previousConnectionState == BluetoothAdapter.STATE_CONNECTED || maxCamViewModel.previousConnectionState == BluetoothAdapter.STATE_DISCONNECTING)) {
                    Toast.makeText(applicationContext, "Connection is lost!", Toast.LENGTH_LONG)
                        .show()
                    startScannerActivity(this)
                }

                val device = maxCamViewModel.bluetoothDevice

                toolbar.connectionInfo = if (device != null) {
                    BleConnectionInfo(connectionState, device.name, device.address)
                } else {
                    null
                }
            }

        mainViewModel.maxcamVersion.observe(this) {
            firmwareVersion.text =
                "MAX32666 - ${it.max32666}\n" +
                        "MAX78000 Video - ${it.max78000_video}\n" +
                        "MAX78000 Audio - ${it.max78000_audio}"
        }

        mainViewModel.mtuResponse.observe(this) {
            maxCamViewModel.setMtu(it.mtu)

            maxCamViewModel.sendData(
                ble_command_packet_t.from(ble_command_e.BLE_COMMAND_GET_VERSION_CMD).toByteArray()
            )
        }

        maxCamViewModel.mtuSize.observe(this) {
            val text = "${bluetoothDevice?.name}"
            firmwareVersion.text = text
        }

        maxCamViewModel.receivedData.observeForever(dataReceivedObserver)

        toolbar.pageTitle = getString(R.string.app_name)

        mainFab.setOnClickListener {
            if (getCurrentFragment() as? LandingPage == null) {
                onBackPressed()
            }
        }
    }

    override fun onBackPressed() {
        val fragment = getCurrentFragment()

        if (fragment == null || fragment as? LandingPage != null) {
            startScannerActivity(this)
        } else {
            super.onBackPressed()
        }
    }

    override fun onDestroy() {
        maxCamViewModel.receivedData.removeObserver(dataReceivedObserver)
        super.onDestroy()
    }

    override fun onBluetoothDeviceClicked(bluetoothDevice: BluetoothDevice?) {
        val fragment = (getCurrentFragment() as? OnBluetoothDeviceClickListener)
        fragment?.onBluetoothDeviceClicked(bluetoothDevice)
    }

    fun changeVisibilityOfTheBottomView(visible: Boolean) {
        bottomView.isVisible = visible
    }
}
