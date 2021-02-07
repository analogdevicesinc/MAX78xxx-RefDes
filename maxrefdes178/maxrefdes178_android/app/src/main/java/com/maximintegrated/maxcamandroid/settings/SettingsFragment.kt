package com.maximintegrated.maxcamandroid.settings

import android.R
import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ArrayAdapter
import android.widget.Spinner
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.ViewModelProviders
import com.maximintegrated.communication.MaxCamViewModel
import com.maximintegrated.maxcamandroid.MainViewModel
import com.maximintegrated.maxcamandroid.MainViewModelFactory
import com.maximintegrated.maxcamandroid.MaxCamApplication
import com.maximintegrated.maxcamandroid.blePacket.ble_command_e
import com.maximintegrated.maxcamandroid.blePacket.ble_command_packet_t
import com.maximintegrated.maxcamandroid.blePacket.debugger_select_e
import com.maximintegrated.maxcamandroid.databinding.FragmentSettingsBinding
import com.maximintegrated.maxcamandroid.utils.SettingsItemListener
import com.maximintegrated.maxcamandroid.utils.setup
import com.maximintegrated.maxcamandroid.utils.startScannerActivity
import com.theartofdev.edmodo.cropper.CropImage
import com.unnamed.b.atv.model.TreeNode
import kotlinx.android.synthetic.main.fragment_settings.*

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
            maxCamViewModel.sendData(ble_command_packet_t.from(command).toByteArray())
        }

        override fun onDisableButtonClick(itemName: String, command: ble_command_e) {
            maxCamViewModel.sendData(ble_command_packet_t.from(command).toByteArray())
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

        binding.max78000VideoCnnItem.setup(
            "Video CNN",
            ble_command_e.BLE_COMMAND_ENABLE_MAX78000_VIDEO_CNN_CMD,
            ble_command_e.BLE_COMMAND_DISABLE_MAX78000_VIDEO_CNN_CMD,
            settingsItemListener
        )

        binding.max78000VideoFlashLedItem.setup(
            "Video Flash LED",
            ble_command_e.BLE_COMMAND_ENABLE_MAX78000_VIDEO_FLASH_LED_CMD,
            ble_command_e.BLE_COMMAND_DISABLE_MAX78000_VIDEO_FLASH_LED_CMD,
            settingsItemListener
        )

        binding.lcdSetting.setup(
            "LCD",
            ble_command_e.BLE_COMMAND_ENABLE_LCD_CMD,
            ble_command_e.BLE_COMMAND_DISABLE_LCD_CMD,
            settingsItemListener
        )

        binding.lcdStats.setup(
            "LCD Show Statistics",
            ble_command_e.BLE_COMMAND_ENABLE_LCD_STATISCTICS_CMD,
            ble_command_e.BLE_COMMAND_DISABLE_LCD_STATISCTICS_CMD,
            settingsItemListener
        )

        binding.lcdProbability.setup(
            "LCD Show Probability",
            ble_command_e.BLE_COMMAND_ENABLE_LCD_PROBABILITY_CMD,
            ble_command_e.BLE_COMMAND_DISABLE_LCD_PROBABILITY_CMD,
            settingsItemListener
        )

        binding.inactivityEnableItem.setup(
            "Inactivity Timer",
            ble_command_e.BLE_COMMAND_ENABLE_INACTIVITY_CMD,
            ble_command_e.BLE_COMMAND_DISABLE_INACTIVITY_CMD,
            settingsItemListener
        )

        binding.max78000VideoEnableItem.setup(
            "Video",
            ble_command_e.BLE_COMMAND_ENABLE_MAX78000_VIDEO_CMD,
            ble_command_e.BLE_COMMAND_DISABLE_MAX78000_VIDEO_CMD,
            settingsItemListener
        )

        binding.max78000AudioEnableItem.setup(
            "Audio",
            ble_command_e.BLE_COMMAND_ENABLE_MAX78000_AUDIO_CMD,
            ble_command_e.BLE_COMMAND_DISABLE_MAX78000_AUDIO_CMD,
            settingsItemListener
        )

        setupAdapter(
            debuggerSelectSpinner,
            debugger_select_e.values().toList().slice(0 until debugger_select_e.values().size - 1)
        )
        sendDebuggerButton.setOnClickListener {
            maxCamViewModel.sendData(
                ble_command_packet_t.from(
                    ble_command_e.BLE_COMMAND_SET_DEBUGGER_CMD,
                    (debuggerSelectSpinner.selectedItem as debugger_select_e).toByteArray()
                ).toByteArray()
            )
        }

        shutdownButton.setOnClickListener {
            val alert = AlertDialog.Builder(requireContext())
            alert.setMessage(getString(com.maximintegrated.maxcamandroid.R.string.are_you_sure_to_shut_device_down))
            alert.setPositiveButton(getString(com.maximintegrated.maxcamandroid.R.string.yes)) { dialog, _ ->
                dialog.dismiss()
                maxCamViewModel.sendData(
                    ble_command_packet_t.from(ble_command_e.BLE_COMMAND_SHUT_DOWN_DEVICE_CMD)
                        .toByteArray()
                )
                startScannerActivity(it.context)
            }
            alert.setNegativeButton(getString(com.maximintegrated.maxcamandroid.R.string.cancel)) { dialog, _ ->
                dialog.dismiss()
            }
            alert.show()
        }

        restartButton.setOnClickListener {
            val alert = AlertDialog.Builder(requireContext())
            alert.setMessage(getString(com.maximintegrated.maxcamandroid.R.string.are_you_sure_to_restart_device))
            alert.setPositiveButton(getString(com.maximintegrated.maxcamandroid.R.string.yes)) { dialog, _ ->
                dialog.dismiss()
                maxCamViewModel.sendData(
                    ble_command_packet_t.from(ble_command_e.BLE_COMMAND_RESTART_DEVICE_CMD)
                        .toByteArray()
                )
                startScannerActivity(it.context)
            }
            alert.setNegativeButton(getString(com.maximintegrated.maxcamandroid.R.string.cancel)) { dialog, _ ->
                dialog.dismiss()
            }
            alert.show()
        }
    }

    private fun setupAdapter(spinner: Spinner, list: List<debugger_select_e>?) {
        ArrayAdapter(
            requireContext(),
            R.layout.simple_spinner_item,
            list ?: emptyList()
        ).also { adapter ->
            adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
            spinner.adapter = adapter
        }
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
