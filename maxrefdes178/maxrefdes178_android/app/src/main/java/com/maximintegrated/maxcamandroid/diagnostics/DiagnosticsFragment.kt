package com.maximintegrated.maxcamandroid.diagnostics

import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.ViewModelProviders
import androidx.lifecycle.observe
import com.maximintegrated.communication.MaxCamViewModel
import com.maximintegrated.maxcamandroid.MainViewModel
import com.maximintegrated.maxcamandroid.MainViewModelFactory
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.nativeLibrary.MaxCamNativeLibrary
import com.maximintegrated.maxcamandroid.view.CustomTreeItem
import com.maximintegrated.maxcamandroid.view.TreeNodeType
import com.maximintegrated.maxcamandroid.view.TreeViewHolder
import com.theartofdev.edmodo.cropper.CropImage
import com.theartofdev.edmodo.cropper.CropImageView
import com.unnamed.b.atv.model.TreeNode
import com.unnamed.b.atv.view.AndroidTreeView
import kotlinx.android.synthetic.main.fragment_diagnostics.*

class DiagnosticsFragment : Fragment() {

    companion object {
        fun newInstance() = DiagnosticsFragment()
    }

    private lateinit var mainViewModel: MainViewModel
    private lateinit var maxCamViewModel: MaxCamViewModel

    private var previousTreeNode: TreeNode? = null

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        return inflater.inflate(R.layout.fragment_diagnostics, container, false)
    }

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

        mainViewModel.selectedTreeItem.observe(viewLifecycleOwner) {
            loadImageTextView.text = it?.text ?: ""
        }

        mainViewModel.treeItemList.observe(viewLifecycleOwner) {
            updateTreeView(it)
        }

        getContentButton.setOnClickListener {
            mainViewModel.onGetContentButtonClicked()
        }

        loadImageButton.setOnClickListener {
            mainViewModel.onLoadImageButtonClicked()
        }

        enterDemoButton.setOnClickListener {
            mainViewModel.onEnterDemoButtonClicked()
        }

        captureImageButton.setOnClickListener {
            mainViewModel.onCaptureImageButtonClicked()
        }

        sendImageButton.setOnClickListener {
            CropImage.activity()
                .setGuidelines(CropImageView.Guidelines.ON)
                .setAspectRatio(3, 4)
                .setMinCropWindowSize(120, 160)
                .setRequestedSize(120, 160)
                .start(requireContext(), this)
        }

        sendTestBytesButton.setOnClickListener {
            val array = ByteArray(30) {
                (it + 1).toByte()
            }
            maxCamViewModel.sendData(array)
        }

        updateTreeView(arrayListOf())
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == CropImage.CROP_IMAGE_ACTIVITY_REQUEST_CODE) {
            if (resultCode == Activity.RESULT_OK && data != null) {
                val result = CropImage.getActivityResult(data)
                mainViewModel.onImageSelected(result.uri)
                Toast.makeText(
                    requireContext(),
                    "Cropping successful, Sample: ",
                    Toast.LENGTH_SHORT
                ).show()
            }
        }
    }

    private fun updateTreeView(list: ArrayList<CustomTreeItem>) {
        diagnosticsTreeViewLinearLayout.removeAllViews()
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
        diagnosticsTreeViewLinearLayout.addView(treeView.view)
    }

}
