package com.maximintegrated.bluetooth.util

import android.content.pm.PackageManager

fun verifyPermissions(grantResults: IntArray): Boolean {
    // At least one result must be checked.
    if (grantResults.isEmpty()) return false

    // Verify that each required permission has been granted, otherwise return false.
    return grantResults.all { it == PackageManager.PERMISSION_GRANTED }
}