package com.maximintegrated.maxcamandroid.face

import android.app.Application
import android.graphics.Bitmap
import android.net.Uri
import android.provider.MediaStore
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
import timber.log.Timber
import java.io.File
import java.io.FileOutputStream

class FaceIdViewModel(private val app: Application) : AndroidViewModel(app) {

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

    private val _personUpdatedEvent = MutableLiveData<Event<Unit>>()
    val personUpdatedEvent: LiveData<Event<Unit>> = _personUpdatedEvent

    private val _warningEvent = MutableLiveData<Event<Int>>()
    val warningEvent: LiveData<Event<Int>> = _warningEvent

    private val _goToDemoFragment = MutableLiveData<Event<Unit>>()
    val goToDemoFragment: LiveData<Event<Unit>> = _goToDemoFragment

    private val _embeddingsFileEvent = MutableLiveData<Event<File>>()
    val embeddingsFileEvent: LiveData<Event<File>> = _embeddingsFileEvent

    private val _scriptInProgress = MutableLiveData<Boolean>(false)
    val scriptInProgress: LiveData<Boolean> = _scriptInProgress

    var selectedDatabase: DbModel? = null
        private set
    var selectedPerson: PersonModel? = null
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

    fun deleteEmbeddingsFile() {
        selectedDatabase?.embeddingsFile?.delete()
        selectedDatabase?.findEmbeddingsFile()
        _embeddingsFileEvent.value = Event(createTempDir())
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

    fun createPerson(name: String) {
        selectedDatabase?.let {
            val person = File(it.dbFolder, name)
            if (!person.exists()) {
                person.mkdirs()
                val list = _persons.value?.toMutableList()
                val model = PersonModel(person)
                list?.add(model)
                _persons.value = list
            } else {
                _warningEvent.value = Event(R.string.person_name_exists)
            }
        }
    }

    fun deletePerson(person: PersonModel) {
        selectedDatabase?.persons?.remove(person)
        person.personFolder.deleteRecursively()
        _persons.value = selectedDatabase?.persons
        _warningEvent.value = Event(R.string.person_is_deleted)
    }

    fun deleteDatabase(db: DbModel) {
        val list = _databases.value?.toMutableList()
        db.delete()
        list?.remove(db)
        _databases.value = list
        _warningEvent.value = Event(R.string.database_is_deleted)
    }

    fun renamePerson(person: PersonModel, name: String) {
        viewModelScope.launch {
            selectedDatabase?.dbFolder?.let {
                val file = File(it, name)
                if (file.exists()) {
                    _warningEvent.value = Event(R.string.person_name_exists)
                } else {
                    val list = _persons.value?.toMutableList()
                    list?.remove(person)
                    _persons.value = list
                    withContext(Dispatchers.IO) {
                        person.rename(file)
                    }
                    list?.add(person)
                    //list?.sort()
                    _persons.value = list
                    _warningEvent.value = Event(R.string.person_is_renamed)
                }
            }
        }
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
                it.refreshImages()
            }
        }
        _personImageDeletedEvent.value = Event(Unit)
    }

    fun selectPersonImage(image: File, person: PersonModel) {
        selectedPerson = person
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

    fun onImageAdded(uri: Uri) {
        try {
            val timestamp = System.currentTimeMillis()
            val name = "MAXREFDES178_" + timestamp + ".png"
            val file = File(selectedPerson?.personFolder, name)
            file.createNewFile()
            file.setReadable(true)
            file.setWritable(true)
            val fOut = FileOutputStream(file)

            //val source = ImageDecoder.createSource(app.contentResolver, uri)
            //val bitmap = ImageDecoder.decodeBitmap(source)
            val bitmap = MediaStore.Images.Media.getBitmap(app.contentResolver, uri)
            //bitmap.config = Bitmap.Config.ARGB_8888
            bitmap.compress(Bitmap.CompressFormat.PNG, 100, fOut)
//            val stream = ByteArrayOutputStream()
//            bitmap.compress(Bitmap.CompressFormat.PNG, 100, stream)
//            val bytes = stream.toByteArray()
            //fOut.flush()
            fOut.close()

            selectedPerson?.refreshImages()

//            val list = persons.value?.toMutableList()
//            val index = list?.indexOf(selectedPerson)
//            list?.remove(selectedPerson)
//            _persons.value = list
//            list?.add(index!!, selectedPerson!!)
//            _persons.value = list
            _personUpdatedEvent.value = Event(Unit)


        } catch (e: Exception) {
            e.printStackTrace()
        }
    }

    fun onCreateSignatureButtonClicked() {
        deleteEmbeddingsFile()
        _scriptInProgress.value = true
        val python = Python.getInstance()
        val script = python.getModule("db_gen.generate_face_db")
        var status = false
        viewModelScope.launch {
            withContext(Dispatchers.IO) {
                try {
                    selectedDatabase?.let {
                        val obj = script.callAttr("create_db", it.dbFolder.path, "embeddings")
                        status = obj.toBoolean()
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
                        Timber.i("Python response : " + status)
                    }
                } catch (e: Exception) {
                    status = false

                }

            }
            getEmbeddingsFile()
            _scriptInProgress.value = false
            if (!status) {
                _warningEvent.value = Event(R.string.signature_create_fail)
            }
        }
    }
}