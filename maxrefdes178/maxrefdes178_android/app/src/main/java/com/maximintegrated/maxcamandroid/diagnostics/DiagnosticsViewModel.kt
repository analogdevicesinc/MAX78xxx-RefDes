package com.maximintegrated.maxcamandroid.diagnostics

import android.app.Application
import android.graphics.Bitmap
import android.graphics.ImageDecoder
import android.net.Uri
import android.provider.MediaStore
import androidx.lifecycle.*
import com.maximintegrated.maxcamandroid.exts.concatenate
import com.maximintegrated.maxcamandroid.exts.toHexToString
import com.maximintegrated.maxcamandroid.nativeLibrary.MaxCamNativeLibrary
import com.maximintegrated.maxcamandroid.view.CustomTreeItem
import com.maximintegrated.maxcamandroid.view.TreeNodeType
import timber.log.Timber
import java.io.ByteArrayOutputStream
import java.lang.Exception

class DiagnosticsViewModel(
    private val app: Application,
    private val maxCamNativeLibrary: MaxCamNativeLibrary
) : AndroidViewModel(app) {

    var payload: ByteArray? = null
        private set

    private val _treeItemList = MutableLiveData<ArrayList<CustomTreeItem>>()
    val treeItemList: LiveData<ArrayList<CustomTreeItem>>
        get() = _treeItemList

    private val _selectedTreeItem = MutableLiveData<CustomTreeItem?>(null)
    val selectedTreeItem: LiveData<CustomTreeItem?>
        get() = _selectedTreeItem

    private var lsFileRequested = false

    fun onBleDataReceived(data: ByteArray) {
        maxCamNativeLibrary.bleDataReceived(data)
    }

    fun onMtuSizeChanged(mtu: Int) {
        maxCamNativeLibrary.setMtu(mtu)
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
        }
    }

    private fun resetPayload() {
        payload = null
    }

    private fun getLsFile() {
        lsFileRequested = true
        _selectedTreeItem.value = null
        resetPayload()
        maxCamNativeLibrary.getFile("lsFile.set")
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
            maxCamNativeLibrary.loadImage(it)
        }
    }

    fun onEnterDemoButtonClicked() {
        maxCamNativeLibrary.enterDemo()
    }

    fun onCaptureImageButtonClicked() {
        maxCamNativeLibrary.captureImage(0)
    }

    fun onImageSelected(uri: Uri){
        try{
            val source = ImageDecoder.createSource(app.contentResolver, uri)
            val bitmap  = ImageDecoder.decodeBitmap(source)
            //bitmap.config = Bitmap.Config.ARGB_8888
            val stream = ByteArrayOutputStream()
            bitmap.compress(Bitmap.CompressFormat.PNG, 100, stream)
            val bytes = stream.toByteArray()
            val timestamp = System.currentTimeMillis()
            maxCamNativeLibrary.sendImage("Android_$timestamp.img", bytes)

        }catch (e: Exception) {
            e.printStackTrace()
        }
    }
}

class DiagnosticsViewModelFactory(
    private val mApplication: Application,
    private val library: MaxCamNativeLibrary
) :
    ViewModelProvider.Factory {

    override fun <T : ViewModel> create(modelClass: Class<T>): T {
        return DiagnosticsViewModel(mApplication, library) as T
    }
}