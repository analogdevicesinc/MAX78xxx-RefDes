package com.maximintegrated.maxcamandroid.view

import android.content.Context
import android.util.AttributeSet
import android.view.View
import com.google.android.material.card.MaterialCardView
import com.maximintegrated.maxcamandroid.R
import kotlinx.android.synthetic.main.menu_item.view.*

class MenuItemView @JvmOverloads constructor(
    context: Context, attrs: AttributeSet? = null, defStyleAttr: Int = 0
) : MaterialCardView(context, attrs, defStyleAttr) {


    init {
        View.inflate(context, R.layout.menu_item, this)
        val attributes = context.obtainStyledAttributes(attrs, R.styleable.MenuItemView, defStyleAttr, 0)
        menuItemImageView.setImageDrawable(attributes.getDrawable(R.styleable.MenuItemView_image))
        menuItemTextView.text = attributes.getString(R.styleable.MenuItemView_text)
        attributes.recycle()
    }
}