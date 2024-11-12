#pragma once

#include <jni.h>
#include <jvmti.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include "MinHook.h"

class FeatherClient;  // Forward declaration

class LunarClientHook {
public:
    using EventCallback = std::function<void(JNIEnv*, jobject)>;

    static bool initialize(JavaVM* jvm);
    static void shutdown();

    static void hookJavaMethod(const char* className, const char* methodName, const char* methodSignature, void* newMethod);
    static void unhookJavaMethod(const char* className, const char* methodName);

    static void registerEventCallback(const std::string& eventName, EventCallback callback);

    // Module toggle functions
    static void toggleAimAssist(bool enabled);
    static void toggleReach(bool enabled);
    static void toggleFullBright(bool enabled);
    static void toggleVelocity(bool enabled);
    static void toggleLeftClicker(bool enabled);
    static void toggleFastPlace(bool enabled);
    static void toggleLegitScaffold(bool enabled);
    static void toggleSprint(bool enabled);
    static void toggleNoFall(bool enabled);

private:
    static JavaVM* s_JavaVM;
    static jvmtiEnv* s_JvmtiEnv;
    static std::unordered_map<std::string, std::vector<EventCallback>> eventCallbacks;
    static std::unordered_map<std::string, void*> originalMethods;

    static void hookAddToSendQueue(JNIEnv* env, jobject packet);
    static void hookUpdateWalkingPlayer(JNIEnv* env);
    static void hookAttackTargetEntityWithCurrentItem(JNIEnv* env, jobject entity);
    static void hookChannelRead0(JNIEnv* env, jobject packet);

    static jclass findClass(JNIEnv* env, const char* name);
    static jmethodID getMethodID(JNIEnv* env, jclass clazz, const char* name, const char* sig);
    static void registerNatives(JNIEnv* env, const char* className, JNINativeMethod* methods, int numMethods);
