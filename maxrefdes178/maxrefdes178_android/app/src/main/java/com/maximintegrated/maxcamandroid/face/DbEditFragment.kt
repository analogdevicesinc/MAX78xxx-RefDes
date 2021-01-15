package com.maximintegrated.maxcamandroid.face

import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.core.view.isVisible
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProviders
import com.google.android.material.snackbar.Snackbar
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.exts.addFragment
import com.maximintegrated.maxcamandroid.utils.EventObserver
import com.theartofdev.edmodo.cropper.CropImage
import com.theartofdev.edmodo.cropper.CropImageView
import kotlinx.android.synthetic.main.edit_text_alert_dialog.view.*
import kotlinx.android.synthetic.main.fragment_db_edit.*
import timber.log.Timber
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
            updateNewPersonFabVisibility()
        }

        faceIdViewModel.personUpdatedEvent.observe(viewLifecycleOwner) {
            personAdapter.notifyDataSetChanged()
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
        updateNewPersonFabVisibility()

        newPersonFab.setOnClickListener {
            showNewPersonDialog()
        }
    }

    private fun updateNewPersonFabVisibility() {
        if (faceIdViewModel.persons.value!!.size < DbModel.PERSON_LIMIT) {
            newPersonFab.visibility = View.VISIBLE
        } else {
            newPersonFab.visibility = View.GONE
        }
    }

    private fun showNewPersonDialog() {
        val alert = AlertDialog.Builder(requireContext())
        val layout = layoutInflater.inflate(R.layout.edit_text_alert_dialog, null)
        layout.dialogTitleTextView.text = requireContext().getString(R.string.new_person)
        layout.dialogEditText.hint = requireContext().getString(R.string.enter_a_person_name)
        alert.setView(layout)
        alert.setPositiveButton(getString(R.string.create)) { dialog, _ ->
            val name = layout.dialogEditText.text.toString().trim()
            faceIdViewModel.createPerson(name)
            dialog.dismiss()
        }
        alert.show()
    }

    override fun onRenameRequested(personModel: PersonModel, name: String) {
        faceIdViewModel.renamePerson(personModel, name)
    }

    override fun onPersonDeleteRequested(personModel: PersonModel) {
        TODO("Not yet implemented")
    }

    override fun onImageClicked(imageFile: File, person: PersonModel) {
        faceIdViewModel.selectPersonImage(imageFile, person)
    }

    override fun onAddImageClicked(person: PersonModel) {
        faceIdViewModel.selectedPerson = person

        CropImage.activity()
            .setGuidelines(CropImageView.Guidelines.ON)
            .setAspectRatio(3, 4)
            .setMinCropWindowSize(120, 160)
            .setRequestedSize(480, 640)
            .start(requireContext(), this)
    }

    override fun onDeleteButtonClicked(vararg model: Any) {
        faceIdViewModel.deletePerson((model[0] as Array<*>)[0] as PersonModel)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == CropImage.CROP_IMAGE_ACTIVITY_REQUEST_CODE) {
            val result = CropImage.getActivityResult(data)
            if (resultCode == Activity.RESULT_OK && data != null) {
                faceIdViewModel.onImageAdded(result!!.uri)
                Toast.makeText(
                    requireContext(),
                    "Cropping successful!",
                    Toast.LENGTH_SHORT
                ).show()
            } else if (resultCode == CropImage.CROP_IMAGE_ACTIVITY_RESULT_ERROR_CODE) {
                result?.let { Timber.e(it.error) }
            }
        }
    }
}