#pragma once

#include "freecam.hpp"

namespace FreeCam {
    class CursorProxy
    {
        inline static UnityResolve::Class *Cursor = nullptr;
        inline static UnityResolve::Method *GetCursorLockState = nullptr;
        // inline static UnityResolve::Method *GetCursorVisible = nullptr;
        inline static UnityResolve::Method *SetCursorLockState = nullptr;
        inline static UnityResolve::Method *SetCursorVisible = nullptr;
    public:
        static void DisableCursor();
        static void EnableCursor();
        static void ToggleCursor();
    };
}
