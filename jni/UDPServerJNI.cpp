#include <string.h>
#include <stdlib.h>
#include "jni_utils.h"
#include "../src/udp_server.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_com_tencent_aisdk_udp_UDPServer_nativeUDPServerStart
(JNIEnv *env, jclass clazz) {
    LOGD("server_start");
    udp_server_start();
}

JNIEXPORT void JNICALL
Java_com_tencent_aisdk_udp_UDPServer_nativeUDPServerStop
(JNIEnv *env, jclass clazz) {
    LOGD("server_stop");
    udp_server_stop();
}

#ifdef __cplusplus
}
#endif