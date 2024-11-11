#include "pch.h"
#include "OpenGLHook.h"
#include "../core/FeatherClient.h"
#include <detours.h>

extern FeatherClient* g_FeatherClient;

typedef BOOL(WINAPI* twglSwapBuffers) (HDC hDc);
twglSwapBuffers owglSwapBuffers;

BOOL WINAPI hkwglSwapBuffers(HDC hDc) {
    // Obtener el entorno JNI y el objeto Minecraft
    JNIEnv* env;
    jsize count;
    JavaVM* jvm;
    JNI_GetCreatedJavaVMs(&jvm, 1, &count);
    jvm->AttachCurrentThread((void**)&env, NULL);

    jclass minecraftClass = env->FindClass("net/minecraft/client/Minecraft");
    jfieldID theMinecraftField = env->GetStaticFieldID(minecraftClass, "theMinecraft", "Lnet/minecraft/client/Minecraft;");
    jobject minecraft = env->GetStaticObjectField(minecraftClass, theMinecraftField);

    // Renderizar nuestro ESP
    g_FeatherClient->render(env, minecraft);

    // Limpiar referencias
    env->DeleteLocalRef(minecraftClass);
    env->DeleteLocalRef(minecraft);

    // Llamar a la función original
    return owglSwapBuffers(hDc);
}

bool OpenGLHook::initialize() {
    HMODULE hMod = GetModuleHandle(L"opengl32.dll");
    if (hMod == NULL) return false;

    void* ptrSwapBuffers = GetProcAddress(hMod, "wglSwapBuffers");
    if (ptrSwapBuffers == NULL) return false;

    // Crear el hook
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)owglSwapBuffers, hkwglSwapBuffers);
    LONG error = DetourTransactionCommit();

    if (error != NO_ERROR) {
        return false;
    }

    return true;
}

void OpenGLHook::renderOverlay() {
    // Esta función puede quedarse vacía ya que el renderizado se maneja en hkwglSwapBuffers
}
