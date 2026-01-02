#include "bootstrap.hpp"
#include "core.hpp"
#include "debug/logger.hpp"
#include "memory/hook.hpp"
#include "memory/memory.hpp"
#include "utype/unity_engine/time.hpp"

#include <array>
#include <chrono>
#include <thread>

#ifdef __ANDROID__
#include "byopen.h"
#else
#include "Windows.h"
#endif

void Bootstrap::Run()
{
    bypassHardenedIL2CPP();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    Debug::Logger::Info("======= Begin FreeCam =======");
    Debug::Logger::Info("Waiting for Unity initializing");
    while (!initializeUnity())
        std::this_thread::sleep_for(std::chrono::seconds(2));
    Debug::Logger::Info("Unity initializing success");
    Debug::Logger::Info("Try attach to internal loop");
    if (!attachToGameLoop<FreeCam::Core::Update>())
    {
        Debug::Logger::Warn("Failed to hook internal loop, fallback to mock loop");
        FreeCam::Core::UseMockLoop = true;
        std::thread(
            []
            {
                Debug::Logger::Info("Main loop started");
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

void Bootstrap::bypassHardenedIL2CPP()
{
#ifdef __ANDROID__
    const auto handle = dlopen("libdl.so", RTLD_NOW);
    const static auto fn_dlsym = dlsym(handle, "dlsym");
    static void *CALL_ORIGNAL;
    DoHook(
        fn_dlsym,
        (void *)+[](void *handle, const char *symbol)
        {
            Debug::Logger::Debug("dlsym {}: {}", handle, symbol);
            const auto module = reinterpret_cast<void *(*)(void *, const char *)>(CALL_ORIGNAL)(handle, symbol);
            if (0 == strcmp(symbol, "il2cpp_init"))
            {
                IL2CPP_LIB_HANDLE = handle;
                UnHook(fn_dlsym);
            }
            return module;
        },
        &CALL_ORIGNAL);
    dlclose(handle);
#endif
}

std::pair<void *, UnityResolve::Mode> Bootstrap::getUnityBackend()
{
#ifdef __ANDROID__
    const auto assembly = IL2CPP_LIB_HANDLE ?: by_dlopen("libil2cpp.so", RTLD_NOW);
#else
    const auto assembly = GetModuleHandleA("GameAssembly.dll");
#endif
    if (assembly)
    {
        Debug::Logger::Info("Found Il2Cpp backend");
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
            Debug::Logger::Info("Found Mono backend: {}", monoModule);
            return {monoHandle, UnityResolve::Mode::Mono};
        }
    }
    Debug::Logger::Error("Failed to find Unity backend");
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

template <auto UpdateFn, auto &&CALL_ORIGNAL>
static void detour_update(void *obj, int index)
{
    CALL_ORIGNAL(obj, index);

    static int lastFrame = -1;
    const int curFrame = UTYPE::Time::get_frameCount();
    if (curFrame == lastFrame || index != 0) return; // 0 -> Update, 1 -> LateUpdate, 2 -> FixedUpdate
    UpdateFn();
    lastFrame = curFrame;
}

template <auto UpdateFn>
bool Bootstrap::attachToGameLoop()
{
    Debug::Logger::Info("Searching for MonoBehaviour::CallUpdateMethod");
#ifdef __ANDROID__
    constexpr auto module = "libunity.so";
    constexpr auto patterns = std::array{
        "FF C3 05 D1 FC A3 00 F9 F5 53 15 A9 F3 7B 16 A9 08 78 40 F9", // 2021.3.x
        "FF C3 05 D1 FC A3 00 F9 F5 53 15 A9 F3 7B 16 A9 08 88 40 F9", // 2021.3.x
        "FF C3 05 D1 FD A3 00 F9 FE 57 15 A9 F4 4F 16 A9 F4 03 01 2A", // 2022.3.33f1
    };
    // constexpr auto patterns = std::array{"FF C3 06 D1 FC B3 00 F9 F9 63 17 A9 F7 5B 18 A9 F5 53 19 A9 F3 7B 1A A9 F3 03 00 AA"}; // ExecutePlayerLoop
#else
    constexpr auto module = "UnityPlayer.dll";
    constexpr auto patterns = std::array{"48 89 5c 24 ? 57 48 83 ec ? 48 8b 41 ? 8b fa 48 8b d9 48 85 c0 74 ? 80 78",
                                         "48 89 5c 24 ? 57 48 83 ec ? 48 8b 81 ? ? ? ? 8b fa 48 8b d9 48 85 c0 74 ? 80 78",
                                         "48 89 5c 24 ? 56 48 83 ec ? 48 8b 81 ? ? ? ? 8b f2",
                                         "48 89 5c 24 ? 57 48 83 ec ? 48 8b 81 ? ? ? ? 8b fa 48 8b d9 48 85 c0 74 ? 80 78",
                                         "48 89 74 24 ? 57 48 83 ec ? 8b f2 48 8b f9 e8 ? ? ? ? 84 c0",
                                         "48 89 5c 24 ? 56 48 83 ec ? 8b f2 48 8b d9 e8 ? ? ? ? 84 c0 0f 85"};
#endif

    CallUpdateMethod_t CallUpdateMethod = nullptr;
    for (const auto pattern : patterns)
    {
        if ((CallUpdateMethod = (CallUpdateMethod_t)PatternScan(module, pattern))) break;
    }
    if (CallUpdateMethod)
    {
        Debug::Logger::Info("Method found at {}, start hooking", (void *)CallUpdateMethod);
        if (Hook(CallUpdateMethod, (detour_update<UpdateFn, CALL_ORIGNAL>)))
        {
            Debug::Logger::Info("Hooking success");
            return true;
        }
    }
    else
    {
        Debug::Logger::Warn("Method not found");
    }
    return false;
}
