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

package com.maximintegrated.maxcamandroid.face

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.view.isVisible
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProviders
import androidx.lifecycle.observe
import com.google.android.material.snackbar.Snackbar
import com.maximintegrated.communication.MaxCamViewModel
import com.maximintegrated.maxcamandroid.MainViewModel
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.utils.EventObserver
import kotlinx.android.synthetic.main.fragment_demo.*
import kotlinx.android.synthetic.main.fragment_demo.view.*
import java.text.SimpleDateFormat
import java.util.*
import com.maximintegrated.maxcamandroid.utils.*


class DemoFragment : Fragment() {

    companion object {
        fun newInstance() = DemoFragment()
    }

    private lateinit var faceIdViewModel: FaceIdViewModel
    private lateinit var maxCamViewModel: MaxCamViewModel
    private lateinit var mainViewModel: MainViewModel


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
        mainViewModel = ViewModelProviders.of(requireActivity()).get(MainViewModel::class.java)

        faceIdViewModel.warningEvent.observe(viewLifecycleOwner, EventObserver {
            Snackbar.make(view, it, Snackbar.LENGTH_SHORT).show()
        })

        faceIdViewModel.scriptInProgress.observe(viewLifecycleOwner) {
            val sdf = SimpleDateFormat("MM/dd/yyyy HH:mm", Locale.getDefault())
            if (it) {
                generatedSignatureTitle.text = getString(
                    R.string.script_in_progress
                )
                sendButton.isEnabled = false
                embeddingResultCardContent.visibility = View.GONE
            } else {
                val file = faceIdViewModel.selectedDatabase?.embeddingsFile
                if (file == null) {
                    generatedSignatureTitle.text = getString(
                        R.string.signature_not_found
                    )
                    sendButton.isEnabled = false
                    embeddingResultCardContent.visibility = View.GONE
                } else {
                    signGenDateTV.text = getString(
                        R.string.signature_created_at,
                        sdf.format(Date(file.lastModified()))
                    )
                    sendButton.isEnabled = true
                    generatedSignatureTitle.text = getString(
                        R.string.signature_generated
                    )
                    embeddingResultCardContent.visibility = View.VISIBLE
                    embeddingResultCard.goodResultCount.text = " : " +
                            faceIdViewModel.selectedDatabase?.embeddingsResult?.subjects?.sumBy {
                                if (it.goodPhotos != null) {
                                    it.goodPhotos!!.size
                                } else {
                                    0
                                }
                            }

                    val badPhotoCount = faceIdViewModel.selectedDatabase?.embeddingsResult?.subjects?.sumBy {
                        if (it.badPhotos != null) {
                            it.badPhotos!!.size
                        } else {
                            0
                        }
                    }
                    embeddingResultCard.badResultCount.text = " : " + badPhotoCount
                    suggestConstraint.visibility =if( badPhotoCount == 0) View.GONE else View.VISIBLE

                }

            }
            progressBar.isVisible = it
            signatureButton.isEnabled = !it

        }
        previousPageButton.setOnClickListener { mainViewModel.onBackPressed() }

        faceIdViewModel.embeddingsFileEvent.observe(viewLifecycleOwner, EventObserver {
            val sdf = SimpleDateFormat("MM/dd/yyyy HH:mm", Locale.getDefault())

            if (!it.isDirectory) {
                signGenDateTV.visibility = View.VISIBLE
                signGenDateTV.text = getString(
                    R.string.signature_created_at,
                    sdf.format(Date(it.lastModified()))
                )
                generatedSignatureTitle.text = getString(
                    R.string.signature_generated
                )
            } else {
                embeddingResultCardContent.visibility = View.GONE
                generatedSignatureTitle.text = getString(
                    R.string.signature_not_found
                )
            }

            progressBar.isVisible = false
        })

        signatureButton.setOnClickListener {
            progressBar.isVisible = true
            //faceIdViewModel.selectedDatabase?.embeddingsFile?.delete()
            faceIdViewModel.onCreateSignatureButtonClicked()
        }
        faceIdViewModel.getEmbeddingsFile()

        sendButton.setOnClickListener {
            faceIdViewModel.selectedDatabase!!.embeddingsFile?.let {
                sendEmbeddings(it, maxCamViewModel, mainViewModel, requireContext())
            }
        }

        mainViewModel.embeddingsSendInProgress.observe(viewLifecycleOwner) {
            sendButton.isEnabled = false
            progressBar.isVisible = true
            signatureButton.isEnabled = false
            if (!it) {
                faceIdViewModel.selectedDatabase?.embeddingsFile?.let { file ->
                    if (!file.isDirectory) {
                        sendButton.isEnabled = true
                    }
                }
                faceIdViewModel.scriptInProgress.value?.let { inProgress ->
                    progressBar.isVisible = inProgress
                    signatureButton.isEnabled = !inProgress
                }
            }
        }

    }


}