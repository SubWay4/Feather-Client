#include "../../pch.h"
#include "MinecraftHook.h"
#include "../core/FeatherClient.h"

extern FeatherClient* g_FeatherClient;

jvmtiEnv* jvmti = nullptr;
JavaVM* jvm = nullptr;

void JNICALL onUpdateWalkingPlayer(jvmtiEnv* jvmti_env, JNIEnv* jni_env, jthread thread, jmethodID method, jboolean was_popped_by_exception, jvalue return_value) {
    jobject player = nullptr;
    jclass playerClass = jni_env->FindClass("net/minecraft/client/entity/EntityPlayerSP");
    jfieldID thePlayerField = jni_env->GetStaticFieldID(playerClass, "thePlayer", "Lnet/minecraft/client/entity/EntityPlayerSP;");
    player = jni_env->GetStaticObjectField(playerClass, thePlayerField);

    jobject minecraft = nullptr;
    jclass minecraftClass = jni_env->FindClass("net/minecraft/client/Minecraft");
    jfieldID theMinecraftField = jni_env->GetStaticFieldID(minecraftClass, "theMinecraft", "Lnet/minecraft/client/Minecraft;");
    minecraft = jni_env->GetStaticObjectField(minecraftClass, theMinecraftField);

    g_FeatherClient->update(jni_env, player, minecraft);

    jni_env->DeleteLocalRef(player);
    jni_env->DeleteLocalRef(playerClass);
    jni_env->DeleteLocalRef(minecraft);
    jni_env->DeleteLocalRef(minecraftClass);
}

bool MinecraftHook::initialize() {
    jsize count;
    if (JNI_GetCreatedJavaVMs(&jvm, 1, &count) != JNI_OK || count == 0) {
        return false;
    }

    jint res = jvm->GetEnv((void**)&jvmti, JVMTI_VERSION_1_0);
    if (res != JNI_OK) {
        return false;
    }

    jvmtiCapabilities capabilities = { 0 };
    capabilities.can_generate_method_exit_events = 1;
    jvmti->AddCapabilities(&capabilities);

    jvmtiEventCallbacks callbacks = { 0 };
    callbacks.MethodExit = onUpdateWalkingPlayer;
    jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks));

    jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_METHOD_EXIT, NULL);

    return true;
}

void MinecraftHook::hookUpdateMethod(JNIEnv* env) {
    jclass playerClass = env->FindClass("net/minecraft/client/entity/EntityPlayerSP");
    jmethodID updateWalkingPlayerMethod = env->GetMethodID(playerClass, "onUpdateWalkingPlayer", "()V");

    jvmti->SetBreakpoint(updateWalkingPlayerMethod, 0);

    env->DeleteLocalRef(playerClass);
}
