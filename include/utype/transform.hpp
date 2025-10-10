#pragma once

#include "UnityResolve.hpp"

namespace UType
{
    using UClass = UnityResolve::Class;
    using UMethod = UnityResolve::Method;
    using UTYPE = UnityResolve::UnityType;

    enum class Space
    {
        World,
        Self
    };
    class Transform : public UTYPE::Transform
    {
    public:
        inline auto GetParent() -> UTYPE::Transform *
        {

            static UMethod *method;
            if (!method) method = GetUClass()->Get<UMethod>("GetParent");
            return method->Invoke<UTYPE::Transform *>(this);
        }
        inline auto SetParent(UTYPE::Transform *const parent) -> void
        {

            static UMethod *method;
            if (!method) method = GetUClass()->Get<UMethod>("SetParent");
            return method->Invoke<void>(this, parent);
        }
        inline auto Rotate(UTYPE::Vector3 axis, float angle, Space relativeTo) -> void
        {

            static UMethod *method;
            if (!method) method = GetUClass()->Get<UMethod>("Rotate", {"UnityEngine.Vector3", "System.Single", "UnityEngine.Space"});
            return method->Invoke<void>(this, axis, angle, relativeTo);
        }

    public:
        inline static auto GetUClass() -> UClass *
        {
            static UClass *klass;
            if (!klass) klass = UnityResolve::Get("UnityEngine.CoreModule.dll")->Get("Transform");
            return klass;
        }
    };
}
