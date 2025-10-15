#include "proxy/cursor.hpp"
#include "debug/logger.hpp"
#include "utype/unity_engine/cursor.hpp"

namespace FreeCam::Proxy
{
    auto Cursor::EnableCursor() -> void
    {
        Debug::Logger::LOGI("Enable cursor");
        UTYPE::Cursor::set_lockState(false);
        UTYPE::Cursor::set_visible(true);
    }
    auto Cursor::DisableCursor() -> void
    {
        Debug::Logger::LOGI("Disable cursor");
        UTYPE::Cursor::set_lockState(true);
        UTYPE::Cursor::set_visible(false);
    }
    auto Cursor::ToggleCursor() -> void
    {
        if (UTYPE::Cursor::get_lockState() == 0)
        {
            DisableCursor();
        }
        else
        {
            EnableCursor();
        }
    }
}
