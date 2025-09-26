#pragma once

#include "utype/camera.hpp"

using UTYPE = UnityResolve::UnityType;
using UMethod = UnityResolve::Method;

namespace FreeCam::Proxy
{
    class Camera
    {
        UType::Camera *camera;
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
        inline static constinit float defaultZoom;
        float currentZoom = defaultZoom;
#ifdef __ANDROID__
        const float zoomSpeed = 20.f;
#else
        const float zoomSpeed = 6000.f;
#endif

    public:
        Camera(UTYPE::Camera *c)
        {
            camera = static_cast<UType::Camera *>(c);
            transform = camera->GetTransform();
            defaultZoom = camera->GetFieldOfView();
            currentZoom = defaultZoom;
            CopyState(c);
        }
        auto CopyState(UTYPE::Camera *c) -> void
        {
            const auto transform = c->GetTransform();
            position = transform->GetPosition();
            const auto angles = transform->GetRotation().ToEuler();
            yaw = angles.y;
            pitch = angles.x;
            roll = angles.z;
        }
        inline auto SetPosition(UTYPE::Vector3 v) -> void { transform->SetPosition(v); }
        inline auto SetRotation(UTYPE::Quaternion v) -> void { transform->SetRotation(v); }
        auto Rotate(UTYPE::Vector2) -> void;
        auto Move(UTYPE::Vector3, bool) -> void;
        auto ZoomIn(float) -> void;
        auto ZoomOut(float) -> void;
        auto ResetZoom() -> void;
    };
}
