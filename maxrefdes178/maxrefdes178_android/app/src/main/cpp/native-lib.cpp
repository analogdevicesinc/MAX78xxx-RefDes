/******************************************************************************
* Copyright (C) 2020 Maxim Integrated Products, Inc., All rights Reserved.
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
*******************************************************************************
*/

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <jni.h>
#include <string>
#include "MAXCAM_Packet.h"
#include "MAXCAM_PacketHelper.h"

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// LOG PRINT
//-----------------------------------------------------------------------------
#include <android/log.h>

#define TAG "ME14_LOG"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG  , TAG, __VA_ARGS__)


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
static jmethodID javaSendNotification;
static jmethodID javaPayloadReceived;
static JNIEnv *g_env_MaxCamNativeLibrary;
static jobject g_thiz_MaxCamNativeLibrary;
int g_mtu = 23;

//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------

/*JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }



    return JNI_VERSION_1_6;
}*/

extern "C"
void sendNotificationHelper(JNIEnv *env, jobject thiz, unsigned char *inputArray, int length) {

    /*
    * Convert C type to java type first, then invoke Java method
    */

    // Create memory space for Java Variable
    jbyteArray javabytes = env->NewByteArray(length);

    // Map C array to Java Object
    env->SetByteArrayRegion(javabytes, 0, length, (jbyte *) inputArray);

    // Call The Target Method
    env->CallVoidMethod(thiz, javaSendNotification, javabytes);
}

extern "C"
void Ble_SendNotification(const uint16_t dataLen, const uint8_t *data) {
    sendNotificationHelper(g_env_MaxCamNativeLibrary, g_thiz_MaxCamNativeLibrary,
                           (unsigned char *) data,
                           (int) dataLen);
}
extern "C"
void payloadReceivedHelper(JNIEnv *env, jobject thiz, unsigned char *inputArray, int length) {

    /*
    * Convert C type to java type first, then invoke Java method
    */

    // Create memory space for Java Variable
    jbyteArray javabytes = env->NewByteArray(length);

    // Map C array to Java Object
    env->SetByteArrayRegion(javabytes, 0, length, (jbyte *) inputArray);

    // Call The Target Method
    env->CallVoidMethod(thiz, javaPayloadReceived, javabytes);
}

extern "C"
void Ble_PayloadReceived(const uint16_t dataLen, const uint8_t *data) {
    payloadReceivedHelper(g_env_MaxCamNativeLibrary, g_thiz_MaxCamNativeLibrary,
                          (unsigned char *) data,
                          (int) dataLen);
}

