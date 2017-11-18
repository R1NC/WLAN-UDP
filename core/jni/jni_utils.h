#ifndef jni_utils_h
#define jni_utils_h

#include <jni.h>
#include <android/log.h>

#define LOG_TAG "#UDPJNI#"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#endif
