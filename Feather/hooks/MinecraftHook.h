#pragma once

#include <jni.h>
#include <jvmti.h>

class MinecraftHook {
public:
    static bool initialize();
    static void hookUpdateMethod(JNIEnv* env);
};

