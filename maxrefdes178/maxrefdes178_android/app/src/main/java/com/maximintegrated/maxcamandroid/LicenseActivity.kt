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