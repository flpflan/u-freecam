#pragma once

#include "umod/utype/unity_engine/core.hpp"

namespace umod::unity_runtime::helper
{
    class TransformHelper
    {
        umod::UTYPE::unity_engine::Transform &trans;

    public:
        TransformHelper(umod::UTYPE::unity_engine::Transform *t) : TransformHelper(*t) {}
        TransformHelper(umod::UTYPE::unity_engine::Transform &t)
            : trans(static_cast<umod::UTYPE::unity_engine::Transform &>(t))
        {
            copyState(trans);
        }
        auto copyState(umod::UTYPE::unity_engine::Transform &t) -> void
        {
            setPosition(t.GetPosition());
            setRotation(t.GetRotation());
        }
        inline auto setPosition(const umod::UTYPE::unity_engine::Vector3 &v) -> void { trans.SetPosition(v); }
        inline auto setRotation(const umod::UTYPE::unity_engine::Quaternion &v) -> void { trans.SetRotation(v); }
        inline auto setLocalPosition(const umod::UTYPE::unity_engine::Vector3 &v) -> void { trans.SetLocalPosition(v); }
        inline auto setLocalRotation(const umod::UTYPE::unity_engine::Quaternion &v) -> void
        {
            trans.SetLocalRotation(v);
        }
        inline auto setLocalScale(const umod::UTYPE::unity_engine::Vector3 &v) -> void { trans.SetLocalScale(v); }
        inline auto setParent(umod::UTYPE::unity_engine::Transform *v) -> void { trans.SetParent(v); }
        inline auto getParent() -> umod::UTYPE::unity_engine::Transform * { return trans.GetParent(); }
        auto rotate(const umod::UTYPE::unity_engine::Vector2) -> void;
        auto move(const umod::UTYPE::unity_engine::Vector3) -> void;
        auto roll(const float) -> void;
        auto resetRoll() -> void;
        inline auto getUTransform() -> umod::UTYPE::unity_engine::Transform * { return &trans; }

    private:
        auto calculToMove(const umod::UTYPE::unity_engine::Vector3) -> umod::UTYPE::unity_engine::Vector3;
        auto calculToRotate(const umod::UTYPE::unity_engine::Vector2) -> umod::UTYPE::unity_engine::Vector2;
    };
}