extern "C"
uint16_t Ble_GetCurrentMtu(void) {
    return g_mtu - 3;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_maximintegrated_maxcamandroid_nativeLibrary_MaxCamNativeLibrary_initialize(JNIEnv *env,
                                                                                    jobject thiz) {
    g_thiz_MaxCamNativeLibrary = thiz;
    g_env_MaxCamNativeLibrary = env;

    /*
     * Register BLE Functions to Packet Helper
     */
    PacketHelper_RegisterSendNotification(Ble_SendNotification);
    PacketHelper_RegisterPayloadReceived(Ble_PayloadReceived);
    PacketHelper_RegisterGetCurrentMtu(Ble_GetCurrentMtu);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_maximintegrated_maxcamandroid_nativeLibrary_MaxCamNativeLibrary_setMtu(JNIEnv *env,
                                                                                jobject thiz,
                                                                                jint mtu) {
    g_mtu = mtu;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_maximintegrated_maxcamandroid_nativeLibrary_MaxCamNativeLibrary_getMaxMtu(JNIEnv *env,
                                                                                   jclass clazz) {
    return MAXCAM_BLE_MAX_MTU_SIZE;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_maximintegrated_maxcamandroid_nativeLibrary_MaxCamNativeLibrary_getVersion(JNIEnv *env,
                                                                                    jclass clazz) {
    char version[64];
    Packet_GetVersion(version);
    return env->NewStringUTF(version);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_maximintegrated_maxcamandroid_nativeLibrary_MaxCamNativeLibrary_sendFile(JNIEnv *env,
                                                                                  jobject thiz,
                                                                                  jstring file_name,
                                                                                  jbyteArray data) {
    jclass thisClass = env->GetObjectClass(thiz);
    javaSendNotification = env->GetMethodID(thisClass, "sendNotification", "([B)V");
    g_thiz_MaxCamNativeLibrary = thiz;
    g_env_MaxCamNativeLibrary = env;

    const char *nativeFileName = env->GetStringUTFChars(file_name, 0);
    // Get the input array from Java
    unsigned char *inputArray = reinterpret_cast<unsigned char *>(env->GetByteArrayElements(data, 0));
    uint32_t inputLen = env->GetArrayLength(data);

    PacketHelper_SendFile(nativeFileName, (const char *) (inputArray), inputLen);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_maximintegrated_maxcamandroid_nativeLibrary_MaxCamNativeLibrary_bleDataReceived(
        JNIEnv *env, jobject thiz, jbyteArray data) {

    jclass thisClass = env->GetObjectClass(thiz);
    javaPayloadReceived = env->GetMethodID(thisClass, "payloadReceived", "([B)V");
    g_thiz_MaxCamNativeLibrary = thiz;
    g_env_MaxCamNativeLibrary = env;

    unsigned char *inputArray = reinterpret_cast<unsigned char *>(env->GetByteArrayElements(data, 0));
    uint32_t packetLen = env->GetArrayLength(data);

    PacketHelper_Receive(packetLen, inputArray);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_maximintegrated_maxcamandroid_nativeLibrary_MaxCamNativeLibrary_getDirectoryRequest(
        JNIEnv *env, jobject thiz) {
    jclass thisClass = env->GetObjectClass(thiz);
    javaSendNotification = env->GetMethodID(thisClass, "sendNotification", "([B)V");
    g_thiz_MaxCamNativeLibrary = thiz;
    g_env_MaxCamNativeLibrary = env;
    PacketHelper_RequestGetDir();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_maximintegrated_maxcamandroid_nativeLibrary_MaxCamNativeLibrary_getFile(JNIEnv *env,
                                                                                 jobject thiz,
                                                                                 jstring file_name) {
    jclass thisClass = env->GetObjectClass(thiz);
    javaSendNotification = env->GetMethodID(thisClass, "sendNotification", "([B)V");
    g_thiz_MaxCamNativeLibrary = thiz;
    g_env_MaxCamNativeLibrary = env;
    const char *nativeFileName = env->GetStringUTFChars(file_name, 0);
    PacketHelper_RequestGetFile(nativeFileName);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_maximintegrated_maxcamandroid_nativeLibrary_MaxCamNativeLibrary_bleReset(JNIEnv *env,
                                                                                  jobject thiz) {
    PacketHelper_ResetPacketNumber();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_maximintegrated_maxcamandroid_nativeLibrary_MaxCamNativeLibrary_enterDemo(JNIEnv *env,
                                                                                   jobject thiz) {
    jclass thisClass = env->GetObjectClass(thiz);
    javaSendNotification = env->GetMethodID(thisClass, "sendNotification", "([B)V");
    g_thiz_MaxCamNativeLibrary = thiz;
    g_env_MaxCamNativeLibrary = env;
    PacketHelper_EnterDEMO();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_maximintegrated_maxcamandroid_nativeLibrary_MaxCamNativeLibrary_captureImage(JNIEnv *env,
                                                                                      jobject thiz,
                                                                                      jint file_length) {
    jclass thisClass = env->GetObjectClass(thiz);
    javaSendNotification = env->GetMethodID(thisClass, "sendNotification", "([B)V");
    g_thiz_MaxCamNativeLibrary = thiz;
    g_env_MaxCamNativeLibrary = env;
    PacketHelper_CaptureImage(static_cast<const uint32_t>(file_length));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_maximintegrated_maxcamandroid_nativeLibrary_MaxCamNativeLibrary_loadImage(JNIEnv *env,
                                                                                   jobject thiz,
                                                                                   jstring filename) {
    jclass thisClass = env->GetObjectClass(thiz);
    javaSendNotification = env->GetMethodID(thisClass, "sendNotification", "([B)V");
    g_thiz_MaxCamNativeLibrary = thiz;
    g_env_MaxCamNativeLibrary = env;
    const char *nativeFileName = env->GetStringUTFChars(filename, 0);
    PacketHelper_LoadImage(nativeFileName);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_maximintegrated_maxcamandroid_nativeLibrary_MaxCamNativeLibrary_sendImage(JNIEnv *env,
                                                                                   jobject thiz,
                                                                                   jstring filename,
                                                                                   jbyteArray content) {
    jclass thisClass = env->GetObjectClass(thiz);
    javaSendNotification = env->GetMethodID(thisClass, "sendNotification", "([B)V");
    g_thiz_MaxCamNativeLibrary = thiz;
    g_env_MaxCamNativeLibrary = env;
    const char *nativeFileName = env->GetStringUTFChars(filename, 0);
    // Get the input array from Java
    char *inputArray = reinterpret_cast<char *>(env->GetByteArrayElements(content, 0));
    auto inputLen = static_cast<uint32_t>(env->GetArrayLength(content));
    PacketHelper_SendImage(nativeFileName, inputArray, inputLen);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_maximintegrated_maxcamandroid_nativeLibrary_MaxCamNativeLibrary_test(JNIEnv *env,
                                                                              jobject thiz,
                                                                              jstring filename,
                                                                              jint length) {
    jclass thisClass = env->GetObjectClass(thiz);
    javaSendNotification = env->GetMethodID(thisClass, "sendNotification", "([B)V");
    g_thiz_MaxCamNativeLibrary = thiz;
    g_env_MaxCamNativeLibrary = env;
}