package com.maximintegrated.maxcamandroid.faceId

import android.view.LayoutInflater
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.google.android.material.button.MaterialButton
import com.maximintegrated.maxcamandroid.R
import kotlinx.android.synthetic.main.scenario_item.view.*

class FaceIdScenarioAdapter(private val listener: ScenarioListener) :
    RecyclerView.Adapter<ScenarioViewHolder>() {

    var scenarios: List<Scenario> = emptyList()
        set(value) {
            field = value
            notifyDataSetChanged()
        }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ScenarioViewHolder {
        return ScenarioViewHolder(parent)
    }

    override fun getItemCount(): Int {
        return scenarios.size
    }

    override fun onBindViewHolder(holder: ScenarioViewHolder, position: Int) {
        val scenario = scenarios[position]
        holder.bind(scenario, listener)
    }

}


class ScenarioViewHolder(parent: ViewGroup) : RecyclerView.ViewHolder(
    LayoutInflater.from(parent.context).inflate(
        R.layout.scenario_item, parent, false
    )
) {

    private val titleTextView: TextView by lazy { itemView.scenarioTitleTextView }
    private val sourceTextView: TextView by lazy { itemView.sourceTextView }
    private val outputTextView: TextView by lazy { itemView.outputTextView }
    private val sizeTextView: TextView by lazy { itemView.sizeTextView }
    private val configTextView: TextView by lazy { itemView.configurationTextView }
    private val editButton: MaterialButton by lazy { itemView.editButton }
    private val selectButton: MaterialButton by lazy { itemView.selectButton }

    fun bind(scenario: Scenario, listener: ScenarioListener) {
        titleTextView.text = scenario.name
        sourceTextView.text = scenario.imageSource.toString()
        outputTextView.text = scenario.imageOutput.toString()
        sizeTextView.text = scenario.imageSize.toString()
        configTextView.text = scenario.configFileName
        editButton.setOnClickListener {
            listener.onEditButtonClicked(scenario)
        }
        selectButton.setOnClickListener {
            listener.onSelectButtonClicked(scenario)
        }
    }
}

interface ScenarioListener {
    fun onEditButtonClicked(scenario: Scenario)
    fun onSelectButtonClicked(scenario: Scenario)
}