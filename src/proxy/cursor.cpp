#include "proxy/cursor.hpp"
#include "debug/logger.hpp"
#include "utype/unity_engine/cursor.hpp"

namespace FreeCam::Proxy
{
    auto Cursor::EnableCursor() -> void
    {
#ifndef __ANDROID__
        Debug::Logger::LOGI("Enable cursor");
        UTYPE::Cursor::set_lockState(false);
        UTYPE::Cursor::set_visible(true);
#endif
    }
    auto Cursor::DisableCursor() -> void
    {
#ifndef __ANDROID__
        Debug::Logger::LOGI("Disable cursor");
        UTYPE::Cursor::set_lockState(true);
        UTYPE::Cursor::set_visible(false);
#endif
    }
    auto Cursor::ToggleCursor() -> void
    {
#ifndef __ANDROID__
        if (UTYPE::Cursor::get_lockState() == 0)
        {
            DisableCursor();
        }
        else
        {
            EnableCursor();
        }
#endif
    }
}
