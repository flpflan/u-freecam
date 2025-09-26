#include "bootstrap.hpp"
#include "core.hpp"
#include "debug/logger.hpp"
#include "memory/hook.hpp"
#include "memory/memory.hpp"
#include "utype/time.hpp"

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
    Debug::Logger::LOGI("======= Begin FreeCam =======");
    Debug::Logger::LOGI("Waiting for Unity initializing");
    while (!initializeUnity())
        std::this_thread::sleep_for(std::chrono::seconds(2));
    Debug::Logger::LOGI("Unity initializing success");
    Debug::Logger::LOGI("Try attach to internal loop");
    if (!attachToGameLoop<FreeCam::Core::Update>())
    {
        Debug::Logger::LOGW("Failed to hook internal loop, fallback to mock loop");
        FreeCam::Core::UseMockLoop = true;
        std::thread(
            []
            {
                Debug::Logger::LOGI("Main loop started");
                UnityResolve::ThreadAttach();
                do
                {
                    FreeCam::Core::Update();
                    std::this_thread::sleep_for(std::chrono::microseconds((int)FreeCam::Core::DeltaTime_us));
                } while (1);
                UnityResolve::ThreadDetach();
            })
            .detach();
    }
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
    orig_update(obj, index);

    static int lastFrame = -1;
    const int curFrame = UType::Time::GetFrameCount();
    if (curFrame == lastFrame || index != 0) return; // 0 -> Update, 1 -> LateUpdate, 2 -> FixedUpdate
    UpdateFn();
    lastFrame = curFrame;
}

template <auto UpdateFn>
bool Bootstrap::attachToGameLoop()
{
    Debug::Logger::LOGI("Searching for MonoBehaviour::CallUpdateMethod");
#ifdef __ANDROID__
    constexpr auto module = "libunity.so";
    constexpr const char *patterns[] = {"FF C3 05 D1 FC A3 00 F9 F5 53 15 A9 F3 7B 16 A9 08 78 40 F9", "FF C3 05 D1 FC A3 00 F9 F5 53 15 A9 F3 7B 16 A9 08 88 40 F9"};
    // constexpr const char *patterns[] = {"FF C3 06 D1 FC B3 00 F9 F9 63 17 A9 F7 5B 18 A9 F5 53 19 A9 F3 7B 1A A9 F3 03 00 AA"}; // ExecutePlayerLoop
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
        if (r)
        {
            Debug::Logger::LOGI("Hooking success");
            return true;
        }
    }
    else
    {
        Debug::Logger::LOGW("Method not found");
    }
    return false;
}
