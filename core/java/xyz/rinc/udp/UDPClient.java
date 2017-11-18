package xyz.rinc.udp;

public class UDPClient {

    public static final int REQ_MAC_ADDRESS = 1;

    public interface Callback {
        public void onUDPResponse(String ip, int req, String data);
    }

    public static void start(Callback callback) {
        nativeUDPClientStart(callback);
    }

    public static void stop() {
        nativeUDPClientStop();
    }

    public static void requestAll(int req) {
        nativeUDPClientRequestAll(req, i2s(di.ipAddress), i2s(di.netmask));
    }

    static {
        System.loadLibrary("UDPClientJNI");
    }

    private static native void nativeUDPClientStart(Callback callback);

    private static native void nativeUDPClientStop();

    private static native void nativeUDPClientRequestAll(int req);
    
}