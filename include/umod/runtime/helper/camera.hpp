#pragma once

#include "umod/utype/unity_engine/core.hpp"

namespace umod::unity_runtime::helper
{
    class CameraHelper
    {
        using Camera = UTYPE::unity_engine::Camera;

        Camera &cam;

        // Properties for Zoom
        const float defaultZoom;
        float currentZoom = defaultZoom;
        const float zoomSpeed = 20.f;

    public:
        explicit CameraHelper(Camera &c) : cam(c), defaultZoom(cam.GetFoV()) {}
        auto zoomIn(float) -> void;
        auto zoomOut(float) -> void;
        auto resetZoom() -> void;
    };
}
