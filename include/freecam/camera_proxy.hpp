#pragma once

#include "freecam.hpp"

namespace FreeCam {
    class CameraProxy
    {
        UTYPE::Camera *camera;
        UTYPE::Transform *transform;

        const float rotationSpeed = 100.0f;
        float yaw;
        float pitch;
        float roll;

        UTYPE::Vector3 position;
        const float moveSpeed = 1.0f;
        const float moveSpeedMultiplier = 5.0f;

    public:
        CameraProxy(UTYPE::Camera *c)
        {
            camera = c;
            transform = camera->GetTransform();
            position = transform->GetPosition();
            const auto angles = transform->GetRotation().ToEuler();
            yaw = angles.y;
            pitch = angles.x;
            roll = angles.z;
        }
        auto Rotate(UTYPE::Vector2) -> void;
        auto Move(UTYPE::Vector3, bool) -> void;
        inline static bool IsCurrentFreeCamera()
        {
            const auto curCam = UTYPE::Camera::GetMain();
            if(curCam == nullptr) return true;
            const auto _n = std::wstring(curCam->GetName()->m_firstChar);
            return std::string(_n.begin(), _n.end()).starts_with("U_rea");
        }
    };
}
