package com.maximintegrated.maxcamandroid.face

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProviders
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.exts.addFragment
import com.maximintegrated.maxcamandroid.utils.EventObserver
import kotlinx.android.synthetic.main.fragment_db_edit.*

class DbEditFragment : Fragment() {

    companion object {
        fun newInstance() = DbEditFragment()
    }

    private lateinit var faceIdViewModel: FaceIdViewModel

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_db_edit, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        faceIdViewModel = ViewModelProviders.of(requireActivity()).get(FaceIdViewModel::class.java)
        stepperView.onBackButtonClicked {
            requireActivity().onBackPressed()
        }
        stepperView.onNextButtonClicked {
            faceIdViewModel.onDemoFragmentRequested()
        }
        faceIdViewModel.goToDemoFragment.observe(viewLifecycleOwner, EventObserver {
            requireActivity().addFragment(DemoFragment.newInstance())
        })
    }
}