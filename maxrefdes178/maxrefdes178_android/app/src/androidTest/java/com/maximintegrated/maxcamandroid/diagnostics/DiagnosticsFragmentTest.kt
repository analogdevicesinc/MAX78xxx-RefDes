package com.maximintegrated.maxcamandroid.diagnostics

import android.app.Activity
import android.app.Instrumentation
import android.content.Intent
import android.graphics.Rect
import androidx.core.content.FileProvider
import androidx.test.espresso.Espresso.onView
import androidx.test.espresso.action.ViewActions.click
import androidx.test.espresso.intent.Intents
import androidx.test.espresso.intent.matcher.IntentMatchers.hasComponent
import androidx.test.espresso.intent.rule.IntentsTestRule
import androidx.test.espresso.matcher.ViewMatchers.withId
import androidx.test.ext.junit.runners.AndroidJUnit4
import androidx.test.filters.MediumTest
import androidx.test.internal.runner.junit4.statement.UiThreadStatement
import androidx.test.rule.GrantPermissionRule
import com.maximintegrated.maxcamandroid.MainActivity
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.ServiceLocator
import com.maximintegrated.maxcamandroid.exts.addFragment
import com.maximintegrated.maxcamandroid.exts.getCurrentFragment
import com.maximintegrated.maxcamandroid.utils.getMaxCamFile
import com.maximintegrated.maxcamandroid.nativeLibrary.AndroidFakeMaxCamNativeLibrary
import com.theartofdev.edmodo.cropper.CropImage
import com.theartofdev.edmodo.cropper.CropImageActivity
import org.hamcrest.MatcherAssert.assertThat
import org.hamcrest.Matchers.`is`
import org.hamcrest.Matchers.allOf
import org.junit.Before
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith

@MediumTest
@RunWith(AndroidJUnit4::class)
class DiagnosticsFragmentTest {

    private val maxCamNativeLibrary = AndroidFakeMaxCamNativeLibrary()

    init {
        ServiceLocator.maxCamNativeLibrary = maxCamNativeLibrary
    }

    @get:Rule
    val intentsTestRule = IntentsTestRule(MainActivity::class.java)
    @Rule
    @JvmField
    var grantPermissionsRule: GrantPermissionRule = GrantPermissionRule.grant(
        android.Manifest.permission.WRITE_EXTERNAL_STORAGE,
        android.Manifest.permission.READ_EXTERNAL_STORAGE
    )

    @Before
    fun setup() {
        intentsTestRule.activity.addFragment(DiagnosticsFragment())
    }

    @Test
    fun onGetContentButtonClicked() {
        onView(withId(R.id.getContentButton)).perform(click())
        val fragment = intentsTestRule.activity.getCurrentFragment() as DiagnosticsFragment
        val root = fragment.rootNode
        val sd = root.children[0]
        val numberOfFiles = maxCamNativeLibrary.fileNames.size
        assertThat(numberOfFiles, `is`(sd.children.size))
    }

    @Test
    fun onLoadImageButtonClicked() {
        onView(withId(R.id.getContentButton)).perform(click())
        UiThreadStatement.runOnUiThread {
            val fragment = intentsTestRule.activity.getCurrentFragment() as DiagnosticsFragment
            val root = fragment.rootNode
            val node = root.children[0].children[0]
            node.viewHolder.view.performClick()
            node.viewHolder.view.performClick()
            node.viewHolder.view.performClick()
        }
        onView(withId(R.id.loadImageButton)).perform(click())
        assertThat("loadImage", `is`(maxCamNativeLibrary.functionArguments[0]))
    }

    @Test
    fun onEnterDemoButtonClicked() {
        onView(withId(R.id.enterDemoButton)).perform(click())
        assertThat("enterDemo", `is`(maxCamNativeLibrary.functionArguments[0]))
    }

    @Test
    fun onCaptureImageButtonClicked() {
        onView(withId(R.id.captureImageButton)).perform(click())
        assertThat("captureImage", `is`(maxCamNativeLibrary.functionArguments[0]))
    }

    @Test
    fun onSendImageButtonClicked() {
        val intent = Intent()
        val uri = FileProvider.getUriForFile(
            intentsTestRule.activity,
            "com.maximintegrated.maxcamandroid.fileprovider",
            getMaxCamFile("testshot.jpg")
        )
        intent.data = uri
        intent.flags = Intent.FLAG_GRANT_READ_URI_PERMISSION
        val cropResult = CropImage.ActivityResult(
            uri,
            uri,
            Exception("test error"),
            floatArrayOf(),
            Rect(),
            0,
            Rect(),
            50
        )
        intent.putExtra(CropImage.CROP_IMAGE_EXTRA_RESULT, cropResult)
        val result = Instrumentation.ActivityResult(Activity.RESULT_OK, intent)

        Intents.intending(
            allOf(
                hasComponent(CropImageActivity::class.java.name)
            )
        ).respondWith(result)
        onView(withId(R.id.sendImageButton)).perform(click())
        assertThat("sendImage", `is`(maxCamNativeLibrary.functionArguments[0]))
    }

    @Test
    fun onSendTestBytesButton() {
        onView(withId(R.id.sendTestBytesButton)).perform(click())
    }
}