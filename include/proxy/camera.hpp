#pragma once

#include "utype/camera.hpp"

using UTYPE = UnityResolve::UnityType;
using UMethod = UnityResolve::Method;

namespace FreeCam::Proxy
{
    class Camera : public UType::Camera
    {
        UType::Camera *camera;
        UTYPE::Transform *transform;

#if ANDROID_MODE
        const float rotationSpeed = 100.f;
#else
        const float rotationSpeed = 500.f;
#endif
        float yaw;
        float pitch;
        float roll;

        UTYPE::Vector3 position;
        const float moveSpeed = 1.f;
        const float moveSpeedMultiplier = 5.f;

        inline static constexpr float defaultZoom = 0.f;
        float currentZoom = defaultZoom;
        const float zommSpeed = 10.f;

    public:
        Camera(UType::Camera *);
        auto Rotate(UTYPE::Vector2) -> void;
        auto Move(UTYPE::Vector3, bool) -> void;
        auto ZoomIn(float) -> void;
        auto ZoomOut(float) -> void;
        auto ResetZoom() -> void;
        static auto IsCurrentFreeCamera() -> bool;
    };
}
