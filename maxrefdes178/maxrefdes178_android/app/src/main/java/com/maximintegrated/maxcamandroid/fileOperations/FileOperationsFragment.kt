package com.maximintegrated.maxcamandroid.fileOperations

import android.app.Activity
import android.content.Intent
import android.graphics.BitmapFactory
import android.os.Bundle
import android.text.SpannableStringBuilder
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.core.content.FileProvider
import androidx.core.text.bold
import androidx.core.view.isVisible
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.ViewModelProviders
import androidx.lifecycle.observe
import com.google.android.material.snackbar.Snackbar
import com.maximintegrated.communication.MaxCamViewModel
import com.maximintegrated.maxcamandroid.*
import com.maximintegrated.maxcamandroid.exts.fromHexStringToMeaningfulAscii
import com.maximintegrated.maxcamandroid.exts.getMaxCamFile
import com.maximintegrated.maxcamandroid.exts.toHexString
import com.maximintegrated.maxcamandroid.exts.toHexToString
import com.maximintegrated.maxcamandroid.nativeLibrary.MaxCamNativeLibrary
import com.maximintegrated.maxcamandroid.view.CustomTreeItem
import com.maximintegrated.maxcamandroid.view.TreeNodeType
import com.maximintegrated.maxcamandroid.view.TreeViewHolder
import com.unnamed.b.atv.model.TreeNode
import com.unnamed.b.atv.view.AndroidTreeView
import kotlinx.android.synthetic.main.activity_main.*
import kotlinx.android.synthetic.main.fragment_file_operations.*
import timber.log.Timber

class FileOperationsFragment : Fragment() {

    companion object {
        private const val PICK_FILE_RESULT_CODE = 143
        fun newInstance() = FileOperationsFragment()
    }

