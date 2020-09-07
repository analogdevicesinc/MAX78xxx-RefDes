package com.maximintegrated.communication

import android.bluetooth.BluetoothGattCharacteristic
import java.util.*

const val BASE_BLUETOOTH_UUID_POSTFIX = "0000-1000-8000-00805F9B34FB"

fun uuidFromShortCode16(shortCode16: String) =
    UUID.fromString("0000$shortCode16-$BASE_BLUETOOTH_UUID_POSTFIX")

fun uuidFromShortCode32(shortCode32: String) =
    UUID.fromString("$shortCode32-$BASE_BLUETOOTH_UUID_POSTFIX")

fun BluetoothGattCharacteristic?.hasProperty(property: Int): Boolean {
    return ((this != null) && (properties and property) > 0)
}

val BluetoothGattCharacteristic?.hasReadProperty
    get() = hasProperty(BluetoothGattCharacteristic.PROPERTY_READ)

val BluetoothGattCharacteristic?.hasWriteProperty
    get() = hasProperty(BluetoothGattCharacteristic.PROPERTY_WRITE)

val BluetoothGattCharacteristic?.hasNotifyProperty
    get() = hasProperty(BluetoothGattCharacteristic.PROPERTY_NOTIFY)

val BluetoothGattCharacteristic?.hasIndicateProperty
    get() = hasProperty(BluetoothGattCharacteristic.PROPERTY_INDICATE)