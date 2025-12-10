#pragma once
#include "debug/logger.hpp"
#include "dobby.h"

#ifdef __ANDROID__

template <typename RTYPE, typename... ARGS>
auto DoHook(RTYPE (*target)(ARGS...), RTYPE (*detour)(ARGS...), RTYPE (*const *orignal)(ARGS...)) -> bool
{
#ifndef NDEBUG
    log_set_level(0);
#endif
    log_set_tag("freecam");
    dobby_enable_near_branch_trampoline();
    Debug::Logger::Info("Hooking {} with detour {}", (void *)target, (void *)detour);
    const auto ok = DobbyHook((void *)target, (dobby_dummy_func_t)detour, (dobby_dummy_func_t *)orignal) == RS_SUCCESS ? true : false;
    if (!ok) Debug::Logger::Warn("Hooking failed");
    return ok;
}
#else
template <typename RTYPE, typename... ARGS>
auto DoHook(RTYPE (*target)(ARGS...), RTYPE (*detour)(ARGS...), RTYPE (*const *orignal)(ARGS...)) -> bool
{
#ifdef NDEBUG
    log_set_level(0);
#endif
    dobby_enable_near_branch_trampoline();
    Debug::Logger::Info("Hooking {} with detour {}", (void *)target, (void *)detour);
    const auto ok = DobbyHook(target, detour, (void **)orignal) == RS_SUCCESS ? true : false;
    if (!ok) Debug::Logger::Warn("Hooking failed");
    return ok;
}
#endif

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
 * template <auto CALL_ORIGNAL>
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

template <typename RTYPE, typename... ARGS>
auto DestoryHook(RTYPE (*target)(ARGS...)) -> bool
{
    return DobbyDestroy(target) == RS_SUCCESS ? true : false;
}

#define UnHook(TARGET) DestoryHook(TARGET)
