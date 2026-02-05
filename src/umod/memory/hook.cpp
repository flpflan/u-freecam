#include "umod/memory/hook.hpp"
#include "umod/debug/logger.hpp"

#include "dobby.h"

#ifdef __ANDROID__
#define FN_PTR void (*)()
#define FN_PTR_PTR void (**)()
#else
#define FN_PTR void *
#define FN_PTR_PTR void **
#endif

namespace umod::memory
{

    bool hook(void *target, void *detour, void *const *orig)
    {
#ifndef NDEBUG
        log_set_level(0);
#endif
#ifdef __ANDROID__
        log_set_tag("freecam");
#endif
        dobby_enable_near_branch_trampoline();
        debug::logger::info("Hooking {} with detour {}", target, detour);
        const auto ok = DobbyHook(target, (FN_PTR)detour, (FN_PTR_PTR)orig) == RS_SUCCESS ? true : false;
        if (!ok) debug::logger::warn("Hooking failed");
        return ok;
    }

    bool unhook(void *target) { return DobbyDestroy(target) == RS_SUCCESS; }
}
