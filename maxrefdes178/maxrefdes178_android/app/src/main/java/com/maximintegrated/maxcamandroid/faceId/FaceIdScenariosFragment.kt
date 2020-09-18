package com.maximintegrated.maxcamandroid.faceId


import androidx.appcompat.app.AlertDialog
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.view.isInvisible
import androidx.core.view.isVisible
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProviders
import androidx.lifecycle.observe
import androidx.recyclerview.widget.RecyclerView
import com.maximintegrated.maxcamandroid.MainActivity
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.exts.addFragment
import kotlinx.android.synthetic.main.activity_main.*
import kotlinx.android.synthetic.main.edit_text_alert_dialog.view.*
import kotlinx.android.synthetic.main.fragment_face_id_scenarios.*
import timber.log.Timber


/**
 * A simple [Fragment] subclass.
 */
class FaceIdScenariosFragment : Fragment(), ScenarioListener {

    companion object {
        fun newInstance() = FaceIdScenariosFragment()
    }

    private lateinit var faceIdViewModel: FaceIdViewModel
    private lateinit var scenarioAdapter: FaceIdScenarioAdapter

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_face_id_scenarios, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        (requireActivity() as? MainActivity)?.mainFab?.isInvisible = true
        faceIdViewModel = ViewModelProviders.of(requireActivity()).get(FaceIdViewModel::class.java)
        scenarioAdapter = FaceIdScenarioAdapter(this)
        recyclerView.adapter = scenarioAdapter
        faceIdViewModel.scenarios.observe(viewLifecycleOwner) {
            Timber.d(it.joinToString(", "))
            scenarioAdapter.scenarios = it
            emptyViews.isVisible = it.isEmpty()
            recyclerView.isVisible = it.isNotEmpty()
        }
        faceIdViewModel.getScenarioList()

        newScenarioFab.setOnClickListener {
            showNewScenarioDialog()
        }

        recyclerView.addOnScrollListener(object : RecyclerView.OnScrollListener() {
            override fun onScrolled(recyclerView: RecyclerView, dx: Int, dy: Int) {
                if (dy < 0 && !newScenarioFab.isShown)
                    newScenarioFab.show()
                else if (dy > 0 && newScenarioFab.isShown)
                    newScenarioFab.hide()
            }

        })
    }

    override fun onEditButtonClicked(scenario: Scenario) {
        requireActivity().addFragment(FaceIdInputParamsFragment.newInstance())
        faceIdViewModel.onScenarioSelectedWithEditOption(scenario)
    }

    override fun onSelectButtonClicked(scenario: Scenario) {
        requireActivity().addFragment(FaceIdUpdateDeviceFragment.newInstance())
        faceIdViewModel.onScenarioSelected(scenario)
    }

    override fun onDeleteButtonClicked(scenario: Scenario) {
        faceIdViewModel.onScenarioDeleted(scenario)
        newScenarioFab.show()
    }

    private fun showNewScenarioDialog() {
        val alert = AlertDialog.Builder(requireContext())
        val layout = layoutInflater.inflate(R.layout.edit_text_alert_dialog, null)
        alert.setView(layout)
        alert.setPositiveButton(getString(R.string.create)) { dialog, _ ->
            val name = layout.dialogEditText.text.toString()
            faceIdViewModel.addNewScenario(name)
            requireActivity().addFragment(FaceIdInputParamsFragment.newInstance())
            dialog.dismiss()
        }
        alert.show()
    }
}
