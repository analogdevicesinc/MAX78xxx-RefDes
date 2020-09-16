package com.maximintegrated.maxcamandroid.faceId

enum class ImageSource(private val value: String) {
    ONBOARD_CAMERA("Onboard Camera"),
    FILE_SYSTEM("File System"),
    ANDROID("Android");

    override fun toString(): String {
        return value
    }
}

enum class ImageOutput(private val value: String) {
    ONBOARD_LCD("Onboard LCD"),
    FILE_SYSTEM("File System"),
    ANDROID("Android");

    override fun toString(): String {
        return value
    }
}

enum class ImageSize(val width: Int, val height: Int) {
    SIZE_120_160(120, 160),
    SIZE_240_320(240, 320);

    override fun toString(): String {
        return "$width x $height"
    }
}

data class Scenario(
    var name: String = "N/A",
    var imageSource: ImageSource = ImageSource.ONBOARD_CAMERA,
    var imageOutput: ImageOutput = ImageOutput.ONBOARD_LCD,
    var imageSize: ImageSize = ImageSize.SIZE_120_160,
    var configFileName: String = "N/A"
) {
    override fun toString(): String {
        return "Image Source: $imageSource\n" +
                "Image Size: $imageSize\n" +
                "Layer Configuration: $configFileName\n" +
                "Image Output: $imageOutput"
    }
}