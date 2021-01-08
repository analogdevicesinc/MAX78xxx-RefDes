package com.maximintegrated.maxcamandroid.blePacket

import androidx.databinding.InverseBindingListener
import java.nio.ByteBuffer
import java.nio.ByteOrder

interface IBlePacket {
//    fun getCommandType() = ble_command_e.BLE_COMMAND_ABORT_CMD
}

enum class ble_packet_type_e {
    BLE_PACKET_TYPE_COMMAND,
    BLE_PACKET_TYPE_PAYLOAD
}

data class ble_packet_info_t(
    var type: ble_packet_type_e = ble_packet_type_e.BLE_PACKET_TYPE_COMMAND,
    var seq: Int
)

data class ble_command_packet_t(
    var header: ble_command_packet_header_t,
    var payload: ByteArray
) {
    init {
        payload = ByteArray(Definitions.BLE_COMMAND_PACKET_MAX_PAYLOAD_SIZE)
    }
}

data class ble_command_packet_header_t(
    var packet_info: ble_packet_info_t,
    var command: Int,
    var total_payload_size: Int
)

data class version_t(
    var major: Byte,
    var minor: Byte,
    var build: Int
)

data class device_version_t(
    var max32666: version_t,
    var max78000_video: version_t,
    var max78000_audio: version_t
) : IBlePacket


data class device_serial_num_t(
    var max32666: ByteArray, //len 13
    var max78000_video: ByteArray, //len 13
    var max78000_audio: ByteArray //len 13
) : IBlePacket

enum class faceid_embed_update_status_e : IBlePacket {
    FACEID_EMBED_UPDATE_STATUS_SUCCESS,
    FACEID_EMBED_UPDATE_STATUS_ERROR_NO_FILE,
    FACEID_EMBED_UPDATE_STATUS_ERROR_UNKNOWN,
    FACEID_EMBED_UPDATE_STATUS_LAST
}

data class max78000_statistics_t(
    var cnn_duration_us: Int,
    var capture_duration_us: Int,
    var communication_duration_us: Int,
    var total_duration_us: Int
)

data class device_statistics_t(
    var max78000_video: max78000_statistics_t,
    var max78000_audio: max78000_statistics_t,
    var lcd_fps: Float,
    var battery_level: Byte,
    var max78000_video_power_uw: Int,
    var max78000_audio_power_uw: Int
) : IBlePacket

enum class ble_command_e {
    //  Command                             Command Payload Description
    // Communication
    BLE_COMMAND_ABORT_CMD,           // None
    BLE_COMMAND_INVALID_RES,             // None
    BLE_COMMAND_NOP_CMD,                 // None

    // Version
    BLE_COMMAND_GET_VERSION_CMD,         // None
    BLE_COMMAND_GET_VERSION_RES {
        override fun parse(arr: ByteArray): device_version_t {

            val max32666: version_t = parseVersion(arr.sliceArray(0..5))
            val max78000_video: version_t = parseVersion(arr.sliceArray(6..11))
            val max78000_audio: version_t = parseVersion(arr.sliceArray(12..17))

            return device_version_t(max32666, max78000_video, max78000_audio)
        }

        private fun parseVersion(arr: ByteArray): version_t {

            val buffer = ByteBuffer.wrap(arr).apply { order(ByteOrder.LITTLE_ENDIAN) }
            val major = buffer.get()
            val minor = buffer.get()
            val build = buffer.int
            return version_t(major, minor, build)

        }

    },         // device_version_t
    BLE_COMMAND_GET_SERIAL_NUM_CMD,      // None
    BLE_COMMAND_GET_SERIAL_NUM_RES {
        override fun parse(arr: ByteArray): device_serial_num_t {
            return device_serial_num_t(
                arr.sliceArray(0..12),
                arr.sliceArray(13..25),
                arr.sliceArray(26..38)
            )
        }
    },      // device_serial_num_t

