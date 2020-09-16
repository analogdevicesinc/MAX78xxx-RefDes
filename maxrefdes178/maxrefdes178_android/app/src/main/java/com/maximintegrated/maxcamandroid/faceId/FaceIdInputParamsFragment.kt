package com.maximintegrated.maxcamandroid.faceId


import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.AdapterView
import android.widget.ArrayAdapter
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProviders
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.exts.addFragment
import kotlinx.android.synthetic.main.fragment_face_id_input_params.*

/**
 * A simple [Fragment] subclass.
 */
class FaceIdInputParamsFragment : Fragment() {

    companion object {
        fun newInstance() = FaceIdInputParamsFragment()
    }

    private lateinit var faceIdViewModel: FaceIdViewModel

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_face_id_input_params, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        faceIdViewModel = ViewModelProviders.of(requireActivity()).get(FaceIdViewModel::class.java)
        initializeViews()
    }

    private fun initializeViews() {
        stepperView.onNextButtonClicked {
            requireActivity().addFragment(FaceIdLayerConfigFragment.newInstance())
        }

        ArrayAdapter.createFromResource(
            requireContext(),
            R.array.image_sources,
            android.R.layout.simple_spinner_item
        ).also { adapter ->
            adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
            imageSourceSpinner.adapter = adapter
        }
        ArrayAdapter.createFromResource(
            requireContext(),
            R.array.image_sizes,
            android.R.layout.simple_spinner_item
        ).also { adapter ->
            adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
            imageSizeSpinner.adapter = adapter
        }
        imageSourceSpinner.onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
            override fun onNothingSelected(parent: AdapterView<*>?) {

            }

            override fun onItemSelected(
                parent: AdapterView<*>?,
                view: View?,
                position: Int,
                id: Long
            ) {
                faceIdViewModel.selectedScenario?.imageSource = ImageSource.values()[position]
            }
        }
        imageSizeSpinner.onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
            override fun onNothingSelected(parent: AdapterView<*>?) {

            }

            override fun onItemSelected(
                parent: AdapterView<*>?,
                view: View?,
                position: Int,
                id: Long
            ) {
                faceIdViewModel.selectedScenario?.imageSize = ImageSize.values()[position]
            }
        }
        faceIdViewModel.selectedScenario?.let {
            imageSourceSpinner.setSelection(it.imageSource.ordinal)
            imageSizeSpinner.setSelection(it.imageSize.ordinal)
        }
    }
}
