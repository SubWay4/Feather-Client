#include "pch.h"
#include "JNIHelper.h"

JNIEnv* JNIHelper::getJNIEnv() {
    JavaVM* jvm;
    jsize vmCount;
    if (JNI_GetCreatedJavaVMs(&jvm, 1, &vmCount) != JNI_OK || vmCount == 0) {
        return nullptr;
    }

    JNIEnv* env;
    jint result = jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (result == JNI_EDETACHED) {
        result = jvm->AttachCurrentThread((void**)&env, nullptr);
    }

    return (result == JNI_OK) ? env : nullptr;
}

jobject JNIHelper::getMinecraftInstance(JNIEnv* env) {
    jclass minecraftClass = findClass(env, "net/minecraft/client/Minecraft");
    if (!minecraftClass) return nullptr;

    jfieldID theMinecraftField = getFieldID(env, minecraftClass, "theMinecraft", "Lnet/minecraft/client/Minecraft;");
    if (!theMinecraftField) return nullptr;

    jobject minecraft = env->GetStaticObjectField(minecraftClass, theMinecraftField);
    env->DeleteLocalRef(minecraftClass);

    return minecraft;
}

jclass JNIHelper::findClass(JNIEnv* env, const char* name) {
    return env->FindClass(name);
}

jmethodID JNIHelper::getMethodID(JNIEnv* env, jclass clazz, const char* name, const char* sig) {
    return env->GetMethodID(clazz, name, sig);
}

jfieldID JNIHelper::getFieldID(JNIEnv* env, jclass clazz, const char* name, const char* sig) {
    return env->GetFieldID(clazz, name, sig);
}