package com.maximintegrated.maxcamandroid.face

import android.text.InputFilter
import android.text.Spanned
import java.io.File

const val DB_FOLDER_NAME = "/Databases"

data class DbModel(
    var dbFolder: File
) {
    companion object {
        const val PERSON_LIMIT = 6
    }


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

    fun initializeModel() {
        persons.clear()
        embeddingsFile = null
        if (dbFolder.exists()) {
            var listOfFoldersForPeople = dbFolder.listFiles()?.toList() ?: emptyList()
            embeddingsFile = listOfFoldersForPeople.find { it.extension == "bin" }
            listOfFoldersForPeople = listOfFoldersForPeople.filter { it.isDirectory }
            listOfFoldersForPeople.forEach { persons.add(PersonModel(it)) }
            persons.sort()
        }
    }

    fun findEmbeddingsFile() {
        val list = dbFolder.listFiles()?.toList() ?: emptyList()
        embeddingsFile = list.find { it.extension == "bin" }
    }
}

class AlphaNumericInputFilter : InputFilter {
    override fun filter(
        source: CharSequence, start: Int, end: Int,
        dest: Spanned, dstart: Int, dend: Int
    ): CharSequence? {

        // Only keep characters that are alphanumeric
        val builder = StringBuilder()
        for (i in start until end) {
            val c = source[i]
            if (Character.isLetterOrDigit(c)) {
                builder.append(c)
            }
        }

        // If all characters are valid, return null, otherwise only return the filtered characters
        val allCharactersValid = builder.length == end - start
        return if (allCharactersValid) null else builder.toString()
    }
}