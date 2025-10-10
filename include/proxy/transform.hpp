#pragma once

#include "utype/transform.hpp"

using UTYPE = UnityResolve::UnityType;
using UMethod = UnityResolve::Method;

namespace FreeCam::Proxy
{
    class Transform
    {
        UTYPE::Transform &trans;
        // Properties for Rotate
#ifdef __ANDROID__
        constexpr static float rotationSpeed = 100.f;
#else
        constexpr static float rotationSpeed = 500.f;
#endif
        float yaw;
        float pitch;
        float roll;
        float localYaw;
        float localPitch;
        float localRoll;

        // Properties for Move
        const float moveSpeed = 1.f;
        const float moveSpeedMultiplier = 5.f;

    public:
        Transform(UTYPE::Transform *t) : Transform(*t) {}
        Transform(UTYPE::Transform &t) : trans(t) { CopyState(trans); }
        auto CopyState(UTYPE::Transform &t) -> void
        {
            SetPosition(t.GetPosition());
            SetRotation(t.GetRotation());
            SetLocalPosition(t.GetLocalPosition());
            SetLocalRotation(t.GetLocalRotation());
        }
        inline auto SetPosition(const UTYPE::Vector3 &v) -> void { trans.SetPosition(v); }
        auto SetRotation(const UTYPE::Quaternion &v) -> void
        {
            const auto angles = v.ToEuler();
            yaw = angles.y;
            pitch = angles.x;
            roll = angles.z;
            trans.SetRotation(v);
        }
        inline auto SetLocalPosition(const UTYPE::Vector3 &v) -> void { trans.SetLocalPosition(v); }
        auto SetLocalRotation(const UTYPE::Quaternion &v) -> void
        {
            const auto angles = v.ToEuler();
            localYaw = angles.y;
            localPitch = angles.x;
            localRoll = angles.z;
            trans.SetLocalRotation(v);
        }
        auto Rotate(UTYPE::Vector2) -> void;
        auto Move(UTYPE::Vector3, bool) -> void;
        auto Roll(float) -> void;
        auto ResetRoll() -> void;
        inline auto GetUTransform() -> UTYPE::Transform * { return &trans; }

    private:
        auto calculToMove(UTYPE::Vector3, bool) -> UTYPE::Vector3;
    };
}
