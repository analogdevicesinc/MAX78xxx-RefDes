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

import android.content.Context
import android.text.InputFilter
import android.view.LayoutInflater
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import androidx.appcompat.app.AlertDialog
import androidx.recyclerview.widget.DiffUtil
import androidx.recyclerview.widget.ListAdapter
import androidx.recyclerview.widget.RecyclerView
import com.google.android.material.button.MaterialButton
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.utils.DeleteListener
import com.maximintegrated.maxcamandroid.utils.askUserForDeleteOperation
import kotlinx.android.synthetic.main.db_item.view.*
import kotlinx.android.synthetic.main.edit_text_alert_dialog.view.*

class DbAdapter(private val listener: DbListener) :
    ListAdapter<DbModel, DbAdapter.DatabaseViewHolder>(DiffCallback) {

    companion object DiffCallback : DiffUtil.ItemCallback<DbModel>() {

        override fun areItemsTheSame(oldItem: DbModel, newItem: DbModel): Boolean {
            return oldItem === newItem
        }

        override fun areContentsTheSame(oldItem: DbModel, newItem: DbModel): Boolean {
            return oldItem.dbFolder == newItem.dbFolder
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): DatabaseViewHolder {
        return DatabaseViewHolder(parent, listener)
    }

    override fun onBindViewHolder(holder: DatabaseViewHolder, position: Int) {
        holder.bind(getItem(position))
    }

    class DatabaseViewHolder(private val parent: ViewGroup, private val listener: DbListener) :
        RecyclerView.ViewHolder(
            LayoutInflater.from(parent.context).inflate(R.layout.db_item, parent, false)
        ) {
        private val databaseNameTextView: TextView by lazy { itemView.databaseNameTextView }
        private val imageCountTextView: TextView by lazy { itemView.imageCountTextView }
        private val personCountTextView: TextView by lazy { itemView.personCountTextView }
        private val deleteButton: MaterialButton by lazy { itemView.deleteButton }
        private val editImageView: ImageView by lazy { itemView.editImageView }
        private val selectButton: MaterialButton by lazy { itemView.selectButton }

        fun bind(dbModel: DbModel) {
            databaseNameTextView.text = dbModel.dbName
            var imageCount = 0
            for (person in dbModel.persons) {
                imageCount += person.imageCount
            }
            imageCountTextView.text = imageCount.toString()
            personCountTextView.text = dbModel.persons.size.toString()
            deleteButton.setOnClickListener {
                askUserForDeleteOperation(parent.context, listener, dbModel)
            }
            editImageView.setOnClickListener {
                showEditDatabaseDialog(dbModel)
            }
            selectButton.setOnClickListener {
                listener.onDatabaseSelected(dbModel)
            }
        }

        private fun showEditDatabaseDialog(dbModel: DbModel) {
            val context = parent.context
            val alert = AlertDialog.Builder(context)
            val layout = LayoutInflater.from(context).inflate(
                R.layout.edit_text_alert_dialog, parent, false
            )
            layout.dialogTitleTextView.text = context.getString(R.string.rename_database)
            layout.dialogEditText.hint = context.getString(R.string.enter_a_database_name)
            val filterArray = arrayOfNulls<InputFilter>(2)
            filterArray[0] = InputFilter.LengthFilter(15)
            filterArray[1] = AlphaNumericInputFilter()
            layout.dialogEditText.filters = filterArray
            layout.dialogEditText.setText(dbModel.dbName)
            alert.setView(layout)
            alert.setPositiveButton(context.getString(R.string.rename)) { dialog, _ ->
                val name = layout.dialogEditText.text.toString().trim()
                if (name != dbModel.dbName) {
                    listener.onRenameRequested(dbModel, name)
                }
                dialog.dismiss()
            }
            alert.show()
        }
    }
}

interface DbListener : DeleteListener {
    fun onRenameRequested(dbModel: DbModel, name: String)
    fun onDatabaseSelected(dbModel: DbModel)
}