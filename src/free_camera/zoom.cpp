#include "feature/free_camera.hpp"
#include "utype/unity_engine/input.hpp"

using enum UTYPE::KeyCode;
using UTYPE::Input;

// template <typename T>
// T Abs(T n)
// {
//     return n >= 0 ? n : -n;
// }
// float getPinchDelta()
// {
//     static float pinchDelta = 0.f;
//     static float lastDistance = 0.f;
//     static int lastCount = 0;
//     pinchDelta = 0.f;
//     if (Input::GetTouchCount() != 2)
//     {
//         lastCount = 0;
//         return pinchDelta;
//     }
//
//     auto t0 = Input::GetTouch(0);
//     auto t1 = Input::GetTouch(1);
//
//     float currentDistance = t0.GetPosition().Distance(t1.GetPosition());
//
//     if (lastCount != 2)
//     {
//         lastDistance = currentDistance;
//     }
//     else
//     {
//         pinchDelta = currentDistance - lastDistance;
//         lastDistance = currentDistance;
//     }
//     lastCount = 2;
//     return pinchDelta;
// }

namespace FreeCam::Feature
{
    auto FreeCamera::updateZoom() -> void
    {
        const bool toZoom = Input::GetKey(Z);
        if (!toZoom && zoom_mode) freeCam->ResetZoom();
        zoom_mode = toZoom;
        if (zoom_mode)
        {
            if (Input::GetKey(X)) freeCam->ZoomIn(1);
            if (Input::GetKey(C)) freeCam->ZoomOut(1);
            const float mouseCenter = Input::GetAxis("Mouse ScrollWheel");
            if (mouseCenter < 0)
            {
                freeCam->ZoomOut(10);
            }
            else if (mouseCenter > 0)
            {
                freeCam->ZoomIn(10);
            };
        }
    }
}
