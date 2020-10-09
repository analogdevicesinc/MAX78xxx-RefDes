package com.maximintegrated.maxcamandroid.fileOperations

import android.app.Activity
import android.app.Instrumentation
import android.content.Intent
import android.content.Intent.FLAG_GRANT_READ_URI_PERMISSION
import androidx.core.content.FileProvider
import androidx.test.espresso.Espresso.onView
import androidx.test.espresso.action.ViewActions.click
import androidx.test.espresso.action.ViewActions.scrollTo
import androidx.test.espresso.assertion.ViewAssertions.matches
import androidx.test.espresso.intent.Intents.intended
import androidx.test.espresso.intent.Intents.intending
import androidx.test.espresso.intent.matcher.IntentMatchers.hasAction
import androidx.test.espresso.intent.matcher.IntentMatchers.isInternal
import androidx.test.espresso.intent.rule.IntentsTestRule
import androidx.test.espresso.matcher.ViewMatchers.*
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
import org.hamcrest.MatcherAssert
import org.hamcrest.Matchers.*
import org.junit.Before
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith


@MediumTest
@RunWith(AndroidJUnit4::class)
class FileOperationsFragmentTest {

    private val selectedFileName = "layerConfig2.txt"
    private val maxCamNativeLibrary = AndroidFakeMaxCamNativeLibrary()

    init {
        ServiceLocator.maxCamNativeLibrary = maxCamNativeLibrary
    }

    @get:Rule
    val intentsTestRule = IntentsTestRule(MainActivity::class.java)
    @Rule @JvmField var grantPermissionsRule: GrantPermissionRule = GrantPermissionRule.grant(
        android.Manifest.permission.WRITE_EXTERNAL_STORAGE, android.Manifest.permission.READ_EXTERNAL_STORAGE
    )

    @Before
    fun stubAllExternalIntents() {
        // By default Espresso Intents does not stub any Intents. Stubbing needs to be setup before
        // every test run. In this case all external Intents will be blocked.
        intending(not(isInternal())).respondWith(
            Instrumentation.ActivityResult(
                Activity.RESULT_OK,
                null
            )
        )
    }

    @Before
    fun setup() {
        intentsTestRule.activity.addFragment(FileOperationsFragment())
    }

    @Test
    fun onSelectFileButtonClickedNull() {
        val intent = Intent()
        intent.data = null
        val result = Instrumentation.ActivityResult(Activity.RESULT_OK, intent)

        intending(
            allOf(
                hasAction(Intent.ACTION_CHOOSER)
            )
        ).respondWith(result)
        onView(withId(R.id.selectFileButton)).perform(click())
        onView(withId(R.id.sendFileSelectedTextView)).check(matches(withText("No file is selected")))
    }

    @Test
    fun onSelectFileButtonClickedAndSend() {
        val intent = Intent()
        val uri = FileProvider.getUriForFile(
            intentsTestRule.activity,
            "com.maximintegrated.maxcamandroid.fileprovider",
            getMaxCamFile(selectedFileName)
        )
        intent.data = uri
        intent.flags = FLAG_GRANT_READ_URI_PERMISSION
        val result = Instrumentation.ActivityResult(Activity.RESULT_OK, intent)

        intending(
            allOf(
                hasAction(Intent.ACTION_CHOOSER)
            )
        ).respondWith(result)
        onView(withId(R.id.selectFileButton)).perform(click())
        onView(withId(R.id.sendFileSelectedTextView)).check(matches(withText(selectedFileName)))
        onView(withId(R.id.sendFileButton)).perform(click())
        MatcherAssert.assertThat(
            "sendFile", `is`(maxCamNativeLibrary.functionArguments[0])
        )
        MatcherAssert.assertThat(
            selectedFileName, `is`(maxCamNativeLibrary.functionArguments[1])
        )
    }

    @Test
    fun onGetContentButtonClicked() {
        onView(withId(R.id.getContentButton)).perform(click())
        val fragment = intentsTestRule.activity.getCurrentFragment() as FileOperationsFragment
        val root = fragment.rootNode
        val sd = root.children[0]
        val numberOfFiles = maxCamNativeLibrary.fileNames.size
        assertThat(numberOfFiles, `is`(sd.children.size))
    }

    @Test
    fun onGetFileButtonClickedTxt() {
        maxCamNativeLibrary.bringTheGivenSuffixedFileToFront("txt")
        onView(withId(R.id.getContentButton)).perform(click())
        val files = maxCamNativeLibrary.fileNames
        val fileContents = maxCamNativeLibrary.fileContents
        clickTreeNode()
        onView(withId(R.id.getFileTextView)).check(matches(withText(files[0])))
        onView(withId(R.id.getFileButton)).perform(scrollTo(), click())
        onView(withId(R.id.contentDisplayTextView)).perform(scrollTo()).check(matches(withText(fileContents[0])))
        onView(withId(R.id.contentDisplayTextView)).check(matches(isDisplayed()))
        onView(withId(R.id.contentDisplayImageView)).check(matches(not(isDisplayed())))
        onView(withText("Open")).perform(click())
        intended(allOf(
            hasAction(Intent.ACTION_VIEW)
        ))
    }

    @Test
    fun onGetFileButtonClickedUnknown() {
        maxCamNativeLibrary.bringTheGivenSuffixedFileToFront("b")
        onView(withId(R.id.getContentButton)).perform(click())
        val files = maxCamNativeLibrary.fileNames
        clickTreeNode()
        onView(withId(R.id.getFileTextView)).check(matches(withText(files[0])))
        onView(withId(R.id.getFileButton)).perform(scrollTo(), click())
        onView(withId(R.id.contentDisplayTextView)).perform(scrollTo()).check(matches(isDisplayed()))
        onView(withId(R.id.contentDisplayImageView)).check(matches(not(isDisplayed())))
    }

    @Test
    fun onGetFileButtonClickedTest() {
        maxCamNativeLibrary.bringTheGivenSuffixedFileToFront("test")
        onView(withId(R.id.getContentButton)).perform(click())
        val files = maxCamNativeLibrary.fileNames
        clickTreeNode()
        onView(withId(R.id.getFileTextView)).check(matches(withText(files[0])))
        onView(withId(R.id.getFileButton)).perform(scrollTo(), click())
        onView(withId(R.id.contentDisplayTextView)).perform(scrollTo()).check(matches(isDisplayed()))
        onView(withId(R.id.contentDisplayImageView)).check(matches(not(isDisplayed())))
    }

    @Test
    fun onGetFileButtonClickedPng() {
        maxCamNativeLibrary.bringTheGivenSuffixedFileToFront("png")
        onView(withId(R.id.getContentButton)).perform(click())
        val files = maxCamNativeLibrary.fileNames
        clickTreeNode()
        onView(withId(R.id.getFileTextView)).check(matches(withText(files[0])))
        onView(withId(R.id.getFileButton)).perform(scrollTo(), click())
        onView(withId(R.id.contentDisplayImageView)).perform(scrollTo()).check(matches(isDisplayed()))
        onView(withId(R.id.contentDisplayTextView)).check(matches(not(isDisplayed())))
    }

    private fun clickTreeNode() {
        UiThreadStatement.runOnUiThread {
            val fragment = intentsTestRule.activity.getCurrentFragment() as FileOperationsFragment
            val root = fragment.rootNode
            val node = root.children[0].children[0]
            node.viewHolder.view.performClick()
        }
    }
}