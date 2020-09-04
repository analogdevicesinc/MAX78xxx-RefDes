package com.maximintegrated.bluetooth.view

import android.content.Context
import android.util.AttributeSet
import android.view.Gravity
import android.widget.Button
import android.widget.ImageView
import android.widget.LinearLayout
import android.widget.TextView
import com.maximintegrated.bluetooth.R

class ErrorView @JvmOverloads constructor(
        context: Context, attrs: AttributeSet? = null, defStyleAttr: Int = 0
) : LinearLayout(context, attrs, defStyleAttr) {

    val errorIcon: ImageView
    val errorText: TextView
    val errorButton: Button

    var errorModel: ErrorModel? = null
        set(value) {
            render(value)
        }

    init {
        inflate(context, R.layout.view_error, this)

        errorIcon = findViewById(R.id.error_view_icon)
        errorText = findViewById(R.id.error_view_text)
        errorButton = findViewById(R.id.error_view_button)

        orientation = VERTICAL
        gravity = Gravity.CENTER
    }

    private fun render(errorModel: ErrorModel?) {
        errorModel?.let {
            errorIcon.setImageResource(it.iconRes)
            errorText.setText(it.textRes)
            errorButton.setText(it.buttonTextRes)
        }
    }
}

data class ErrorModel(val iconRes: Int, val textRes: Int, val buttonTextRes: Int)