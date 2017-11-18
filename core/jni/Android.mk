LOCAL_PATH := $(call my-dir)

# build executable for device UDP Server
include $(CLEAR_VARS)
LOCAL_MODULE := UDPServerEXE
LOCAL_SRC_FILES := \
../src/main_server.c \
../src/udp_server.c \
../src/udp_util.c \
../src/net_util.c \
../src/utils.c
include $(BUILD_EXECUTABLE)

# build executable for device UDP Client
include $(CLEAR_VARS)
LOCAL_MODULE := UDPClientEXE
LOCAL_SRC_FILES := \
../src/main_client.c \
../src/udp_client.c \
../src/udp_util.c \
../src/net_util.c \
../src/utils.c
include $(BUILD_EXECUTABLE)

# build lib for native UDP Server
include $(CLEAR_VARS)
LOCAL_MODULE := UDPServer
LOCAL_SRC_FILES := \
../src/udp_server.c \
../src/udp_util.c \
../src/net_util.c \
../src/utils.c
include $(BUILD_SHARED_LIBRARY)

# build lib for native UDP Client
include $(CLEAR_VARS)
LOCAL_MODULE := UDPClient
LOCAL_SRC_FILES := \
../src/udp_client.c \
../src/udp_util.c \
../src/net_util.c \
../src/utils.c
include $(BUILD_SHARED_LIBRARY)

# build lib for jni UDP Server
include $(CLEAR_VARS)
LOCAL_MODULE := UDPServerJNI
LOCAL_SRC_FILES := \
UDPServerJNI.cpp \
../src/udp_server.c \
../src/udp_util.c \
../src/net_util.c \
../src/utils.c
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)

# build lib for jni UDP Client
include $(CLEAR_VARS)
LOCAL_MODULE := UDPClientJNI
LOCAL_SRC_FILES := \
UDPClientJNI.cpp \
../src/udp_client.c \
../src/udp_util.c \
../src/net_util.c \
../src/utils.c
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)