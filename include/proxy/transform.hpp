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
        inline auto SetRotation(const UTYPE::Quaternion &v) -> void { trans.SetRotation(v); }
        inline auto SetLocalPosition(const UTYPE::Vector3 &v) -> void { trans.SetLocalPosition(v); }
        inline auto SetLocalRotation(const UTYPE::Quaternion &v) -> void { trans.SetLocalRotation(v); }
        auto Rotate(const UTYPE::Vector2) -> void;
        auto Move(const UTYPE::Vector3, const bool) -> void;
        auto Roll(const float) -> void;
        auto ResetRoll() -> void;
        inline auto GetUTransform() -> UTYPE::Transform * { return &trans; }

    private:
        auto calculToMove(const UTYPE::Vector3, const bool) -> UTYPE::Vector3;
        auto calculToRotate(const UTYPE::Vector2) -> UTYPE::Vector2;
    };
}
