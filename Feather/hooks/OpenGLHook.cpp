#include "pch.h"
#include "OpenGLHook.h"
#include <MinHook.h>

// Definición de tipos de punteros a funciones
typedef BOOL(WINAPI* SwapBuffers_t)(HDC);
typedef BOOL(WINAPI* wglSwapBuffers_t)(HDC);

// Variables para almacenar los punteros a las funciones originales
SwapBuffers_t originalSwapBuffers = nullptr;
wglSwapBuffers_t originalWglSwapBuffers = nullptr;

// Funciones de hook
BOOL WINAPI hookedSwapBuffers(HDC hdc) {
    // Aquí puedes agregar tu lógica personalizada antes de llamar a la función original
    return originalSwapBuffers(hdc);
}

BOOL WINAPI hookedWglSwapBuffers(HDC hdc) {
    // Aquí puedes agregar tu lógica personalizada antes de llamar a la función original
    return originalWglSwapBuffers(hdc);
}

bool OpenGLHook::initialize() {
    if (MH_Initialize() != MH_OK) {
        return false;
    }

    HMODULE hOpenGL = GetModuleHandleA("opengl32.dll");
    if (!hOpenGL) {
        return false;
    }

    void* pSwapBuffers = GetProcAddress(hOpenGL, "SwapBuffers");
    void* pWglSwapBuffers = GetProcAddress(hOpenGL, "wglSwapBuffers");

    if (MH_CreateHook(pSwapBuffers, &hookedSwapBuffers, reinterpret_cast<LPVOID*>(&originalSwapBuffers)) != MH_OK) {
        return false;
    }

    if (MH_CreateHook(pWglSwapBuffers, &hookedWglSwapBuffers, reinterpret_cast<LPVOID*>(&originalWglSwapBuffers)) != MH_OK) {
        return false;
    }

    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
        return false;
    }

    return true;
}

void OpenGLHook::shutdown() {
    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
}
