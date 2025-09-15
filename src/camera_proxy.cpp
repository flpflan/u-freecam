#include "freecam.hpp"
#include "freecam/camera_proxy.hpp"

namespace FreeCam {
    template <typename T>
    constexpr inline T Clamp(T n, T bb, T bt)
    {
        if (n < bb) return bb;
        if (n > bt) return bt;
        return n;
    }
    auto CameraProxy::Rotate(UTYPE::Vector2 input) -> void
    {
        yaw += input.x * rotationSpeed * FreeCam::DeltaTime_s;
        pitch -= input.y * rotationSpeed * FreeCam::DeltaTime_s;

        pitch = Clamp(pitch, -80.0f, 80.0f);
        const auto euler = UTYPE::Quaternion().Euler(pitch, yaw, roll);
        transform->SetRotation(euler);
    }
    auto CameraProxy::Move(UTYPE::Vector3 input, bool sprint = true) -> void
    {
        const auto speed = sprint ? moveSpeed * moveSpeedMultiplier : moveSpeed;

        const auto forward = transform->GetForward();
        const auto right = transform->GetRight();
        const auto up = transform->GetUp();
        // forward.y = 0;
        // right.y = 0;
        // forward.Normalize();
        // right.Normalize();
        const auto move = UTYPE::Vector3(right.x * input.x + forward.x * input.y + up.x * input.z,
                                         right.y * input.x + forward.y * input.y + up.y * input.z,
                                         right.z * input.x + forward.z * input.y + up.z * input.z);

        position.y += move.y * speed * FreeCam::DeltaTime_s;
        position.z += move.z * speed * FreeCam::DeltaTime_s;
        position.x += move.x * speed * FreeCam::DeltaTime_s;
        transform->SetPosition(position);
    }
}
