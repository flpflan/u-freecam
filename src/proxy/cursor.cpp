#include "proxy/cursor.hpp"
#include "debug/logger.hpp"
#include "utype/cursor.hpp"

namespace FreeCam::Proxy
{
    auto Cursor::EnableCursor() -> void
    {
        Debug::Logger::LOGI("Enable cursor");
        UType::Cursor::SetLockState(true);
        UType::Cursor::SetVisible(false);
    }
    auto Cursor::DisableCursor() -> void
    {
        Debug::Logger::LOGI("Disable cursor");
        UType::Cursor::SetLockState(false);
        UType::Cursor::SetVisible(true);
    }
    auto Cursor::ToggleCursor() -> void
    {
        if (UType::Cursor::GetLockState() == 0)
        {
            DisableCursor();
        }
        else
        {
            EnableCursor();
        }
    }
}
