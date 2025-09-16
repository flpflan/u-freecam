#include "bootstrap.hpp"
#include <thread>

#if ANDROID_MODE
__attribute__((constructor)) void on_load() { std::thread(Bootstrap::Run).detach(); }

__attribute__((destructor)) void on_unload() { Bootstrap::Shutdown(); }
#else
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        std::thread(Bootstrap::run).detach();
        break;

    case DLL_PROCESS_DETACH:
        if (lpReserved == nullptr)
        {
            Bootstrap::shutdown();
        }
        break;

    default:
        break;
    }

    return TRUE;
}
#endif
