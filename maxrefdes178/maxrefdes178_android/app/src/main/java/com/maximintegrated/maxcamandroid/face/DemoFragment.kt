package com.maximintegrated.maxcamandroid.face

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.view.isInvisible
import androidx.core.view.isVisible
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProviders
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.utils.EventObserver
import kotlinx.android.synthetic.main.fragment_db_edit.stepperView
import kotlinx.android.synthetic.main.fragment_demo.*
import java.text.SimpleDateFormat
import java.util.*

class DemoFragment : Fragment() {

    companion object {
        fun newInstance() = DemoFragment()
    }

    private lateinit var faceIdViewModel: FaceIdViewModel

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_demo, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        faceIdViewModel = ViewModelProviders.of(requireActivity()).get(FaceIdViewModel::class.java)
        faceIdViewModel.embeddingsFileEvent.observe(viewLifecycleOwner, EventObserver {
            val sdf = SimpleDateFormat("MM/dd/yyyy HH:mm", Locale.getDefault())
            embeddingsTextView.text = it.name
            embeddingsDateTextView.text = sdf.format(Date(it.lastModified()))
            progressBar.isVisible = false
        })
        stepperView.onBackButtonClicked {
            requireActivity().onBackPressed()
        }
        stepperView.onNextButtonClicked {
            //faceIdViewModel.onDemoFragmentRequested()
        }
        signatureButton.setOnClickListener {
            progressBar.isVisible = true
            faceIdViewModel.onCreateSignatureButtonClicked()
        }
        faceIdViewModel.getEmbeddingsFile()
    }
}