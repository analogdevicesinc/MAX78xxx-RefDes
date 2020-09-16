package com.maximintegrated.maxcamandroid.view

import android.content.Context
import android.util.AttributeSet
import android.view.View
import com.google.android.material.card.MaterialCardView
import com.maximintegrated.maxcamandroid.R
import kotlinx.android.synthetic.main.horizontal_stepper_view.view.*

enum class Step {
    FIRST,
    SECOND,
    THIRD
}

class HorizontalStepperView @JvmOverloads constructor(
    context: Context, attrs: AttributeSet? = null, defStyleAttr: Int = 0
) : MaterialCardView(context, attrs, defStyleAttr) {

    var enableBackButton: Boolean = true
        set(value) {
            field = value
            backButton.isEnabled = value
        }

    var enableNextButton: Boolean = true
        set(value) {
            field = value
            nextButton.isEnabled = value
        }

    var backButtonText: String = context.getString(R.string.back)
        set(value) {
            field = value
            backButton.text = value
        }

    var nextButtonText: String = context.getString(R.string.next)
        set(value) {
            field = value
            nextButton.text = value
        }

    var enableProgress: Boolean = true
        set(value) {
            field = value
            progressBar.isEnabled = value
        }

    var firstStepNumber: Int = 1
        set(value) {
            field = value
            firstButton.text = value.toString()
        }

    var secondStepNumber: Int = 1
        set(value) {
            field = value
            secondButton.text = value.toString()
        }

    var thirdStepNumber: Int = 1
        set(value) {
            field = value
            thirdButton.text = value.toString()
        }

    var firstStepDescription: String = ""
        set(value) {
            field = value
            firstTextView.text = value
        }

    var secondStepDescription: String = ""
        set(value) {
            field = value
            secondTextView.text = value
        }

    var thirdStepDescription: String = ""
        set(value) {
            field = value
            thirdTextView.text = value
        }

    /*@ColorRes
    var color: Int = R.color.outlined_button_background
        set(value) {
            field = value
            firstButton.background.setTint(context.color(value))
            secondButton.background.setTint(context.color(value))
            thirdButton.background.setTint(context.color(value))
            connectionView1.setBackgroundColor(context.color(value))
            connectionView2.setBackgroundColor(context.color(value))
        }*/

    var progress: Int = 0
        set(value) {
            field = value
            progressBar.progress = value
        }

    var currentStep: Step = Step.FIRST
        set(value) {
            field = value
            when (value) {
                Step.FIRST -> {
                    firstButton.isEnabled = true
                    secondButton.isEnabled = false
                    thirdButton.isEnabled = false
                }
                Step.SECOND -> {
                    firstButton.isEnabled = false
                    secondButton.isEnabled = true
                    thirdButton.isEnabled = false
                }
                Step.THIRD -> {
                    firstButton.isEnabled = false
                    secondButton.isEnabled = false
                    thirdButton.isEnabled = true
                }
            }
        }

    init {
        View.inflate(context, R.layout.horizontal_stepper_view, this)

        val attributes =
            context.obtainStyledAttributes(
                attrs,
                R.styleable.HorizontalStepperView,
                defStyleAttr,
                0
            )

        firstStepNumber =
            attributes.getInteger(R.styleable.HorizontalStepperView_firstStepNumber, 1)
        secondStepNumber =
            attributes.getInteger(R.styleable.HorizontalStepperView_secondStepNumber, 2)
        thirdStepNumber =
            attributes.getInteger(R.styleable.HorizontalStepperView_thirdStepNumber, 3)
        firstStepDescription =
            attributes.getString(R.styleable.HorizontalStepperView_firstStepDescription) ?: ""
        secondStepDescription =
            attributes.getString(R.styleable.HorizontalStepperView_secondStepDescription) ?: ""
        thirdStepDescription =
            attributes.getString(R.styleable.HorizontalStepperView_thirdStepDescription) ?: ""
        progress = attributes.getInteger(R.styleable.HorizontalStepperView_stepperProgress, 0)
        enableProgress =
            attributes.getBoolean(R.styleable.HorizontalStepperView_enableProgress, true)
        enableBackButton =
            attributes.getBoolean(R.styleable.HorizontalStepperView_enableBackButton, true)
        enableNextButton =
            attributes.getBoolean(R.styleable.HorizontalStepperView_enableNextButton, true)
        backButtonText =
            attributes.getString(R.styleable.HorizontalStepperView_backButtonText)
                ?: context.getString(R.string.back)
        nextButtonText =
            attributes.getString(R.styleable.HorizontalStepperView_nextButtonText)
                ?: context.getString(R.string.next)
        val step = attributes.getInteger(R.styleable.HorizontalStepperView_currentStep, 0)
        currentStep = Step.values()[step]
        attributes.recycle()
    }

    fun onBackButtonClicked(function: () -> Unit) {
        backButton.setOnClickListener {
            function()
        }
    }

    fun onNextButtonClicked(function: () -> Unit) {
        nextButton.setOnClickListener {
            function()
        }
    }
}