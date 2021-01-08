package com.maximintegrated.maxcamandroid

import android.app.Application
import android.graphics.Bitmap
import android.net.Uri
import android.provider.MediaStore
import android.provider.OpenableColumns
import androidx.annotation.VisibleForTesting
import androidx.lifecycle.*
import com.maximintegrated.maxcamandroid.blePacket.IBlePacket
import com.maximintegrated.maxcamandroid.blePacket.ble_command_e
import com.maximintegrated.maxcamandroid.blePacket.device_version_t
import com.maximintegrated.maxcamandroid.nativeLibrary.IMaxCamNativeLibrary
import com.maximintegrated.maxcamandroid.utils.concatenate
import com.maximintegrated.maxcamandroid.utils.toHexToString
import com.maximintegrated.maxcamandroid.utils.toInt
import com.maximintegrated.maxcamandroid.view.CustomTreeItem
import com.maximintegrated.maxcamandroid.view.TreeNodeType
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import timber.log.Timber
import java.io.ByteArrayOutputStream
import java.io.File
import java.io.FileOutputStream

class MainViewModel(
    private val app: Application,
    private val maxCamNativeLibrary: IMaxCamNativeLibrary
) : AndroidViewModel(app) {

    private var numberOfBytesSent = 0

    var payload: ByteArray? = null
        private set

    private val _selectedFileName = MutableLiveData<String>()
    val selectedFileName: LiveData<String> = _selectedFileName

    private var selectedFile: File? = null

    private val _fileSendOperation = MutableLiveData<OperationState>(OperationState.NONE)
    val fileSendOperation: LiveData<OperationState> = _fileSendOperation

    private val _fileGetOperation = MutableLiveData<OperationState>(OperationState.NONE)
    val fileGetOperation: LiveData<OperationState> = _fileGetOperation

    private val _treeItemList = MutableLiveData<ArrayList<CustomTreeItem>>()
    val treeItemList: LiveData<ArrayList<CustomTreeItem>> = _treeItemList

    private val _selectedTreeItem = MutableLiveData<CustomTreeItem?>(null)
    val selectedTreeItem: LiveData<CustomTreeItem?> = _selectedTreeItem

    var latestReceivedTreeItem: CustomTreeItem? = null
        private set

    private var fileWriter: FileWriter? = null

    private var lastOperation = Operations.NONE

    private var lsFileSize = 0

    private val _demoBitmap = MutableLiveData<Bitmap?>()
    val demoBitmap: LiveData<Bitmap?> = _demoBitmap

    fun onBleDataReceived(data: ByteArray) {
        maxCamNativeLibrary.bleDataReceived(data)
    }

    fun onMtuSizeChanged(mtu: Int) {
        maxCamNativeLibrary.setMtu(mtu)
    }

    fun onFileSelected(uri: Uri?) {
        Timber.d("selected file uri: ${uri?.toString()}")
        uri?.let {
            app.contentResolver.query(it, null, null, null, null)
        }?.use { cursor ->
            val nameIndex = cursor.getColumnIndex(OpenableColumns.DISPLAY_NAME)
            cursor.moveToFirst()
            _selectedFileName.value = cursor.getString(nameIndex)
            println("Selected File Name: ${_selectedFileName.value}")
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

    @VisibleForTesting
    fun onFileSelected(file: File?) {
        selectedFile = file
        _selectedFileName.value = file?.name
    }

    fun onFileSendButtonClicked() {
        if (selectedFile != null && selectedFileName.value != null) {
            numberOfBytesSent = 0
            _fileSendOperation.value = OperationState.PROGRESS
            val data = selectedFile!!.readBytes()
            lastOperation = Operations.SEND_FILE
            maxCamNativeLibrary.sendFile(selectedFileName.value, data)
        }
    }

    fun onFileSendProgress(byteSent: Int) {
        numberOfBytesSent += byteSent
        if (selectedFile != null && selectedFile!!.length() <= numberOfBytesSent) {
            _fileSendOperation.value = OperationState.SUCCESS
        }
    }

    fun onFileSendCompleted(state: OperationState) {
        _fileSendOperation.value = state
    }

    fun onGetContentButtonClicked() {
        resetPayload()
        lastOperation = Operations.GET_DIRECTORY
        maxCamNativeLibrary.getDirectoryRequest()
    }

    @ExperimentalUnsignedTypes
    fun onPayloadReceived(data: ByteArray) {

        //ble_command_e.values()[commandInt].parse(data)
        var commandInt: Int = ble_command_e.BLE_COMMAND_GET_VERSION_RES as Int

        var command: ble_command_e = ble_command_e.values()[commandInt]

        var rawPacket: IBlePacket = command.parse(data)

        when (command) {
            ble_command_e.BLE_COMMAND_GET_VERSION_RES -> {
                val packet: device_version_t = rawPacket as device_version_t

                Timber.d(
                    "MAX32666 version %d.%d.%d".format(
                        packet.max32666.major,
                        packet.max32666.minor,
                        packet.max32666.build
                    )
                )
                Timber.d(
                    "MAX7800 Video version %d.%d.%d".format(
                        packet.max78000_video.major,
                        packet.max78000_video.minor,
                        packet.max78000_video.build
                    )
                )
                Timber.d(
                    "MAX7800 Audio version %d.%d.%d".format(
                        packet.max78000_audio.major,
                        packet.max78000_audio.minor,
                        packet.max78000_audio.build
                    )
                )

            }

        }


        payload = if (payload != null) {
            payload?.concatenate(data)
        } else {
            data
        }
        if (lastOperation == Operations.GET_DIRECTORY) {
            lsFileSize = payload?.toInt() ?: 0
            getLsFile()
        } else if (lastOperation == Operations.GET_LS) {
            if (payload?.size == lsFileSize) {
                val str = payload!!.toHexToString()
                updateTreeItemList(str)
            }
        } else if (lastOperation == Operations.GET_FILE) {
            fileWriter?.write(data)
            if (payload?.size == _selectedTreeItem.value?.size) {
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
        lastOperation = Operations.GET_LS
        maxCamNativeLibrary.getFile("lsFile.set")
    }

    fun onGetFileButtonClicked() {
        resetPayload()
        lastOperation = Operations.GET_FILE
        latestReceivedTreeItem = _selectedTreeItem.value
        if (latestReceivedTreeItem != null) {
            val fileName = latestReceivedTreeItem!!.text
            val size = latestReceivedTreeItem!!.size
            fileWriter?.close()
            fileWriter = FileWriter.open(fileName)
            if (size == 0) {
                fileWriter?.close()
                fileWriter = null
            } else {
                maxCamNativeLibrary.getFile(fileName)
                _fileGetOperation.value = OperationState.PROGRESS
            }
        } else {
            _fileGetOperation.value = OperationState.NONE
        }
    }

    fun onGetFileCompleted() {
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
            if (spaceIndex == -1) continue
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

    fun onTreeItemSelected(item: CustomTreeItem) {
        _selectedTreeItem.value = item
    }

    fun onTreeItemDeselected() {
        _selectedTreeItem.value = null
    }

    fun onLoadImageButtonClicked() {
        _selectedTreeItem.value?.text?.let {
            lastOperation = Operations.LOAD_IMAGE
            maxCamNativeLibrary.loadImage(it)
        }
    }

    fun onEnterDemoButtonClicked() {
        lastOperation = Operations.ENTER_DEMO
        maxCamNativeLibrary.enterDemo()
    }

    fun onCaptureImageButtonClicked() {
        lastOperation = Operations.CAPTURE_IMAGE
        maxCamNativeLibrary.captureImage(0)
    }

    fun onImageSelected(uri: Uri) {
        try {
            //val source = ImageDecoder.createSource(app.contentResolver, uri)
            //val bitmap = ImageDecoder.decodeBitmap(source)
            val bitmap = MediaStore.Images.Media.getBitmap(app.contentResolver, uri)
            //bitmap.config = Bitmap.Config.ARGB_8888
            val stream = ByteArrayOutputStream()
            bitmap.compress(Bitmap.CompressFormat.PNG, 100, stream)
            val bytes = stream.toByteArray()
            val timestamp = System.currentTimeMillis()
            lastOperation = Operations.SEND_IMAGE
            maxCamNativeLibrary.sendImage("Android_$timestamp.img", bytes)
        } catch (e: Exception) {
            e.printStackTrace()
        }
    }

    fun onUpdateDeviceButtonClicked() {

    }

    fun onRunDemoButtonClicked() {

    }

    fun onDemoImageSelected(uri: Uri?) {
        if (uri == null) {
            _demoBitmap.value = null
        } else {
            try {
                val bitmap = MediaStore.Images.Media.getBitmap(app.contentResolver, uri)
                _demoBitmap.value = bitmap
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }
    }
}

@Suppress("UNCHECKED_CAST")
class MainViewModelFactory(
    private val mApplication: Application,
    private val library: IMaxCamNativeLibrary
) :
    ViewModelProvider.Factory {

    override fun <T : ViewModel> create(modelClass: Class<T>): T {
        return MainViewModel(mApplication, library) as T
    }
}