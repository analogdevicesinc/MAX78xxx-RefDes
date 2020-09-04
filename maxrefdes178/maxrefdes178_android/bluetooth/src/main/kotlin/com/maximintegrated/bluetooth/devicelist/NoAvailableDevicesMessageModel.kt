package com.maximintegrated.bluetooth.devicelist

import android.widget.TextView
import com.airbnb.epoxy.EpoxyModel
import com.airbnb.epoxy.EpoxyModelClass
import com.maximintegrated.bluetooth.R

@EpoxyModelClass
abstract class NoAvailableDevicesMessageModel : EpoxyModel<TextView>() {

    override fun getDefaultLayout() = R.layout.model_no_available_devices_message

}