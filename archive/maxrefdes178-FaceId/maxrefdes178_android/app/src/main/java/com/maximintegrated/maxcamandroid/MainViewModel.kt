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

import android.app.Application
import android.os.Handler
import android.widget.Toast
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import com.maximintegrated.maxcamandroid.blePacket.*
import com.maximintegrated.maxcamandroid.utils.Event
import com.maximintegrated.maxcamandroid.utils.concatenate
import timber.log.Timber

class MainViewModel(private val app: Application) : AndroidViewModel(app) {

    var payload: ByteArray? = null
        private set

    private val _embeddingsSendInProgress = MutableLiveData<Boolean>(false)
    val embeddingsSendInProgress: LiveData<Boolean> = _embeddingsSendInProgress

    fun setEmbeddingsSendInProgress(inProgress: Boolean) {
        _embeddingsSendInProgress.value = inProgress
    }

    private val _maxcamVersion = MutableLiveData<device_version_t>()
    val maxcamVersion: LiveData<device_version_t> = _maxcamVersion


    private val _mtuResponse = MutableLiveData<ble_mtu_response>()
    val mtuResponse: LiveData<ble_mtu_response> = _mtuResponse

    private fun setMaxcamVersion(deviceVersion: device_version_t) {
        _maxcamVersion.value = deviceVersion
    }

    private var sendTimeoutCallback = Runnable {}
    private var sendTimeoutHandler = Handler()

    private val _onBackPressed = MutableLiveData<Event<Boolean>>()
    val onBackPressed: LiveData<Event<Boolean>> = _onBackPressed

    fun onBackPressed(){
        _onBackPressed.value = Event(true)
    }

    fun setSendTimeout() {
        sendTimeoutHandler.removeCallbacks(sendTimeoutCallback)
        sendTimeoutCallback = Runnable {
            setEmbeddingsSendInProgress(false)
            val context = app.applicationContext
            Toast.makeText(
                context,
                "Send signature failed!",
                Toast.LENGTH_LONG
            ).show()
        }
        sendTimeoutHandler.postDelayed(sendTimeoutCallback, 10000);
    }

    private fun abortSendTimeout() {
        sendTimeoutHandler.removeCallbacks(sendTimeoutCallback)
    }

    @ExperimentalUnsignedTypes
    fun onBleDataReceived(data: ByteArray) {

        val packet_info = ble_packet_info_t.parse(data[0]);
        if (packet_info.type == ble_packet_type_e.BLE_PACKET_TYPE_COMMAND) {

            val commandPacket = ble_command_packet_t.parse(data)

            val blePacket: IBlePacket = commandPacket.header.command.parse(commandPacket.payload)

            when (commandPacket.header.command) {
                ble_command_e.BLE_COMMAND_GET_VERSION_RES -> {
                    val packet: device_version_t = blePacket as device_version_t
                    setMaxcamVersion(packet)
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
                ble_command_e.BLE_COMMAND_MTU_CHANGE_RES -> {
                    val packet: ble_mtu_response = blePacket as ble_mtu_response
                    _mtuResponse.value = packet

                }
                ble_command_e.BLE_COMMAND_FACEID_EMBED_UPDATE_RES -> {
                    val packet: faceid_embed_update_status_e =
                        blePacket as faceid_embed_update_status_e
                    val context = app.applicationContext

                    Toast.makeText(
                        context,
                        if (packet == faceid_embed_update_status_e.FACEID_EMBED_UPDATE_STATUS_SUCCESS)
                            R.string.signature_update_success
                        else
                            R.string.signature_update_fail,
                        Toast.LENGTH_LONG
                    ).show()
                    setEmbeddingsSendInProgress(false)
                    abortSendTimeout()

                }
                else -> {
                }
            }

        } else //packet_info.type == ble_packet_type_e.BLE_PACKET_TYPE_PAYLOAD
        {


        }

        payload = if (payload != null) {
            payload?.concatenate(data)
        } else {
            data
        }
    }
}
