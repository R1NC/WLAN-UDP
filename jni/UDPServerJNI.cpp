#include <string.h>
#include <stdlib.h>
#include "jni_utils.h"
#include "../src/udp_server.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_com_tencent_aisdk_udp_UDPServer_nativeUDPServerStart
(JNIEnv *env, jclass clazz, jstring device_info_json) {
    const char* device_info_jsonChar = env->GetStringUTFChars(device_info_json, 0);
    LOGD("server_start -> device_info_json:%s", device_info_jsonChar);
    udp_server_start(device_info_jsonChar);
    env->ReleaseStringUTFChars(device_info_json, device_info_jsonChar);
}

JNIEXPORT void JNICALL
Java_com_tencent_aisdk_udp_UDPServer_nativeUDPServerStop
(JNIEnv *env, jclass clazz) {
    udp_server_stop();
    LOGD("server_stop");
}

#ifdef __cplusplus
}
#endif