#include "feature/free_camera.hpp"
#include "proxy/time.hpp"
#include "utils.hpp"
#include "utype/unity_engine/input.hpp"

using enum UTYPE::KeyCode;
using UTYPE::Input;

class AccelerationTimer
{
private:
    constexpr static float AccelTime = 10.f;
    float accelTimer = 0.f;

public:
    float Tick()
    {
        accelTimer += FreeCam::Proxy::Time::GetDeltaTime_s();
        accelTimer = Clamp(accelTimer, 0.f, AccelTime);
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

namespace FreeCam::Feature
{
    auto FreeCamera::updateMove() -> void
    {
        UTYPE::Vector3 toMove(0, 0, 0);
        // toMove.x = UnityApi::GetAxis("Horizontal");
        // toMove.y = UnityApi::GetAxis("Vertical");
        if (Input::GetKey(Space)) toMove.z = 1;
        if (Input::GetKey(Ctrl_L)) toMove.z = -1;
        if (Input::GetKey(W)) toMove.y = 1;
        if (Input::GetKey(S)) toMove.y = -1;
        if (Input::GetKey(A)) toMove.x = -1;
        if (Input::GetKey(D)) toMove.x = 1;
        if (toMove.x || toMove.y || toMove.z)
        {
            const float baseSpeed = Input::GetKey(SHIFT_L) ? baseMoveSpeed * 5 : baseMoveSpeed;
            const float t = Input::GetKey(SHIFT_L) ? AccelTimer.Tick() : AccelTimer.Reset();
            const auto curve = DualEase(t);
            const float speed = Lerp(baseSpeed, maxMoveSpeed, curve);
            if (Input::GetKey(M))
            {
                freeTrans->Move(toMove * speed);
            }
            else
            {
                if (!attach_mode)
                {
                    anchorTrans->Move(toMove * speed);
                }
                else
                {
                    freeTrans->Move(toMove * speed);
                }
            }
        }
        else
            AccelTimer.Reset();
        if (Input::GetKeyDown(M) && Input::GetKey(SHIFT_L)) freeTrans->SetLocalPosition(UTYPE::Vector3(0, 0, 0));
    }
}
