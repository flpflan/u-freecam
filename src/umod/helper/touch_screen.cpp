#include "umod/runtime/helper/touch_screen.hpp"
#include "umod/UnityResolve.hpp"

using UTYPE = UnityResolve::UnityType;
using UMethod = UnityResolve::Method;
using UClass = UnityResolve::Class;

class TouchControl : public UTYPE::Object
{
};

class UTouchScreen : public UTYPE::Object
{
};

namespace umod::umod::unity_runtie::helper
{
    namespace
    {
        static UClass *kTouchscreen = nullptr;
        static UMethod *GetCurrent = nullptr;
        static UMethod *GetTouches = nullptr;
        static float PinchDelta = 0;
    }
    namespace TouchScreenUtils
    {

        auto UpdatePinchDelta() -> void
        {
            if (!kTouchscreen) kTouchscreen = UnityResolve::Get("Unity.InputSystem.dll")->Get("Touchscreen", "UnityEngine.InputSystem");
            if (!GetCurrent) GetCurrent = kTouchscreen->Get<UMethod>("get_current");
            const auto touchScreen = GetCurrent->Invoke<UTouchScreen *>();
            if (touchScreen == nullptr) return;
            PinchDelta = 0.f;

            if (!GetTouches) GetTouches = kTouchscreen->Get<UMethod>("get_touches");
            const auto touches = GetTouches->Invoke<UTYPE::Array<TouchControl *>>().ToVector();
        }
    }
}
