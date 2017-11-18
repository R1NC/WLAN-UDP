#include <string.h>
#include <stdlib.h>
#include "jni_utils.h"

#include "../src/udp_client.h"

#ifdef __cplusplus
extern "C" {
#endif

JavaVM* jvm;
jobject java_udp_callback;

void jni_udp_callback(const char* ip, int req, char* data) {
    LOGD("callback -> ip:%s req:%d data:%s", ip, req, data);
    JNIEnv *env;
    if (jvm->AttachCurrentThread(&env, NULL) != JNI_OK) {
        LOGE("attach JVM thread failed");
        return;
    }
    jclass clazz = env->GetObjectClass(java_udp_callback);
    if (clazz) {
        jmethodID methodId = env->GetMethodID(clazz, "onUDPResponse", "(Ljava/lang/String;ILjava/lang/String;)V");
        if (methodId) {
            env->CallVoidMethod(java_udp_callback, methodId, env->NewStringUTF(ip), req, env->NewStringUTF(data));
        }
    }
    if (jvm->DetachCurrentThread() != JNI_OK) {
        LOGE("detach JVM thread failed");
    }
}

JNIEXPORT void JNICALL
Java_xyz_rinc_udp_UDPClient_nativeUDPClientStart
(JNIEnv *env, jclass clazz, jobject callback) {
    LOGD("client_start");
    if (env->GetJavaVM(&jvm) != JNI_OK) {
        LOGE("get JVM failed");           
    }
    java_udp_callback = (jobject)env->NewGlobalRef(callback);
    udp_client_start(jni_udp_callback);
}

JNIEXPORT void JNICALL
Java_xyz_rinc_udp_UDPClient_nativeUDPClientStop
(JNIEnv *env, jclass clazz) {
    LOGD("client_stop");
    udp_client_stop();
}

JNIEXPORT void JNICALL
Java_xyz_rinc_udp_UDPClient_nativeUDPClientRequestAll
(JNIEnv *env, jclass clazz, jint req) {
    LOGD("client_request_all -> req:%d ", req);
    udp_client_request_all(req);
}

#ifdef __cplusplus
}
#endif