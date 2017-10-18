package xyz.rinc.udp;

import android.content.Context;
import android.net.DhcpInfo;
import android.net.wifi.WifiManager;

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

    public static void request(String ip, int req) {
        nativeUDPClientRequest(ip, req);
    }

    /*
     * <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE"></uses-permission>
     * <uses-permission android:name="android.permission.ACCESS_WIFI_STATE"></uses-permission>
     */
    public static boolean requestAll(Context context, int req) {
        if (context != null) {
            WifiManager wm = (WifiManager)context.getSystemService(Context.WIFI_SERVICE);
            if (wm != null && wm.getWifiState() == WIFI_STATE_ENABLED) {
                DhcpInfo di = wm.getDhcpInfo();
                if (di != null && di.ipAddress > 0 && di.netmask > 0) {
                    nativeUDPClientRequestAll(req, i2s(di.ipAddress), i2s(di.netmask));
                    return true;
                }
            }
        }
        return false;
    }

    private static String i2s(int i) {  
        return (i & 0xFF) + "." + (0xFF & i >> 8) + "." + (0xFF & i >> 16) + "." + (0xFF & i >> 24);  
    }  

    static {
        System.loadLibrary("UDPClientJNI");
    }

    private static native void nativeUDPClientStart(Callback callback);

    private static native void nativeUDPClientStop();

    private static native void nativeUDPClientRequest(String ip, int req);

    private static native void nativeUDPClientRequestAll(int req, String lanIp, String subnetMask);
    
}