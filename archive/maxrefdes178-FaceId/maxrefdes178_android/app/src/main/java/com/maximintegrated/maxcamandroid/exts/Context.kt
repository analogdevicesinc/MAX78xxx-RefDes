/*
 * Copyright (C) Maxim Integrated Products, Inc., All rights Reserved.
 *
 * This software is protected by copyright laws of the United States and
 * of foreign countries. This material may also be protected by patent laws
 * and technology transfer regulations of the United States and of foreign
 * countries. This software is furnished under a license agreement and/or a
 * nondisclosure agreement and may only be used or reproduced in accordance
 * with the terms of those agreements. Dissemination of this information to
 * any party or parties not specified in the license agreement and/or
 * nondisclosure agreement is expressly prohibited.
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 */

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

inline fun FragmentActivity.addFragment(fragment: Fragment, @IdRes containerId: Int = R.id.fragmentContainer, backStackName: String? = null, animation: Boolean = true) {
    supportFragmentManager.beginTransaction().apply {
        if (animation) {
            setCustomAnimations(
                R.anim.bottom_sheet_slide_in,
                R.anim.bottom_sheet_slide_out,
                R.anim.bottom_sheet_slide_in,
                R.anim.bottom_sheet_slide_out
            )
        }
    }
        .addToBackStack(backStackName)
        .replace(containerId, fragment)
        .commit()
    if (!animation) {
        supportFragmentManager.popBackStack()
    }
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