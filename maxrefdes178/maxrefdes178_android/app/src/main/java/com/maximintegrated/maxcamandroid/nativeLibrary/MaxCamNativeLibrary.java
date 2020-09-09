package com.maximintegrated.maxcamandroid.nativeLibrary;

public class MaxCamNativeLibrary {

    static  {
        System.loadLibrary("native-lib");
    }

    private JniListener jniListener;

    private MaxCamNativeLibrary(){
        initialize();
    }

    private static MaxCamNativeLibrary INSTANCE = null;

    synchronized public static MaxCamNativeLibrary getInstance(JniListener listener){
         if (INSTANCE == null){
             INSTANCE = new MaxCamNativeLibrary();
             INSTANCE.jniListener = listener;
         }
         return INSTANCE;
    }

    private native void initialize();

    public native static void setMtu(int mtu);

    public native static int getMaxMtu();

    public native static String getVersion();

    public native static void sendFile(String fileName, byte[] data);

    public native static void bleDataReceived(byte[] data);

    public native static void getDirectoryRequest();

    public native static void getFile(String fileName);

    public native static void bleReset();

    public native static void enterDemo();

    public native static void captureImage(int fileLength);

    public native static void loadImage(String filename);

    public native static void sendImage(String filename, byte[] content);

    private void sendNotification(byte[] data){
        if(jniListener != null){
            jniListener.sendNotification(data);
        }
    }

    private void payloadReceived(byte[] payload){
        if(jniListener != null){
            jniListener.payloadReceived(payload);
        }
    }

    interface JniListener {
        void sendNotification(byte[] data);
        void payloadReceived(byte[] payload);
    }
}
