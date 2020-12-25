package com.maximintegrated.maxcamandroid

import com.chaquo.python.android.PyApplication
import com.chibatching.kotpref.Kotpref
import com.maximintegrated.maxcamandroid.nativeLibrary.IMaxCamNativeLibrary
import timber.log.Timber

class MaxCamApplication : PyApplication() {

    val maxCamNativeLibrary: IMaxCamNativeLibrary
        get() = ServiceLocator.provideMaxCamNativeLibrary()

    override fun onCreate() {
        super.onCreate()
        Kotpref.init(this)
        Timber.plant(Timber.DebugTree())
    }
}