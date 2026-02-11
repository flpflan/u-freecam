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

static HMODULE g_hModule{};

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        g_hModule = hModule;

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

#define X3_NOT_INITIALIZED -536805375

typedef int32_t(__stdcall *t_x3_Dispatch)(OUT void *Function, IN uint32_t Type);
static t_x3_Dispatch x3_Dispatch{};

static std::wstring getCurrentDllDirectory()
{ 
    wchar_t path[MAX_PATH]{};
    GetModuleFileNameW(g_hModule, path, MAX_PATH);
    std::wstring sPath = {path};
    const auto pos = sPath.find_last_of(L"\\/");
    return sPath.substr(0, pos);
}

extern "C" __declspec(dllexport) int32_t x3_barrier_x64_1(void *FunctionAddress, uint32_t Type)
{
    if (x3_Dispatch == nullptr)
    {
        const auto orignalPath = getCurrentDllDirectory() + L"\\x3_x64.xem.bak";

        HMODULE hX3 = LoadLibraryW(orignalPath.c_str());
        if (hX3 == nullptr)
        {
            MessageBoxA(0, "LoadLibraryA failed!", "Error", 0);
            return X3_NOT_INITIALIZED;
        }

        x3_Dispatch = reinterpret_cast<t_x3_Dispatch>(GetProcAddress(hX3, reinterpret_cast<LPCSTR>(1)));
        if (x3_Dispatch == nullptr)
        {
            MessageBoxA(0, "GetProcAddress failed!", "Error", 0);
            return X3_NOT_INITIALIZED;
        }
    }

    return x3_Dispatch(FunctionAddress, Type);
}
#endif
