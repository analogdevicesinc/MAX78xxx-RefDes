package com.maximintegrated.maxcamandroid.utils

import android.content.Context
import android.os.Environment
import androidx.appcompat.app.AlertDialog
import com.maximintegrated.maxcamandroid.FileWriter
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.face.DbModel
import java.io.File
import java.util.concurrent.Executors

const val ROOT_FRAGMENT = "ROOT_FRAGMENT"

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

public fun askUserForDeleteOperation(context: Context, listener: DeleteListener, vararg model: Any) {
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

