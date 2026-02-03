#include "umod/runtime/helper/transform.hpp"
#include "umod/runtime/helper/time.hpp"
#include "umod/utils/math.hpp"

namespace umod::unity_runtime::helper
{
    using namespace utils::math;
    using namespace UTYPE::unity_engine;

    Vector3 TransformHelper::calculToMove(const Vector3 input)
    {
        const auto forward = trans.GetForward();
        const auto right = trans.GetRight();
        const auto up = trans.GetUp();
        return Vector3(right.x * input.x + forward.x * input.y + up.x * input.z,
                       right.y * input.x + forward.y * input.y + up.y * input.z,
                       right.z * input.x + forward.z * input.y + up.z * input.z);
    }
    auto TransformHelper::calculToRotate(const Vector2 input) -> Vector2 { return input; }
    auto TransformHelper::rotate(const Vector2 input) -> void
    {
        const auto rotateDelta = calculToRotate(input) * TimeUtils::getDeltaTime_s();

        auto [pitch, yaw, roll] = trans.GetRotation().ToEuler();
        yaw += rotateDelta.x;
        pitch -= rotateDelta.y;

        pitch = clamp(pitch, -80.0f, 80.0f);
        const auto euler = Quaternion().Euler(pitch, yaw, roll);
        trans.SetRotation(euler);
    }
    auto TransformHelper::move(const Vector3 input) -> void
    {
        const auto moveDelta = calculToMove(input) * TimeUtils::getDeltaTime_s();
        trans.SetPosition(trans.GetPosition() + moveDelta);
    }
    auto TransformHelper::roll(const float toRoll) -> void
    {
        const auto forward = trans.GetForward();
        auto [pitch, yaw, roll] = trans.GetRotation().ToEuler();
        roll += toRoll * TimeUtils::getDeltaTime_s();
        roll = clamp(roll, -180.f, 180.f);
        const auto euler = Quaternion().Euler(pitch, yaw, roll);
        trans.SetRotation(euler);
    }
    auto TransformHelper::resetRoll() -> void
    {
        const auto [pitch, yaw, _] = trans.GetRotation().ToEuler();
        const auto euler = Quaternion().Euler(pitch, yaw, 0);
        trans.SetRotation(euler);
    }
}
