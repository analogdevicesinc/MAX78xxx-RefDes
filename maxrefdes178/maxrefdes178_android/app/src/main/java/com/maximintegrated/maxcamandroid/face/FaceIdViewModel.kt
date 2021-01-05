package com.maximintegrated.maxcamandroid.face

import android.app.Application
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.viewModelScope
import com.chaquo.python.Python
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.utils.Event
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.io.File

class FaceIdViewModel(app: Application) : AndroidViewModel(app) {

    private val _databases = MutableLiveData<List<DbModel>>(emptyList())
    val databases: LiveData<List<DbModel>> = _databases


    private val _persons = MutableLiveData<List<PersonModel>>(emptyList())
    val persons: LiveData<List<PersonModel>> = _persons

    private val _databaseSelectedEvent = MutableLiveData<Event<Unit>>()
    val databaseSelectedEvent: LiveData<Event<Unit>> = _databaseSelectedEvent

    private val _personImageSelectedEvent = MutableLiveData<Event<Unit>>()
    val personImageSelectedEvent: LiveData<Event<Unit>> = _personImageSelectedEvent

    private val _personImageDeletedEvent = MutableLiveData<Event<Unit>>()
    val personImageDeletedEvent: LiveData<Event<Unit>> = _personImageDeletedEvent

    private val _warningEvent = MutableLiveData<Event<Int>>()
    val warningEvent: LiveData<Event<Int>> = _warningEvent

    private val _goToDemoFragment = MutableLiveData<Event<Unit>>()
    val goToDemoFragment: LiveData<Event<Unit>> = _goToDemoFragment

    private val _embeddingsFileEvent = MutableLiveData<Event<File>>()
    val embeddingsFileEvent: LiveData<Event<File>> = _embeddingsFileEvent

    var selectedDatabase: DbModel? = null
        private set
    var selectedPersonImage: File? = null
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

    fun getPersonList() {
        //if not null
        selectedDatabase?.let {
            _persons.value = it.persons
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

    fun deletePersonImage(image: File) {
        selectedDatabase?.persons?.forEach {
            if (it.images.contains(image)) {
                it.images.remove(image)
                image.delete()
            }
        }
        _personImageDeletedEvent.value = Event(Unit)
    }

    fun selectPersonImage(image: File) {
        selectedPersonImage = image
        _personImageSelectedEvent.value = Event(Unit)
    }

    fun selectDatabase(db: DbModel) {
        selectedDatabase = db
        _databaseSelectedEvent.value = Event(Unit)
    }

    fun onDemoFragmentRequested() {
        _goToDemoFragment.value = Event(Unit)
    }

    fun getEmbeddingsFile() {
        selectedDatabase?.findEmbeddingsFile()
        selectedDatabase?.embeddingsFile?.let {
            _embeddingsFileEvent.value = Event(it)
        }
    }

    fun onCreateSignatureButtonClicked() {
        val python = Python.getInstance()
        val script = python.getModule("db_gen.generate_face_db")
        viewModelScope.launch {
            withContext(Dispatchers.IO) {
                selectedDatabase?.let {
                    script.callAttr("create_db", it.dbFolder.path, "embeddings")
                    /*val obj1 = script.callAttr("get_current_dir")
                    val obj2 = script.callAttr("get_file_dir", it.dbFile.path)
                    val obj3 = script.callAttr("get_include_dir")
                    Timber.d("obj1: $obj1")
                    Timber.d("obj2: $obj2")
                    Timber.d("obj3: $obj3")*/
                    /*
                        obj1: /data/user/0/com.maximintegrated.maxcamandroid/files/chaquopy/AssetFinder/app/db_gen
                        obj2: /storage/emulated/0/Android/data/com.maximintegrated.maxcamandroid/files/Databases/Database #1
                        obj3: /data/user/0/com.maximintegrated.maxcamandroid/files/chaquopy/AssetFinder/app/include
                    */
                }
            }
            getEmbeddingsFile()
        }
    }
}