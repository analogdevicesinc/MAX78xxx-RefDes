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