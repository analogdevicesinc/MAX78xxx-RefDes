package com.maximintegrated.maxcamandroid.face

import android.app.Application
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.viewModelScope
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.utils.Event
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.io.File

class FaceIdViewModel(app: Application) : AndroidViewModel(app) {

    private val _databases = MutableLiveData<List<DbModel>>(emptyList())
    val databases: LiveData<List<DbModel>> = _databases

    private val _databaseSelectedEvent = MutableLiveData<Event<Unit>>()
    val databaseSelectedEvent: LiveData<Event<Unit>> = _databaseSelectedEvent

    private val _warningEvent = MutableLiveData<Event<Int>>()
    val warningEvent: LiveData<Event<Int>> = _warningEvent

    var selectedDatabase: DbModel? = null
        private set

    private var dbRootFolder: File? = null

    init {
        val extFilesDir = app.getExternalFilesDir(null)
        extFilesDir?.let {
            dbRootFolder = File(it, DB_FOLDER_NAME).also { folder ->
                if (!folder.exists()) {
                    folder.mkdirs()
                }
            }
        }
    }

    fun getDatabaseList() {
        selectedDatabase = null
        viewModelScope.launch {
            _databases.value = internalGetDatabaseList()
        }
    }

    private suspend fun internalGetDatabaseList(): List<DbModel> {
        return withContext(Dispatchers.IO) {
            val list: ArrayList<DbModel> = arrayListOf()
            val databases = dbRootFolder?.listFiles()?.toList() ?: emptyList()
            for (db in databases) {
                if (db.isDirectory) {
                    list.add(DbModel(db))
                }
            }
            list
        }
    }

    fun createDatabase(name: String) {
        dbRootFolder?.let {
            val database = File(it, name)
            if (!database.exists()) {
                database.mkdirs()
                val list = _databases.value?.toMutableList()
                val model = DbModel(database)
                list?.add(model)
                _databases.value = list
                selectDatabase(model)
            } else {
                _warningEvent.value = Event(R.string.database_name_exists)
            }
        }
    }

    fun deleteDatabase(db: DbModel) {
        val list = _databases.value?.toMutableList()
        db.delete()
        list?.remove(db)
        _databases.value = list
        _warningEvent.value = Event(R.string.database_is_deleted)
    }

    fun renameDatabase(db: DbModel, name: String) {
        viewModelScope.launch {
            dbRootFolder?.let {
                val file = File(dbRootFolder, name)
                if (file.exists()) {
                    _warningEvent.value = Event(R.string.database_name_exists)
                } else {
                    val list = _databases.value?.toMutableList()
                    val index = list?.remove(db)
                    _databases.value = list
                    withContext(Dispatchers.IO) {
                        db.rename(file)
                    }
                    list?.add(db)
                    _databases.value = list
                    _warningEvent.value = Event(R.string.database_is_renamed)
                }
            }
        }
    }

    fun selectDatabase(db: DbModel) {
        selectedDatabase = db
        _databaseSelectedEvent.value = Event(Unit)
    }
}