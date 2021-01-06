package com.maximintegrated.maxcamandroid.face

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProviders
import com.bumptech.glide.Glide
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.utils.DeleteListener
import com.maximintegrated.maxcamandroid.utils.askUserForDeleteOperation
import kotlinx.android.synthetic.main.fragment_image_view.*
import java.io.File

class ImageViewFragment : Fragment(), DeleteListener {

    companion object {
        fun newInstance() = ImageViewFragment()
    }

    private lateinit var faceIdViewModel: FaceIdViewModel

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_image_view, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        faceIdViewModel = ViewModelProviders.of(requireActivity()).get(FaceIdViewModel::class.java)
        Glide.with(this).load(faceIdViewModel.selectedPersonImage).into(imageView)
        deleteButton.setOnClickListener {
            askUserForDeleteOperation(
                view.context,
                this
            )
        }
    }

    override fun onDeleteButtonClicked(vararg model: Any) {
        faceIdViewModel.deletePersonImage(faceIdViewModel.selectedPersonImage!!)
        activity?.onBackPressed()
    }


}