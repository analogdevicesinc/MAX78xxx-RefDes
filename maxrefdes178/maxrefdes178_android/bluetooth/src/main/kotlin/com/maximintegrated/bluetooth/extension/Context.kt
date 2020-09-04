package com.maximintegrated.bluetooth.extension

import android.content.Context
import android.content.pm.PackageManager
import androidx.annotation.DimenRes
import androidx.core.content.ContextCompat


fun Context.getDimension(@DimenRes id: Int): Int = this.resources.getDimensionPixelSize(id)

fun Context.hasPermission(permission: String) =
    ContextCompat.checkSelfPermission(this, permission) == PackageManager.PERMISSION_GRANTED

fun Context.hasPermissions(vararg permissions: String): Boolean = permissions.all {
    ContextCompat.checkSelfPermission(this, it) == PackageManager.PERMISSION_GRANTED
}