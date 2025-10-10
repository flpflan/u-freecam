#include "proxy/transform.hpp"
#include "proxy/time.hpp"
#include "utils.hpp"

namespace FreeCam::Proxy
{
    UTYPE::Vector3 Transform::calculToMove(UTYPE::Vector3 input, bool sprint)
    {
        const auto forward = trans.GetForward();
        const auto right = trans.GetRight();
        const auto up = trans.GetUp();
        // forward.y = 0;
        // right.y = 0;
        // forward.Normalize();
        // right.Normalize();

        // clang-format off
        return UTYPE::Vector3(right.x * input.x + forward.x * input.y + up.x * input.z,
                              right.y * input.x + forward.y * input.y + up.y * input.z,
                              right.z * input.x + forward.z * input.y + up.z * input.z);
        // clang-format on
    }
    auto Transform::Rotate(UTYPE::Vector2 input) -> void
    {
        yaw += input.x * rotationSpeed * Time::GetDeltaTime_s();
        pitch -= input.y * rotationSpeed * Time::GetDeltaTime_s();

        pitch = Clamp(pitch, -80.0f, 80.0f);
        const auto euler = UTYPE::Quaternion().Euler(pitch, yaw, roll);
        trans.SetRotation(euler);
    }
    auto Transform::Move(UTYPE::Vector3 input, bool sprint = true) -> void
    {
        const auto speed = sprint ? moveSpeed * moveSpeedMultiplier : moveSpeed;
        const auto move = calculToMove(input, sprint);
        position.y += move.y * speed * Time::GetDeltaTime_s();
        position.z += move.z * speed * Time::GetDeltaTime_s();
        position.x += move.x * speed * Time::GetDeltaTime_s();
        trans.SetPosition(position);
    }
    auto Transform::LocalMove(UTYPE::Vector3 input, bool sprint = true) -> void
    {
        const auto speed = sprint ? moveSpeed * moveSpeedMultiplier : moveSpeed;
        const auto move = calculToMove(input, sprint);
        localPosition.y += move.y * speed * Time::GetDeltaTime_s();
        localPosition.z += move.z * speed * Time::GetDeltaTime_s();
        localPosition.x += move.x * speed * Time::GetDeltaTime_s();
        trans.SetPosition(localPosition);
    }
}
