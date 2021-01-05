package com.maximintegrated.maxcamandroid.face

import java.io.File

const val DB_FOLDER_NAME = "/Databases"

data class DbModel(
    var dbFolder: File
) {

    val persons: ArrayList<PersonModel> = arrayListOf()
    val dbName: String get() = dbFolder.nameWithoutExtension
    var embeddingsFile: File? = null

    init {
        initializeModel()
    }

    fun delete() {
        dbFolder.deleteRecursively()
    }

    fun rename(file: File) {
        dbFolder.renameTo(file)
        dbFolder = file
        initializeModel()
    }

    private fun initializeModel() {
        persons.clear()
        embeddingsFile = null
        if (dbFolder.exists()) {
            var listOfFoldersForPeople = dbFolder.listFiles()?.toList() ?: emptyList()
            embeddingsFile = listOfFoldersForPeople.find { it.extension == "bin" }
            listOfFoldersForPeople = listOfFoldersForPeople.filter { it.isDirectory }
            listOfFoldersForPeople.forEach { persons.add(PersonModel(it)) }
        }
    }

    fun findEmbeddingsFile() {
        val list = dbFolder.listFiles()?.toList() ?: emptyList()
        embeddingsFile = list.find { it.extension == "bin" }
    }
}