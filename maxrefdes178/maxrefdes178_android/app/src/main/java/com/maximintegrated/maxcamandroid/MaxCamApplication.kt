package com.maximintegrated.maxcamandroid

import android.app.Application
import com.chibatching.kotpref.Kotpref
import com.maximintegrated.maxcamandroid.nativeLibrary.IMaxCamNativeLibrary
import timber.log.Timber

class MaxCamApplication : Application(){

    val maxCamNativeLibrary: IMaxCamNativeLibrary
        get() = ServiceLocator.provideMaxCamNativeLibrary()

    override fun onCreate() {
        super.onCreate()
        Kotpref.init(this)
        Timber.plant(Timber.DebugTree())
    }
}