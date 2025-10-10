#pragma once

#include "UnityResolve.hpp"

namespace UType
{
    using UClass = UnityResolve::Class;
    using UMethod = UnityResolve::Method;
    using UTYPE = UnityResolve::UnityType;

    struct Quaternion : public UTYPE::Quaternion
    {
    public:
        inline static auto AngleAxis(float angle, UTYPE::Vector3 axis) -> Quaternion
        {
            static UMethod *method;
            if (!method) method = GetUClass()->Get<UMethod>("AngleAxis", {"System.Single", "UnityEngine.Vector3"});
            return method->Invoke<Quaternion>(angle, axis);
        }

    public:
        inline static auto GetUClass() -> UClass *
        {
            static UClass *klass;
            if (!klass) klass = UnityResolve::Get("UnityEngine.CoreModule.dll")->Get("Quaternion");
            return klass;
        }
    };
}