    // SD Card
    BLE_COMMAND_GET_SD_INSERTED_CMD,     // None
    BLE_COMMAND_GET_SD_INSERTED_RES,     // sd_status_e
    BLE_COMMAND_WRITE_SD_FILE_CMD,       // File name size (uint8_t) + File name string + File content
    BLE_COMMAND_WRITE_SD_FILE_RES,       // sd_status_e
    BLE_COMMAND_READ_SD_FILE_CMD,        // File name string
    BLE_COMMAND_READ_SD_FILE_RES,        // sd_status_e + File content
    BLE_COMMAND_GET_SD_CONTENT_CMD,      // None
    BLE_COMMAND_GET_SD_CONTENT_RES,      // (file_info_header_t + File name string) * Number of files
    BLE_COMMAND_GET_SD_FREE_SPACE_CMD,   // None
    BLE_COMMAND_GET_SD_FREE_SPACE_RES,   // Free space size (uint32_t)
    BLE_COMMAND_DELETE_SD_FILE_CMD,      // File name string
    BLE_COMMAND_DELETE_SD_FILE_RES,      // sd_status_e
    BLE_COMMAND_FORMAT_SD_CMD,           // None
    BLE_COMMAND_FORMAT_SD_RES,           // sd_status_e

    // External Flash
    BLE_COMMAND_WRITE_EXT_FILE_CMD,      // File name size (uint8_t) + File name string + File content
    BLE_COMMAND_WRITE_EXT_FILE_RES,      // ext_status_e
    BLE_COMMAND_READ_EXT_FILE_CMD,       // File name string
    BLE_COMMAND_READ_EXT_FILE_RES,       // ext_status_e + File content
    BLE_COMMAND_GET_EXT_CONTENT_CMD,     // None
    BLE_COMMAND_GET_EXT_CONTENT_RES,     // (file_info_header_t + File name string) * Number of files
    BLE_COMMAND_GET_EXT_FREE_SPACE_CMD,  // None
    BLE_COMMAND_GET_EXT_FREE_SPACE_RES,  // Free space size (uint32_t)
    BLE_COMMAND_DELETE_EXT_FILE_CMD,     // File name string
    BLE_COMMAND_DELETE_EXT_FILE_RES,     // ext_status_e
    BLE_COMMAND_FORMAT_EXT_CMD,          // None
    BLE_COMMAND_FORMAT_EXT_RES,          // ext_status_e

    // Firmware Update from SD Card
    BLE_COMMAND_FW_UPDATE_MAX32666_SD_CMD,        // File name string
    BLE_COMMAND_FW_UPDATE_MAX32666_SD_RES,        // fw_update_status_e
    BLE_COMMAND_FW_UPDATE_MAX78000_SD_VIDEO_CMD,  // File name string
    BLE_COMMAND_FW_UPDATE_MAX78000_SD_VIDEO_RES,  // fw_update_status_e
    BLE_COMMAND_FW_UPDATE_MAX78000_SD_AUDIO_CMD,  // File name string
    BLE_COMMAND_FW_UPDATE_MAX78000_SD_AUDIO_RES,  // fw_update_status_e
    BLE_COMMAND_FW_UPDATE_COMBINED_SD_CMD,        // File name string
    BLE_COMMAND_FW_UPDATE_COMBINED_SD_RES,        // fw_update_status_e

    // Firmware Update from External Flash
    BLE_COMMAND_FW_UPDATE_MAX32666_EXT_CMD,       // File name string
    BLE_COMMAND_FW_UPDATE_MAX32666_EXT_RES,       // fw_update_status_e
    BLE_COMMAND_FW_UPDATE_MAX78000_EXT_VIDEO_CMD, // File name string
    BLE_COMMAND_FW_UPDATE_MAX78000_EXT_VIDEO_RES, // fw_update_status_e
    BLE_COMMAND_FW_UPDATE_MAX78000_EXT_AUDIO_CMD, // File name string
    BLE_COMMAND_FW_UPDATE_MAX78000_EXT_AUDIO_RES, // fw_update_status_e
    BLE_COMMAND_FW_UPDATE_COMBINED_EXT_CMD,       // File name string
    BLE_COMMAND_FW_UPDATE_COMBINED_EXT_RES,       // fw_update_status_e

    // FaceID Embeddings Update
    BLE_COMMAND_FACEID_EMBED_UPDATE_CMD,          // FaceID embeddings binary content
    BLE_COMMAND_FACEID_EMBED_UPDATE_RES {
        override fun parse(arr: ByteArray): faceid_embed_update_status_e {
            return faceid_embed_update_status_e.values()[arr[0].toInt()]
        }
    },          // faceid_embed_update_status_e
    BLE_COMMAND_FACEID_EMBED_UPDATE_SD_CMD,       // File name string
    BLE_COMMAND_FACEID_EMBED_UPDATE_SD_RES,       // faceid_embed_update_status_e
    BLE_COMMAND_FACEID_EMBED_UPDATE_EXT_CMD,      // File name string
    BLE_COMMAND_FACEID_EMBED_UPDATE_EXT_RES,      // faceid_embed_update_status_e

