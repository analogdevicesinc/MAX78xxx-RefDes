package com.maximintegrated.maxcamandroid.nativeLibrary;

public interface IMaxCamNativeLibrary {
    void setJniListener(JniListener listener);

    void setMtu(int mtu);

    int getMaxMtu();

    String getVersion();

    void sendFile(String fileName, byte[] data);

    void bleDataReceived(byte[] data);

    void getDirectoryRequest();

    void getFile(String fileName);

    void bleReset();

    void enterDemo();

    void captureImage(int fileLength);

    void loadImage(String filename);

    void sendImage(String filename, byte[] content);

    void sendNotification(byte[] data);

    void payloadReceived(byte[] payload);

    interface JniListener {
        void sendNotification(byte[] data);

        void payloadReceived(byte[] payload);
    }
}
