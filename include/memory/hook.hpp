#include "debug/logger.hpp"

#ifdef __ANDROID__
#include "dobby.h"
#else
#endif

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
