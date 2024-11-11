#pragma once
#include <jni.h>

class JNIHelper {
public:
    static JNIEnv* getJNIEnv();
    static jobject getMinecraftInstance(JNIEnv* env);
    static jclass findClass(JNIEnv* env, const char* name);
    static jmethodID getMethodID(JNIEnv* env, jclass clazz, const char* name, const char* sig);
    static jfieldID getFieldID(JNIEnv* env, jclass clazz, const char* name, const char* sig);
};