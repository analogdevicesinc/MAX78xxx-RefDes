package com.maximintegrated.maxcamandroid.nativeLibrary

import java.io.File
import java.nio.ByteBuffer
import java.nio.ByteOrder
import kotlin.math.min

class FakeMaxCamNativeLibrary : IMaxCamNativeLibrary {

    private var listener: IMaxCamNativeLibrary.JniListener? = null
    var mtu = 23
        private set

    var files: HashMap<String, String> = hashMapOf()

    var lsFileContent = ""
        private set

    var functionArguments: ArrayList<Any> = arrayListOf()

    init {
        files["a.txt"] = "1234567890"
        files["b"] = "1234567890".toByteArray().joinToString()
        files["c.png"] = "imageFile"
        files["d.set"] = ""

        for ((k, v) in files) {
            lsFileContent += "$k ${v.length}\n"
        }

        lsFileContent = lsFileContent.substring(0 until lsFileContent.length - 1)
    }

    override fun setJniListener(listener: IMaxCamNativeLibrary.JniListener?) {
        this.listener = listener
    }

    override fun setMtu(mtu: Int) {
        this.mtu = min(mtu, maxMtu)
    }

    override fun getMaxMtu(): Int {
        return 251
    }

    override fun getVersion(): String {
        return "1.0.0"
    }

    override fun sendFile(fileName: String?, data: ByteArray?) {
        functionArguments.clear()
        functionArguments.add("sendFile")
        functionArguments.add(fileName ?: Any())
        functionArguments.add(data ?: Any())
    }

    override fun bleDataReceived(data: ByteArray?) {
        payloadReceived(data)
    }

    override fun getDirectoryRequest() {
        val payload = ByteBuffer.allocate(4)
        payload.order(ByteOrder.LITTLE_ENDIAN)
        payload.putInt(lsFileContent.length)
        payloadReceived(payload.array())
        functionArguments.clear()
        functionArguments.add("getDirectoryRequest")
    }

    override fun getFile(fileName: String?) {
        if (fileName == "lsFile.set") {
            val bytes = lsFileContent.toByteArray()
            val payload1 = bytes.sliceArray(0 until bytes.size / 2)
            val payload2 = bytes.sliceArray(bytes.size / 2 until bytes.size)
            payloadReceived(payload1)
            payloadReceived(payload2)
        } else {
            val file = files[fileName]
            file?.let {
                payloadReceived(it.toByteArray())
            }
        }
        functionArguments.clear()
        functionArguments.add("getFile")
        functionArguments.add(fileName ?: Any())
    }

    override fun bleReset() {
        functionArguments.clear()
        functionArguments.add("bleReset")
    }

    override fun enterDemo() {
        functionArguments.clear()
        functionArguments.add("enterDemo")
    }

    override fun captureImage(fileLength: Int) {
        functionArguments.clear()
        functionArguments.add("captureImage")
        functionArguments.add(fileLength)
    }

    override fun loadImage(filename: String?) {
        functionArguments.clear()
        functionArguments.add("loadImage")
        functionArguments.add(filename ?: Any())
    }

    override fun sendImage(filename: String?, content: ByteArray?) {
        functionArguments.clear()
        functionArguments.add("sendImage")
        functionArguments.add(content ?: Any())
    }

    override fun sendNotification(data: ByteArray?) {
        listener?.sendNotification(data)
    }

    override fun payloadReceived(payload: ByteArray?) {
        listener?.payloadReceived(payload)
    }
}