package com.maximintegrated.maxcamandroid.faceId


import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.view.isVisible
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.ViewModelProviders
import androidx.lifecycle.observe
import com.maximintegrated.maxcamandroid.MainViewModel
import com.maximintegrated.maxcamandroid.MainViewModelFactory
import com.maximintegrated.maxcamandroid.MaxCamApplication
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.utils.ROOT_FRAGMENT
import com.maximintegrated.maxcamandroid.exts.replaceFragment
import com.maximintegrated.maxcamandroid.main.MainFragment
import com.maximintegrated.maxcamandroid.view.CustomTreeItem
import com.maximintegrated.maxcamandroid.view.TreeNodeType
import com.maximintegrated.maxcamandroid.view.TreeViewHolder
import com.theartofdev.edmodo.cropper.CropImage
import com.theartofdev.edmodo.cropper.CropImageView
import com.unnamed.b.atv.model.TreeNode
import com.unnamed.b.atv.view.AndroidTreeView
import kotlinx.android.synthetic.main.fragment_face_id_demo.*

/**
 * A simple [Fragment] subclass.
 */
class FaceIdDemoFragment : Fragment() {

    companion object {
        fun newInstance() = FaceIdDemoFragment()
    }

    private lateinit var mainViewModel: MainViewModel
    private lateinit var faceIdViewModel: FaceIdViewModel

    private var previousTreeNode: TreeNode? = null

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_face_id_demo, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        faceIdViewModel = ViewModelProviders.of(requireActivity()).get(FaceIdViewModel::class.java)

        mainViewModel =
            ViewModelProvider(
                requireActivity(),
                MainViewModelFactory(
                    requireActivity().application,
                    (requireActivity().application as MaxCamApplication).maxCamNativeLibrary
                )
            ).get(MainViewModel::class.java)

        initializeViews()

        mainViewModel.treeItemList.observe(viewLifecycleOwner) {
            updateTreeView(it)
        }

        mainViewModel.demoBitmap.observe(viewLifecycleOwner) {
            if (it != null) {
                demoImageView.setImageBitmap(it)
            }
        }
    }

    private fun initializeViews() {
        stepperView.onBackButtonClicked {
            requireActivity().onBackPressed()
        }
        stepperView.onNextButtonClicked {
            requireActivity().replaceFragment(fragment = MainFragment.newInstance(), backStackName = ROOT_FRAGMENT)
        }
        when (faceIdViewModel.selectedScenario?.imageSource) {
            ImageSource.FILE_SYSTEM -> {
                androidViews.isVisible = false
                fileSystemViews.isVisible = true
            }
            ImageSource.ANDROID -> {
                androidViews.isVisible = true
                fileSystemViews.isVisible = false
            }
            else -> {
                androidViews.isVisible = false
                fileSystemViews.isVisible = false
            }
        }

        getContentButton.setOnClickListener {
            mainViewModel.onGetContentButtonClicked()
        }

        selectImageButton.setOnClickListener {
            CropImage.activity()
                .setGuidelines(CropImageView.Guidelines.ON)
                .setAspectRatio(3, 4)
                .setMinCropWindowSize(120, 160)
                .setRequestedSize(120, 160)
                .start(requireContext(), this)
        }
        runDemoButton.setOnClickListener {
            mainViewModel.onRunDemoButtonClicked()
        }

        updateTreeView(arrayListOf())
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == CropImage.CROP_IMAGE_ACTIVITY_REQUEST_CODE) {
            if (resultCode == Activity.RESULT_OK && data != null) {
                val result = CropImage.getActivityResult(data)
                mainViewModel.onDemoImageSelected(result.uri)
            }
        }
    }

    private fun updateTreeView(list: ArrayList<CustomTreeItem>) {
        treeViewLinearLayout.removeAllViews()
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
        treeViewLinearLayout.addView(treeView.view)
    }
}
