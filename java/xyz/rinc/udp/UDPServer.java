package xyz.rinc.udp;

public class UDPServer {

    public static void start(String deviceInfoJson) {
        nativeUDPServerStart(deviceInfoJson);
    }

    public static void stop() {
        nativeUDPServerStop();
    }

    static {
        System.loadLibrary("UDPServerJNI");
    }

    private static native void nativeUDPServerStart(String deviceInfoJson);

    private static native void nativeUDPServerStop();
    
}