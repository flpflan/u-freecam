#include "bootstrap.hpp"
#include <thread>

#ifdef __ANDROID__
__attribute__((constructor)) void on_load() { std::thread(Bootstrap::Run).detach(); }

__attribute__((destructor)) void on_unload() { Bootstrap::Shutdown(); }
#else
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        std::thread(Bootstrap::Run).detach();
        break;

    case DLL_PROCESS_DETACH:
        if (lpReserved == nullptr)
        {
            Bootstrap::Shutdown();
        }
        break;

    default:
        break;
    }

    return TRUE;
}
#endif
