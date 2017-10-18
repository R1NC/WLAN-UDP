package xyz.rinc.udp;

public class UDPServer {

    public static void start() {
        nativeUDPServerStart();
    }

    public static void stop() {
        nativeUDPServerStop();
    }

    static {
        System.loadLibrary("UDPServerJNI");
    }

    private static native void nativeUDPServerStart();

    private static native void nativeUDPServerStop();
    
}