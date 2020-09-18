package com.maximintegrated.maxcamandroid.faceId

import android.app.Application
import android.os.Environment
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.viewModelScope
import com.google.gson.Gson
import com.maximintegrated.maxcamandroid.exts.getMaxCamScenarioFile
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.io.File

class FaceIdViewModel(app: Application) : AndroidViewModel(app) {

    companion object {
        const val SCENARIO_FOLDER_NAME = "/MAXCAM/Scenarios"
        const val CONFIG_FOLDER_NAME = "/MAXCAM/Configs"
    }

    private val _scenarios = MutableLiveData<List<Scenario>>(emptyList())
    val scenarios: LiveData<List<Scenario>>
        get() = _scenarios

    var selectedScenario: Scenario? = null

    private val gson = Gson()

    private val _configFiles = MutableLiveData<List<Pair<File, String>>>(emptyList())
    val configFiles: LiveData<List<Pair<File, String>>>
        get() = _configFiles

    fun getScenarioList() {
        viewModelScope.launch {
            _scenarios.value = internalGetScenarioList()
        }
    }

    var editScenario = false
        private set

    private suspend fun internalGetScenarioList(): List<Scenario> {
        return withContext(Dispatchers.IO) {
            val list: ArrayList<Scenario> = arrayListOf()
            val directory = File(Environment.getExternalStorageDirectory(), SCENARIO_FOLDER_NAME)
            if (!directory.exists()) {
                directory.mkdirs()
            }
            val files = directory.listFiles()?.toList() ?: emptyList()
            for (file in files) {
                val json = file.readText()
                val scenario = gson.fromJson<Scenario>(json, Scenario::class.java)
                if (scenario != null) {
                    scenario.name = file.nameWithoutExtension
                    list.add(scenario)
                }
            }
            list
        }
    }

    fun addNewScenario(scenarioName: String) {
        val scenario = Scenario(scenarioName)
        val list = _scenarios.value?.toMutableList()
        list?.add(scenario)
        _scenarios.value = list
        onScenarioSelected(scenario)
        saveScenario()
    }

    fun saveScenario() {
        selectedScenario?.let {
            viewModelScope.launch {
                internalSaveScenario(it)
            }
        }
    }


    private suspend fun internalSaveScenario(scenario: Scenario) {
        withContext(Dispatchers.IO) {
            val json = gson.toJson(scenario)
            val file = getMaxCamScenarioFile(scenario.name + ".json")
            file.parentFile?.mkdirs()
            file.writeText(json)
        }
    }

    fun onScenarioSelected(scenario: Scenario) {
        selectedScenario = scenario
        editScenario = false
    }

    fun onScenarioSelectedWithEditOption(scenario: Scenario) {
        selectedScenario = scenario
        editScenario = true
    }

    fun onScenarioDeleted(scenario: Scenario) {
        getMaxCamScenarioFile(scenario.name + ".json").delete()
        val list = (_scenarios.value ?: emptyList()).toMutableList()
        list.remove(scenario)
        _scenarios.value = list
    }

    fun getConfigFiles() {
        viewModelScope.launch {
            _configFiles.value = internalGetConfigFiles()
        }
    }

    private suspend fun internalGetConfigFiles(): List<Pair<File, String>> {
        return withContext(Dispatchers.IO) {
            val list: ArrayList<Pair<File, String>> = arrayListOf()
            val directory = File(Environment.getExternalStorageDirectory(), CONFIG_FOLDER_NAME)
            if (!directory.exists()) {
                directory.mkdirs()
            }
            val files = directory.listFiles()?.toList() ?: emptyList()
            for (file in files) {
                val text = file.readText()
                if (text != "") {
                    list.add(Pair(file, text))
                }
            }
            list
        }
    }
}