package com.maximintegrated.communication

import android.app.Application
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothProfile
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import timber.log.Timber

class MaxCamViewModel(application: Application) : AndroidViewModel(application), DeviceManagerCallbacks{

    var bluetoothDevice: BluetoothDevice? = null
        private set

    private val _connectionState = MutableLiveData<Int>()
    val connectionState: LiveData<Int>
        get() = _connectionState

    private val _receivedData = MutableLiveData<ByteArray>()
    val receivedData: LiveData<ByteArray>
        get() = _receivedData

    private val _batteryLevel = MutableLiveData<Int>()
    val batteryLevel: LiveData<Int>
        get() = _batteryLevel

    private val _isDeviceSupported = MutableLiveData<Boolean>()
    val isDeviceSupported: LiveData<Boolean>
        get() = _isDeviceSupported

    private val deviceManager = DeviceManager(application)

    val isConnected: Boolean
        get() = connectionState.value == BluetoothAdapter.STATE_CONNECTED

    private val _mtuSize = MutableLiveData<Int>()
    val mtuSize: LiveData<Int>
        get() = _mtuSize

    init {

        deviceManager.setGattCallbacks(this)
    }

    override fun onCleared() {
        super.onCleared()
        if(deviceManager.isConnected){
            //TODO: stop
            //TODO: disconnect
        }
    }

    fun connect(device: BluetoothDevice, requestedMtu: Int){
        if(bluetoothDevice == null){
            bluetoothDevice = device
            deviceManager.connect(device).enqueue()
            deviceManager.requestMtu(requestedMtu)
                .with { _, mtu ->
                    deviceManager.maxCamBleMtu = mtu
                    _mtuSize.value = mtu
                }.enqueue()
        }
    }

    fun disconnect() {
        bluetoothDevice = null
        deviceManager.disconnect().enqueue()
    }

    fun reconnect() {
        deviceManager.disconnect().enqueue()

        bluetoothDevice?.let {
            deviceManager
                .connect(it)
                .retry(3, 100)
                .useAutoConnect(false)
                .enqueue()
        }
    }

    fun sendData(data: ByteArray){
        deviceManager.sendData(data)
    }

    override fun onDeviceDisconnecting(device: BluetoothDevice) {
        _connectionState.value = BluetoothProfile.STATE_DISCONNECTING
        Timber.i("Disconnecting from device $device")
    }

    override fun onDeviceDisconnected(device: BluetoothDevice) {
        _connectionState.value = BluetoothProfile.STATE_DISCONNECTED
        Timber.i("Disconnected from device $device")
    }

    override fun onDeviceConnecting(device: BluetoothDevice) {
        _connectionState.value = BluetoothProfile.STATE_CONNECTING
        Timber.i("Connecting to device $device")
    }

    override fun onDeviceConnected(device: BluetoothDevice) {
        _connectionState.value = BluetoothProfile.STATE_CONNECTED
        Timber.i("Connected to device $device")
    }

    override fun onDeviceNotSupported(device: BluetoothDevice) {
        _isDeviceSupported.value = false
        Timber.e("Unsupported device $device")
    }

    override fun onBondingFailed(device: BluetoothDevice) {
        Timber.e("Bonding to device $device failed")
    }

    override fun onServicesDiscovered(device: BluetoothDevice, optionalServicesFound: Boolean) {
        Timber.i("Services of device $device discovered")
    }

    override fun onBondingRequired(device: BluetoothDevice) {
        Timber.d("Bonding to device $device is required")
    }

    override fun onLinkLossOccurred(device: BluetoothDevice) {
        Timber.e("Link to device $device is lost")
    }

    override fun onBonded(device: BluetoothDevice) {
        Timber.d("Bonded to device $device")
    }

    override fun onDeviceReady(device: BluetoothDevice) {
        _isDeviceSupported.value = true
        Timber.i("Device $device is ready")
    }

    override fun onError(device: BluetoothDevice, message: String, errorCode: Int) {
        Timber.e("An error occurred (Device: $device, ErrorCode: $errorCode, Message: $message)")
    }

    override fun onBatteryLevelChanged(device: BluetoothDevice, batteryLevel: Int) {
        _batteryLevel.value = batteryLevel
        Timber.i("Battery level changed ($device, $batteryLevel)")
    }

    override fun onDataReceived(device: BluetoothDevice, packet: ByteArray) {
        _receivedData.value = packet
        Timber.i("Data Received: ${packet.contentToString()}")
    }

    override fun onWriteCompleted(byteSent: Int, packetSize: Int) {
        Timber.i("Write to char is completed: $byteSent / $packetSize")
    }
}