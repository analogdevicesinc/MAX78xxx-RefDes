package com.maximintegrated.maxcamandroid

import android.content.Intent
import android.content.SharedPreferences
import android.os.Build
import android.os.Bundle
import android.text.Html
import android.text.method.LinkMovementMethod
import androidx.appcompat.app.AppCompatActivity
import com.maximintegrated.maxcamandroid.utils.getAppName
import kotlinx.android.synthetic.main.activity_license.*

class LicenseActivity : AppCompatActivity() {
    val agreedKey = "agreed"
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_license)

        val sharedPrefs: SharedPreferences = getSharedPreferences(packageName, MODE_PRIVATE)
        val editor: SharedPreferences.Editor = sharedPrefs.edit()

        if (sharedPrefs.getBoolean(agreedKey, false)) {
            startScannerActivity()
        }


        val licenseText = "<p><b>Privacy Notice & License Agreement</b></p><br/>" +
                "<p>By Installing the " + this.getAppName() + " (the \"App\") you acknowledge: This App is owned by Maxim Integrated Products, Inc.(\"Maxim\")</p><br/>" +
                "<p>The full <a href=\"https://www.maximintegrated.com/en/aboutus/legal/privacy-policy/mobile-app-privacy-policy.html/tb_privacy#privacy-policy\">Privacy Policy</a> is accessible by clicking the link. " +
                "By selecting I Agree, you acknowledged you reviewed the above Privacy Notice and that you agree to the " +
                "<a href=\"https://www.maximintegrated.com/en/aboutus/legal/privacy-policy/mobile-app-privacy-policy.html/tb_privacy#license\">End User License Agreement</a>.</p>"
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
            licenseTextView.text = Html.fromHtml(licenseText, Html.FROM_HTML_MODE_COMPACT)
        } else {
            licenseTextView.text = Html.fromHtml(licenseText)
        }
        licenseTextView.movementMethod = LinkMovementMethod.getInstance()
        agreeButton.setOnClickListener {
            editor.putBoolean(agreedKey, true)
            editor.commit()
            startScannerActivity()
        }

    }

    private fun startScannerActivity() {
        startActivity(Intent(applicationContext, ScannerActivity::class.java))
        finish()
    }
}