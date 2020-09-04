package com.maximintegrated.bluetooth.devicelist

import android.widget.TextView
import com.airbnb.epoxy.EpoxyModel
import com.airbnb.epoxy.EpoxyModelClass
import com.maximintegrated.bluetooth.R

@EpoxyModelClass
abstract class PairedDeviceListHeaderModel : EpoxyModel<TextView>() {

    override fun getDefaultLayout() = R.layout.model_paired_device_list_header

}