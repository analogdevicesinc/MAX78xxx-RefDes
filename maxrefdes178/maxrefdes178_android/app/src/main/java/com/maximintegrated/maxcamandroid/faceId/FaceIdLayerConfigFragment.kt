package com.maximintegrated.maxcamandroid.faceId


import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.AdapterView
import android.widget.ArrayAdapter
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProviders
import androidx.lifecycle.observe
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.exts.addFragment
import kotlinx.android.synthetic.main.fragment_face_id_layer_config.*

/**
 * A simple [Fragment] subclass.
 */
class FaceIdLayerConfigFragment : Fragment() {

    companion object {
        fun newInstance() = FaceIdLayerConfigFragment()
    }

    private lateinit var faceIdViewModel: FaceIdViewModel

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_face_id_layer_config, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        faceIdViewModel = ViewModelProviders.of(requireActivity()).get(FaceIdViewModel::class.java)
        initializeViews()
        faceIdViewModel.configFiles.observe(viewLifecycleOwner) { configs ->
            val list = configs.map { it.first.name }
            val adapter = ArrayAdapter(requireContext(), android.R.layout.simple_spinner_item, list)
            adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
            configurationSpinner.adapter = adapter
            if (list.isNotEmpty()) {
                faceIdViewModel.selectedScenario?.let {
                    if (list.contains(it.configFileName)) {
                        configurationSpinner.setSelection(list.indexOf(it.configFileName))
                    }
                }
            }
        }
        faceIdViewModel.getConfigFiles()
    }

    private fun initializeViews() {
        stepperView.onBackButtonClicked {
            requireActivity().onBackPressed()
        }
        stepperView.onNextButtonClicked {
            requireActivity().addFragment(FaceIdOutputParamsFragment.newInstance())
        }

        configurationSpinner.onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
            override fun onNothingSelected(parent: AdapterView<*>?) {

            }

            override fun onItemSelected(
                parent: AdapterView<*>?,
                view: View?,
                position: Int,
                id: Long
            ) {
                val item = faceIdViewModel.configFiles.value?.get(position)
                faceIdViewModel.selectedScenario?.configFileName = item?.first?.name ?: ""
                configurationDetailsTextView.text = item?.second
            }
        }
    }
}
