package com.maximintegrated.maxcamandroid.face

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import java.io.File

data class PersonModel(
    var personFolder: File
) {
    val nameSurname: String get() = personFolder.nameWithoutExtension
    var images = ArrayList<File>()

    init {
        initializeModel()
    }

    fun delete() {
        personFolder.deleteRecursively()
    }

    fun rename(file: File) {
        personFolder.renameTo(file)
        initializeModel()
    }

    private fun initializeModel() {

        if (personFolder.exists()) {
            images = (personFolder.listFiles()?.toList() ?: arrayListOf()) as ArrayList<File>
            images.forEach{File(it.toURI().path) }
            images.add(createTempFile())
        }
    }

}