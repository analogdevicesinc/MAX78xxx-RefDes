package com.maximintegrated.maxcamandroid

import android.bluetooth.BluetoothDevice
import android.content.Context
import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.WindowManager
import androidx.core.view.isVisible
import androidx.lifecycle.ViewModelProviders
import com.maximintegrated.bluetooth.devicelist.OnBluetoothDeviceClickListener
import com.maximintegrated.communication.MaxCamViewModel
import com.maximintegrated.maxcamandroid.exts.getCurrentFragment
import com.maximintegrated.maxcamandroid.exts.replaceFragment
import com.maximintegrated.maxcamandroid.main.LandingPage
import com.maximintegrated.maxcamandroid.main.MainFragment
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity(), OnBluetoothDeviceClickListener {

    private var bluetoothDevice: BluetoothDevice? = null
    private lateinit var maxCamViewModel: MaxCamViewModel

    companion object {
        private const val KEY_BLUETOOTH_DEVICE = "com.maximintegrated.maxcamandroid.BLUETOOTH_DEVICE"

        fun start(context: Context, bluetoothDevice: BluetoothDevice) {
            context.startActivity(
                Intent(context, MainActivity::class.java).apply {
                    flags = Intent.FLAG_ACTIVITY_NEW_TASK or Intent.FLAG_ACTIVITY_CLEAR_TASK
                    putExtra(KEY_BLUETOOTH_DEVICE, bluetoothDevice)
                })
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        window.addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON)
        supportActionBar!!.hide()

        appVersion.text = getString(R.string.app_version, BuildConfig.VERSION_NAME)
        bluetoothDevice = intent.getParcelableExtra(KEY_BLUETOOTH_DEVICE)
        firmwareVersion.text = bluetoothDevice?.name
        progressBar.isVisible = false
        replaceFragment(MainFragment.newInstance())

        maxCamViewModel = ViewModelProviders.of(this).get(MaxCamViewModel::class.java)
        bluetoothDevice?.let {
            maxCamViewModel.connect(it, getRequestedMtuSize())
        }

        mainFab.setOnClickListener {
            if(getCurrentFragment() as? LandingPage == null){
                onBackPressed()
            }
        }
    }

    override fun onBackPressed() {
        val fragment = getCurrentFragment()

        if(fragment == null || fragment as? LandingPage != null){
            startActivity(
                Intent(this, ScannerActivity::class.java).apply {
                    flags = Intent.FLAG_ACTIVITY_NEW_TASK or Intent.FLAG_ACTIVITY_CLEAR_TASK
                })
        }else{
            super.onBackPressed()
        }
    }

    override fun onBluetoothDeviceClicked(bluetoothDevice: BluetoothDevice) {
        val fragment = (getCurrentFragment() as? OnBluetoothDeviceClickListener)
        fragment?.onBluetoothDeviceClicked(bluetoothDevice)
    }

    private fun getRequestedMtuSize(): Int {
        return 247
    }
}
