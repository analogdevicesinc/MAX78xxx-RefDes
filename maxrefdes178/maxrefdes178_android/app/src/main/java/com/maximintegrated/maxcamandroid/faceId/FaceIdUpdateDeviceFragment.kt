package com.maximintegrated.maxcamandroid.faceId


import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.view.isVisible
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.ViewModelProviders
import com.maximintegrated.maxcamandroid.MainViewModel
import com.maximintegrated.maxcamandroid.MainViewModelFactory
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.exts.addFragment
import com.maximintegrated.maxcamandroid.nativeLibrary.MaxCamNativeLibrary
import kotlinx.android.synthetic.main.fragment_face_id_update_device.*

/**
 * A simple [Fragment] subclass.
 */
class FaceIdUpdateDeviceFragment : Fragment() {

    companion object {
        fun newInstance() = FaceIdUpdateDeviceFragment()
    }

    private lateinit var mainViewModel: MainViewModel
    private lateinit var faceIdViewModel: FaceIdViewModel

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_face_id_update_device, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        faceIdViewModel = ViewModelProviders.of(requireActivity()).get(FaceIdViewModel::class.java)

        mainViewModel =
            ViewModelProvider(
                requireActivity(),
                MainViewModelFactory(
                    requireActivity().application,
                    MaxCamNativeLibrary.getInstance()
                )
            ).get(MainViewModel::class.java)
        initializeViews()
    }

    private fun initializeViews() {

        saveScenarioButton.isVisible = faceIdViewModel.editScenario
        saveScenarioButton.setOnClickListener {
            faceIdViewModel.saveScenario()
        }
        stepperView.onBackButtonClicked {
            requireActivity().onBackPressed()
        }
        stepperView.onNextButtonClicked {
            requireActivity().addFragment(FaceIdDemoFragment.newInstance())
        }
        faceIdViewModel.selectedScenario?.let {
            parametersTextView.text = it.toString()
        }
        updateDeviceButton.setOnClickListener {
            mainViewModel.onUpdateDeviceButtonClicked()
        }
    }
}
