#include "freecam/module.hpp"
#include "umod/bootstrap.hpp"
#include "umod/core.hpp"

#include <thread>

#ifdef __ANDROID__
#include "umod/debug/crash_handler.hpp"
__attribute__((constructor)) void on_load()
{
    umod::debug::crash_handler::setup();

    umod::bootstrap::addFeature(freecam::kDesc);

    std::thread(umod::bootstrap::run).detach();
}

__attribute__((destructor)) void on_unload() { umod::core::shutdown(); }
#else
#include "Windows.h"
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        umod::bootstrap::addFeature(freecam::kDesc);
        std::thread(umod::core::run).detach();
        break;

    case DLL_PROCESS_DETACH:
        if (lpReserved == nullptr)
        {
            umod::bootstrap::shutdown();
        }
        break;

    default:
        break;
    }

    return TRUE;
}
#endif
