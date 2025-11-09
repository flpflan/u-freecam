#include "feature/free_camera.hpp"
#include "proxy/time.hpp"
#include "utils.hpp"
#include "utype/unity_engine/input.hpp"

using enum UTYPE::KeyCode;
using UTYPE::Input;

class AccelerationCurve
{
private:
    float accelTimer = 0.f;

public:
    float Smoothstep()
    {
        constexpr float accelTime = 5.f;
        if (Input::GetKey(SHIFT_L))
            accelTimer += FreeCam::Proxy::Time::GetDeltaTime_s();
        else
            accelTimer -= FreeCam::Proxy::Time::GetDeltaTime_s();
        accelTimer = Clamp(accelTimer, 0.f, accelTime);
        float t = accelTimer / accelTime;
        float curve = 3 * t * t - 2 * t * t * t; // smoothstep
        return curve;
    }
} AccelerationCurve;

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
            const auto curve = AccelerationCurve.Smoothstep();
            const float speed = Lerp(baseMoveSpeed, maxMoveSpeed, curve);
            toMove = toMove * speed;
            if (Input::GetKey(M))
            {
                freeTrans->Move(toMove);
            }
            else
            {
                if (!attach_mode)
                {
                    anchorTrans->Move(toMove);
                }
                else
                {
                    freeTrans->Move(toMove);
                }
            }
        }
        if (Input::GetKeyDown(M) && Input::GetKey(SHIFT_L)) freeTrans->SetLocalPosition(UTYPE::Vector3(0, 0, 0));
    }
}