    // Device Settings
    BLE_COMMAND_DISABLE_BLE_CMD,                      // None
    BLE_COMMAND_SHUT_DOWN_DEVICE_CMD,                 // None
    BLE_COMMAND_ENABLE_MAX78000_AUDIO_CMD,            // None
    BLE_COMMAND_DISABLE_MAX78000_AUDIO_CMD,           // None
    BLE_COMMAND_ENABLE_MAX78000_VIDEO_CMD,            // None
    BLE_COMMAND_DISABLE_MAX78000_VIDEO_CMD,           // None
    BLE_COMMAND_ENABLE_MAX78000_VIDEO_CNN_CMD,        // None
    BLE_COMMAND_DISABLE_MAX78000_VIDEO_CNN_CMD,       // None
    BLE_COMMAND_ENABLE_MAX78000_AUDIO_CNN_CMD,        // None
    BLE_COMMAND_DISABLE_MAX78000_AUDIO_CNN_CMD,       // None
    BLE_COMMAND_ENABLE_MAX78000_VIDEO_FLASH_LED_CMD,  // None
    BLE_COMMAND_DISABLE_MAX78000_VIDEO_FLASH_LED_CMD, // None
    BLE_COMMAND_ENABLE_MAX78000_VIDEO_AUDIO_POWER,    // None
    BLE_COMMAND_DISABLE_MAX78000_VIDEO_AUDIO_POWER,   // None
    BLE_COMMAND_ENABLE_LCD_CMD,                       // None
    BLE_COMMAND_DISABLE_LCD_CMD,                      // None
    BLE_COMMAND_ENABLE_LCD_STATISCTICS_CMD,           // None
    BLE_COMMAND_DISABLE_LCD_STATISCTICS_CMD,          // None
    BLE_COMMAND_ENABLE_LCD_PROBABILITY_CMD,           // None
    BLE_COMMAND_DISABLE_LCD_PROBABILITY_CMD,          // None
    BLE_COMMAND_ENABLE_SEND_STATISTICS_CMD,           // None
    BLE_COMMAND_DISABLE_SEND_STATISTICS_CMD,          // None
    BLE_COMMAND_ENABLE_SEND_CLASSIFICATION_CMD,       // None
    BLE_COMMAND_DISABLE_SEND_CLASSIFICATION_CMD,      // None

    // Statistics
    BLE_COMMAND_GET_STATISTICS_RES {
        private fun parseStatistics(arr: ByteArray): max78000_statistics_t {
            val buffer = ByteBuffer.wrap(arr).apply { order(ByteOrder.LITTLE_ENDIAN) }
            return max78000_statistics_t(buffer.int, buffer.int, buffer.int, buffer.int)
        }

        override fun parse(arr: ByteArray): device_statistics_t {

            var tempArr: ByteArray = arr
            var video = parseStatistics(arr.sliceArray(0 until 16))
            var audio = parseStatistics(arr.sliceArray(16 until 32))
            tempArr = arr.sliceArray(32 until arr.size)
            var buffer = ByteBuffer.wrap(tempArr).apply { ByteOrder.LITTLE_ENDIAN }

            return device_statistics_t(
                video,
                audio,
                buffer.float,
                buffer.get(),
                buffer.int,
                buffer.int
            )
        }
    },      // device_statistics_t

    // Classification
    BLE_COMMAND_GET_MAX78000_VIDEO_CLASSIFICATION_RES, // classification_result_t
    BLE_COMMAND_GET_MAX78000_AUDIO_CLASSIFICATION_RES, // classification_result_t

    // Debugger Selection
    BLE_COMMAND_SET_DEBUGGER_CMD,        // debugger_select_e

    BLE_COMMAND_LAST;

    open fun parse(arr: ByteArray): IBlePacket {
        return device_serial_num_t(byteArrayOf(0), byteArrayOf(0), byteArrayOf(0))
    }
}


class Definitions {
    fun parse(commandInt: Int, arr: ByteArray): IBlePacket {
        return ble_command_e.values()[commandInt].parse(arr)
    }

    companion object {
        const val BLE_COMMAND_PACKET_MAX_PAYLOAD_SIZE = 243
    }
}