/*
 * Copyright (C) Maxim Integrated Products, Inc., All rights Reserved.
 *
 * This software is protected by copyright laws of the United States and
 * of foreign countries. This material may also be protected by patent laws
 * and technology transfer regulations of the United States and of foreign
 * countries. This software is furnished under a license agreement and/or a
 * nondisclosure agreement and may only be used or reproduced in accordance
 * with the terms of those agreements. Dissemination of this information to
 * any party or parties not specified in the license agreement and/or
 * nondisclosure agreement is expressly prohibited.
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 */

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