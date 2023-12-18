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

import android.text.InputFilter
import android.view.LayoutInflater
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import androidx.appcompat.app.AlertDialog
import androidx.recyclerview.widget.DiffUtil
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.ListAdapter
import androidx.recyclerview.widget.RecyclerView
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.utils.DeleteListener
import com.maximintegrated.maxcamandroid.utils.askUserForDeleteOperation
import kotlinx.android.synthetic.main.edit_text_alert_dialog.view.*
import kotlinx.android.synthetic.main.fragment_image_view.*
import kotlinx.android.synthetic.main.person_item.view.*
import java.io.File

class PersonAdapter(private val listener: PersonListener) :
    ListAdapter<PersonModel, PersonAdapter.PersonViewHolder>(DiffCallback) {
    val recyclerViewPool = RecyclerView.RecycledViewPool()

    companion object DiffCallback : DiffUtil.ItemCallback<PersonModel>() {

        override fun areItemsTheSame(oldItem: PersonModel, newItem: PersonModel): Boolean {
            return oldItem === newItem
        }

        override fun areContentsTheSame(oldItem: PersonModel, newItem: PersonModel): Boolean {
            return oldItem.personFolder == newItem.personFolder
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): PersonViewHolder {
        return PersonViewHolder(parent, listener, recyclerViewPool)
    }

    override fun onBindViewHolder(holder: PersonViewHolder, position: Int) {
        holder.bind(getItem(position))
    }

    class PersonViewHolder(
        private val parent: ViewGroup,
        private val listener: PersonListener,
        private val pool: RecyclerView.RecycledViewPool
    ) :
        RecyclerView.ViewHolder(
            LayoutInflater.from(parent.context).inflate(R.layout.person_item, parent, false)
        ), PersonImageListener {


        private val personNameTextView: TextView by lazy { itemView.databaseNameTextView }
        private val personNameEditImageView: ImageView by lazy { itemView.editImageView }
        private val recyclerView: RecyclerView by lazy { itemView.recyclerView }
        private val deleteImageView: ImageView by lazy { itemView.deleteImageView }

        fun bind(model: PersonModel) {
            val layoutManager =
                LinearLayoutManager(recyclerView.context, LinearLayoutManager.HORIZONTAL, false)
            layoutManager.initialPrefetchItemCount = model.images.size
            val personImageAdapter = PersonImageAdapter(this, model)
            personImageAdapter.submitList(model.images)
            recyclerView.layoutManager = layoutManager
            recyclerView.adapter = personImageAdapter
            recyclerView.setRecycledViewPool(pool)

            personNameTextView.text = model.nameSurname
            personNameEditImageView.setOnClickListener { showEditPersonDialog(model) }
            deleteImageView.setOnClickListener {
                askUserForDeleteOperation(
                    parent.context,
                    listener,
                    model
                )
            }
        }

        private fun showEditPersonDialog(person: PersonModel) {
            val context = parent.context
            val alert = AlertDialog.Builder(context)
            val layout = LayoutInflater.from(context).inflate(
                R.layout.edit_text_alert_dialog, parent, false
            )
            layout.dialogTitleTextView.text = context.getString(R.string.rename_person)
            layout.dialogEditText.hint = context.getString(R.string.enter_a_person_name)
            val filterArray = arrayOfNulls<InputFilter>(2)
            filterArray[0] = InputFilter.LengthFilter(15)
            filterArray[1] = AlphaNumericInputFilter()
            layout.dialogEditText.filters = filterArray
            layout.dialogEditText.setText(person.nameSurname)
            alert.setView(layout)
            alert.setPositiveButton(context.getString(R.string.rename)) { dialog, _ ->
                val name = layout.dialogEditText.text.toString().trim()
                if (name != person.nameSurname) {
                    listener.onRenameRequested(person, name)
                }
                dialog.dismiss()
            }
            alert.show()
        }

        override fun onImageClicked(imageFile: File, person: PersonModel) {
            listener.onImageClicked(imageFile, person)
        }

        override fun onAddImageClicked(tempImageFile: File) {
            listener.onAddImageClicked(tempImageFile)
        }

    }
}

interface PersonListener : PersonImageListener, DeleteListener {
    fun onRenameRequested(personModel: PersonModel, name: String)
    fun onPersonDeleteRequested(personModel: PersonModel)
}