#include "pch.h"
#include "core/FeatherClient.h"
#include <iostream>
#include <thread>

FeatherClient* g_FeatherClient = nullptr;

DWORD WINAPI MainThread(LPVOID lpParam) {
    g_FeatherClient = new FeatherClient();
    g_FeatherClient->initialize(lpParam, "1.8.9"); // Asumiendo que estamos usando la versión 1.8.9 de Lunar Client

    std::thread updateThread([&]() {
        while (true) {
            JNIEnv* env = g_FeatherClient->GetJNIEnv();
            if (env) {
                jobject minecraft = g_FeatherClient->GetMinecraftInstance(env);
                if (minecraft) {
                    jclass playerClass = env->FindClass("net/minecraft/client/entity/EntityPlayerSP");
                    jmethodID getPlayerMethod = env->GetMethodID(playerClass, "getPlayer", "()Lnet/minecraft/client/entity/EntityPlayerSP;");
                    jobject player = env->CallObjectMethod(minecraft, getPlayerMethod);

                    if (player) {
                        g_FeatherClient->update(env, player, minecraft);
                        env->DeleteLocalRef(player);
                    }

                    env->DeleteLocalRef(playerClass);
                    env->DeleteLocalRef(minecraft);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        });

    while (true) {
        g_FeatherClient->showModuleInterface();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    updateThread.join();

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        if (g_FeatherClient) {
            delete g_FeatherClient;
            g_FeatherClient = nullptr;
        }
        break;
    }
    return TRUE;
}
