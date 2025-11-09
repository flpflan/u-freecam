#pragma once
#include "debug/logger.hpp"
#include "dobby.h"

#ifdef __ANDROID__

template <typename RTYPE, typename... ARGS>
auto DoHook(RTYPE (*target)(ARGS...), RTYPE (*detour)(ARGS...), RTYPE (**orignal)(ARGS...)) -> bool
{
#ifndef NDEBUG
    log_set_level(0);
#endif
    log_set_tag("freecam");
    dobby_enable_near_branch_trampoline();
    Debug::Logger::LOGI("Hooking {} with detour {}", (void *)target, (void *)detour);
    const auto ok = DobbyHook((void *)target, (dobby_dummy_func_t)detour, (dobby_dummy_func_t *)orignal) == RS_SUCCESS ? true : false;
    if (!ok) Debug::Logger::LOGW("Hooking failed");
    return ok;
}
#else
template <typename RTYPE, typename... ARGS>
auto DoHook(RTYPE (*target)(ARGS...), RTYPE (*detour)(ARGS...), RTYPE (*orignal)(ARGS...)) -> bool
{
#ifdef NDEBUG
    log_set_level(0);
#endif
    dobby_enable_near_branch_trampoline();
    Debug::Logger::LOGI("Hooking {} with detour {}", (void *)target, (void *)detour);
    const auto ok = DobbyHook(target, detour, orignal) == RS_SUCCESS ? true : false;
    if (!ok) Debug::Logger::LOGW("Hooking failed");
    return ok;
}
#endif

template <typename RTYPE, typename... ARGS>
constexpr RTYPE (*__init_orig__(RTYPE (*)(ARGS...)))(ARGS...)
{
    return nullptr;
}

#define Hook(TARGET, DETOUR)                                                                                                                                                                           \
    ({                                                                                                                                                                                                 \
        static auto CALL_ORIGNAL = __init_orig__(TARGET);                                                                                                                                              \
        DoHook(TARGET, DETOUR, &CALL_ORIGNAL);                                                                                                                                                         \
    })

// #define Hook(TARGET, DETOUR) \
//     ( \
//         [=] \
//         { \
//             static auto CALL_ORIGNAL = __init_orig__(TARGET); \
//             return DoHook(TARGET, DETOUR, &CALL_ORIGNAL); \
//         })()
