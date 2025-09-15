#include "freecam/cursor_proxy.hpp"
#include "debug.hpp"

namespace FreeCam {
    auto CursorProxy::EnableCursor() -> void {
        Debug::LOG("enable cursor");
        if (!Cursor) Cursor = UnityResolve::Get("UnityEngine.CoreModule.dll")->Get("Cursor");
        if (!SetCursorLockState) SetCursorLockState = Cursor->Get<UnityResolve::Method>("set_lockState");
        if (!SetCursorVisible) SetCursorVisible = Cursor->Get<UnityResolve::Method>("set_visible");

        SetCursorVisible->Invoke<void>(true);
        SetCursorLockState->Invoke<void>(0);
    }
    auto CursorProxy::DisableCursor() -> void
    {
        Debug::LOG("disable cursor");
        if (!Cursor) Cursor = UnityResolve::Get("UnityEngine.CoreModule.dll")->Get("Cursor");
        if (!SetCursorLockState) SetCursorLockState = Cursor->Get<UnityResolve::Method>("set_lockState");
        if (!SetCursorVisible) SetCursorVisible = Cursor->Get<UnityResolve::Method>("set_visible");

        SetCursorVisible->Invoke<void>(false);
        SetCursorLockState->Invoke<void>(1);
    }
    auto CursorProxy::ToggleCursor() -> void
    {
        if (!Cursor) Cursor = UnityResolve::Get("UnityEngine.CoreModule.dll")->Get("Cursor");
        if (!GetCursorLockState) GetCursorLockState = Cursor->Get<UnityResolve::Method>("get_lockState");
        //if (!GetCursorVisible) GetCursorVisible = Cursor->Get<UnityResolve::Method>("get_visible");

        if (GetCursorLockState->Invoke<int>()==0)
        {
            DisableCursor();
        }
        else
        {
            EnableCursor();
        }
    }
}
