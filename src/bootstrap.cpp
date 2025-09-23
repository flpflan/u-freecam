#include "bootstrap.hpp"
#include "UnityResolve.hpp"
#include "core.hpp"
#include "debug/logger.hpp"
#include "dobby.h"
#include "memory/memory.hpp"

#include <chrono>
#include <thread>

#ifdef __ANDROID__
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
    attachToGameUpdate<FreeCam::Core::Update>();
}

void Bootstrap::Shutdown() {}

std::pair<void *, UnityResolve::Mode> Bootstrap::getUnityBackend()
{
#ifdef __ANDROID__
    const auto assembly = by_dlopen("libil2cpp.so", RTLD_NOW);
#else
    const auto assembly = GetModuleHandleA("GameAssembly.dll");
#endif
    if (assembly)
    {
        Debug::Logger::LOGI("Found Il2Cpp backend");
        return {assembly, UnityResolve::Mode::Il2Cpp};
    }

    const auto monoModules = {"mono-2.0-bdwgc.dll", "mono.dll"};

    for (const auto &monoModule : monoModules)
    {
#ifdef __ANDROID__
        const auto monoHandle = by_dlopen(monoModule, RTLD_NOW);
#else
        const auto monoHandle = GetModuleHandleA(monoModule);
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

void (*orig_update)(void *, int);
void (*detour_update)(void *, int);
template <auto UpdateFn>
void Bootstrap::attachToGameUpdate()
{
    Debug::Logger::LOGI("Searching for MonoBehaviour::CallUpdateMethod");
#ifdef __ANDROID__
    const auto patterns = {"FF C3 05 D1 FC A3 00 F9 F5 53 15 A9 F3 7B 16 A9 08 78 40 F9", "FF C3 05 D1 FC A3 00 F9 F5 53 15 A9 F3 7B 16 A9 08 88 40 F9"};
#else
    const auto patterns = {"48 89 5c 24 ? 57 48 83 ec ? 48 8b 41 ? 8b fa 48 8b d9 48 85 c0 74 ? 80 78",
                           "48 89 5c 24 ? 57 48 83 ec ? 48 8b 81 ? ? ? ? 8b fa 48 8b d9 48 85 c0 74 ? 80 78",
                           "48 89 5c 24 ? 56 48 83 ec ? 48 8b 81 ? ? ? ? 8b f2",
                           "48 89 5c 24 ? 57 48 83 ec ? 48 8b 81 ? ? ? ? 8b fa 48 8b d9 48 85 c0 74 ? 80 78",
                           "48 89 74 24 ? 57 48 83 ec ? 8b f2 48 8b f9 e8 ? ? ? ? 84 c0",
                           "48 89 5c 24 ? 56 48 83 ec ? 8b f2 48 8b d9 e8 ? ? ? ? 84 c0 0f 85"};
#endif
    uintptr_t CallUpdateMethod = 0;
    for (auto pattern : patterns)
    {
        if ((CallUpdateMethod = PatternScan("libunity.so", pattern))) break;
    }
    if (CallUpdateMethod)
    {
        Debug::Logger::LOGI("Method found, start hooking");
        DobbyHook((void *)CallUpdateMethod, (dobby_dummy_func_t)detour_update, (dobby_dummy_func_t *)&orig_update);
        detour_update = [](void *obj, int methodIndex)
        {
            Debug::Logger::LOGI("{}", methodIndex);
            orig_update(obj, methodIndex);
            if (methodIndex == 0) UpdateFn();
        };
        Debug::Logger::LOGI("Hooking success");
    }
    else
    {
        Debug::Logger::LOGW("Could not find MonoBehaviour::CallUpdateMethod method, fallback to mock loop");
        std::thread(
            []
            {
                Debug::Logger::LOGI("Main loop started");
                while (true)
                {
                    UpdateFn();
                    std::this_thread::sleep_for(std::chrono::microseconds((int)FreeCam::Core::DeltaTime_us));
                }
            })
            .detach();
    }
}
