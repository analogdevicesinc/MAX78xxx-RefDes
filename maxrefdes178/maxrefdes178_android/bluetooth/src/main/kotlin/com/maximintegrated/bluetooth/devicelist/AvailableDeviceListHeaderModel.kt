package com.maximintegrated.bluetooth.devicelist

import android.view.View
import android.widget.ProgressBar
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyHolder
import com.airbnb.epoxy.EpoxyModelClass
import com.airbnb.epoxy.EpoxyModelWithHolder
import com.maximintegrated.bluetooth.R
import com.maximintegrated.bluetooth.devicelist.AvailableDeviceListHeaderModel.BluetoothDeviceListHeaderHolder

@EpoxyModelClass
abstract class AvailableDeviceListHeaderModel :
        EpoxyModelWithHolder<BluetoothDeviceListHeaderHolder>() {

    @EpoxyAttribute var discovering = false

    override fun getDefaultLayout() = R.layout.model_available_device_list_header

    override fun bind(holder: BluetoothDeviceListHeaderHolder) {
        super.bind(holder)

        holder.progressView.visibility = if (discovering) View.VISIBLE else View.GONE
    }

    inner class BluetoothDeviceListHeaderHolder : EpoxyHolder() {
        lateinit var progressView: ProgressBar

        override fun bindView(view: View) {
            progressView = view.findViewById(R.id.progress)
        }
    }
}