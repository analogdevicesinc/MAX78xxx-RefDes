package com.maximintegrated.maxcamandroid
import com.maximintegrated.maxcamandroid.utils.getMaxCamFile
import com.maximintegrated.maxcamandroid.utils.ioThread
import java.io.File
import java.util.concurrent.LinkedBlockingDeque

class FileWriter private constructor(fileName: String) {

    companion object {
        const val FOLDER_NAME = "/MAXCAM/Received"
        private val POISON_PILL = Any()

        fun open(filePath: String): FileWriter {
            return FileWriter(filePath)
        }

        interface FileWriterListener {
            fun onCompleted(isSuccessful: Boolean)
        }
    }

    private val linesQueue = LinkedBlockingDeque<Any>()

    var isOpen = true
        private set

    var listener: FileWriterListener? = null

    private var packetLossOccurred = false

    val file = getMaxCamFile(fileName)

    init {
        ioThread {
            file.parentFile?.mkdirs()

            file.outputStream().use { out ->
                var count = 0
                var flushed = false
                while (true) {
                    val line = linesQueue.take()
                    if (line == POISON_PILL) {
                        break
                    }

                    count++

                    out.write((line as Array<*>)[0] as ByteArray)
                    if (count > 10000) {
                        out.flush()
                        flushed = true
                        count = 0
                    }

                }
                if (count == 0 && !flushed) {
                    listener?.onCompleted(false)
                    file.delete()
                } else if (packetLossOccurred) {
                    listener?.onCompleted(false)
                    val renamedFile = File(
                        file.parentFile,
                        File.separator + file.nameWithoutExtension + "!!!PacketLoss!!!." + file.extension
                    )
                    file.renameTo(renamedFile)
                } else {
                    listener?.onCompleted(true)
                }
            }
        }
    }

    fun write(vararg columns: Any) {
        if (isOpen) {
            linesQueue.offer(columns)
        } else {
            throw IllegalStateException("Writer is not open!")
        }
    }

    fun close(packetLossOccurred: Boolean = false) {
        this.packetLossOccurred = packetLossOccurred
        if (isOpen) {
            isOpen = false
            linesQueue.offer(POISON_PILL)
        } else {
            throw IllegalStateException("Writer is already closed!")
        }
    }
}

