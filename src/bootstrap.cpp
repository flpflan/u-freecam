#include "bootstrap.hpp"
#include "core.hpp"
#include "debug/logger.hpp"
#include "memory/hook.hpp"
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

    constexpr const char *monoModules[] = {"mono-2.0-bdwgc.dll", "mono.dll"};

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
    const auto [module, mode] = getUnityBackend();
    if (module == nullptr)
    {
        return false;
    }

    UnityResolve::Init(module, mode);

    return true;
}

using CallUpdateMethod_t = void (*)(void *, int);

static CallUpdateMethod_t orig_update;

template <auto UpdateFn>
static void detour_update(void *obj, int index)
{
    Debug::Logger::LOGI("methodIndex: {}", index);
    orig_update(obj, index);
    if (index == 0) UpdateFn(); // 0 -> Update, 1 -> LateUpdate, 2 -> FixedUpdate
}

template <auto UpdateFn>
void Bootstrap::attachToGameUpdate()
{
    Debug::Logger::LOGI("Searching for MonoBehaviour::CallUpdateMethod");
#ifdef __ANDROID__
    constexpr auto module = "libunity.so";
    constexpr const char *patterns[] = {"FF C3 05 D1 FC A3 00 F9 F5 53 15 A9 F3 7B 16 A9 08 78 40 F9", "FF C3 05 D1 FC A3 00 F9 F5 53 15 A9 F3 7B 16 A9 08 88 40 F9"};
#else
    constexpr auto module = "UnityPlayer.dll";
    constexpr const char *patterns[] = {"48 89 5c 24 ? 57 48 83 ec ? 48 8b 41 ? 8b fa 48 8b d9 48 85 c0 74 ? 80 78",
                                        "48 89 5c 24 ? 57 48 83 ec ? 48 8b 81 ? ? ? ? 8b fa 48 8b d9 48 85 c0 74 ? 80 78",
                                        "48 89 5c 24 ? 56 48 83 ec ? 48 8b 81 ? ? ? ? 8b f2",
                                        "48 89 5c 24 ? 57 48 83 ec ? 48 8b 81 ? ? ? ? 8b fa 48 8b d9 48 85 c0 74 ? 80 78",
                                        "48 89 74 24 ? 57 48 83 ec ? 8b f2 48 8b f9 e8 ? ? ? ? 84 c0",
                                        "48 89 5c 24 ? 56 48 83 ec ? 8b f2 48 8b d9 e8 ? ? ? ? 84 c0 0f 85"};
#endif

    CallUpdateMethod_t CallUpdateMethod = NULL;
    for (const auto pattern : patterns)
    {
        if ((CallUpdateMethod = (CallUpdateMethod_t)PatternScan(module, pattern))) break;
    }
    if (CallUpdateMethod)
    {
        Debug::Logger::LOGI("Method found at {}, start hooking", (void *)CallUpdateMethod);
        const auto r = DoHook((void *)CallUpdateMethod, (void *)detour_update<UpdateFn>, (void **)&orig_update);
        if (r) return Debug::Logger::LOGI("Hooking success");
    }
    else
    {
        Debug::Logger::LOGW("Method not found");
    }

    Debug::Logger::LOGW("Failed to hooking internal update, fallback to mock loop");
    std::thread(
        []
        {
            Debug::Logger::LOGI("Main loop started");
            {
                UpdateFn();
                std::this_thread::sleep_for(std::chrono::microseconds((int)FreeCam::Core::DeltaTime_us));
            }
            while (1)
                ;
        })
        .detach();
}
