package com.maximintegrated.maxcamandroid.main

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProviders
import androidx.lifecycle.observe
import com.maximintegrated.communication.MaxCamViewModel
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.view.BleConnectionInfo
import kotlinx.android.synthetic.main.app_bar_layout.*

class MainFragment : Fragment(), LandingPage {

    private lateinit var maxCamViewModel: MaxCamViewModel

    companion object {
        fun newInstance(): MainFragment{
            return MainFragment()
        }
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        return inflater.inflate(R.layout.fragment_main, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        maxCamViewModel = ViewModelProviders.of(requireActivity()).get(MaxCamViewModel::class.java)

        maxCamViewModel.connectionState
            .observe(this) { connectionState ->
                val device = maxCamViewModel.bluetoothDevice
                toolbar.connectionInfo = if (device != null) {
                    BleConnectionInfo(connectionState, device.name, device.address)
                } else {
                    null
                }
            }

        toolbar.pageTitle = "MAXCAM AI85"
    }
}