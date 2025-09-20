#pragma once

#include "utype/cursor.hpp"

namespace FreeCam::Proxy
{
    class Cursor : public UType::Cursor
    {
    public:
        static void DisableCursor();
        static void EnableCursor();
        static void ToggleCursor();
    };
}
