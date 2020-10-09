package com.maximintegrated.maxcamandroid.utils

import com.maximintegrated.maxcamandroid.utils.fromHexStringToMeaningfulAscii
import org.hamcrest.MatcherAssert.assertThat
import org.hamcrest.Matchers
import org.junit.Test


class UtilsTest {

    @Test
    fun fromHexStringToMeaningfulAscii() {
        val hex = "20406080"
        val expectedString = "?@`?"
        assertThat(expectedString, Matchers.`is`(hex.fromHexStringToMeaningfulAscii()))
    }
}