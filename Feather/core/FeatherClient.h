#pragma once

#include <jni.h>
#include "ModuleManager.h"
#include "LunarClientHook.h"

class FeatherClient {
public:
    FeatherClient();
    ~FeatherClient();

    bool initialize(LPVOID lpParam, const char* version);
    void update(JNIEnv* env, jobject player, jobject minecraft);
    void render(JNIEnv* env, jobject minecraft);

    void toggleModule(const std::string& moduleName, bool enabled);
    bool isModuleEnabled(const std::string& moduleName) const;

private:
    bool initializeJNI(LPVOID lpParam);
    jobject GetMinecraftInstance(JNIEnv* env);

    JavaVM* javaVM;
    ModuleManager moduleManager;

};