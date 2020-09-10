package com.maximintegrated.maxcamandroid.view

enum class TreeNodeType {
    FOLDER_FULL,
    FOLDER_EMPTY,
    FOLDER_PHOTO,
    FILE_IMAGE,
    FILE_TEXT,
    UNKOWN_TYPE,
    SD_STORAGE
}

data class CustomTreeItem(val type: TreeNodeType, val text: String, var size: Int)