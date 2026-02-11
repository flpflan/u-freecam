#include "umod/runtime/helper/cursor.hpp"
#include "umod/debug/logger.hpp"
#include "umod/utype/unity_engine/cursor.hpp"

namespace umod::unity_runtime::helper
{
    using namespace UTYPE::unity_engine;

    namespace
    {
        static LockState storedLockState{};
    }

    auto CursorUtils::enableCursor() -> void
    {
#ifndef __ANDROID__
        debug::logger::info("Enable cursor");
        Cursor::set_lockState(LockState::None);
        // Cursor::set_visible(true);
#endif
    }
    auto CursorUtils::disableCursor() -> void
    {
#ifndef __ANDROID__
        debug::logger::info("Disable cursor");
        Cursor::set_lockState(LockState::Locked);
        //Cursor::set_visible(false);
#endif
    }
    auto CursorUtils::toggleCursor() -> void
    {
#ifndef __ANDROID__
        if (Cursor::get_lockState() == LockState::Locked)
            enableCursor();
        else
            disableCursor();
#endif
    }
    auto CursorUtils::backup() -> void
    {
#ifndef __ANDROID__
        debug::logger::info("Backup cursor");
        storedLockState = Cursor::get_lockState();
        disableCursor();
#endif
    }
    auto CursorUtils::resume() -> void
    {
#ifndef __ANDROID__
        debug::logger::info("Resume cursor");
        if (storedLockState != LockState::Locked)
            enableCursor();
        else
            disableCursor();
#endif
    }
}
