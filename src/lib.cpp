#include "umod/bootstrap.hpp"
#include "umod/core.hpp"

#include "freecam/module.hpp"
#include "webui.hpp"

#include <thread>

#ifdef __ANDROID__
#include "umod/debug/crash_handler.hpp"
__attribute__((constructor)) void on_load()
{
    umod::debug::crash_handler::setup();

    std::thread(webui::start).detach();

    umod::bootstrap::addFeature(freecam::kDesc);
    std::thread(umod::bootstrap::run).detach();
}

__attribute__((destructor)) void on_unload()
{
    webui::stop();
    umod::core::shutdown();
}
#else
#include "Windows.h"
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);

        std::thread(webui::start).detach();

        umod::bootstrap::addFeature(freecam::kDesc);
        std::thread(umod::bootstrap::run).detach();
        break;

    case DLL_PROCESS_DETACH:
        if (lpReserved == nullptr)
        {
            webui::stop();
            umod::core::shutdown();
        }
        break;

    default:
        break;
    }

    return TRUE;
}
#endif
