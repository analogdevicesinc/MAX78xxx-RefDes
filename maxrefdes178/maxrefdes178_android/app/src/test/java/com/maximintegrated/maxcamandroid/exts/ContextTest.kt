package com.maximintegrated.maxcamandroid.exts

import androidx.test.ext.junit.runners.AndroidJUnit4
import androidx.test.platform.app.InstrumentationRegistry
import com.maximintegrated.maxcamandroid.R
import org.hamcrest.MatcherAssert.assertThat
import org.hamcrest.Matchers.notNullValue
import org.junit.Test
import org.junit.runner.RunWith

@RunWith(AndroidJUnit4::class)
class ContextTest {

    @Test
    fun color() {
        val context = InstrumentationRegistry.getInstrumentation().targetContext
        assertThat(context.color(R.color.color_primary), notNullValue())
    }

    @Test
    fun drawable() {
        val context = InstrumentationRegistry.getInstrumentation().targetContext
        assertThat(context.drawable(R.drawable.ic_face_id), notNullValue())
    }
}