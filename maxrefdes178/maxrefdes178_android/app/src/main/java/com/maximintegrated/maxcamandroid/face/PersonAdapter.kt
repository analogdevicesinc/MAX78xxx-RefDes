package com.maximintegrated.maxcamandroid.face

import android.view.LayoutInflater
import android.view.ViewGroup
import android.widget.TextView
import androidx.appcompat.app.AlertDialog
import androidx.recyclerview.widget.DiffUtil
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.ListAdapter
import androidx.recyclerview.widget.RecyclerView
import com.maximintegrated.maxcamandroid.R
import kotlinx.android.synthetic.main.edit_text_alert_dialog.view.*
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
        private val recyclerView: RecyclerView by lazy { itemView.recyclerView }
        private lateinit var personModel: PersonModel

        fun bind(model: PersonModel) {
            personModel = model
            val layoutManager =
                LinearLayoutManager(recyclerView.context, LinearLayoutManager.HORIZONTAL, false)
            layoutManager.initialPrefetchItemCount = personModel.images.size
            val personImageAdapter = PersonImageAdapter(this, model)
            personImageAdapter.submitList(personModel.images)
            recyclerView.layoutManager = layoutManager
            recyclerView.adapter = personImageAdapter
            recyclerView.setRecycledViewPool(pool)


            personNameTextView.text = personModel.nameSurname
//            personModel.images.observe(viewLifecycleOwner) {
//                dbAdapter.submitList(it)
//                emptyViews.isVisible = it.isEmpty()
//                recyclerView.isVisible = it.isNotEmpty()
//            }
//            personNameTextView.text = PersonModel.personFile.nameWithoutExtension

        }


        private fun showEditDatabaseDialog(PersonModel: PersonModel) {
            val context = parent.context
            val alert = AlertDialog.Builder(context)
            val layout = LayoutInflater.from(context).inflate(
                R.layout.edit_text_alert_dialog, parent, false
            )
            layout.dialogTitleTextView.text = context.getString(R.string.rename_database)
            layout.dialogEditText.hint = context.getString(R.string.enter_a_database_name)
            layout.dialogEditText.setText(PersonModel.personFolder.nameWithoutExtension)
            alert.setView(layout)
            alert.setPositiveButton(context.getString(R.string.rename)) { dialog, _ ->
                val name = layout.dialogEditText.text.toString().trim()
                if (name != PersonModel.personFolder.nameWithoutExtension) {
                    listener.onRenameRequested(PersonModel, name)
                }
                dialog.dismiss()
            }
            alert.show()
        }

        override fun onDeleteButtonClicked(imageFile: File, person: PersonModel) {
            listener.onDeleteButtonClicked(imageFile, person)
        }

        override fun onImageClicked(imageFile: File, person: PersonModel) {
            listener.onImageClicked(imageFile, person)
        }

        override fun onAddImageClicked(person: PersonModel) {
            listener.onAddImageClicked(person)
        }


        //PersonImageAdapter listener methods


    }
}

interface PersonListener : PersonImageListener {
    fun onDeleteButtonClicked(personModel: PersonModel)
    fun onRenameRequested(personModel: PersonModel, name: String)
    fun onDatabaseSelected(personModel: PersonModel)
}