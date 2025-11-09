#include "proxy/transform.hpp"
#include "proxy/time.hpp"
#include "utils.hpp"

namespace FreeCam::Proxy
{
    UTYPE::Vector3 Transform::calculToMove(const UTYPE::Vector3 input)
    {
        const auto forward = trans.GetForward();
        const auto right = trans.GetRight();
        const auto up = trans.GetUp();
        // clang-format off
        return UTYPE::Vector3(right.x * input.x + forward.x * input.y + up.x * input.z,
                              right.y * input.x + forward.y * input.y + up.y * input.z,
                              right.z * input.x + forward.z * input.y + up.z * input.z);
        // clang-format on
    }
    auto Transform::calculToRotate(const UTYPE::Vector2 input) -> UTYPE::Vector2 { return input; }
    auto Transform::Rotate(const UTYPE::Vector2 input) -> void
    {
        const auto rotateDelta = calculToRotate(input) * Time::GetDeltaTime_s();

        auto [pitch, yaw, roll] = trans.GetRotation().ToEuler();
        yaw += rotateDelta.x;
        pitch -= rotateDelta.y;

        pitch = Clamp(pitch, -80.0f, 80.0f);
        const auto euler = UTYPE::Quaternion().Euler(pitch, yaw, roll);
        trans.SetRotation(euler);
    }
    auto Transform::Move(const UTYPE::Vector3 input) -> void
    {
        const auto moveDelta = calculToMove(input) * Time::GetDeltaTime_s();
        trans.SetPosition(trans.GetPosition() + moveDelta);
    }
    auto Transform::Roll(const float toRoll) -> void
    {
        const auto forward = trans.GetForward();
        auto [pitch, yaw, roll] = trans.GetRotation().ToEuler();
        roll += toRoll * Time::GetDeltaTime_s();
        roll = Clamp(roll, -180.f, 180.f);
        const auto euler = UTYPE::Quaternion().Euler(pitch, yaw, roll);
        trans.SetRotation(euler);
    }
    auto Transform::ResetRoll() -> void
    {
        const auto [pitch, yaw, _] = trans.GetRotation().ToEuler();
        const auto euler = UTYPE::Quaternion().Euler(pitch, yaw, 0);
        trans.SetRotation(euler);
    }
}
