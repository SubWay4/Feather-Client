#include "pch.h"
#include "FeatherClient.h"
#include <iostream>

FeatherClient::FeatherClient() : javaVM(nullptr), moduleManager(ModuleManager::getInstance()) {}

FeatherClient::~FeatherClient() {
    LunarClientHook::shutdown();
}

void FeatherClient::initialize(LPVOID lpParam, const std::string& version) {
    if (!initializeJNI(lpParam)) {
        std::cout << "Failed to initialize JNI" << std::endl;
        return;
    }

    if (!LunarClientHook::initialize(javaVM)) {
        std::cout << "Failed to initialize LunarClientHook" << std::endl;
        return;
    }

    // Register event callbacks
    LunarClientHook::registerEventCallback("AddToSendQueue", [this](JNIEnv* env, jobject packet) {
        // Implement packet modification logic here
        std::cout << "Packet sent" << std::endl;
        });

    LunarClientHook::registerEventCallback("UpdateWalkingPlayer", [this](JNIEnv* env, jobject) {
        // Implement walking player update logic here
        std::cout << "Walking player updated" << std::endl;
        });

    LunarClientHook::registerEventCallback("AttackTargetEntityWithCurrentItem", [this](JNIEnv* env, jobject entity) {
        // Implement attack logic here
        std::cout << "Entity attacked" << std::endl;
        });

    LunarClientHook::registerEventCallback("ChannelRead0", [this](JNIEnv* env, jobject packet) {
        // Implement packet receive logic here
        std::cout << "Packet received" << std::endl;
        });

    std::cout << "FeatherClient initialized for Lunar Client " << version << std::endl;
}

void FeatherClient::update(JNIEnv* env, jobject player, jobject minecraft) {
    if (env && player && minecraft) {
        // Aquí puedes implementar la lógica de actualización para cada módulo activo
        if (isModuleEnabled("AimAssist")) {
            // Implementar lógica de AimAssist
        }
        if (isModuleEnabled("Reach")) {
            // Implementar lógica de Reach
        }
        // ... implementar lógica para otros módulos ...

        LunarClientHook::hookUpdateWalkingPlayer(env);
    }
}

void FeatherClient::render(JNIEnv* env, jobject minecraft) {
    if (env && minecraft) {
        // Aquí puedes implementar la lógica de renderizado para cada módulo activo
        if (isModuleEnabled("ESP")) {
            // Implementar lógica de ESP
        }
        if (isModuleEnabled("FullBright")) {
            // Implementar lógica de FullBright
        }
        // ... implementar lógica para otros módulos visuales ...

        // Ejemplo de cómo obtener información del jugador para renderizar
        jclass playerClass = env->FindClass("net/minecraft/client/entity/EntityPlayerSP");
        jmethodID getPlayerMethod = env->GetMethodID(playerClass, "getPlayer", "()Lnet/minecraft/client/entity/EntityPlayerSP;");
        jobject player = env->CallObjectMethod(minecraft, getPlayerMethod);

        if (player) {
            // Usar la información del jugador para renderizar
            // ...

            env->DeleteLocalRef(player);
        }

        env->DeleteLocalRef(playerClass);
    }
}

JNIEnv* FeatherClient::GetJNIEnv() {
    if (!javaVM) {
        return nullptr;
    }

    JNIEnv* env;
    jint result = javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (result == JNI_EDETACHED) {
        if (javaVM->AttachCurrentThread((void**)&env, nullptr) != 0) {
            return nullptr;
        }
    }
    else if (result != JNI_OK) {
        return nullptr;
    }

    return env;
}

jobject FeatherClient::GetMinecraftInstance(JNIEnv* env) {
    // TODO: Implementar la lógica real para obtener la instancia de Minecraft
    // Esta es una implementación de ejemplo y necesitará ser adaptada para Lunar Client
    static jclass minecraftClass = env->FindClass("net/minecraft/client/Minecraft");
    if (minecraftClass == nullptr) {
        std::cout << "Failed to find Minecraft class" << std::endl;
        return nullptr;
    }

    static jmethodID getMinecraftMethod = env->GetStaticMethodID(minecraftClass, "getMinecraft", "()Lnet/minecraft/client/Minecraft;");
    if (getMinecraftMethod == nullptr) {
        std::cout << "Failed to find getMinecraft method" << std::endl;
        return nullptr;
    }

    jobject minecraftInstance = env->CallStaticObjectMethod(minecraftClass, getMinecraftMethod);
    if (minecraftInstance == nullptr) {
        std::cout << "Failed to get Minecraft instance" << std::endl;
        return nullptr;
    }

    return minecraftInstance;
}

void FeatherClient::toggleModule(const std::string& moduleName, bool enabled) {
    moduleManager.toggleModule(moduleName, enabled);
}

bool FeatherClient::isModuleEnabled(const std::string& moduleName) const {
    return moduleManager.isModuleEnabled(moduleName);
}

void FeatherClient::showModuleInterface() {
    std::cout << "=== Feather Client Module Interface ===" << std::endl;
    std::cout << "1. AimAssist: " << (isModuleEnabled("AimAssist") ? "Enabled" : "Disabled") << std::endl;
    std::cout << "2. Reach: " << (isModuleEnabled("Reach") ? "Enabled" : "Disabled") << std::endl;
    std::cout << "3. FullBright: " << (isModuleEnabled("FullBright") ? "Enabled" : "Disabled") << std::endl;
    std::cout << "4. Velocity: " << (isModuleEnabled("Velocity") ? "Enabled" : "Disabled") << std::endl;
    std::cout << "5. LeftClicker: " << (isModuleEnabled("LeftClicker") ? "Enabled" : "Disabled") << std::endl;
    std::cout << "6. FastPlace: " << (isModuleEnabled("FastPlace") ? "Enabled" : "Disabled") << std::endl;
    std::cout << "7. LegitScaffold: " << (isModuleEnabled("LegitScaffold") ? "Enabled" : "Disabled") << std::endl;
    std::cout << "8. Sprint: " << (isModuleEnabled("Sprint") ? "Enabled" : "Disabled") << std::endl;
    std::cout << "9. NoFall: " << (isModuleEnabled("NoFall") ? "Enabled" : "Disabled") << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Enter the number of the module to toggle: ";

    int choice;
    std::cin >> choice;

    if (choice >= 1 && choice <= 9) {
        const char* moduleNames[] = { "AimAssist", "Reach", "FullBright", "Velocity", "LeftClicker", "FastPlace", "LegitScaffold", "Sprint", "NoFall" };
        std::string moduleName = moduleNames[choice - 1];
        bool newState = !isModuleEnabled(moduleName);
        toggleModule(moduleName, newState);
        std::cout << moduleName << " is now " << (newState ? "Enabled" : "Disabled") << std::endl;
    }
    else if (choice != 0) {
        std::cout << "Invalid choice. Please try again." << std::endl;
    }
}

bool FeatherClient::initializeJNI(LPVOID lpParam) {
    JavaVM* jvm = *(JavaVM**)lpParam;
    javaVM = jvm;

    JNIEnv* env;
    if (jvm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        std::cout << "Failed to get JNI environment" << std::endl;
        return false;
    }

    return true;
}
