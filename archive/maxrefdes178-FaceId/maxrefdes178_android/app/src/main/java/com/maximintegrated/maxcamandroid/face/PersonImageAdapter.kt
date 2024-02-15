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

import android.opengl.Visibility
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup

import androidx.recyclerview.widget.DiffUtil
import androidx.recyclerview.widget.ListAdapter
import androidx.recyclerview.widget.RecyclerView
import com.bumptech.glide.Glide
import com.google.android.material.card.MaterialCardView
import com.maximintegrated.maxcamandroid.R
import kotlinx.android.synthetic.main.person_image_item.view.*
import java.io.File

class PersonImageAdapter(
    private val listener: PersonImageListener,
    private val personModel: PersonModel
) :
    ListAdapter<File, PersonImageAdapter.PersonImageViewHolder>(DiffCallback) {

    companion object DiffCallback : DiffUtil.ItemCallback<File>() {

        override fun areItemsTheSame(oldItem: File, newItem: File): Boolean {
            return oldItem === newItem
        }

        override fun areContentsTheSame(oldItem: File, newItem: File): Boolean {
            return oldItem == newItem
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): PersonImageViewHolder {
        return PersonImageViewHolder(parent, listener, personModel)
    }

    override fun onBindViewHolder(holder: PersonImageViewHolder, position: Int) {
        holder.bind(getItem(position))
    }

    class PersonImageViewHolder(
        private val parent: ViewGroup,
        private val listener: PersonImageListener,
        private val personModel: PersonModel
    ) :
        RecyclerView.ViewHolder(
            LayoutInflater.from(parent.context).inflate(R.layout.person_image_item, parent, false)
        ) {
        private val imageCard: MaterialCardView by lazy { itemView.image_card }


        fun bind(imageFile: File) {

            if (imageFile.isDirectory) {
                Glide.with(parent.context).load(R.drawable.ic_add_black).centerCrop()
                    .placeholder(R.drawable.ic_add_black).into(itemView.image_card.imageView)
                imageCard.setOnClickListener {
                    listener.onAddImageClicked(imageFile)
                }
            } else {
                Glide.with(parent.context).load(imageFile).centerCrop()
                    .placeholder(R.drawable.ic_add_black).into(itemView.image_card.imageView)
                imageCard.setOnClickListener {
                    listener.onImageClicked(imageFile, personModel)
                }
                if (personModel.goodPhotos.contains(imageFile.name)) {
                    imageCard.good.visibility = View.VISIBLE
                    /*
                    imageCard.strokeWidth = convertDpToPx(parent.context, 3f).toInt()
                    imageCard.strokeColor = ContextCompat.getColor(parent.context, R.color.color_success)
                     */
                } else if (personModel.badPhotos.contains(imageFile.name)) {
                    /*
                    imageCard.strokeWidth = convertDpToPx(parent.context, 3f).toInt()
                    imageCard.strokeColor = ContextCompat.getColor(parent.context, R.color.color_error)
                     */
                    imageCard.bad.visibility = View.VISIBLE
                }
            }


        }

    }
}

interface PersonImageListener {
    fun onImageClicked(imageFile: File, person: PersonModel)
    fun onAddImageClicked(tempImageFile: File)
}