package com.maximintegrated.maxcamandroid.face

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.view.isVisible
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProviders
import com.maximintegrated.communication.MaxCamViewModel
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.blePacket.*
import com.maximintegrated.maxcamandroid.utils.EventObserver
import kotlinx.android.synthetic.main.fragment_db_edit.stepperView
import kotlinx.android.synthetic.main.fragment_demo.*
import java.io.File
import java.text.SimpleDateFormat
import java.util.*
import kotlin.math.min


class DemoFragment : Fragment() {

    companion object {
        fun newInstance() = DemoFragment()
    }

    private lateinit var faceIdViewModel: FaceIdViewModel
    private lateinit var maxCamViewModel: MaxCamViewModel


    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_demo, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        faceIdViewModel = ViewModelProviders.of(requireActivity()).get(FaceIdViewModel::class.java)
        maxCamViewModel = ViewModelProviders.of(requireActivity()).get(MaxCamViewModel::class.java)


        faceIdViewModel.scriptInProgress.observe(viewLifecycleOwner) {
            val sdf = SimpleDateFormat("MM/dd/yyyy HH:mm", Locale.getDefault())
            if (it) {
                embeddingsTextView.text = getString(
                    R.string.script_in_progress
                )
                sendButton.isEnabled = false
            } else {
                val file = faceIdViewModel.selectedDatabase?.embeddingsFile
                if (file == null) {
                    embeddingsTextView.text = getString(
                        R.string.signature_not_found
                    )
                    sendButton.isEnabled = false
                } else {
                    embeddingsTextView.text = getString(
                        R.string.signature_created_at,
                        sdf.format(Date(file.lastModified()))
                    )
                    sendButton.isEnabled = true
                }

            }

            progressBar.isVisible = it
            signatureButton.isEnabled = !it

        }

        faceIdViewModel.embeddingsFileEvent.observe(viewLifecycleOwner, EventObserver {
            val sdf = SimpleDateFormat("MM/dd/yyyy HH:mm", Locale.getDefault())

            embeddingsTextView.text = if (!it.isDirectory) getString(
                R.string.signature_created_at,
                sdf.format(Date(it.lastModified()))
            )
            else
                getString(
                    R.string.signature_not_found
                )

            progressBar.isVisible = false
        })
        stepperView.onBackButtonClicked {
            requireActivity().onBackPressed()
        }
        stepperView.onNextButtonClicked {
            //faceIdViewModel.onDemoFragmentRequested()
        }
        signatureButton.setOnClickListener {
            progressBar.isVisible = true
            //faceIdViewModel.selectedDatabase?.embeddingsFile?.delete()
            faceIdViewModel.onCreateSignatureButtonClicked()
        }
        faceIdViewModel.getEmbeddingsFile()
        sendButton.setOnClickListener {
            faceIdViewModel.selectedDatabase!!.embeddingsFile?.let {
                sendEmbeddings(it)
            }
        }
    }

    fun sendEmbeddings(embeddingsFile: File) {
        var embeddingsArr: ByteArray = embeddingsFile.readBytes()


        var command_packet_payload_size: Int =
            maxCamViewModel.packetSize - ble_command_packet_header_t.size()
        var payload_packet_payload_size: Int =
            maxCamViewModel.packetSize - ble_payload_packet_header_t.size()
        var spentPayloadSize = 0
        var remainingSize = embeddingsArr.size


        var commandPacket = ble_command_packet_t.from(
            ble_command_e.BLE_COMMAND_FACEID_EMBED_UPDATE_CMD,
            embeddingsArr,
            min(command_packet_payload_size, embeddingsArr.size),
            embeddingsArr.size
        )

        maxCamViewModel.sendData(commandPacket.toByteArray())

        spentPayloadSize += command_packet_payload_size
        remainingSize -= command_packet_payload_size
        while (remainingSize > 0) {
            var payloadPacket = ble_payload_packet_t.from(
                embeddingsArr.sliceArray(spentPayloadSize until embeddingsArr.size),
                min(payload_packet_payload_size, remainingSize)
            )
            maxCamViewModel.sendData(payloadPacket.toByteArray())
            spentPayloadSize += payload_packet_payload_size
            remainingSize -= payload_packet_payload_size
        }

    }
}