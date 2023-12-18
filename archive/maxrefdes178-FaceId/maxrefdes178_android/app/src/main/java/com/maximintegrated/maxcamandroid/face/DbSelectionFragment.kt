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

import android.os.Bundle
import android.text.InputFilter
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.appcompat.app.AlertDialog
import androidx.core.view.isVisible
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProviders
import androidx.lifecycle.observe
import androidx.recyclerview.widget.RecyclerView
import com.google.android.material.snackbar.Snackbar
import com.maximintegrated.maxcamandroid.MainActivity
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.exts.addFragment
import com.maximintegrated.maxcamandroid.utils.EventObserver
import kotlinx.android.synthetic.main.edit_text_alert_dialog.view.*
import kotlinx.android.synthetic.main.fragment_db_selection.*

class DbSelectionFragment : Fragment(), DbListener {

    companion object {
        fun newInstance() = DbSelectionFragment()
    }

    private lateinit var faceIdViewModel: FaceIdViewModel
    private lateinit var dbAdapter: DbAdapter

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_db_selection, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        (requireActivity() as? MainActivity)?.changeVisibilityOfTheBottomView(false)
        faceIdViewModel = ViewModelProviders.of(requireActivity()).get(FaceIdViewModel::class.java)
        dbAdapter = DbAdapter(this)
        recyclerView.adapter = dbAdapter
        faceIdViewModel.databases.observe(viewLifecycleOwner) {
            dbAdapter.submitList(it)
            emptyViews.isVisible = it.isEmpty()
            recyclerView.isVisible = it.isNotEmpty()
        }
        faceIdViewModel.warningEvent.observe(viewLifecycleOwner, EventObserver {
            Snackbar.make(view, it, Snackbar.LENGTH_SHORT).show()
        })
        faceIdViewModel.databaseSelectedEvent.observe(viewLifecycleOwner, EventObserver {
            requireActivity().addFragment(DbEditFragment.newInstance())
        })
        faceIdViewModel.getDatabaseList()
        newDatabaseFab.setOnClickListener {
            showNewDatabaseDialog()
        }
        recyclerView.addOnScrollListener(object : RecyclerView.OnScrollListener() {
            override fun onScrolled(recyclerView: RecyclerView, dx: Int, dy: Int) {
                if (dy < 0 && !newDatabaseFab.isShown)
                    newDatabaseFab.show()
                else if (dy > 0 && newDatabaseFab.isShown)
                    newDatabaseFab.hide()
            }

        })
    }

    private fun showNewDatabaseDialog() {
        val alert = AlertDialog.Builder(requireContext())
        val layout = layoutInflater.inflate(R.layout.edit_text_alert_dialog, null)
        layout.dialogTitleTextView.text = requireContext().getString(R.string.new_database)
        layout.dialogEditText.hint = requireContext().getString(R.string.enter_a_database_name)
        val filterArray = arrayOfNulls<InputFilter>(2)
        filterArray[0] = InputFilter.LengthFilter(15)
        filterArray[1] = AlphaNumericInputFilter()
        layout.dialogEditText.filters = filterArray
        alert.setView(layout)
        alert.setPositiveButton(getString(R.string.create)) { dialog, _ ->
            val name = layout.dialogEditText.text.toString().trim()
            faceIdViewModel.createDatabase(name)
            dialog.dismiss()
        }
        alert.show()
    }


    override fun onRenameRequested(dbModel: DbModel, name: String) {
        faceIdViewModel.renameDatabase(dbModel, name)
    }

    override fun onDatabaseSelected(dbModel: DbModel) {
        faceIdViewModel.selectDatabase(dbModel)
    }

    override fun onDeleteButtonClicked(vararg model: Any) {
        if (model != null) {
            faceIdViewModel.deleteDatabase((model[0] as Array<*>)[0] as DbModel)
        }

    }
}