package com.maximintegrated.maxcamandroid.exts

import android.util.TypedValue
import android.view.View
import com.google.android.material.card.MaterialCardView

inline fun View.dpToPx(dp: Int): Float = TypedValue.applyDimension(
    TypedValue.COMPLEX_UNIT_DIP, dp.toFloat(),
    context.resources.displayMetrics
)

inline fun MaterialCardView.setForegroundSelectable() {
    val typedValue = TypedValue()
    context.theme.resolveAttribute(android.R.attr.selectableItemBackground, typedValue, true)
    this.foreground = context.drawable(typedValue.resourceId)
}