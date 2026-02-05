#include "freecam/freecam.hpp"
#include "umod/utype/unity_engine/input.hpp"

#include "user/config.hpp"

using namespace umod::UTYPE::unity_engine;

using namespace user;
using enum user::config::freecam::keybind::KeyCode;

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

namespace freecam
{
    auto FreeCamera::updateZoom() -> void
    {
        using namespace user::config::freecam;
        const bool toZoom = Input::GetKey(keybind::ZoomMode);
        if (!toZoom && kFlags.zoom_mode) cameraHelper_->resetZoom();
        kFlags.zoom_mode = toZoom;
        if (kFlags.zoom_mode)
        {
            if (Input::GetKey(keybind::ZoomIn)) cameraHelper_->zoomIn(1);
            if (Input::GetKey(keybind::ZoomOut)) cameraHelper_->zoomOut(1);
            const float mouseCenter = Input::GetAxis("Mouse ScrollWheel");
            if (mouseCenter < 0)
            {
                cameraHelper_->zoomOut(10);
            }
            else if (mouseCenter > 0)
            {
                cameraHelper_->zoomIn(10);
            };
        }
    }
}
