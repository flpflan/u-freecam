#include "freecam/freecam.hpp"
#include "umod/runtime/helper/time.hpp"
#include "umod/utils/math.hpp"
#include "umod/runtime/helper/input.hpp"

#include "user/config.hpp"

using namespace umod::UTYPE::unity_engine;
using namespace umod::unity_runtime::helper;
using namespace umod::utils;

using enum user_config::freecam::keybind::Key;

class AccelerationTimer
{
private:
    constexpr static float AccelTime = 10.f;
    float accelTimer = 0.f;

public:
    float Tick()
    {
        const auto deltaTime = TimeUtils::getDeltaTime_s();
        accelTimer += deltaTime != 0 ? deltaTime : to_seconds(user_config::core::MockLoopDeltaTime);
        accelTimer = math::clamp(accelTimer, 0.f, AccelTime);
        return accelTimer / AccelTime;
    }
    constexpr float Reset() { return (accelTimer = 0.f); }
} AccelTimer;

constexpr static float Smoothstep(const float t) { return 3 * t * t - 2 * t * t * t; }

constexpr static float EaseOutQuad(const float u) { return 1.f - (1.f - u) * (1.f - u); }
constexpr static float EaseInQuad(const float u) { return u * u; }
constexpr static float DualEase(const float t)
{
    if (t < 0.5f)
    {
        const float u = t / 0.5f;
        return 0.5f * EaseOutQuad(u);
    }
    else
    {
        const float u = (t - 0.5f) / 0.5f;
        return 0.5f + 0.5f * EaseInQuad(u);
    }
}

namespace freecam
{
    auto FreeCamera::updateMove() -> void
    {
        using namespace user_config::freecam;
        Vector3 toMove(0, 0, 0);
        // toMove.x = UnityApi::GetAxis("Horizontal");
        // toMove.y = UnityApi::GetAxis("Vertical");
        if (InputUtils::GetKey(keybind::Up)) toMove.z = 1;
        if (InputUtils::GetKey(keybind::Down)) toMove.z = -1;
        if (InputUtils::GetKey(keybind::Forward)) toMove.y = 1;
        if (InputUtils::GetKey(keybind::Back)) toMove.y = -1;
        if (InputUtils::GetKey(keybind::Left)) toMove.x = -1;
        if (InputUtils::GetKey(keybind::Right)) toMove.x = 1;
        if (toMove.x || toMove.y || toMove.z)
        {
            const float baseSpeed =
                InputUtils::GetKey(keybind::SpeedUp) ? property::BaseMoveSpeed * 5 : property::BaseMoveSpeed;
            const float t =
                InputUtils::GetKey(keybind::SpeedUp) ? AccelTimer.Tick() : AccelTimer.Reset(); // 累计加速时间
            const auto curve = DualEase(t);
            const float speed = math::lerp(baseSpeed, property::MaxMoveSpeed, curve);

            if (InputUtils::GetKey(keybind::PinAnchor) || kFlags.attach_mode)
                freeTransHelper_->move(toMove * speed);
            else
                anchorTransHelper_->move(toMove * speed);
        }
        else
            AccelTimer.Reset();
        if (InputUtils::GetKeyDown(keybind::PinAnchor) && InputUtils::GetKey(keybind::SpeedUp))
            freeTransHelper_->setLocalPosition(Vector3(0, 0, 0));
    }
}
