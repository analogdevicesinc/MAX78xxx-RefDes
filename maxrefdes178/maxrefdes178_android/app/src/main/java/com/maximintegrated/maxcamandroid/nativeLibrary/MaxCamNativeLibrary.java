package com.maximintegrated.maxcamandroid.nativeLibrary;

public class MaxCamNativeLibrary implements IMaxCamNativeLibrary {

    static {
        System.loadLibrary("native-lib");
    }

    private JniListener jniListener;

    public MaxCamNativeLibrary() {
        initialize();
    }

    @Override
    public void setJniListener(JniListener listener) {
        jniListener = listener;
    }

    private native void initialize();

    @Override
    public native void setMtu(int mtu);

    @Override
    public native int getMaxMtu();

    @Override
    public native String getVersion();

    @Override
    public native void sendFile(String fileName, byte[] data);

    @Override
    public native void bleDataReceived(byte[] data);

    @Override
    public native void getDirectoryRequest();

    @Override
    public native void getFile(String fileName);

    @Override
    public native void bleReset();

    @Override
    public native void enterDemo();

    @Override
    public native void captureImage(int fileLength);

    @Override
    public native void loadImage(String filename);

    @Override
    public native void sendImage(String filename, byte[] content);

    @Override
    public void sendNotification(byte[] data) {
        if (jniListener != null) {
            jniListener.sendNotification(data);
        }
    }

    @Override
    public void payloadReceived(byte[] payload) {
        if (jniListener != null) {
            jniListener.payloadReceived(payload);
        }
    }
}
