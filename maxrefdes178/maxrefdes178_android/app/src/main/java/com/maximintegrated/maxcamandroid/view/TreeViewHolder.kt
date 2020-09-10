package com.maximintegrated.maxcamandroid.view

import android.content.Context
import android.graphics.Color
import android.view.LayoutInflater
import android.view.View
import android.widget.ImageView
import android.widget.TextView
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.core.view.isInvisible
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.exts.color
import com.unnamed.b.atv.model.TreeNode

class TreeViewHolder(context: Context) : TreeNode.BaseNodeViewHolder<CustomTreeItem>(context) {

    private var imageView: ImageView? = null
    private var arrowView: ImageView? = null
    private var textView: TextView? = null
    private var layout: ConstraintLayout? = null

    private var defaultTextColor = 0

    override fun createNodeView(node: TreeNode?, value: CustomTreeItem?): View {
        val inflater = LayoutInflater.from(context)
        val view = inflater.inflate(R.layout.tree_view_item, null, false)
        imageView = view.findViewById(R.id.node_icon)
        arrowView = view.findViewById(R.id.node_arrow_icon)
        textView = view.findViewById(R.id.node_text_view)
        layout = view.findViewById(R.id.node_layout)
        var text = value?.text
        if (node?.isLeaf == true) {
            text += " (" + value?.size + " bytes)"
            arrowView?.isInvisible = true
        }
        textView?.text = text
        defaultTextColor = textView!!.currentTextColor

        val imageId = when (value?.type) {
            TreeNodeType.SD_STORAGE -> R.drawable.ic_sd_storage
            TreeNodeType.FOLDER_FULL -> R.drawable.ic_folder_full
            TreeNodeType.FOLDER_EMPTY -> R.drawable.ic_folder_empty
            TreeNodeType.FOLDER_PHOTO -> R.drawable.ic_photo_library
            TreeNodeType.FILE_IMAGE -> R.drawable.ic_image
            TreeNodeType.FILE_TEXT -> R.drawable.ic_text_file
            else -> R.drawable.ic_file
        }

        imageView?.setImageResource(imageId)

        return view
    }

    override fun toggle(active: Boolean) {
        arrowView?.setImageResource(if (active) R.drawable.ic_arrow_down else R.drawable.ic_arrow_right)
    }

    fun changeBackground(isSelected: Boolean) {
        layout?.setBackgroundColor(if (isSelected) context.color(R.color.color_purple) else Color.TRANSPARENT)
        textView?.setTextColor(if(isSelected) Color.WHITE else defaultTextColor)
    }
}