package com.maximintegrated.maxcamandroid.exts

import android.os.Environment
import com.maximintegrated.maxcamandroid.FileWriter
import com.maximintegrated.maxcamandroid.faceId.FaceIdViewModel
import timber.log.Timber
import java.io.File
import java.util.concurrent.Executors
import kotlin.experimental.and

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

fun ByteArray.toSpecialFormat(): String {
    val sb = StringBuilder()
    val hexString = toHexString()
    val ascii = hexString.fromHexStringToString()
    for (i in ascii.indices) {
        sb.append("${ascii[i]}(${hexString.substring(2 * i, 2 * i + 2)}) ")
    }
    return sb.toString()
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

fun getMaxCamScenarioFile(fileName: String): File {
    return File(
        Environment.getExternalStorageDirectory().toString() + FaceIdViewModel.SCENARIO_FOLDER_NAME,
        fileName
    )
}

fun getMaxCamConfigFile(fileName: String): File {
    return File(
        Environment.getExternalStorageDirectory().toString() + FaceIdViewModel.CONFIG_FOLDER_NAME,
        fileName
    )
}
