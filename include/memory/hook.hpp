#pragma once
#include "debug/logger.hpp"
#include "dobby.h"

#ifdef __ANDROID__
#define FN_PTR void (*)()
#define FN_PTR_PTR void (**)()
#else
#define FN_PTR void *
#define FN_PTR_PTR void **
#endif

inline auto DoHook(void *target, void *detour, void **orignal) -> bool
{
#ifndef NDEBUG
    log_set_level(0);
#endif
    log_set_tag("freecam");
    dobby_enable_near_branch_trampoline();
    Debug::Logger::Info("Hooking {} with detour {}", target, detour);
    const auto ok = DobbyHook(target, (FN_PTR)detour, (FN_PTR_PTR)orignal) == RS_SUCCESS ? true : false;
    if (!ok) Debug::Logger::Warn("Hooking failed");
    return ok;
}
#undef FN_PTR
#undef FN_PTR_PTR

template <typename RTYPE, typename... ARGS>
auto DoHook(RTYPE (*target)(ARGS...), RTYPE (*detour)(ARGS...), RTYPE (*const *orignal)(ARGS...)) -> bool
{
    return DoHook((void *)target, (void *)detour, (void **)orignal);
}

constexpr void(*__init_orig__(void *)) { return nullptr; }
template <typename RTYPE, typename... ARGS>
constexpr RTYPE (*__init_orig__(RTYPE (*)(ARGS...)))(ARGS...)
{
    return nullptr;
}

/*
 * Usage(1):
 * bool target(int x) { print(x); return x == 1; }
 * const bool hook_success = Hook(target, +[](int x){ CALL_ORIGNAL(x); return true; })
 *
 * Usage(2):
 * bool target(int x) { return x == 1; }
 * template <auto&& CALL_ORIGNAL>
 * bool detour(int x) { CALL_ORIGNAL(x); return true; }
 * const bool hook_success = Hook(target, detour<CALL_ORIGNAL>);
 */
#ifndef _MSC_VER
#define Hook(TARGET, DETOUR)                                                                                                                                                                           \
    ({                                                                                                                                                                                                 \
        const static auto CALL_ORIGNAL = __init_orig__(TARGET);                                                                                                                                        \
        DoHook(TARGET, DETOUR, &CALL_ORIGNAL);                                                                                                                                                         \
    })

#else
#define Hook(TARGET, DETOUR)                                                                                                                                                                           \
    (                                                                                                                                                                                                  \
        [=]                                                                                                                                                                                            \
        {                                                                                                                                                                                              \
            static auto CALL_ORIGNAL = __init_orig__(TARGET);                                                                                                                                          \
            return DoHook(TARGET, DETOUR, &CALL_ORIGNAL);                                                                                                                                              \
        })()
#endif

inline auto DestoryHook(void *target) -> bool { return DobbyDestroy(target) == RS_SUCCESS; }

template <typename RTYPE, typename... ARGS>
inline auto DestoryHook(RTYPE (*target)(ARGS...)) -> bool
{
    return DestoryHook((void *)target);
}

#define UnHook(TARGET) DestoryHook(TARGET)
