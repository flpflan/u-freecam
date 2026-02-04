#include "umod/bootstrap.hpp"
#include "umod/core.hpp"
#include "umod/debug/logger.hpp"
#include "umod/feature.hpp"
#include "umod/memory/hook.hpp"
#include "umod/memory/scanner.hpp"

#include <vector>

#ifdef __ANDROID__
#include "umod/platform/android.hpp"
using namespace umod::platform::android;
#endif

#include "user/config.hpp"

using namespace std::chrono_literals;

namespace umod::bootstrap
{
    namespace
    {
        static std::vector<feature::Module> modules;
#ifdef __ANDROID__
        static std::optional<ASymbolQuery> IL2CPP_LIB_HANDLE;
#endif
    }

    namespace
    {
        static std::pair<void *, UnityResolve::Mode> getUnityBackend()
        {
#ifdef __ANDROID__
            void *assembly{};
            if (!IL2CPP_LIB_HANDLE) IL2CPP_LIB_HANDLE = ASymbolQuery::fromSymbol("il2cpp_init");
            if (!IL2CPP_LIB_HANDLE) IL2CPP_LIB_HANDLE = ASymbolQuery::fromModule("libil2cpp.so");
            if (IL2CPP_LIB_HANDLE) assembly = IL2CPP_LIB_HANDLE.operator->();
#else
            const auto assembly = GetModuleHandleA("GameAssembly.dll");
#endif
            if (assembly)
            {
                debug::logger::info("Found Il2Cpp backend");
                return {assembly, UnityResolve::Mode::Il2Cpp};
            }

            constexpr const char *kMonoModules[] = {"mono-2.0-bdwgc.dll", "mono.dll"};

            for (const auto &monoModule : kMonoModules)
            {
#ifdef __ANDROID__
                void *monoHandle{};
                IL2CPP_LIB_HANDLE = ASymbolQuery::fromModule(monoModule);
                if (IL2CPP_LIB_HANDLE) monoHandle = IL2CPP_LIB_HANDLE.operator->();
#else
                const auto monoHandle = GetModuleHandleA(monoModule);
#endif
                if (monoHandle)
                {
                    debug::logger::info("Found Mono backend: {}", monoModule);
                    return {monoHandle, UnityResolve::Mode::Mono};
                }
            }
            debug::logger::error("Failed to find Unity backend");
            return {nullptr, UnityResolve::Mode::Mono};
        }

        static void *getPlayerLoop()
        {
            void *playerLoop{};
            debug::logger::info("Searching for MonoBehaviour::CallUpdateMethod");
#if defined(__ANDROID__) && defined(__aarch64__)
            constexpr auto kModule = "libunity.so";
            constexpr auto kPatterns = std::array{
                "FF C3 05 D1 FC A3 00 F9 F5 53 15 A9 F3 7B 16 A9 08 78 40 F9 F3 03 00 AA F4 03 01 2A", // 2021.3.56f2 / 2021.3.34f5
                "FF C3 05 D1 FD A3 00 F9 FE 57 15 A9 F4 4F 16 A9 F4 03 01 2A F3 03 00 AA ? FD FF 97", // 2022.3.33f1 / 2022.3.62f2 / 2022.3.51f1
                "FF C3 05 D1 FC A3 00 F9 F5 53 15 A9 F3 7B 16 A9 08 88 40 F9", // 2021.3.x
            };
            // constexpr auto patterns = std::array{"FF C3 06 D1 FC B3 00 F9 F9 63 17 A9 F7 5B 18 A9 F5 53 19 A9 F3 7B 1A A9 F3 03 00 AA"}; // ExecutePlayerLoop
#else
            constexpr auto kModule = "UnityPlayer.dll";
            constexpr auto kPatterns =
                std::array{"48 89 5c 24 ? 57 48 83 ec ? 48 8b 41 ? 8b fa 48 8b d9 48 85 c0 74 ? 80 78",       // 2017
                           "48 89 5c 24 ? 57 48 83 ec ? 48 8b 81 ? ? ? ? 8b fa 48 8b d9 48 85 c0 74 ? 80 78", // 2018
                           "48 89 5c 24 ? 56 48 83 ec ? 48 8b 81 ? ? ? ? 8b f2",                              // 2019
                           "48 89 5c 24 ? 57 48 83 ec ? 48 8b 81 ? ? ? ? 8b fa 48 8b d9 48 85 c0 74 ? 80 78", // 2021
                           "48 89 74 24 ? 57 48 83 ec ? 8b f2 48 8b f9 e8 ? ? ? ? 84 c0",                     // 2022
                           "48 89 5c 24 ? 56 48 83 ec ? 8b f2 48 8b d9 e8 ? ? ? ? 84 c0 0f 85"};              // 6000
#endif

            for (const auto pattern : kPatterns)
            {
                debug::logger::info("Using pattern: {}", pattern);
                if ((playerLoop = (void *)memory::patternScan(kModule, pattern))) break;
            }
            if (playerLoop)
                debug::logger::info("Method found at {}", playerLoop);
            else
                debug::logger::warn("Pattern scan not found");
            return playerLoop;
        }
    }

    namespace loader
    {
#ifdef __ANDROID__
        static void bypassHardenedIL2CPP()
        {
            if (!user::config::loader::Hardened) return;

            const auto query = ASymbolQuery::fromModule("libdl.so");
            // if (!handle) return Debug::Logger::Debug("dlerror {}", dlerror());

            using dlsym_t = void *(*)(void *, const char *);
            // using JNI_OnLoad_t = jint (*)(JavaVM *, void *);
            const auto fn_dlsym = (dlsym_t)query->resolve("dlsym");
            Hook(
                fn_dlsym,
                +[](void *handle, const char *sym_name)
                {
                    debug::logger::debug("Capture dlsym {}: {}", handle, sym_name);
                    const auto symbol = CALL_ORIGNAL(handle, sym_name);
                    // if (0 == strcmp(sym_name, "JNI_OnLoad"))
                    // {
                    //     // Hijack JavaVM
                    //     if (!sJavaVM) Hook((JNI_OnLoad_t)symbol, (_Hook_JNI_OnLoad<CALL_ORIGNAL>));
                    // }
                    if (0 == strcmp(sym_name, "il2cpp_init"))
                    {
                        // Hijack libil2cpp.so handle
                        IL2CPP_LIB_HANDLE = (ASymbolQuery{ASymbolQuery::TYPE::Hijacked, handle});
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
        }
#else
        static void bypassHardenedIL2CPP()
        {
            if (!user::config::loader::Hardened) return;
        }
#endif
        static bool initializeUnity()
        {
            const auto [module, mode] = getUnityBackend();
            if (module == nullptr) return false;

            UnityResolve::Init(module, mode);

            return true;
        }
    }

    template <>
    void addFeature(feature::Module::Desc features...)
    {
        modules.emplace_back(features);
    }

    void run()
    {
        loader::bypassHardenedIL2CPP();

        std::this_thread::sleep_for(user::config::loader::WaitBeforeInit);
        debug::logger::info("======= Begin FreeCam =======");
        debug::logger::info("Waiting for Unity initializing");

        while (!loader::initializeUnity())
            std::this_thread::sleep_for(2s);

        debug::logger::info("Unity initializing success");
        debug::logger::info("Try attach to internal loop");

        debug::logger::info("Unity initializing success");
        std::thread([] { core::run(getPlayerLoop(), modules); }).detach();
    }
}
