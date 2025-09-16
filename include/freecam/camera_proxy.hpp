#pragma once

#include "freecam.hpp"

namespace FreeCam {
    class CameraProxy
    {
        UTYPE::Camera *camera;
        UTYPE::Transform *transform;

#if ANDROID_MODE
        const float rotationSpeed = 100.0f;
#else
        const float rotationSpeed = 500.0f;
#endif
        float yaw;
        float pitch;
        float roll;

        UTYPE::Vector3 position;
        const float moveSpeed = 1.0f;
        const float moveSpeedMultiplier = 5.0f;

    public:
        CameraProxy(UTYPE::Camera *);
        auto Rotate(UTYPE::Vector2) -> void;
        auto Move(UTYPE::Vector3, bool) -> void;
        static auto IsCurrentFreeCamera() -> bool;
    };
}
