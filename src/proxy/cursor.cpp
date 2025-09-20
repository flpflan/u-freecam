#include "proxy/cursor.hpp"
#include "debug/logger.hpp"

namespace FreeCam::Proxy
{
    auto Cursor::EnableCursor() -> void
    {
        Debug::Logger::LOGI("Enable cursor");
        visible = true;
        lockState = 0;
    }
    auto Cursor::DisableCursor() -> void
    {
        Debug::Logger::LOGI("Disable cursor");
        visible = false;
        lockState = 1;
    }
    auto Cursor::ToggleCursor() -> void
    {
        if (lockState == 0)
        {
            DisableCursor();
        }
        else
        {
            EnableCursor();
        }
    }
}
