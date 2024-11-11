#include "pch.h"
#include "FeatherClient.h"
#include "../hooks/MinecraftHook.h"
#include "../hooks/OpenGLHook.h"
#include "../utils/JNIHelper.h"
#include "../utils/VersionMapping.h"

FeatherClient::FeatherClient() {
    modules.push_back(std::make_unique<ReachModule>());
    modules.push_back(std::make_unique<ESPModule>());

    reachModule = dynamic_cast<ReachModule*>(modules[0].get());
    espModule = dynamic_cast<ESPModule*>(modules[1].get());
}

FeatherClient::~FeatherClient() {
    // No need for explicit deletion, unique_ptr handles it
    modules.clear();
}
void FeatherClient::initialize(LPVOID lpParam, const std::string& version) {
    MinecraftHook::initialize();
    OpenGLHook::initialize();
    VersionMapping::initialize(version);

    // Consider replacing this with a more sophisticated loop or threading mechanism
    while (true) {
        Sleep(10);
    }
}

void FeatherClient::update(JNIEnv* env, jobject player, jobject minecraft) {
    if (reachModule) {
        reachModule->onUpdate(env, player);
    }
    // Update other modules as needed
    for (const auto& module : modules) {
        module->onUpdate(env, player);
    }
}

void FeatherClient::render(JNIEnv* env, jobject minecraft) {
    static jclass minecraftClass = JNIHelper::findClass(env, VersionMapping::getClassName("Minecraft").c_str());
    if (minecraftClass) {
        static jmethodID getPlayerMethod = JNIHelper::getMethodID(env, minecraftClass, VersionMapping::getMethodName("getPlayer").c_str(), "()Lnet/minecraft/entity/player/EntityPlayer;");
        if (getPlayerMethod) {
            jobject player = env->CallObjectMethod(minecraft, getPlayerMethod);
            if (player) {
                // Renderizar ESP u otros overlays aquí
                env->DeleteLocalRef(player);
            }
        }
    }
}
