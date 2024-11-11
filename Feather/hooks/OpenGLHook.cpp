#include "pch.h"
#include "OpenGLHook.h"
#include "../core/FeatherClient.h"
#include <detours.h>

extern FeatherClient* g_FeatherClient;

typedef BOOL(WINAPI* twglSwapBuffers) (HDC hDc);
twglSwapBuffers owglSwapBuffers = nullptr;

BOOL WINAPI hkwglSwapBuffers(HDC hDc) {
    // Perform any pre-render operations here

    // Call the original function
    BOOL result = owglSwapBuffers(hDc);

    // Perform any post-render operations here
    if (g_FeatherClient) {
        JNIEnv* env = g_FeatherClient->getJNIEnv();
        if (env) {
            jobject minecraft = g_FeatherClient->getMinecraftInstance(env);
            if (minecraft) {
                g_FeatherClient->render(env, minecraft);
                env->DeleteLocalRef(minecraft);
            }
        }
    }

    return result;
}

bool OpenGLHook::initialize() {
    HMODULE hMod = GetModuleHandle(TEXT("opengl32.dll"));
    if (hMod == NULL) return false;

    void* ptrSwapBuffers = GetProcAddress(hMod, "wglSwapBuffers");
    if (ptrSwapBuffers == NULL) return false;

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)owglSwapBuffers, hkwglSwapBuffers);
    LONG error = DetourTransactionCommit();

    return (error == NO_ERROR);
}

void OpenGLHook::shutdown() {
    if (owglSwapBuffers) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)owglSwapBuffers, hkwglSwapBuffers);
        DetourTransactionCommit();
    }
}