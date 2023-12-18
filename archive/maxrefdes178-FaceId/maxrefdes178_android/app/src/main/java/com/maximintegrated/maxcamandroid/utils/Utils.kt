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

package com.maximintegrated.maxcamandroid.utils

import android.content.Context
import android.content.Intent
import android.os.AsyncTask
import android.os.Environment
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import com.maximintegrated.communication.MaxCamViewModel
import com.maximintegrated.maxcamandroid.FileWriter
import com.maximintegrated.maxcamandroid.MainViewModel
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.ScannerActivity
import com.maximintegrated.maxcamandroid.blePacket.*
import com.maximintegrated.maxcamandroid.databinding.SettingsItemBinding
import java.io.ByteArrayOutputStream
import java.io.File
import java.io.InputStream
import java.nio.ByteBuffer
import java.util.concurrent.Executors
import kotlin.math.min

const val ROOT_FRAGMENT = "ROOT_FRAGMENT"

fun Context.getAppName(): String = applicationInfo.loadLabel(packageManager).toString()

fun ByteArray.concatenate(other: ByteArray): ByteArray {
    val result = this.copyOf(size + other.size)
    System.arraycopy(other, 0, result, size, other.size)
    return result
}

@ExperimentalUnsignedTypes
fun ByteArray.toInt(): Int {
    var result = 0
    var shift = 0
    for (byte in this) {
        result += ((byte.toUInt() and 0xFFu) shl shift).toInt()
        shift += 8
    }
    return result
}

fun Int.toLittleEndianByte(): ByteArray {
    return ByteBuffer.allocate(Int.SIZE_BYTES).putInt(this).array().reversedArray()
}

fun ByteArray.toHexToString(): String {
    val hex = toHexString()
    return hex.fromHexStringToString()
}

fun ByteArray.toHexString() = joinToString("") { "%02x".format(it) }

fun String.fromHexStringToString(): String {
    val sb = StringBuilder()
    val hexData = toCharArray()
    var count = 0
    while (count < hexData.size - 1) {
        val firstDigit = Character.digit(hexData[count], 16)
        val lastDigit = Character.digit(hexData[count + 1], 16)
        val decimal = firstDigit * 16 + lastDigit
        sb.append(decimal.toChar())
        count += 2
    }
    return sb.toString()
}

fun String.fromHexStringToMeaningfulAscii(): String {
    val sb = StringBuilder()
    val hexData = toCharArray()
    var count = 0
    while (count < hexData.size - 1) {
        val firstDigit = Character.digit(hexData[count], 16)
        val lastDigit = Character.digit(hexData[count + 1], 16)
        var decimal = firstDigit * 16 + lastDigit
        if (decimal < 0x21 || decimal > 0x7E) {
            decimal = 0x3F // '?'
        }
        sb.append(decimal.toChar())
        count += 2
    }
    return sb.toString()
}

interface DeleteListener {
    fun onDeleteButtonClicked(vararg model: Any)
}

public fun askUserForDeleteOperation(
    context: Context,
    listener: DeleteListener,
    vararg model: Any
) {
    val alert = AlertDialog.Builder(context)
    alert.setMessage(context.getString(R.string.are_you_sure_to_delete_it))
    alert.setPositiveButton(context.getString(R.string.yes)) { dialog, _ ->
        dialog.dismiss()
        listener.onDeleteButtonClicked(model)
    }
    alert.setNegativeButton(context.getString(R.string.cancel)) { dialog, _ ->
        dialog.dismiss()
    }
    alert.show()
}

fun showWarningPopup(
    context: Context,
    warningMessage: String
) {
    val alert = AlertDialog.Builder(context)
    alert.setTitle(R.string.warning)
    alert.setMessage(warningMessage)
    alert.setPositiveButton(R.string.OK) { dialog, _ ->
        dialog.dismiss()
    }
    alert.show()
}

fun sendDefaultEmbeddings(
    maxCamViewModel: MaxCamViewModel,
    mainViewModel: MainViewModel,
    context: Context
) {
    try {
        val inputStream: InputStream = context?.assets?.open("embeddings_default.bin")!!

        val buffer = ByteArray(8192)
        var bytesRead: Int
        val output = ByteArrayOutputStream()
        while (inputStream.read(buffer).also { bytesRead = it } != -1) {
            output.write(buffer, 0, bytesRead)
        }
        val byteArr: ByteArray = output.toByteArray()
        sendEmbeddings(byteArr, maxCamViewModel, mainViewModel, context)
    } catch (e: Exception) {
        e.printStackTrace()
    }
}

fun sendEmbeddings(
    embeddingsFile: File,
    maxCamViewModel: MaxCamViewModel,
    mainViewModel: MainViewModel,
    context: Context
) {
    sendEmbeddings(embeddingsFile.readBytes(), maxCamViewModel, mainViewModel, context)
}

