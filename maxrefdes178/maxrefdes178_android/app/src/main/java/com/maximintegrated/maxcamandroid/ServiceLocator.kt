package com.maximintegrated.maxcamandroid

import androidx.annotation.VisibleForTesting
import com.maximintegrated.maxcamandroid.nativeLibrary.IMaxCamNativeLibrary
import com.maximintegrated.maxcamandroid.nativeLibrary.MaxCamNativeLibrary

object ServiceLocator {
    var maxCamNativeLibrary: IMaxCamNativeLibrary? = null
        @VisibleForTesting set

    fun provideMaxCamNativeLibrary(): IMaxCamNativeLibrary {
        synchronized(this) {
            return maxCamNativeLibrary ?: MaxCamNativeLibrary().also {
                maxCamNativeLibrary = it
            }
        }
    }
}