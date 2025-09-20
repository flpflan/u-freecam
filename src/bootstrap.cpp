#include "bootstrap.hpp"
#include "core.hpp"
#include "debug/logger.hpp"

#include <thread>

#if ANDROID_MODE
#include "byopen.h"
#else
#include "Windows.h"
#endif

void Bootstrap::Run()
{
    std::this_thread::sleep_for(std::chrono::seconds(5));
    Debug::Logger::Init();

    Debug::Logger::LOGI("Waiting for Unity initializing");
    while (!initializeUnity())
        std::this_thread::sleep_for(std::chrono::seconds(2));
    Debug::Logger::LOGI("Unity initializing success");
    std::thread(FreeCam::Core::Hack).detach();
}

void Bootstrap::Shutdown() {}

std::pair<void *, UnityResolve::Mode> Bootstrap::getUnityBackend()
{
#if ANDROID_MODE
    const auto assembly = by_dlopen("libil2cpp.so", RTLD_NOW);
#else
    const auto assembly = GetModuleHandleA("GameAssembly.dll");
#endif
    if (assembly)
    {
        Debug::Logger::LOGI("Found Il2Cpp backend");
        return {assembly, UnityResolve::Mode::Il2Cpp};
    }

    std::vector<std::string> monoModules = {"mono-2.0-bdwgc.dll", "mono.dll"};

    for (const auto &monoModule : monoModules)
    {
#if ANDROID_MODE
        const auto monoHandle = by_dlopen(monoModule.c_str(), RTLD_NOW);
#else
        const auto monoHandle = GetModuleHandleA(monoModule.c_str());
#endif
        if (monoHandle)
        {
            Debug::Logger::LOGI("Found Mono backend: {}", monoModule);
            return {monoHandle, UnityResolve::Mode::Mono};
        }
    }
    Debug::Logger::LOGE("Failed to find Unity backend");
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
