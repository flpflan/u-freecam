#include "proxy/camera.hpp"
#include "debug/logger.hpp"
#include "proxy/time.hpp"

template <typename T>
constexpr inline static T Clamp(T n, T bb, T bt)
{
    if (n < bb) return bb;
    if (n > bt) return bt;
    return n;
}

namespace FreeCam::Proxy
{
    auto Camera::Rotate(UTYPE::Vector2 input) -> void
    {
        yaw += input.x * rotationSpeed * Time::GetDeltaTime_s();
        pitch -= input.y * rotationSpeed * Time::GetDeltaTime_s();

        pitch = Clamp(pitch, -80.0f, 80.0f);
        const auto euler = UTYPE::Quaternion().Euler(pitch, yaw, roll);
        transform->SetRotation(euler);
    }
    auto Camera::Move(UTYPE::Vector3 input, bool sprint = true) -> void
    {
        const auto speed = sprint ? moveSpeed * moveSpeedMultiplier : moveSpeed;

        const auto forward = transform->GetForward();
        const auto right = transform->GetRight();
        const auto up = transform->GetUp();
        // forward.y = 0;
        // right.y = 0;
        // forward.Normalize();
        // right.Normalize();

        // clang-format off
        const auto move = UTYPE::Vector3(right.x * input.x + forward.x * input.y + up.x * input.z,
                                         right.y * input.x + forward.y * input.y + up.y * input.z,
                                         right.z * input.x + forward.z * input.y + up.z * input.z);
        // clang-format on

        position.y += move.y * speed * Time::GetDeltaTime_s();
        position.z += move.z * speed * Time::GetDeltaTime_s();
        position.x += move.x * speed * Time::GetDeltaTime_s();
        transform->SetPosition(position);
    }
    // auto Camera::ZoomIn(float am) -> void
    // {
    //     currentZoom -= am * zommSpeed * Time::GetDeltaTime_s();
    //     currentZoom = Clamp(currentZoom, 1.f, 179.f);
    //     fieldOfView = currentZoom;
    // }
    // auto Camera::ZoomOut(float am) -> void
    // {
    //     if (fieldOfView > defaultZoom) return;
    //     return ZoomIn(-am);
    // }
    // auto Camera::ResetZoom() -> void
    // {
    //     currentZoom = defaultZoom;
    //     return ZoomIn(0);
    // }
}
