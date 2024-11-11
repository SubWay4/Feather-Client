#pragma once
#include <jni.h>
#include <jvmti.h>
#include <Windows.h>
#include <functional>
#include <unordered_map>

class LunarClientHook {
public:
    static bool initialize(JavaVM* jvm);
    static void shutdown();

    // Hooks
    static void hookAddToSendQueue(JNIEnv* env, jobject packet);
    static void hookUpdateWalkingPlayer(JNIEnv* env);
    static void hookAttackTargetEntityWithCurrentItem(JNIEnv* env, jobject entity);
    static void hookChannelRead0(JNIEnv* env, jobject packet);

    // Módulos
    static void toggleAimAssist(bool enabled);
    static void toggleReach(bool enabled);
    static void toggleFullBright(bool enabled);
    static void toggleVelocity(bool enabled);
    static void toggleLeftClicker(bool enabled);
    static void toggleFastPlace(bool enabled);
    static void toggleLegitScaffold(bool enabled);
    static void toggleSprint(bool enabled);
    static void toggleNoFall(bool enabled);

    // Event system
    typedef std::function<void(JNIEnv*, jobject)> EventCallback;
    static void registerEventCallback(const std::string& eventName, EventCallback callback);

private:
    static JavaVM* s_JavaVM;
    static jvmtiEnv* s_JvmtiEnv;

    static jclass findClass(JNIEnv* env, const char* name);
    static jmethodID getMethodID(JNIEnv* env, jclass clazz, const char* name, const char* sig);
    static void registerNatives(JNIEnv* env, const char* className, JNINativeMethod* methods, int numMethods);
    static void hookJavaMethod(const char* className, const char* methodName, const char* methodSignature, void* newMethod);
    static void unhookJavaMethod(const char* className, const char* methodName);

    static std::unordered_map<std::string, std::vector<EventCallback>> eventCallbacks;
    static std::unordered_map<std::string, void*> originalMethods;
};
