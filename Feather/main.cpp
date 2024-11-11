#include "pch.h"
#include "core/FeatherClient.h"

FeatherClient* g_FeatherClient = nullptr;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_FeatherClient = new FeatherClient();
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)g_FeatherClient->initialize, nullptr, 0, nullptr);
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
