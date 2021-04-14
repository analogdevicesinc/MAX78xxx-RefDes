package com.maximintegrated.maxcamandroid

import com.chaquo.python.android.PyApplication
import com.chibatching.kotpref.Kotpref
import timber.log.Timber

class MaxCamApplication : PyApplication() {

    override fun onCreate() {
        super.onCreate()
        Kotpref.init(this)
        Timber.plant(Timber.DebugTree())
    }
}