#include "umod/runtime/helper/camera.hpp"
#include "umod/runtime/helper/time.hpp"
#include "umod/utils/math.hpp"

using namespace umod::utils::math;

namespace umod::unity_runtime::helper
{
    auto CameraHelper::zoomIn(float am) -> void
    {
        currentZoom -= am * zoomSpeed * TimeUtils::getDeltaTime_s();
        currentZoom = clamp(currentZoom, 1.f, 150.f);
        cam.SetFoV(currentZoom);
    }
    auto CameraHelper::zoomOut(float am) -> void
    {
        // if (cam.GetFieldOfView() > defaultZoom) return;
        return zoomIn(-am);
    }
    auto CameraHelper::resetZoom() -> void
    {
        currentZoom = defaultZoom;
        return zoomIn(0);
    }
}
