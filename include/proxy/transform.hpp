#pragma once

#include "utype/unity_engine/core.hpp"

namespace FreeCam::Proxy
{
    class Transform
    {
        UTYPE::Transform &trans;

    public:
        Transform(UTYPE::Transform *t) : Transform(*t) {}
        Transform(UTYPE::Transform &t) : trans(static_cast<UTYPE::Transform &>(t)) { CopyState(trans); }
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
        inline auto SetLocalScale(const UTYPE::Vector3 &v) -> void { trans.SetLocalScale(v); }
        inline auto SetParent(UTYPE::Transform *v) -> void { trans.SetParent(v); }
        inline auto GetParent() -> UTYPE::Transform * { return trans.GetParent(); }
        auto Rotate(const UTYPE::Vector2) -> void;
        auto Move(const UTYPE::Vector3) -> void;
        auto Roll(const float) -> void;
        auto ResetRoll() -> void;
        inline auto GetUTransform() -> UTYPE::Transform * { return &trans; }

    private:
        auto calculToMove(const UTYPE::Vector3) -> UTYPE::Vector3;
        auto calculToRotate(const UTYPE::Vector2) -> UTYPE::Vector2;
    };
}
