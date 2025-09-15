#include "main.hpp"

#include "freecam.hpp"
#include <dlfcn.h>
#include <thread>
#include "byopen.h"
#include "debug.hpp"

void Main::run()
{
    std::this_thread::sleep_for(std::chrono::seconds(5));
    FreeCam::Debug::LOG("waiting for unity initialized");
    while(!initializeUnity())
        std::this_thread::sleep_for(std::chrono::seconds(2));
    FreeCam::Debug::LOG("unity initializing success");
    std::thread(FreeCam::FreeCam::Hack).detach();
}

void Main::shutdown()
{
}


std::pair<void*, UnityResolve::Mode> Main::getUnityBackend()
{
    if (auto assembly = by_dlopen("libil2cpp.so", RTLD_NOW))
    {
        return {assembly, UnityResolve::Mode::Il2Cpp};
    }

    return {nullptr, UnityResolve::Mode::Mono};
}

bool Main::initializeUnity()
{
    auto [module, mode] = getUnityBackend();
    if (module == nullptr)
    {
        return false;
    }

    UnityResolve::Init(module, mode);

    return true;
}

