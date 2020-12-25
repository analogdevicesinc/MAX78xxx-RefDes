package com.maximintegrated.maxcamandroid.face

import android.os.Bundle
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
        alert.setView(layout)
        alert.setPositiveButton(getString(R.string.create)) { dialog, _ ->
            val name = layout.dialogEditText.text.toString().trim()
            faceIdViewModel.createDatabase(name)
            dialog.dismiss()
        }
        alert.show()
    }

    override fun onDeleteButtonClicked(dbModel: DbModel) {
        faceIdViewModel.deleteDatabase(dbModel)
    }

    override fun onRenameRequested(dbModel: DbModel, name: String) {
        faceIdViewModel.renameDatabase(dbModel, name)
    }

    override fun onDatabaseSelected(dbModel: DbModel) {
        faceIdViewModel.selectDatabase(dbModel)
    }
}