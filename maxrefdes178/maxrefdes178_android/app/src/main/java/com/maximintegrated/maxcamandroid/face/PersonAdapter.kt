package com.maximintegrated.maxcamandroid.face

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

        override fun onAddImageClicked(person: PersonModel) {
            listener.onAddImageClicked(person)
        }

    }
}

interface PersonListener : PersonImageListener, DeleteListener {
    fun onRenameRequested(personModel: PersonModel, name: String)
    fun onPersonDeleteRequested(personModel: PersonModel)
}