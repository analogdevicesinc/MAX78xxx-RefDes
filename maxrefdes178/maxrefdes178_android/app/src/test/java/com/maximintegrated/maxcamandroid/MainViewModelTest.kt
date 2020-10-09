package com.maximintegrated.maxcamandroid

import androidx.arch.core.executor.testing.InstantTaskExecutorRule
import androidx.test.core.app.ApplicationProvider
import androidx.test.ext.junit.runners.AndroidJUnit4
import com.maximintegrated.maxcamandroid.utils.toHexToString
import com.maximintegrated.maxcamandroid.nativeLibrary.FakeMaxCamNativeLibrary
import com.maximintegrated.maxcamandroid.nativeLibrary.IMaxCamNativeLibrary
import com.maximintegrated.maxcamandroid.view.CustomTreeItem
import com.maximintegrated.maxcamandroid.view.TreeNodeType
import org.hamcrest.MatcherAssert.assertThat
import org.hamcrest.Matchers.`is`
import org.hamcrest.Matchers.nullValue
import org.junit.Before
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith
import java.io.File

@ExperimentalUnsignedTypes
@RunWith(AndroidJUnit4::class)
class MainViewModelTest {

    private val listener = object : IMaxCamNativeLibrary.JniListener {
        override fun sendNotification(data: ByteArray?) {

        }

        override fun payloadReceived(payload: ByteArray?) {
            payload?.let {
                viewModel.onPayloadReceived(it)
            }
        }
    }

    // Executes each task synchronously using Architecture Components.
    @get:Rule
    var instantExecutorRule = InstantTaskExecutorRule()
    private lateinit var viewModel: MainViewModel
    private val maxCamNativeLibrary = FakeMaxCamNativeLibrary()

    @Before
    fun setup() {
        maxCamNativeLibrary.setJniListener(listener)
        viewModel = MainViewModel(ApplicationProvider.getApplicationContext(), maxCamNativeLibrary)
    }

    @Test
    fun onBleDataReceived() {
        val array = ByteArray(10) { i -> i.toByte() }
        viewModel.onBleDataReceived(array)
        assertThat(array, `is`(viewModel.payload))
    }

    @Test
    fun onMtuSizeChanged() {
        var mtu = 50
        viewModel.onMtuSizeChanged(mtu)
        assertThat(maxCamNativeLibrary.mtu, `is`(50))
        mtu = 300
        viewModel.onMtuSizeChanged(mtu)
        assertThat(maxCamNativeLibrary.mtu, `is`(maxCamNativeLibrary.maxMtu))
    }

    @Test
    fun onFileSendButtonClicked() {
        var file: File? = null
        viewModel.onFileSelected(file)
        viewModel.onFileSendButtonClicked()
        file = File("test")
        file.writeText("test")
        viewModel.onFileSelected(file)
        viewModel.onFileSendButtonClicked()
        val operation = viewModel.fileSendOperation.getOrAwaitValue()
        assertThat(operation, `is`(OperationState.PROGRESS))
    }

    @Test
    fun onFileSendProgress() {
        viewModel.onFileSendProgress(0)
        val file = File("test")
        file.writeText("test")
        viewModel.onFileSelected(file)
        val size = file.length()
        viewModel.onFileSendButtonClicked()
        viewModel.onFileSendProgress(0)
        var operation = viewModel.fileSendOperation.getOrAwaitValue()
        assertThat(operation, `is`(OperationState.PROGRESS))
        viewModel.onFileSendProgress(size.toInt())
        operation = viewModel.fileSendOperation.getOrAwaitValue()
        assertThat(operation, `is`(OperationState.SUCCESS))
    }

    @Test
    fun onFileSendCompleted() {
        viewModel.onFileSendCompleted(OperationState.SUCCESS)
        var operation = viewModel.fileSendOperation.getOrAwaitValue()
        assertThat(operation, `is`(OperationState.SUCCESS))
        viewModel.onFileSendCompleted(OperationState.FAIL)
        operation = viewModel.fileSendOperation.getOrAwaitValue()
        assertThat(operation, `is`(OperationState.FAIL))
    }

    @Test
    fun onGetContentButtonClicked() {
        viewModel.onGetContentButtonClicked()
        assertThat(viewModel.payload!!.toHexToString(), `is`(maxCamNativeLibrary.lsFileContent))
    }

    @Test
    fun onGetFileButtonClicked() {
        viewModel.onGetContentButtonClicked()
        viewModel.onTreeItemDeselected()
        viewModel.onGetFileButtonClicked()
        assertThat(viewModel.fileGetOperation.getOrAwaitValue(), `is`(OperationState.NONE))
        viewModel.onTreeItemSelected(viewModel.treeItemList.value!!.last())
        viewModel.onGetFileButtonClicked()
        assertThat(viewModel.fileGetOperation.getOrAwaitValue(), `is`(OperationState.NONE))
        viewModel.onTreeItemSelected(viewModel.treeItemList.value!![0])
        viewModel.onGetFileButtonClicked()
        assertThat(viewModel.fileGetOperation.getOrAwaitValue(), `is`(OperationState.PROGRESS))
    }

    @Test
    fun onGetFileCompleted() {
        viewModel.onGetFileButtonClicked()
        assertThat(viewModel.fileGetOperation.getOrAwaitValue(), `is`(OperationState.NONE))
    }

    @Test
    fun onTreeItemSelected() {
        val item = CustomTreeItem(TreeNodeType.UNKOWN_TYPE, "test", 1000)
        viewModel.onTreeItemSelected(item)
        assertThat(viewModel.selectedTreeItem.getOrAwaitValue(), `is`(item))
    }

    @Test
    fun onTreeItemDeselected() {
        viewModel.onTreeItemDeselected()
        assertThat(viewModel.selectedTreeItem.getOrAwaitValue(), `is`(nullValue()))
    }

    @Test
    fun onLoadImageButtonClicked() {
        viewModel.onTreeItemSelected(CustomTreeItem(TreeNodeType.UNKOWN_TYPE, "test", 1000))
        viewModel.onLoadImageButtonClicked()
        assertThat("loadImage", `is`(maxCamNativeLibrary.functionArguments[0]))
        assertThat("test", `is`(maxCamNativeLibrary.functionArguments[1]))
    }

    @Test
    fun onEnterDemoButtonClicked() {
        viewModel.onEnterDemoButtonClicked()
        assertThat("enterDemo", `is`(maxCamNativeLibrary.functionArguments[0]))
    }

    @Test
    fun onCaptureImageButtonClicked() {
        viewModel.onCaptureImageButtonClicked()
        assertThat("captureImage", `is`(maxCamNativeLibrary.functionArguments[0]))
        assertThat(0, `is`(maxCamNativeLibrary.functionArguments[1]))
    }

    @Test
    fun onUpdateDeviceButtonClicked() {
        viewModel.onUpdateDeviceButtonClicked()
    }

    @Test
    fun onRunDemoButtonClicked() {
        viewModel.onRunDemoButtonClicked()
    }
}