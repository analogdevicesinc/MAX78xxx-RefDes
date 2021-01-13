package com.maximintegrated.maxcamandroid.face

import android.view.LayoutInflater
import android.view.ViewGroup
import androidx.appcompat.app.AlertDialog
import androidx.cardview.widget.CardView
import androidx.recyclerview.widget.DiffUtil
import androidx.recyclerview.widget.ListAdapter
import androidx.recyclerview.widget.RecyclerView
import com.bumptech.glide.Glide
import com.maximintegrated.maxcamandroid.R
import kotlinx.android.synthetic.main.edit_text_alert_dialog.view.*
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
        private val imageCard: CardView by lazy { itemView.image_card }


        fun bind(imageFile: File) {

            if (imageFile.isDirectory) {
                imageCard.setOnClickListener {
                    listener.onAddImageClicked(personModel)
                }
            } else {
                Glide.with(parent.context).load(imageFile).centerCrop()
                    .placeholder(R.drawable.ic_add_black).into(itemView.image_card.imageView)
                imageCard.setOnClickListener {
                    listener.onImageClicked(imageFile, personModel)
                }
            }


        }

    }
}

interface PersonImageListener {
    fun onImageClicked(imageFile: File, person: PersonModel)
    fun onAddImageClicked(person: PersonModel)
}