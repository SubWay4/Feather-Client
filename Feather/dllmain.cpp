#include "pch.h"
#include "core/FeatherClient.h"

FeatherClient* g_FeatherClient = nullptr;

DWORD WINAPI InitializeClient(LPVOID lpParam) {
    FeatherClient* client = static_cast<FeatherClient*>(lpParam);
    client->initialize(lpParam, "1.8.9");  // Pasa la versión deseada aquí
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_FeatherClient = new FeatherClient();
        CreateThread(nullptr, 0, InitializeClient, g_FeatherClient, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        if (g_FeatherClient)
        {
            delete g_FeatherClient;
            g_FeatherClient = nullptr;
        }
        break;
    }
    return TRUE;
}
