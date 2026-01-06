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
#include "platform/android.hpp"
#else
#include "Windows.h"
#endif

void Bootstrap::Run()
{
    if (Hardened) bypassHardenedIL2CPP();
    std::this_thread::sleep_for(std::chrono::seconds(SecondsBeforeInit));
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

static std::shared_ptr<Handle> IL2CPP_LIB_HANDLE;

void Bootstrap::bypassHardenedIL2CPP()
{
#ifdef __ANDROID__
    const auto handle = A_get_handle("libdl.so");
    // if (!handle) return Debug::Logger::Debug("dlerror {}", dlerror());

    using dlsym_t = void *(*)(void *, const char *);
    using JNI_OnLoad_t = jint (*)(JavaVM *, void *);
    const auto fn_dlsym = (dlsym_t)A_symbol_resolve(handle.get(), "dlsym");
    Hook(
        fn_dlsym,
        +[](void *handle, const char *sym_name)
        {
            Debug::Logger::Debug("dlsym {}: {}", handle, sym_name);
            const auto symbol = CALL_ORIGNAL(handle, sym_name);
            // if (0 == strcmp(sym_name, "JNI_OnLoad"))
            // {
            //     // Hijack JavaVM
            //     if (!sJavaVM) Hook((JNI_OnLoad_t)symbol, (_Hook_JNI_OnLoad<CALL_ORIGNAL>));
            // }
            if (0 == strcmp(sym_name, "il2cpp_init"))
            {
                // Hijack libil2cpp.so handle
                IL2CPP_LIB_HANDLE = std::make_unique<Handle>(Handle{handle, Handle::Hijacked});
                // UnHook(fn_dlsym);
            }
            return symbol;
        });
    std::thread(
        [=]
        {
            while (!IL2CPP_LIB_HANDLE)
            {
                std::this_thread::sleep_for(100ms);
            }
            UnHook(fn_dlsym);
        })
        .detach();
#endif
}

std::pair<void *, UnityResolve::Mode> Bootstrap::getUnityBackend()
{
#ifdef __ANDROID__
    IL2CPP_LIB_HANDLE = IL2CPP_LIB_HANDLE ?: GetMoudleFromSymbol("il2cpp_init") ?: A_get_handle("libil2cpp.so");
    const auto assembly = IL2CPP_LIB_HANDLE.get();
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
        const auto monoHandle = A_get_handle(monoModule).release();
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
#if defined(__ANDROID__) && defined(__aarch64__)
    constexpr auto module = "libunity.so";
    constexpr auto patterns = std::array{
        "FF C3 05 D1 FC A3 00 F9 F5 53 15 A9 F3 7B 16 A9 08 78 40 F9", // 2021.3.x
        "FF C3 05 D1 FC A3 00 F9 F5 53 15 A9 F3 7B 16 A9 08 88 40 F9", // 2021.3.x
        "FF C3 05 D1 FD A3 00 F9 FE 57 15 A9 F4 4F 16 A9 F4 03 01 2A", // 2022.3.33f1
        "FF C3 05 D1 FD A3 00 F9 FE 57 15 A9 F4 4F 16 A9 F3 03 01 2A", // 2022.3.51f1
    };
    // constexpr auto patterns = std::array{"FF C3 06 D1 FC B3 00 F9 F9 63 17 A9 F7 5B 18 A9 F5 53 19 A9 F3 7B 1A A9 F3 03 00 AA"}; // ExecutePlayerLoop
#else
    constexpr auto module = "UnityPlayer.dll";
    constexpr auto patterns = std::array{"48 89 5c 24 ? 57 48 83 ec ? 48 8b 41 ? 8b fa 48 8b d9 48 85 c0 74 ? 80 78",       // 2017
                                         "48 89 5c 24 ? 57 48 83 ec ? 48 8b 81 ? ? ? ? 8b fa 48 8b d9 48 85 c0 74 ? 80 78", // 2018
                                         "48 89 5c 24 ? 56 48 83 ec ? 48 8b 81 ? ? ? ? 8b f2",                              // 2019
                                         "48 89 5c 24 ? 57 48 83 ec ? 48 8b 81 ? ? ? ? 8b fa 48 8b d9 48 85 c0 74 ? 80 78", // 2021
                                         "48 89 74 24 ? 57 48 83 ec ? 8b f2 48 8b f9 e8 ? ? ? ? 84 c0",                     // 2022
                                         "48 89 5c 24 ? 56 48 83 ec ? 8b f2 48 8b d9 e8 ? ? ? ? 84 c0 0f 85"};              // 6000
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
