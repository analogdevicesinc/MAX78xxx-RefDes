/*
 * Copyright (C) Maxim Integrated Products, Inc., All rights Reserved.
 *
 * This software is protected by copyright laws of the United States and
 * of foreign countries. This material may also be protected by patent laws
 * and technology transfer regulations of the United States and of foreign
 * countries. This software is furnished under a license agreement and/or a
 * nondisclosure agreement and may only be used or reproduced in accordance
 * with the terms of those agreements. Dissemination of this information to
 * any party or parties not specified in the license agreement and/or
 * nondisclosure agreement is expressly prohibited.
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 */

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