    private lateinit var mainViewModel: MainViewModel
    private lateinit var maxCamViewModel: MaxCamViewModel

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        return inflater.inflate(R.layout.fragment_file_operations, container, false)
    }

    private var previousTreeNode: TreeNode? = null

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        mainViewModel =
            ViewModelProvider(
                requireActivity(),
                MainViewModelFactory(
                    requireActivity().application,
                    MaxCamNativeLibrary.getInstance()
                )
            ).get(MainViewModel::class.java)
        maxCamViewModel = ViewModelProviders.of(requireActivity()).get(MaxCamViewModel::class.java)

        maxCamViewModel.writeStatus.observe(viewLifecycleOwner) {
            val byteSent = it.first
            val totalSize = it.second
            Timber.d("byteSent $byteSent  totalSize $totalSize")
            if (byteSent == -1) {
                Toast.makeText(
                    requireContext(),
                    "Error occurred while sending data.",
                    Toast.LENGTH_SHORT
                ).show()
                if(mainViewModel.fileSendOperation.value == OperationState.PROGRESS){
                    mainViewModel.onFileSendCompleted(OperationState.FAIL)
                }
            } else if (byteSent > 0) {
                if(mainViewModel.fileSendOperation.value == OperationState.PROGRESS){
                    mainViewModel.onFileSendProgress(byteSent)
                }
            }
        }

        mainViewModel.selectedFileName.observe(viewLifecycleOwner) {
            sendFileSelectedTextView.text = it
        }

        mainViewModel.fileSendOperation.observe(viewLifecycleOwner) {
            (requireActivity() as MainActivity).progressBar.isVisible =
                it == OperationState.PROGRESS
            if (it == OperationState.SUCCESS) {
                Toast.makeText(requireContext(), "File is sent successfully", Toast.LENGTH_SHORT)
                    .show()
            }
        }

        mainViewModel.selectedTreeItem.observe(viewLifecycleOwner) {
            getFileTextView.text = it?.text ?: ""
        }

        mainViewModel.treeItemList.observe(viewLifecycleOwner) {
            updateTreeView(it)
        }

        mainViewModel.fileGetOperation.observe(viewLifecycleOwner) {
            (requireActivity() as MainActivity).progressBar.isVisible =
                it == OperationState.PROGRESS
            getFileButton.isEnabled = it != OperationState.PROGRESS
            if (it == OperationState.SUCCESS) {
                mainViewModel.onGetFileCompleted()
                mainViewModel.latestReceivedTreeItem?.let { item ->
                    val dotIndex = item.text.lastIndexOf('.')
                    val intentType: String
                    when (item.text.substring(dotIndex + 1)) {
                        "jpg", "jpeg", "png" -> {
                            mainViewModel.payload?.let { payload ->
                                updateFileContentImageView(payload)
                            }
                            intentType = "image/*"
                        }
                        "txt", "set" -> {
                            mainViewModel.payload?.let { payload ->
                                updateFileContentTextView(payload.toHexToString())
                            }
                            intentType = "text/plain"
                        }
                        else -> {
                            mainViewModel.payload?.let { payload ->
                                updateFileContentTextView(payload)
                            }
                            intentType = "text/plain"
                        }
                    }
                    Snackbar.make(
                        view,
                        item.text + " received.",
                        Snackbar.LENGTH_SHORT
                    )
                        .setAction("Open") {
                            val file = getMaxCamFile(item.text)
                            if (file.exists()) {
                                val intent = Intent(Intent.ACTION_VIEW)
                                val uri = FileProvider.getUriForFile(
                                    requireContext(),
                                    "com.maximintegrated.maxcamandroid.fileprovider",
                                    file
                                )
                                intent.setDataAndType(uri, intentType)
                                intent.flags = Intent.FLAG_GRANT_READ_URI_PERMISSION
                                startActivity(intent)
                            }
                        }
                        .show()
                }
            }
        }

        selectFileButton.setOnClickListener {
            var chooseFileIntent = Intent(Intent.ACTION_GET_CONTENT)
            chooseFileIntent.type = "*/*"
            chooseFileIntent =
                Intent.createChooser(chooseFileIntent, getString(R.string.choose_a_file))
            startActivityForResult(chooseFileIntent, PICK_FILE_RESULT_CODE)
        }

        sendFileButton.setOnClickListener {
            mainViewModel.onFileSendButtonClicked()
        }

        getContentButton.setOnClickListener {
            mainViewModel.onGetContentButtonClicked()
        }

        getFileButton.setOnClickListener {
            mainViewModel.onGetFileButtonClicked()
        }

        updateTreeView(arrayListOf())
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == PICK_FILE_RESULT_CODE) {
            if (resultCode == Activity.RESULT_OK) {
                mainViewModel.onFileSelected(data?.data)
            }
        }
    }

    private fun updateTreeView(list: ArrayList<CustomTreeItem>) {
        contentTreeViewLinearLayout.removeAllViews()
        previousTreeNode = null
        val rootNode = TreeNode.root()
        var sdCardTitle = "ME14 - SD STORAGE"
        if (list.isEmpty()) {
            sdCardTitle += "(Not ready!)"
        }
        val sdCard =
            TreeNode(CustomTreeItem(TreeNodeType.SD_STORAGE, sdCardTitle, 0)).setViewHolder(
                TreeViewHolder(requireContext())
            )

        for (item in list) {
            val node = TreeNode(item).setViewHolder(TreeViewHolder(requireContext()))
            sdCard.addChild(node)
        }

        rootNode.addChild(sdCard)
        val treeView = AndroidTreeView(requireContext(), rootNode)
        treeView.setDefaultAnimation(true)
        treeView.setDefaultContainerStyle(R.style.TreeNodeStyleCustom)
        treeView.setDefaultViewHolder(TreeViewHolder::class.java)
        treeView.setDefaultNodeClickListener { node, value ->
            if (node.isLeaf) {
                (previousTreeNode?.viewHolder as? TreeViewHolder)?.changeBackground(false)
                val item = value as CustomTreeItem
                previousTreeNode = if (node == previousTreeNode) {
                    mainViewModel.onTreeItemDeselected()
                    null
                } else {
                    (node.viewHolder as TreeViewHolder).changeBackground(true)
                    mainViewModel.onTreeItemSelected(item)
                    node
                }
            }

        }
        treeView.setDefaultNodeLongClickListener { node, value ->
            if (node.isLeaf) {
                (previousTreeNode?.viewHolder as? TreeViewHolder)?.changeBackground(false)
                val item = value as CustomTreeItem
                previousTreeNode = if (node == previousTreeNode) {
                    mainViewModel.onTreeItemDeselected()
                    null
                } else {
                    (node.viewHolder as TreeViewHolder).changeBackground(true)
                    mainViewModel.onTreeItemSelected(item)
                    node
                }
            }
            return@setDefaultNodeLongClickListener true
        }
        contentTreeViewLinearLayout.addView(treeView.view)
    }

    private fun updateFileContentTextView(text: String) {
        contentDisplayImageView.isVisible = false
        contentDisplayTextView.isVisible = true
        contentDisplayTextView.text = text
    }

    private fun updateFileContentTextView(data: ByteArray) {
        contentDisplayImageView.isVisible = false
        contentDisplayTextView.isVisible = true
        val sb = SpannableStringBuilder()
        if (data.size > 256) {
            val first = data.sliceArray(0..127)
            val second = data.sliceArray((data.size - 128) until data.size)

            val hex1 = first.toHexString()
            val ascii1 = hex1.fromHexStringToMeaningfulAscii()
            val hex2 = second.toHexString()
            val ascii2 = hex2.fromHexStringToMeaningfulAscii()
            sb.bold { appendLine("Hex Content:") }
                .append(hex1).appendLine(" ...").append("... ").appendLine(hex2).appendLine()
            sb.bold { appendLine("Ascii Content:") }
                .append(ascii1).appendLine(" ...").append("... ").appendLine(ascii2)
        } else {
            val hex = data.toHexString()
            val ascii = hex.fromHexStringToMeaningfulAscii()
            sb.bold { appendLine("Hex Content:") }.appendLine(hex).appendLine()
            sb.bold { appendLine("Ascii Content") }.appendLine(ascii)
        }
        contentDisplayTextView.text = sb
    }

    private fun updateFileContentImageView(data: ByteArray) {
        contentDisplayImageView.isVisible = true
        contentDisplayTextView.isVisible = false
        val bitmap = BitmapFactory.decodeByteArray(data, 0, data.size)
        Timber.d("bitmap w*h = ${bitmap.width} x ${bitmap.height}")
        contentDisplayImageView.setImageBitmap(bitmap)
    }
}
