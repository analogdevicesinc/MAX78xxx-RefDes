package com.maximintegrated.maxcamandroid.fileOperations

import android.app.Application
import android.net.Uri
import android.provider.OpenableColumns
import androidx.lifecycle.*
import com.maximintegrated.maxcamandroid.FileWriter
import com.maximintegrated.maxcamandroid.OperationState
import com.maximintegrated.maxcamandroid.exts.concatenate
import com.maximintegrated.maxcamandroid.exts.toHexToString
import com.maximintegrated.maxcamandroid.nativeLibrary.MaxCamNativeLibrary
import com.maximintegrated.maxcamandroid.view.CustomTreeItem
import com.maximintegrated.maxcamandroid.view.TreeNodeType
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import timber.log.Timber
import java.io.File
import java.io.FileOutputStream

class FileOperationsViewModel(
    private val app: Application,
    private val maxCamNativeLibrary: MaxCamNativeLibrary
) : AndroidViewModel(app) {

    var numberOfBytesSent = 0
        private set

    var payload: ByteArray? = null
        private set

    private val _selectedFileName = MutableLiveData<String>()
    val selectedFileName: LiveData<String>
        get() = _selectedFileName

    var selectedFile: File? = null
        private set

    private val _fileSendOperation = MutableLiveData<OperationState>(OperationState.NONE)
    val fileSendOperation: LiveData<OperationState>
        get() = _fileSendOperation

    private val _fileGetOperation = MutableLiveData<OperationState>(OperationState.NONE)
    val fileGetOperation: LiveData<OperationState>
        get() = _fileGetOperation

    private val _fileReceived = MutableLiveData<File?>(null)
    val fileReceived: LiveData<File?>
        get() = _fileReceived

    private val _treeItemList = MutableLiveData<ArrayList<CustomTreeItem>>()
    val treeItemList: LiveData<ArrayList<CustomTreeItem>>
        get() = _treeItemList

    private val _selectedTreeItem = MutableLiveData<CustomTreeItem?>(null)
    val selectedTreeItem: LiveData<CustomTreeItem?>
        get() = _selectedTreeItem

    var latestReceivedTreeItem: CustomTreeItem? = null
        private set

    private var fileWriter: FileWriter? = null

    private var lsFileRequested = false

    fun onBleDataReceived(data: ByteArray) {
        maxCamNativeLibrary.bleDataReceived(data)
    }

    fun onMtuSizeChanged(mtu: Int) {
        maxCamNativeLibrary.setMtu(mtu)
    }

    fun onFileSelected(uri: Uri?) {
        uri?.let {
            app.contentResolver.query(it, null, null, null, null)
        }?.use { cursor ->
            val nameIndex = cursor.getColumnIndex(OpenableColumns.DISPLAY_NAME)
            cursor.moveToFirst()
            _selectedFileName.value = cursor.getString(nameIndex)
            Timber.d("Selected File Name: ${_selectedFileName.value}")
            viewModelScope.launch {
                selectedFile = getFileFromUri(uri)
            }
        }
    }

    private suspend fun getFileFromUri(uri: Uri): File? {
        return withContext(Dispatchers.IO) {
            val outputDir = app.cacheDir
            val outputFile = File.createTempFile("fileToBeSent", "bin", outputDir)
            val fos = FileOutputStream(outputFile)
            val inputStream = app.contentResolver.openInputStream(uri)
            val buffer = ByteArray(1024)
            var len = inputStream?.read(buffer) ?: -1
            while (len != -1) {
                fos.write(buffer, 0, len)
                len = inputStream?.read(buffer) ?: -1
            }
            inputStream?.close()
            fos.close()
            outputFile
        }
    }

    fun onFileSendButtonClicked() {
        if (selectedFile != null && selectedFileName.value != null) {
            numberOfBytesSent = 0
            _fileSendOperation.value = OperationState.PROGRESS
            val data = selectedFile!!.readBytes()
            maxCamNativeLibrary.sendFile(selectedFileName.value, data)
        }
    }

    fun onFileSendProgress(byteSent: Int) {
        numberOfBytesSent += byteSent
        if (selectedFile != null && selectedFile!!.length() < numberOfBytesSent) {
            _fileSendOperation.value = OperationState.SUCCESS
        }
    }

    fun onFileSendCompleted(state: OperationState) {
        _fileSendOperation.value = state
    }

    fun onGetContentButtonClicked() {
        maxCamNativeLibrary.getDirectoryRequest()
        getLsFile()
    }

    fun onPayloadReceived(data: ByteArray) {
        payload = if (payload != null) {
            payload?.concatenate(data)
        } else {
            data
        }
        if (lsFileRequested) {
            val str = payload!!.toHexToString()
            updateTreeItemList(str)
            _fileGetOperation.value = OperationState.SUCCESS
        } else {
            fileWriter?.write(data)
            if(payload?.size == _selectedTreeItem.value?.size){
                fileWriter?.close()
                fileWriter = null
                _fileGetOperation.value = OperationState.SUCCESS
            }
        }
    }

    private fun resetPayload() {
        payload = null
    }

    private fun getLsFile() {
        _selectedTreeItem.value = null
        latestReceivedTreeItem = null
        resetPayload()
        maxCamNativeLibrary.getFile("lsFile.set")
        _fileGetOperation.value = OperationState.PROGRESS
        lsFileRequested = true
    }

    fun onGetFileButtonClicked(){
        resetPayload()
        lsFileRequested = false
        latestReceivedTreeItem = _selectedTreeItem.value
        if(latestReceivedTreeItem != null){
            val fileName = latestReceivedTreeItem!!.text
            val size = latestReceivedTreeItem!!.size
            if(fileWriter != null){
                fileWriter?.close()
            }
            fileWriter = FileWriter.open(fileName)
            if(size == 0) {
                fileWriter?.close()
                fileWriter = null
            }else {
                maxCamNativeLibrary.getFile(fileName)
                _fileGetOperation.value = OperationState.PROGRESS
            }
        }else{
            _fileGetOperation.value = OperationState.NONE
        }
    }

    fun onGetFileCompleted(){
        _fileGetOperation.value = OperationState.NONE
    }

    private fun updateTreeItemList(str: String) {
        Timber.d("updateTreeItemList: $str")
        val fileAndSizes = str.split("\n")
        val deviceFiles: ArrayList<CustomTreeItem> = arrayListOf()
        for (fileAndSize in fileAndSizes) {
            if (fileAndSize.startsWith("System Volume Information")) continue
            val dotIndex = fileAndSize.lastIndexOf('.')
            val spaceIndex = fileAndSize.lastIndexOf(' ')
            if(spaceIndex == -1) continue
            val fileNameWithExtension = fileAndSize.substring(0, spaceIndex)
            val fileSize = fileAndSize.substring(spaceIndex + 1).toIntOrNull() ?: 0
            var extension = ""
            if (dotIndex != -1) {
                extension = fileNameWithExtension.substring(dotIndex + 1)
            }
            val fileType = when (extension.toLowerCase()) {
                "txt" -> TreeNodeType.FILE_TEXT
                "jpg", "jpeg", "png" -> TreeNodeType.FILE_IMAGE
                else -> TreeNodeType.UNKOWN_TYPE
            }
            val item = CustomTreeItem(fileType, fileNameWithExtension, fileSize)
            deviceFiles.add(item)
        }
        _treeItemList.value = deviceFiles
    }

    fun onTreeItemSelected(item: CustomTreeItem){
        _selectedTreeItem.value = item
    }

    fun onTreeItemDeselected(){
        _selectedTreeItem.value = null
    }
}

class FileOperationsViewModelFactory(
    private val mApplication: Application,
    private val library: MaxCamNativeLibrary
) :
    ViewModelProvider.Factory {

    override fun <T : ViewModel> create(modelClass: Class<T>): T {
        return FileOperationsViewModel(mApplication, library) as T
    }
}