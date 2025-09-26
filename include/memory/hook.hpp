#pragma once
#include "debug/logger.hpp"
#include "dobby.h"

#ifdef __ANDROID__

static auto DoHook(void *address, void *detour, void **orig) -> bool
{
#ifdef NDEBUG
    log_set_level(0);
#endif
    log_set_tag("freecam");
    dobby_enable_near_branch_trampoline();
    Debug::Logger::LOGD("Hooking {} with detour {}", address, detour);
    return DobbyHook(address, (dobby_dummy_func_t)detour, (dobby_dummy_func_t *)orig) == RS_SUCCESS ? true : false;
}
#else
static auto DoHook(void *address, void *detour, void **orig) -> bool
{
#ifdef NDEBUG
    log_set_level(0);
#endif
    dobby_enable_near_branch_trampoline();
    Debug::Logger::LOGD("Hooking {} with detour {}", address, detour);
    return DobbyHook(address, detour, orig) == RS_SUCCESS ? true : false;
}
#endif
