package com.maximintegrated.maxcamandroid

import android.app.Application
import com.chibatching.kotpref.Kotpref
import timber.log.Timber

class MaxCamApplication : Application(){

    override fun onCreate() {
        super.onCreate()
        Kotpref.init(this)
        Timber.plant(Timber.DebugTree())
    }
}