@file:Suppress("NOTHING_TO_INLINE")

package com.maximintegrated.maxcamandroid.exts

import android.content.Context
import androidx.annotation.ColorRes
import androidx.annotation.DrawableRes
import androidx.annotation.IdRes
import androidx.core.content.ContextCompat
import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentActivity
import com.maximintegrated.maxcamandroid.R

inline fun Context.color(@ColorRes colorResId: Int) = ContextCompat.getColor(this, colorResId)

inline fun Context.drawable(@DrawableRes drawableResId: Int) =
    ContextCompat.getDrawable(this, drawableResId)

inline fun FragmentActivity.addFragment(fragment: Fragment, @IdRes containerId: Int = R.id.fragmentContainer) {
    supportFragmentManager.beginTransaction()
        .setCustomAnimations(
            R.anim.bottom_sheet_slide_in,
            R.anim.bottom_sheet_slide_out,
            R.anim.bottom_sheet_slide_in,
            R.anim.bottom_sheet_slide_out
        )
        .replace(containerId, fragment)
        .addToBackStack(null)
        .commit()
}

inline fun FragmentActivity.replaceFragment(fragment: Fragment, @IdRes containerId: Int = R.id.fragmentContainer) {
    supportFragmentManager.beginTransaction()
        .setCustomAnimations(
            R.anim.bottom_sheet_slide_in,
            R.anim.bottom_sheet_slide_out,
            R.anim.bottom_sheet_slide_in,
            R.anim.bottom_sheet_slide_out
        )
        .replace(containerId, fragment)
        .commit()
}

inline fun  FragmentActivity.getCurrentFragment(): Fragment? {
    return supportFragmentManager.findFragmentById(R.id.fragmentContainer)
}