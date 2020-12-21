package com.maximintegrated.maxcamandroid.face

import java.io.File

const val DB_FOLDER_NAME = "/Databases"

data class DbModel(
    var dbFile: File
) {

    var listOfFoldersForPeople: List<File> = listOf()
    var images: HashMap<String, List<File>> = hashMapOf()
    var embeddingsFile: File? = null

    init {
        initializeModel()
    }

    fun delete() {
        dbFile.deleteRecursively()
    }

    fun rename(file: File) {
        dbFile.renameTo(file)
        dbFile = file
        initializeModel()
    }

    private fun initializeModel() {
        listOfFoldersForPeople = listOf()
        images.clear()
        embeddingsFile = null
        if (dbFile.exists()) {
            listOfFoldersForPeople = dbFile.listFiles()?.toList() ?: emptyList()
            embeddingsFile = listOfFoldersForPeople.find { it.extension == "bin" }
            listOfFoldersForPeople = listOfFoldersForPeople.filter { it.isDirectory }
            for (folder in listOfFoldersForPeople) {
                val name = folder.nameWithoutExtension
                val list = folder.listFiles()?.toList() ?: emptyList()
                images[name] = list
            }
        }
    }
}