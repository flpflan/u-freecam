#include "proxy/camera.hpp"
#include "proxy/time.hpp"
#include "utils.hpp"

namespace FreeCam::Proxy
{
    auto Camera::ZoomIn(float am) -> void
    {
        currentZoom -= am * zoomSpeed * Time::GetDeltaTime_s();
        currentZoom = Clamp(currentZoom, 1.f, 150.f);
        cam.SetFoV(currentZoom);
    }
    auto Camera::ZoomOut(float am) -> void
    {
        // if (cam.GetFieldOfView() > defaultZoom) return;
        return ZoomIn(-am);
    }
    auto Camera::ResetZoom() -> void
    {
        currentZoom = defaultZoom;
        return ZoomIn(0);
    }
}