fun sendEmbeddings(
    embeddingsArr: ByteArray,
    maxCamViewModel: MaxCamViewModel,
    mainViewModel: MainViewModel,
    context: Context
) {

    val alert = AlertDialog.Builder(context)
    alert.setMessage(context.getString(R.string.are_you_sure_to_send_signature))
    alert.setPositiveButton(context.getString(R.string.upload_signature)) { dialog, _ ->
        dialog.dismiss()

        if (maxCamViewModel.mtuSize.value != null) {
            mainViewModel.setEmbeddingsSendInProgress(true)

            val command_packet_payload_size: Int =
                maxCamViewModel.packetSize - ble_command_packet_header_t.size()
            val payload_packet_payload_size: Int =
                maxCamViewModel.packetSize - ble_payload_packet_header_t.size()
            var spentPayloadSize = 0
            var remainingSize = embeddingsArr.size

            val commandPacket = ble_command_packet_t.from(
                ble_command_e.BLE_COMMAND_FACEID_EMBED_UPDATE_CMD,
                embeddingsArr,
                min(command_packet_payload_size, embeddingsArr.size),
                embeddingsArr.size
            )

            mainViewModel.setSendTimeout()
            maxCamViewModel.sendData(commandPacket.toByteArray())

            spentPayloadSize += command_packet_payload_size
            remainingSize -= command_packet_payload_size
            while (remainingSize > 0) {
                val payloadPacket = ble_payload_packet_t.from(
                    embeddingsArr.sliceArray(spentPayloadSize until embeddingsArr.size),
                    min(payload_packet_payload_size, remainingSize)
                )
                maxCamViewModel.sendData(payloadPacket.toByteArray())
                spentPayloadSize += payload_packet_payload_size
                remainingSize -= payload_packet_payload_size
            }
        } else {
            Toast.makeText(
                context,
                "Connection issue! Mtu is not set yet",
                Toast.LENGTH_LONG
            ).show()
        }
    }
    alert.setNegativeButton(context.getString(R.string.cancel)) { dialog, _ ->
        dialog.dismiss()
    }
    alert.show()
}


private val IO_EXECUTOR = Executors.newSingleThreadExecutor()

/**
 * Utility method to run blocks on a dedicated background thread, used for io/database work.
 */
fun ioThread(f: () -> Unit) {
    IO_EXECUTOR.execute(f)
}

fun getMaxCamFile(fileName: String): File {
    return File(
        Environment.getExternalStorageDirectory().toString() + FileWriter.FOLDER_NAME,
        fileName
    )


}

interface SettingsItemListener {
    fun onEnableButtonClick(itemName: String, command: ble_command_e)
    fun onDisableButtonClick(itemName: String, command: ble_command_e)
}

fun SettingsItemBinding.setup(
    itemName: String,
    enableCommand: ble_command_e,
    disableCommand: ble_command_e,
    listener: SettingsItemListener
) {
    descriptionTextView.text = itemName
    enableButton.setOnClickListener {
        listener.onEnableButtonClick(itemName, enableCommand)
    }
    disableButton.setOnClickListener {
        listener.onDisableButtonClick(itemName, disableCommand)
    }
}

fun SettingsItemBinding.setEnabled(
    isEnabled: Boolean
) {
    descriptionTextView.isEnabled = isEnabled
    enableButton.isEnabled = isEnabled
    disableButton.isEnabled = isEnabled
}

class doAsync(val handler: () -> Unit) : AsyncTask<Void, Void, Void>() {
    override fun doInBackground(vararg params: Void?): Void? {
        handler()
        return null
    }
}

fun startScannerActivity(packageContext: Context) {
    packageContext.startActivity(
        Intent(packageContext, ScannerActivity::class.java).apply {
            flags = Intent.FLAG_ACTIVITY_NEW_TASK or Intent.FLAG_ACTIVITY_CLEAR_TASK
        })
}

/**
 * This method converts dp unit to equivalent pixels, depending on device density.
 *
 * @param dp      A value in dp (density independent pixels) unit. Which we need to convert into pixels
 * @param context Context to get resources and device specific display metrics
 * @return A float value to represent px equivalent to dp depending on device density
 */
public fun convertDpToPx(context: Context, dp: Float): Float {
    return dp * context.resources.displayMetrics.density
}

/**
 * This method converts device specific pixels to density independent pixels.
 *
 * @param px      A value in px (pixels) unit. Which we need to convert into db
 * @param context Context to get resources and device specific display metrics
 * @return A float value to represent dp equivalent to px value
 */
public fun convertPxToDp(context: Context, px: Float): Float {
    return px / context.resources.displayMetrics.density
}

