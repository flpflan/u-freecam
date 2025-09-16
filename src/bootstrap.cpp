#include "bootstrap.hpp"

#include "freecam.hpp"
#include <thread>
#include "debug.hpp"
#if ANDROID_MODE
#include "byopen.h"
#else
#include "Windows.h"
#endif

void Bootstrap::Run()
{
    std::this_thread::sleep_for(std::chrono::seconds(5));
    Debug::LOG("waiting for unity initialized");
    while(!initializeUnity())
        std::this_thread::sleep_for(std::chrono::seconds(2));
    Debug::LOG("unity initializing success");
    std::thread(FreeCam::FreeCam::Hack).detach();
}

void Bootstrap::Shutdown()
{
}


std::pair<void*, UnityResolve::Mode> Bootstrap::getUnityBackend()
{
#if ANDROID_MODE
    const auto assembly = by_dlopen("libil2cpp.so", RTLD_NOW);
#else
    const auto assembly = GetModuleHandleA("GameAssembly.dll");
#endif
    if (assembly)
    {
        return {assembly, UnityResolve::Mode::Il2Cpp};
    }

    return {nullptr, UnityResolve::Mode::Mono};
}

bool Bootstrap::initializeUnity()
{
    auto [module, mode] = getUnityBackend();
    if (module == nullptr)
    {
        return false;
    }

    UnityResolve::Init(module, mode);

    return true;
}

