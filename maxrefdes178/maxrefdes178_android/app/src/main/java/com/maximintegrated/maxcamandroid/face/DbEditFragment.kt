package com.maximintegrated.maxcamandroid.face

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.view.isVisible
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProviders
import com.google.android.material.snackbar.Snackbar
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.exts.addFragment
import com.maximintegrated.maxcamandroid.utils.EventObserver
import kotlinx.android.synthetic.main.fragment_db_edit.*
import java.io.File

class DbEditFragment : Fragment(), PersonListener {

    companion object {
        fun newInstance() = DbEditFragment()
    }

    private lateinit var faceIdViewModel: FaceIdViewModel
    private lateinit var personAdapter: PersonAdapter

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
        personAdapter = PersonAdapter(this)
        recyclerView.adapter = personAdapter
        faceIdViewModel.getPersonList()
        faceIdViewModel.persons.observe(viewLifecycleOwner) {
            personAdapter.submitList(it)
            emptyViews.isVisible = it.isEmpty()
            recyclerView.isVisible = it.isNotEmpty()
        }

        stepperView.onBackButtonClicked {
            requireActivity().onBackPressed()
        }
        stepperView.onNextButtonClicked {
            faceIdViewModel.onDemoFragmentRequested()
        }
        faceIdViewModel.goToDemoFragment.observe(viewLifecycleOwner, EventObserver {
            requireActivity().addFragment(DemoFragment.newInstance())
        })
        faceIdViewModel.personImageSelectedEvent.observe(viewLifecycleOwner, EventObserver {
            requireActivity().addFragment(ImageViewFragment.newInstance())
        })
        faceIdViewModel.warningEvent.observe(viewLifecycleOwner, EventObserver {
            Snackbar.make(view, it, Snackbar.LENGTH_SHORT).show()
        })
    }

    override fun onRenameRequested(personModel: PersonModel, name: String) {
        faceIdViewModel.renamePerson(personModel, name)
    }

    override fun onDatabaseSelected(personModel: PersonModel) {
        TODO("Not yet implemented")
    }

    override fun onImageClicked(imageFile: File, person: PersonModel) {
        faceIdViewModel.selectPersonImage(imageFile, person)
    }

    override fun onAddImageClicked(person: PersonModel) {
        TODO("Not yet implemented")
    }
}