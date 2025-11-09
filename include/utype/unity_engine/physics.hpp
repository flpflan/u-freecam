#pragma once

#include "UnityResolve.hpp"
#include "utype/utype.hpp"

namespace UTYPE
{
    using Physics = UnityResolve::UnityType::Physics;

    class Collider : public UTYPE::Collider
    {
        UNITY_CLASS_DECL("UnityEngine.PhysicsModule.dll", Collider)
    };

    struct RaycastHit : public UTYPE::RaycastHit
    {
        UNITY_CLASS_DECL("UnityEngine.PhysicsModule.dll", RaycastHit)
    public:
        UNITY_METHOD(Collider *, get_collider, (void))
    };
}
