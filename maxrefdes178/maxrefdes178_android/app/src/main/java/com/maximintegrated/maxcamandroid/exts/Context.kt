@file:Suppress("NOTHING_TO_INLINE")

package com.maximintegrated.maxcamandroid.exts

import android.content.Context
import androidx.annotation.ColorRes
import androidx.annotation.DrawableRes
import androidx.annotation.IdRes
import androidx.core.content.ContextCompat
import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentActivity
import androidx.fragment.app.FragmentManager
import com.maximintegrated.maxcamandroid.R

inline fun Context.color(@ColorRes colorResId: Int) = ContextCompat.getColor(this, colorResId)

inline fun Context.drawable(@DrawableRes drawableResId: Int) =
    ContextCompat.getDrawable(this, drawableResId)

inline fun FragmentActivity.addFragment(fragment: Fragment, @IdRes containerId: Int = R.id.fragmentContainer, backStackName: String? = null) {
    supportFragmentManager.beginTransaction()
        .setCustomAnimations(
            R.anim.bottom_sheet_slide_in,
            R.anim.bottom_sheet_slide_out,
            R.anim.bottom_sheet_slide_in,
            R.anim.bottom_sheet_slide_out
        )
        .replace(containerId, fragment)
        .addToBackStack(backStackName)
        .commit()
}

inline fun FragmentActivity.replaceFragment(fragment: Fragment, @IdRes containerId: Int = R.id.fragmentContainer, backStackName: String? = null) {
    supportFragmentManager.popBackStack(backStackName, FragmentManager.POP_BACK_STACK_INCLUSIVE)
    supportFragmentManager.beginTransaction()
        .setCustomAnimations(
            R.anim.bottom_sheet_slide_in,
            R.anim.bottom_sheet_slide_out,
            R.anim.bottom_sheet_slide_in,
            R.anim.bottom_sheet_slide_out
        )
        .replace(containerId, fragment)
        .addToBackStack(backStackName)
        .commit()
}

inline fun  FragmentActivity.getCurrentFragment(): Fragment? {
    return supportFragmentManager.findFragmentById(R.id.fragmentContainer)
}