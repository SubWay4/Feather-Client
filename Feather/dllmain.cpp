
#include "../utils/meta_jni.hpp"
#include "../utils/mappings.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <Windows.h>
#include <stdexcept>

// Variable global para controlar la ejecución del hilo principal
bool g_running = true;

// Función para crear una consola externa
void CreateConsole() {
    if (!AllocConsole()) {
        throw std::runtime_error("No se pudo crear la consola");
    }
    FILE* f;
    if (freopen_s(&f, "CONOUT$", "w", stdout) != 0 ||
        freopen_s(&f, "CONOUT$", "w", stderr) != 0 ||
        freopen_s(&f, "CONIN$", "r", stdin) != 0) {
        throw std::runtime_error("No se pudieron redirigir los flujos estándar");
    }
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();
}

// Función para liberar la consola
void ReleaseConsole() {
    if (!FreeConsole()) {
        std::cerr << "Error al liberar la consola" << std::endl;
    }
}

// Función para inyectar y ejecutar el cliente
void mainThread(void* dll) {
    try {
        CreateConsole();
        std::cout << "Cliente de Minecraft iniciado." << std::endl;

        JavaVM* jvm = nullptr;
        if (JNI_GetCreatedJavaVMs(&jvm, 1, nullptr) != JNI_OK || jvm == nullptr) {
            throw std::runtime_error("No se pudo obtener la JVM");
        }

        JNIEnv* env = nullptr;
        if (jvm->AttachCurrentThread((void**)&env, nullptr) != JNI_OK || env == nullptr) {
            throw std::runtime_error("No se pudo adjuntar el hilo a la JVM");
        }

        jni::init();
        jni::set_thread_env(env);

        maps::Minecraft minecraft{};
        maps::Minecraft theMinecraft = minecraft.theMinecraft.get();
        maps::EntityPlayerSP thePlayer = theMinecraft.thePlayer.get();

        std::cout << "Enviando mensaje de chat..." << std::endl;
        thePlayer.sendChatMessage(maps::String::create("¡Hola desde el cliente personalizado!"));

        std::cout << "Haciendo clic..." << std::endl;
        theMinecraft.clickMouse();

        while (g_running) {
            if (GetAsyncKeyState(VK_END) & 0x8000) {
                std::cout << "Tecla END presionada. Desinyectando..." << std::endl;
                g_running = false;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        jni::shutdown();
        if (jvm->DetachCurrentThread() != JNI_OK) {
            std::cerr << "Error al desadjuntar el hilo de la JVM" << std::endl;
        }

        std::cout << "Cliente de Minecraft finalizado." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Error desconocido" << std::endl;
    }

    ReleaseConsole();
    FreeLibraryAndExitThread(static_cast<HMODULE>(dll), 0);
}

// Punto de entrada para la DLL
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        if (CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)mainThread, hModule, 0, nullptr) == nullptr) {
            return FALSE;
        }
        break;
    case DLL_PROCESS_DETACH:
        g_running = false;
        break;
    }
    return TRUE;
}
