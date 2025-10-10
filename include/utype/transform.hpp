#pragma once

#include "UnityResolve.hpp"

namespace UType
{
    using UClass = UnityResolve::Class;
    using UMethod = UnityResolve::Method;
    using UTYPE = UnityResolve::UnityType;

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

    public:
        inline static auto GetUClass() -> UClass *
        {
            static UClass *klass;
            if (!klass) klass = UnityResolve::Get("UnityEngine.CoreModule.dll")->Get("Transform");
            return klass;
        }
    };
}
