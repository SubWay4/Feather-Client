#include "pch.h"
#include "FeatherClient.h"
#include "../hooks/MinecraftHook.h"
#include "../hooks/OpenGLHook.h"


FeatherClient::FeatherClient() {
    reachModule = new ReachModule();
    espModule = new ESPModule();
    modules.push_back(reachModule);
    modules.push_back(espModule);
}

FeatherClient::~FeatherClient() {
    for (auto module : modules) {
        delete module;
    }
    modules.clear();
}

void FeatherClient::initialize(LPVOID lpParam) {
    MinecraftHook::initialize();
    OpenGLHook::initialize();

    while (true) {
        Sleep(10);
    }
}

void FeatherClient::update(JNIEnv* env, jobject player, jobject minecraft) {
    reachModule->onUpdate(env, player);
    // Otros módulos que necesiten actualizarse
}

void FeatherClient::render(JNIEnv* env, jobject minecraft) {
    espModule->onRender(env, minecraft);
}
