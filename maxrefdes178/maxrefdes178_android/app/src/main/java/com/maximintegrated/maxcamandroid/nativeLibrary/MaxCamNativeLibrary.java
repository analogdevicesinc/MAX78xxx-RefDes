package com.maximintegrated.maxcamandroid.nativeLibrary;

public class MaxCamNativeLibrary {

    static {
        System.loadLibrary("native-lib");
    }

    private JniListener jniListener;

    //NOTE: Do not use it
    private MaxCamNativeLibrary() {
        initialize();
    }

    private static MaxCamNativeLibrary INSTANCE = null;

    synchronized public static MaxCamNativeLibrary getInstance() {
        if (INSTANCE == null) {
            INSTANCE = new MaxCamNativeLibrary();
        }
        return INSTANCE;
    }

    public void setJniListener(JniListener listener) {
        jniListener = listener;
    }

    private native void initialize();

    public native void setMtu(int mtu);

    public native static int getMaxMtu();

    public native static String getVersion();

    public native void sendFile(String fileName, byte[] data);

    public native void bleDataReceived(byte[] data);

    public native void getDirectoryRequest();

    public native void getFile(String fileName);

    public native void bleReset();

    public native void enterDemo();

    public native void captureImage(int fileLength);

    public native void loadImage(String filename);

    public native void sendImage(String filename, byte[] content);

    public void sendNotification(byte[] data) {
        if (jniListener != null) {
            jniListener.sendNotification(data);
        }
    }

    public void payloadReceived(byte[] payload) {
        if (jniListener != null) {
            jniListener.payloadReceived(payload);
        }
    }

    public interface JniListener {
        void sendNotification(byte[] data);

        void payloadReceived(byte[] payload);
    }
}
