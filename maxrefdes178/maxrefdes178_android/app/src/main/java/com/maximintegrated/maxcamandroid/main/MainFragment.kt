package com.maximintegrated.maxcamandroid.main

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProviders
import com.maximintegrated.communication.MaxCamViewModel
import com.maximintegrated.maxcamandroid.MainActivity
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.exts.addFragment
import com.maximintegrated.maxcamandroid.face.DbSelectionFragment
import com.maximintegrated.maxcamandroid.settings.SettingsFragment
import kotlinx.android.synthetic.main.fragment_main.*

class MainFragment : Fragment(), LandingPage {

    companion object {
        fun newInstance(): MainFragment {
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
        (requireActivity() as? MainActivity)?.changeVisibilityOfTheBottomView(true)

        faceIdButton.setOnClickListener {
            requireActivity().addFragment(DbSelectionFragment.newInstance())
        }
        settingsButton.setOnClickListener {
            requireActivity().addFragment(SettingsFragment.newInstance())
        }
    }
}