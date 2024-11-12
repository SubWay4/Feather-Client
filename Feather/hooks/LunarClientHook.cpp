#include "pch.h"
#include "LunarClientHook.h"
#include <iostream>
#include <MinHook.h>

JavaVM* LunarClientHook::s_JavaVM = nullptr;
jvmtiEnv* LunarClientHook::s_JvmtiEnv = nullptr;
std::unordered_map<std::string, std::vector<LunarClientHook::EventCallback>> LunarClientHook::eventCallbacks;
std::unordered_map<std::string, void*> LunarClientHook::originalMethods;

bool LunarClientHook::initialize(JavaVM* jvm) {
    s_JavaVM = jvm;
    jint res = s_JavaVM->GetEnv((void**)&s_JvmtiEnv, JVMTI_VERSION_1_2);
    if (res != JNI_OK) {
        std::cout << "Failed to get JVMTI environment" << std::endl;
        return false;
    }

    jvmtiCapabilities capabilities = { 0 };
    capabilities.can_generate_all_class_hook_events = 1;
    capabilities.can_retransform_classes = 1;
    capabilities.can_retransform_any_class = 1;
    s_JvmtiEnv->AddCapabilities(&capabilities);

    // Initialize MinHook
    if (MH_Initialize() != MH_OK) {
        std::cout << "Failed to initialize MinHook" << std::endl;
        return false;
    }

    // Implement method hooking
    hookJavaMethod("net/minecraft/network/NetworkManager", "addToSendQueue", "(Lnet/minecraft/network/Packet;)V", (void*)hookAddToSendQueue);
    hookJavaMethod("net/minecraft/client/entity/EntityPlayerSP", "onUpdateWalkingPlayer", "()V", (void*)hookUpdateWalkingPlayer);
    hookJavaMethod("net/minecraft/client/entity/EntityPlayerSP", "attackTargetEntityWithCurrentItem", "(Lnet/minecraft/entity/Entity;)V", (void*)hookAttackTargetEntityWithCurrentItem);
    hookJavaMethod("net/minecraft/network/NetworkManager", "channelRead0", "(Lio/netty/channel/ChannelHandlerContext;Lnet/minecraft/network/Packet;)V", (void*)hookChannelRead0);

    return true;
}

void LunarClientHook::hookJavaMethod(const char* className, const char* methodName, const char* methodSignature, void* newMethod) {
    JNIEnv* env;
    s_JavaVM->GetEnv((void**)&env, JNI_VERSION_1_6);

    jclass targetClass = env->FindClass(className);
    if (targetClass == nullptr) {
        std::cout << "Failed to find class: " << className << std::endl;
        return;
    }

    jmethodID targetMethod = env->GetMethodID(targetClass, methodName, methodSignature);
    if (targetMethod == nullptr) {
        std::cout << "Failed to find method: " << methodName << std::endl;
        return;
    }

    void* originalMethod = (void*)targetMethod;

    // Use MinHook to hook the method
    MH_STATUS status = MH_CreateHook(originalMethod, newMethod, reinterpret_cast<void**>(&originalMethod));
    if (status != MH_OK) {
        std::cout << "Failed to create hook: " << MH_StatusToString(status) << std::endl;
        return;
    }

    status = MH_EnableHook(originalMethod);
    if (status != MH_OK) {
        std::cout << "Failed to enable hook: " << MH_StatusToString(status) << std::endl;
        return;
    }

    // Store the original method for later use (e.g., unhooking)
    std::string key = std::string(className) + "::" + methodName;
    originalMethods[key] = originalMethod;

    std::cout << "Successfully hooked method: " << className << "::" << methodName << std::endl;
}

void LunarClientHook::unhookJavaMethod(const char* className, const char* methodName) {
    std::string key = std::string(className) + "::" + methodName;
    auto it = originalMethods.find(key);
    if (it == originalMethods.end()) {
        std::cout << "Method not found for unhooking: " << key << std::endl;
        return;
    }

    void* originalMethod = it->second;

    MH_STATUS status = MH_DisableHook(originalMethod);
    if (status != MH_OK) {
        std::cout << "Failed to disable hook: " << MH_StatusToString(status) << std::endl;
        return;
    }

    originalMethods.erase(it);
    std::cout << "Successfully unhooked method: " << key << std::endl;
}

