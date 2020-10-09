package com.maximintegrated.maxcamandroid.faceId

import android.os.Environment
import androidx.test.core.app.ActivityScenario
import androidx.test.espresso.Espresso.onView
import androidx.test.espresso.action.ViewActions.*
import androidx.test.espresso.assertion.ViewAssertions.matches
import androidx.test.espresso.contrib.RecyclerViewActions
import androidx.test.espresso.matcher.RootMatchers.isDialog
import androidx.test.espresso.matcher.ViewMatchers.*
import androidx.test.ext.junit.runners.AndroidJUnit4
import androidx.test.filters.MediumTest
import androidx.test.rule.GrantPermissionRule
import com.maximintegrated.maxcamandroid.MainActivity
import com.maximintegrated.maxcamandroid.R
import com.maximintegrated.maxcamandroid.RecyclerViewDescendantAction
import com.maximintegrated.maxcamandroid.exts.addFragment
import com.maximintegrated.maxcamandroid.exts.getCurrentFragment
import com.maximintegrated.maxcamandroid.utils.getMaxCamScenarioFile
import kotlinx.coroutines.test.runBlockingTest
import org.hamcrest.Matchers.notNullValue
import org.junit.Before
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith
import java.io.File

@MediumTest
@RunWith(AndroidJUnit4::class)
class FaceIdScenariosFragmentTest {

    private lateinit var activityScenario: ActivityScenario<MainActivity>

    @Rule
    @JvmField
    var grantPermissionsRule: GrantPermissionRule = GrantPermissionRule.grant(
        android.Manifest.permission.WRITE_EXTERNAL_STORAGE,
        android.Manifest.permission.READ_EXTERNAL_STORAGE
    )

    @Before
    fun setup() {
        val directory =
            File(Environment.getExternalStorageDirectory(), FaceIdViewModel.SCENARIO_FOLDER_NAME)
        if (directory.exists()) {
            directory.deleteRecursively()
        }
        activityScenario = ActivityScenario.launch(MainActivity::class.java)
        activityScenario.onActivity {
            it.addFragment(FaceIdScenariosFragment.newInstance())
        }
    }

    @Test
    fun listScenariosEmpty() = runBlockingTest {
        onView(withId(R.id.emptyImageView)).check(matches(isDisplayed()))
    }

    @Test
    fun addScenario() = runBlockingTest {
        onView(withId(R.id.newScenarioFab)).perform(click())
        onView(withId(R.id.dialogEditText)).inRoot(isDialog()).perform(replaceText("test"))
        onView(withText("Create")).inRoot(isDialog()).perform(click())
        activityScenario.onActivity {
            assertThat(it.getCurrentFragment() as FaceIdInputParamsFragment, notNullValue())
            it.onBackPressed()
        }
        onView(withId(R.id.recyclerView)).check(matches(isDisplayed()))
    }

    @Test
    fun addMultipleScenarioAndCheckFabVisibility() = runBlockingTest {
        onView(withId(R.id.newScenarioFab)).perform(click())
        onView(withId(R.id.dialogEditText)).inRoot(isDialog()).perform(replaceText("test0"))
        onView(withText("Create")).inRoot(isDialog()).perform(click())
        val directory =
            File(Environment.getExternalStorageDirectory(), FaceIdViewModel.SCENARIO_FOLDER_NAME)
        val file = directory.listFiles()[0]
        for (i in 1..5) {
            file.copyTo(getMaxCamScenarioFile("test$i"))
        }
        activityScenario.onActivity {
            it.onBackPressed()
        }
        onView(withId(R.id.recyclerView)).perform(swipeUp())
        onView(withId(R.id.newScenarioFab)).check(matches(withEffectiveVisibility(Visibility.GONE)))
        onView(withId(R.id.recyclerView)).perform(swipeDown())
        onView(withId(R.id.newScenarioFab)).check(matches(isDisplayed()))
    }

    @Test
    fun onEditButtonClicked() = runBlockingTest {
        onView(withId(R.id.newScenarioFab)).perform(click())
        onView(withId(R.id.dialogEditText)).inRoot(isDialog()).perform(replaceText("test"))
        onView(withText("Create")).inRoot(isDialog()).perform(click())
        activityScenario.onActivity {
            it.onBackPressed()
        }
        onView(withId(R.id.recyclerView)).perform(
            RecyclerViewActions.actionOnItemAtPosition<ScenarioViewHolder>(
                0, RecyclerViewDescendantAction.clickChildViewWithId(R.id.editButton)
            )
        )
        activityScenario.onActivity {
            assertThat(it.getCurrentFragment() as FaceIdInputParamsFragment, notNullValue())
        }
    }

    @Test
    fun onSelectButtonClicked() = runBlockingTest {
        onView(withId(R.id.newScenarioFab)).perform(click())
        onView(withId(R.id.dialogEditText)).inRoot(isDialog()).perform(replaceText("test"))
        onView(withText("Create")).inRoot(isDialog()).perform(click())
        activityScenario.onActivity {
            it.onBackPressed()
        }
        onView(withId(R.id.recyclerView)).perform(
            RecyclerViewActions.actionOnItemAtPosition<ScenarioViewHolder>(
                0, RecyclerViewDescendantAction.clickChildViewWithId(R.id.selectButton)
            )
        )
        activityScenario.onActivity {
            assertThat(it.getCurrentFragment() as FaceIdUpdateDeviceFragment, notNullValue())
        }
    }

    @Test
    fun onDeleteButtonClicked() = runBlockingTest {
        onView(withId(R.id.newScenarioFab)).perform(click())
        onView(withId(R.id.dialogEditText)).inRoot(isDialog()).perform(replaceText("test"))
        onView(withText("Create")).inRoot(isDialog()).perform(click())
        activityScenario.onActivity {
            it.onBackPressed()
        }
        onView(withId(R.id.recyclerView)).perform(
            RecyclerViewActions.actionOnItemAtPosition<ScenarioViewHolder>(
                0, RecyclerViewDescendantAction.clickChildViewWithId(R.id.deleteButton)
            )
        )
        onView(withText("Yes")).inRoot(isDialog()).perform(click())
        onView(withId(R.id.emptyImageView)).check(matches(isDisplayed()))
    }
}