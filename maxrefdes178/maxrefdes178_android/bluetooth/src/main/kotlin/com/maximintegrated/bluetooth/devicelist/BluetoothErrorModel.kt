package com.maximintegrated.bluetooth.devicelist

import android.view.View
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModel
import com.airbnb.epoxy.EpoxyModelClass
import com.maximintegrated.bluetooth.R
import com.maximintegrated.bluetooth.view.ErrorModel
import com.maximintegrated.bluetooth.view.ErrorView

@EpoxyModelClass
abstract class BluetoothErrorModel : EpoxyModel<ErrorView>() {

    @EpoxyAttribute lateinit var errorModel: ErrorModel
    @EpoxyAttribute(hash = false) var errorButtonClickListener: View.OnClickListener? = null

    override fun getDefaultLayout() = R.layout.model_bluetooth_error

    override fun bind(view: ErrorView) {
        view.errorModel = errorModel
        view.errorButton.setOnClickListener(errorButtonClickListener)
    }
}