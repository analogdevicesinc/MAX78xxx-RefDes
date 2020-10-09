package com.maximintegrated.maxcamandroid.main

import android.os.Bundle
import androidx.fragment.app.testing.launchFragmentInContainer
import androidx.test.core.app.ActivityScenario
import androidx.test.espresso.Espresso.onView
import androidx.test.espresso.action.ViewActions.click
import androidx.test.espresso.assertion.ViewAssertions.matches
import androidx.test.espresso.matcher.ViewMatchers.*
import androidx.test.ext.junit.runners.AndroidJUnit4
import androidx.test.filters.MediumTest
import com.maximintegrated.maxcamandroid.MainActivity
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.ServiceLocator
import com.maximintegrated.maxcamandroid.diagnostics.DiagnosticsFragment
import com.maximintegrated.maxcamandroid.exts.getCurrentFragment
import com.maximintegrated.maxcamandroid.faceId.FaceIdScenariosFragment
import com.maximintegrated.maxcamandroid.fileOperations.FileOperationsFragment
import com.maximintegrated.maxcamandroid.nativeLibrary.AndroidFakeMaxCamNativeLibrary
import org.hamcrest.Matchers.*
import org.junit.Test
import org.junit.runner.RunWith


@MediumTest
@RunWith(AndroidJUnit4::class)
class MainFragmentTest {

    private lateinit var activityScenario: ActivityScenario<MainActivity>

    private fun setup() {
        ServiceLocator.maxCamNativeLibrary = AndroidFakeMaxCamNativeLibrary()
        activityScenario = ActivityScenario.launch(MainActivity::class.java)
    }

    fun onBackPressedImMainFragment() {
        setup()
        onView(withId(R.id.mainFab)).perform(click())
        Thread.sleep(300)
        onView(withId(R.id.warningTextView)).check(matches(isDisplayed()))
    }

    @Test
    fun onFileOperationsClicked() {
        setup()
        onView(withId(R.id.fileOperationsButton)).perform(click())
        Thread.sleep(200)
        activityScenario.onActivity { activity ->
            assertThat(
                activity.getCurrentFragment() as FileOperationsFragment,
                `is`(notNullValue())
            )
        }
        Thread.sleep(500)
        onView(withId(R.id.mainFab)).perform(click())
        activityScenario.onActivity { activity ->
            assertThat(
                activity.getCurrentFragment() as MainFragment,
                `is`(notNullValue())
            )
        }
    }

    @Test
    fun onDiagnosticsClicked() {
        setup()
        onView(withId(R.id.diagnosticsButton)).perform(click())
        Thread.sleep(200)
        activityScenario.onActivity { activity ->
            assertThat(
                activity.getCurrentFragment() as DiagnosticsFragment,
                `is`(notNullValue())
            )
        }
        Thread.sleep(500)
        onView(withId(R.id.mainFab)).perform(click())
        activityScenario.onActivity { activity ->
            assertThat(
                activity.getCurrentFragment() as MainFragment,
                `is`(notNullValue())
            )
        }
    }

    @Test
    fun onFaceIdClicked() {
        setup()
        onView(withId(R.id.faceIdButton)).perform(click())
        Thread.sleep(200)
        activityScenario.onActivity { activity ->
            assertThat(
                activity.getCurrentFragment() as FaceIdScenariosFragment,
                `is`(notNullValue())
            )
        }
        Thread.sleep(500)
    }

    @Test
    fun isolatedFragment() {
        val scenario = launchFragmentInContainer<MainFragment>(Bundle(), R.style.Theme_Maxim)
        scenario.onFragment {
            assertThat(it.requireActivity() as? MainActivity, `is`(nullValue()))
        }
    }
}