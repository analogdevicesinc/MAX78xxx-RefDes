package com.maximintegrated.maxcamandroid.face

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import java.io.File

data class PersonModel(
    var personFolder: File
) : Comparable<PersonModel> {
    companion object {
        const val PHOTO_LIMIT = 8
    }

    override fun compareTo(other: PersonModel): Int {
        return this.nameSurname.compareTo(other.nameSurname)
    }

    val nameSurname: String get() = personFolder.nameWithoutExtension
    var images: MutableList<File> = mutableListOf()
    val imageCount: Int get() = images.count { !it.isDirectory }

    init {
        initializeModel()
    }

    fun delete() {
        personFolder.deleteRecursively()
    }

    fun rename(file: File) {
        personFolder.renameTo(file)
        personFolder = file
        initializeModel()
    }

    fun refreshImages() {
        if (personFolder.exists()) {
            images = personFolder.listFiles()?.toMutableList() ?: mutableListOf()
            images.sortBy { it.lastModified() }
            images.forEach { File(it.toURI().path) }
            if (images.size < PHOTO_LIMIT) {
                images.add(createTempDir())
            }
        }
    }

    private fun initializeModel() {
        refreshImages()
    }
}