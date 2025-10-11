#pragma once

#include "UnityResolve.hpp"

namespace UType
{
    using UClass = UnityResolve::Class;
    using UMethod = UnityResolve::Method;
    using UTYPE = UnityResolve::UnityType;

    class Collider : public UTYPE::Collider
    {
    };

    struct RaycastHit : public UTYPE::RaycastHit
    {
    public:
        auto GetCollider() -> Collider *
        {

            static UMethod *method;
            if (!method) method = GetUClass()->Get<UMethod>("get_collider");
            return method->Invoke<Collider *>(this);
        }

    public:
        inline static auto GetUClass() -> UClass *
        {
            static UClass *klass;
            if (!klass) klass = UnityResolve::Get("UnityEngine.PhysicsModule.dll")->Get("RaycastHit");
            return klass;
        }
    };
}
