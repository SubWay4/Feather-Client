#pragma once
#include <vector>
#include <memory>
#include "../hooks/LunarClientHook.h"
#include "ModuleManager.h"

class FeatherClient {
public:
    FeatherClient();
    ~FeatherClient();

    void initialize(LPVOID lpParam, const std::string& version);
    void update(JNIEnv* env, jobject player, jobject minecraft);
    void render(JNIEnv* env, jobject minecraft);
    JNIEnv* GetJNIEnv();
    jobject GetMinecraftInstance(JNIEnv* env);

    // Interfaz para manejar m�dulos
    void toggleModule(const std::string& moduleName, bool enabled);
    bool isModuleEnabled(const std::string& moduleName) const;

    // M�todo para mostrar la interfaz de usuario
    void showModuleInterface();

private:
    JavaVM* javaVM;  // Puntero a la maquina virtual de Java
    bool initializeJNI(LPVOID lpParam);
    ModuleManager& moduleManager;
};
