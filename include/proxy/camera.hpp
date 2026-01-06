#pragma once

#include "utype/unity_engine/core.hpp"

namespace FreeCam::Proxy
{
    class Camera
    {
        UTYPE::Camera &cam;

        // Properties for Zoom
        const float defaultZoom;
        float currentZoom = defaultZoom;
        const float zoomSpeed = 20.f;

    public:
        Camera(UTYPE::Camera &c) : cam(static_cast<UTYPE::Camera &>(c)), defaultZoom(cam.GetFoV()) {}
        Camera(UTYPE::Camera *c) : Camera(*c) {}
        auto ZoomIn(float) -> void;
        auto ZoomOut(float) -> void;
        auto ResetZoom() -> void;
        auto GetUCamera() -> UTYPE::Camera * { return &cam; }
    };
}
