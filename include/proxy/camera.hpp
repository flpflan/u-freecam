#pragma once

#include "utype/camera.hpp"

using UTYPE = UnityResolve::UnityType;
using UMethod = UnityResolve::Method;

namespace FreeCam::Proxy
{
    class Camera
    {
        UTYPE::Transform *transform;

        // Properties for Rotate
#ifdef __ANDROID__
        const float rotationSpeed = 100.f;
#else
        const float rotationSpeed = 500.f;
#endif
        float yaw;
        float pitch;
        float roll;

        // Properties for Move
        UTYPE::Vector3 position;
        const float moveSpeed = 1.f;
        const float moveSpeedMultiplier = 5.f;

        // Properties for Zoom
        inline static constexpr float defaultZoom = 0.f;
        float currentZoom = defaultZoom;
        const float zommSpeed = 10.f;

    public:
        Camera(UType::Camera *c)
        {
            transform = c->GetTransform();
            position = transform->GetPosition();
            const auto angles = transform->GetRotation().ToEuler();
            yaw = angles.y;
            pitch = angles.x;
            roll = angles.z;
        }
        auto GetTransform() { return transform; }
        auto Rotate(UTYPE::Vector2) -> void;
        auto Move(UTYPE::Vector3, bool) -> void;
        auto ZoomIn(float) -> void;
        auto ZoomOut(float) -> void;
        auto ResetZoom() -> void;
    };
}