void LunarClientHook::shutdown() {
    // Unhook all methods
    for (const auto& pair : originalMethods) {
        std::string fullName = pair.first;
        size_t separatorPos = fullName.find("::");
        if (separatorPos != std::string::npos) {
            std::string className = fullName.substr(0, separatorPos);
            std::string methodName = fullName.substr(separatorPos + 2);
            unhookJavaMethod(className.c_str(), methodName.c_str());
        }
    }

    if (s_JvmtiEnv) {
        s_JvmtiEnv->DisposeEnvironment();
        s_JvmtiEnv = nullptr;
    }
    s_JavaVM = nullptr;
    eventCallbacks.clear();
    originalMethods.clear();

    MH_Uninitialize();
}


void LunarClientHook::hookAddToSendQueue(JNIEnv* env, jobject packet) {
    for (auto& callback : eventCallbacks["AddToSendQueue"]) {
        callback(env, packet);
    }
}

void LunarClientHook::hookUpdateWalkingPlayer(JNIEnv* env) {
    for (auto& callback : eventCallbacks["UpdateWalkingPlayer"]) {
        callback(env, nullptr);
    }
}

void LunarClientHook::hookAttackTargetEntityWithCurrentItem(JNIEnv* env, jobject entity) {
    for (auto& callback : eventCallbacks["AttackTargetEntityWithCurrentItem"]) {
        callback(env, entity);
    }
}

void LunarClientHook::hookChannelRead0(JNIEnv* env, jobject packet) {
    for (auto& callback : eventCallbacks["ChannelRead0"]) {
        callback(env, packet);
    }
}

void LunarClientHook::toggleAimAssist(bool enabled) {
    std::cout << "AimAssist " << (enabled ? "enabled" : "disabled") << std::endl;
}

void LunarClientHook::toggleReach(bool enabled) {
    std::cout << "Reach " << (enabled ? "enabled" : "disabled") << std::endl;
}

void LunarClientHook::toggleFullBright(bool enabled) {
    std::cout << "FullBright " << (enabled ? "enabled" : "disabled") << std::endl;
}

void LunarClientHook::toggleVelocity(bool enabled) {
    std::cout << "Velocity " << (enabled ? "enabled" : "disabled") << std::endl;
}

void LunarClientHook::toggleLeftClicker(bool enabled) {
    std::cout << "LeftClicker " << (enabled ? "enabled" : "disabled") << std::endl;
}

void LunarClientHook::toggleFastPlace(bool enabled) {
    std::cout << "FastPlace " << (enabled ? "enabled" : "disabled") << std::endl;
}

void LunarClientHook::toggleLegitScaffold(bool enabled) {
    std::cout << "LegitScaffold " << (enabled ? "enabled" : "disabled") << std::endl;
}

void LunarClientHook::toggleSprint(bool enabled) {
    std::cout << "Sprint " << (enabled ? "enabled" : "disabled") << std::endl;
}

void LunarClientHook::toggleNoFall(bool enabled) {
    std::cout << "NoFall " << (enabled ? "enabled" : "disabled") << std::endl;
}

void LunarClientHook::registerEventCallback(const std::string& eventName, EventCallback callback) {
    eventCallbacks[eventName].push_back(callback);
}

jclass LunarClientHook::findClass(JNIEnv* env, const char* name) {
    return env->FindClass(name);
}

jmethodID LunarClientHook::getMethodID(JNIEnv* env, jclass clazz, const char* name, const char* sig) {
    return env->GetMethodID(clazz, name, sig);
}

void LunarClientHook::registerNatives(JNIEnv* env, const char* className, JNINativeMethod* methods, int numMethods) {
    jclass clazz = findClass(env, className);
    if (clazz == nullptr) {
        std::cout << "Failed to find class: " << className << std::endl;
        return;
    }
    if (env->RegisterNatives(clazz, methods, numMethods) < 0) {
        std::cout << "Failed to register native methods for " << className << std::endl;
    }
}
