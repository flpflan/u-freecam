#pragma once

#include "utype/core.hpp"

using UTYPE = UnityResolve::UnityType;
using UMethod = UnityResolve::Method;

namespace FreeCam::Proxy
{
    class Camera
    {
        UType::Camera &cam;

        // Properties for Zoom
        inline static constinit float defaultZoom;
        float currentZoom = defaultZoom;
        const float zoomSpeed = 20.f;

    public:
        Camera(UTYPE::Camera &c) : cam(static_cast<UType::Camera &>(c))
        {
            defaultZoom = cam.GetFieldOfView();
            currentZoom = defaultZoom;
        }
        Camera(UTYPE::Camera *c) : Camera(*c) {}
        auto ZoomIn(float) -> void;
        auto ZoomOut(float) -> void;
        auto ResetZoom() -> void;
    };
}
