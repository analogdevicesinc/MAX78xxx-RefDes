package com.maximintegrated.maxcamandroid.settings

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
import com.maximintegrated.communication.MaxCamViewModel
import com.maximintegrated.maxcamandroid.MainViewModel
import com.maximintegrated.maxcamandroid.MainViewModelFactory
import com.maximintegrated.maxcamandroid.MaxCamApplication
import com.maximintegrated.maxcamandroid.blePacket.ble_command_e
import com.maximintegrated.maxcamandroid.databinding.FragmentSettingsBinding
import com.maximintegrated.maxcamandroid.utils.SettingsItemListener
import com.maximintegrated.maxcamandroid.utils.setup
import com.theartofdev.edmodo.cropper.CropImage
import com.unnamed.b.atv.model.TreeNode

class SettingsFragment : Fragment() {

    companion object {
        fun newInstance() = SettingsFragment()
    }

    private lateinit var mainViewModel: MainViewModel
    private lateinit var maxCamViewModel: MaxCamViewModel

    lateinit var rootNode: TreeNode
        private set

    private var previousTreeNode: TreeNode? = null

    private var _binding: FragmentSettingsBinding? = null
    private val binding get() = _binding!!

    private var settingsItemListener: SettingsItemListener = object : SettingsItemListener {
        override fun onEnableButtonClick(itemName: String, command: ble_command_e) {
            //maxCamViewModel.sendData()
        }

        override fun onDisableButtonClick(itemName: String, command: ble_command_e) {
            //maxCamViewModel.sendData()
        }
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        _binding = FragmentSettingsBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        mainViewModel =
            ViewModelProvider(
                requireActivity(),
                MainViewModelFactory(
                    requireActivity().application,
                    (requireActivity().application as MaxCamApplication).maxCamNativeLibrary
                )
            ).get(MainViewModel::class.java)

        maxCamViewModel = ViewModelProviders.of(requireActivity()).get(MaxCamViewModel::class.java)

        binding.max78000VideoCnnItem.setup("MAX78000 VIDEO CNN",ble_command_e.BLE_COMMAND_ENABLE_MAX78000_VIDEO_CNN_CMD, ble_command_e.BLE_COMMAND_DISABLE_MAX78000_VIDEO_CNN_CMD, settingsItemListener)
        binding.max78000VideoFlashLedItem.setup("MAX78000 VIDEO Flash Led",ble_command_e.BLE_COMMAND_ENABLE_MAX78000_VIDEO_FLASH_LED_CMD, ble_command_e.BLE_COMMAND_DISABLE_MAX78000_VIDEO_FLASH_LED_CMD, settingsItemListener)

        /*
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
*/
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == CropImage.CROP_IMAGE_ACTIVITY_REQUEST_CODE) {
            if (resultCode == Activity.RESULT_OK && data != null) {
                val result = CropImage.getActivityResult(data)
                mainViewModel.onImageSelected(result!!.uri)
                Toast.makeText(
                    requireContext(),
                    "Cropping successful!",
                    Toast.LENGTH_SHORT
                ).show()
            }
        }
    }
/*
    private fun updateTreeView(list: ArrayList<CustomTreeItem>) {
        diagnosticsTreeViewLinearLayout.removeAllViews()
        previousTreeNode = null
        rootNode = TreeNode.root()
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
        diagnosticsTreeViewLinearLayout.addView(treeView.view)
        treeView.expandAll()
    }
*/
}
