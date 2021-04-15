/*
 * Copyright (C) Maxim Integrated Products, Inc., All rights Reserved.
 *
 * This software is protected by copyright laws of the United States and
 * of foreign countries. This material may also be protected by patent laws
 * and technology transfer regulations of the United States and of foreign
 * countries. This software is furnished under a license agreement and/or a
 * nondisclosure agreement and may only be used or reproduced in accordance
 * with the terms of those agreements. Dissemination of this information to
 * any party or parties not specified in the license agreement and/or
 * nondisclosure agreement is expressly prohibited.
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 */

package com.maximintegrated.maxcamandroid.face

import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.text.InputFilter
import android.text.InputFilter.LengthFilter
import android.view.*
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.core.view.isInvisible
import androidx.core.view.isVisible
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProviders
import androidx.lifecycle.observe
import com.google.android.material.snackbar.Snackbar
import com.maximintegrated.maxcamandroid.MainActivity
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.exts.addFragment
import com.maximintegrated.maxcamandroid.utils.EventObserver
import com.theartofdev.edmodo.cropper.CropImage
import com.theartofdev.edmodo.cropper.CropImageView
import kotlinx.android.synthetic.main.app_bar_layout.*
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
            enableNextButton()
        }

        faceIdViewModel.personUpdatedEvent.observe(viewLifecycleOwner) {
            faceIdViewModel.persons?.let {
                personAdapter.submitList(null)
                personAdapter.submitList(it.value)
            }
            enableNextButton()
        }

        faceIdDemoButton.setOnClickListener {
            faceIdViewModel.onDemoFragmentRequested()
        }

        faceIdViewModel.goToDemoFragment.observe(viewLifecycleOwner, EventObserver {
            requireActivity().addFragment(DemoFragment.newInstance())
        })
        faceIdViewModel.personImageSelectedEvent.observe(viewLifecycleOwner, EventObserver {
            requireActivity().addFragment(ImageViewFragment.newInstance())
        })
        //THIS WARNING EVENT IS IMPORTANT
        //it reloads the page with correct data, workaround for now
        faceIdViewModel.warningEvent.observe(viewLifecycleOwner, EventObserver {
            Snackbar.make(view, it, Snackbar.LENGTH_SHORT).show()
            requireActivity().addFragment(DbEditFragment.newInstance(), animation = false)
        })
        updateNewPersonFabVisibility()

        newPersonFab.setOnClickListener {
            showNewPersonDialog()
        }

        (activity as MainActivity).toolbar.apply {
            menu.clear()
            inflateMenu(R.menu.info_menu)
            setOnMenuItemClickListener { item ->
                if (item.itemId == R.id.infoMenuItem) {
                    showNewPhotoHelpDialog()
                    return@setOnMenuItemClickListener true
                }
                return@setOnMenuItemClickListener false
            }
        }
    }

    private fun updateNewPersonFabVisibility() {
        if (faceIdViewModel.persons.value!!.size < DbModel.PERSON_LIMIT) {
            newPersonFab.isVisible = true
        } else {
            newPersonFab.isInvisible = true
        }
    }

    private fun showNewPersonDialog() {
        val alert = AlertDialog.Builder(requireContext())
        val layout = layoutInflater.inflate(R.layout.edit_text_alert_dialog, null)
        layout.dialogTitleTextView.text = requireContext().getString(R.string.new_person)
        layout.dialogEditText.hint = requireContext().getString(R.string.enter_a_person_name)
        val filterArray = arrayOfNulls<InputFilter>(2)
        filterArray[0] = LengthFilter(15)
        filterArray[1] = AlphaNumericInputFilter()
        layout.dialogEditText.filters = filterArray
        alert.setView(layout)
        alert.setPositiveButton(getString(R.string.create)) { dialog, _ ->
            val name = layout.dialogEditText.text.toString().trim()
            faceIdViewModel.createPerson(name)
            dialog.dismiss()
        }
        alert.show()
    }

    private fun showNewPhotoHelpDialog() {
        val alert = AlertDialog.Builder(requireContext())
        alert.setTitle(requireContext().getString(R.string.photo_help_title))
        alert.setMessage(requireContext().getString(R.string.photo_help_content))
        alert.setPositiveButton("OK", null)
        alert.show()
    }

    private fun enableNextButton() {
        var imageCount = 0
        faceIdViewModel.selectedDatabase?.persons?.forEach { personModel -> imageCount += personModel.imageCount }
        faceIdDemoButton.isEnabled = imageCount > 0
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

    override fun onAddImageClicked(tempImageFile: File) {
        faceIdViewModel.selectedDatabase?.persons?.forEach {
            if (it.images.contains(tempImageFile)) {
                faceIdViewModel.selectedPerson = it
            }
        }

        CropImage.activity()
            .setGuidelines(CropImageView.Guidelines.ON)
            .setAspectRatio(3, 4)
            .setMinCropWindowSize(480, 640)
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

    override fun onDestroyView() {
        super.onDestroyView()
        (activity as MainActivity).toolbar.apply {
            menu.clear()
        }
    }
}