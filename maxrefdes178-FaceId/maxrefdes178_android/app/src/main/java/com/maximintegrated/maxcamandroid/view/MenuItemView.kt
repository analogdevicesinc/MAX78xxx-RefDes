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

package com.maximintegrated.maxcamandroid.view

import android.content.Context
import android.util.AttributeSet
import android.view.View
import androidx.core.content.ContextCompat
import com.google.android.material.card.MaterialCardView
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.exts.dpToPx
import com.maximintegrated.maxcamandroid.exts.setForegroundSelectable
import kotlinx.android.synthetic.main.menu_item.view.*

class MenuItemView @JvmOverloads constructor(
    context: Context, attrs: AttributeSet? = null, defStyleAttr: Int = 0
) : MaterialCardView(context, attrs, defStyleAttr) {


    init {
        View.inflate(context, R.layout.menu_item, this)

        val contentPadding = dpToPx(8).toInt()
        setContentPadding(contentPadding, contentPadding, contentPadding, contentPadding)
        setCardBackgroundColor(ContextCompat.getColorStateList(context, R.color.color_primary))

        isClickable = true
        isFocusable = true

        setForegroundSelectable()

        val attributes = context.obtainStyledAttributes(attrs, R.styleable.MenuItemView, defStyleAttr, 0)
        menuItemImageView.setImageDrawable(attributes.getDrawable(R.styleable.MenuItemView_image))
        menuItemTextView.text = attributes.getString(R.styleable.MenuItemView_text)
        attributes.recycle()
    }
}