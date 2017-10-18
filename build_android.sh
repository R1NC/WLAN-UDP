rm -rf obj
rm -rf jniLibs
ndk-build NDK_PROJECT_PATH=. NDK_APPLICATION_MK=./jni/Application.mk NDK_LIBS_OUT=./jniLibs
rm -rf obj
