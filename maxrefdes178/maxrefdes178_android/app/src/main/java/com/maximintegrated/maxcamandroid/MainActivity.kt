package com.maximintegrated.maxcamandroid

import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.WindowManager
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.isVisible
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.ViewModelProviders
import androidx.lifecycle.observe
import com.maximintegrated.bluetooth.devicelist.OnBluetoothDeviceClickListener
import com.maximintegrated.communication.MaxCamViewModel
import com.maximintegrated.maxcamandroid.exts.addFragment
import com.maximintegrated.maxcamandroid.exts.getCurrentFragment
import com.maximintegrated.maxcamandroid.main.LandingPage
import com.maximintegrated.maxcamandroid.main.MainFragment
import com.maximintegrated.maxcamandroid.nativeLibrary.IMaxCamNativeLibrary
import com.maximintegrated.maxcamandroid.utils.ROOT_FRAGMENT
import com.maximintegrated.maxcamandroid.view.BleConnectionInfo
import kotlinx.android.synthetic.main.activity_main.*
import kotlinx.android.synthetic.main.app_bar_layout.*
import timber.log.Timber

class MainActivity : AppCompatActivity(), OnBluetoothDeviceClickListener,
    IMaxCamNativeLibrary.JniListener {

    private var bluetoothDevice: BluetoothDevice? = null

    private lateinit var mainViewModel: MainViewModel
    private lateinit var maxCamViewModel: MaxCamViewModel

    private lateinit var maxCamNativeLibrary: IMaxCamNativeLibrary

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
        supportActionBar!!.hide()
        maxCamNativeLibrary = (application as MaxCamApplication).maxCamNativeLibrary
        maxCamNativeLibrary.setJniListener(this)
        appVersion.text = getString(R.string.app_version, BuildConfig.VERSION_NAME)
        bluetoothDevice = intent.getParcelableExtra(KEY_BLUETOOTH_DEVICE)

        /*
        val text = "${bluetoothDevice?.name} - ${maxCamNativeLibrary.version}"
        firmwareVersion.text = text

         */
        addFragment(fragment = MainFragment.newInstance(), backStackName = ROOT_FRAGMENT)
        mainViewModel =
            ViewModelProvider(
                this,
                MainViewModelFactory(application, maxCamNativeLibrary)
            ).get(MainViewModel::class.java)
        maxCamViewModel = ViewModelProviders.of(this).get(MaxCamViewModel::class.java)
        bluetoothDevice?.let {
            maxCamViewModel.connect(it, maxCamNativeLibrary.maxMtu)
        }
        maxCamViewModel.connectionState
            .observe(this) { connectionState ->
                val device = maxCamViewModel.bluetoothDevice
                if (connectionState == BluetoothAdapter.STATE_CONNECTED) {
                    maxCamNativeLibrary.bleReset()
                }

                toolbar.connectionInfo = if (device != null) {
                    BleConnectionInfo(connectionState, device.name, device.address)
                } else {
                    null
                }
            }

        mainViewModel.maxcamVersion.observe(this) {
            firmwareVersion.text = "${bluetoothDevice?.name} - ${it.max32666}"
        }
        maxCamViewModel.mtuSize.observe(this) {
            mainViewModel.onMtuSizeChanged(it)

            val text = "${bluetoothDevice?.name} - ${maxCamNativeLibrary.version}"
            firmwareVersion.text = text

            sendNotification(byteArrayOf(2, 3, 0, 0, 0, 0))
        }

        maxCamViewModel.receivedData.observeForever(dataReceivedObserver)

        toolbar.pageTitle = "MAXCAM AI85"

        mainFab.setOnClickListener {
            if (getCurrentFragment() as? LandingPage == null) {
                onBackPressed()
            }
        }
    }

    override fun onBackPressed() {
        val fragment = getCurrentFragment()

        if (fragment == null || fragment as? LandingPage != null) {
            startActivity(
                Intent(this, ScannerActivity::class.java).apply {
                    flags = Intent.FLAG_ACTIVITY_NEW_TASK or Intent.FLAG_ACTIVITY_CLEAR_TASK
                })
        } else {
            super.onBackPressed()
        }
    }

    override fun onDestroy() {
        maxCamNativeLibrary.setJniListener(null)
        maxCamViewModel.receivedData.removeObserver(dataReceivedObserver)
        super.onDestroy()
    }

    override fun onBluetoothDeviceClicked(bluetoothDevice: BluetoothDevice?) {
        val fragment = (getCurrentFragment() as? OnBluetoothDeviceClickListener)
        fragment?.onBluetoothDeviceClicked(bluetoothDevice)
    }

    override fun sendNotification(data: ByteArray?) {
        data?.let {
            maxCamViewModel.sendData(data)
        }
    }

    @ExperimentalUnsignedTypes
    override fun payloadReceived(payload: ByteArray?) {
        payload?.let {
            mainViewModel.onPayloadReceived(payload)
        }
    }

    fun changeVisibilityOfTheBottomView(visible: Boolean) {
        bottomView.isVisible = visible
    }
}
