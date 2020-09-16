package com.maximintegrated.maxcamandroid.main

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.view.isInvisible
import androidx.core.view.isVisible
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProviders
import com.maximintegrated.communication.MaxCamViewModel
import com.maximintegrated.maxcamandroid.MainActivity
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.diagnostics.DiagnosticsFragment
import com.maximintegrated.maxcamandroid.exts.addFragment
import com.maximintegrated.maxcamandroid.faceId.FaceIdScenariosFragment
import com.maximintegrated.maxcamandroid.fileOperations.FileOperationsFragment
import kotlinx.android.synthetic.main.activity_main.*
import kotlinx.android.synthetic.main.fragment_main.*
import timber.log.Timber

class MainFragment : Fragment(), LandingPage {

    private lateinit var maxCamViewModel: MaxCamViewModel

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
        (requireActivity() as? MainActivity)?.mainFab?.isVisible = true
        maxCamViewModel = ViewModelProviders.of(requireActivity()).get(MaxCamViewModel::class.java)

        fileOperationsButton.setOnClickListener {
            requireActivity().addFragment(FileOperationsFragment.newInstance())
        }

        diagnosticsButton.setOnClickListener {
            requireActivity().addFragment(DiagnosticsFragment.newInstance())
        }

        faceIdButton.setOnClickListener {
            requireActivity().addFragment(FaceIdScenariosFragment.newInstance())
        }
    }
}