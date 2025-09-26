#pragma once

#include "UnityResolve.hpp"

namespace UType
{
    using UClass = UnityResolve::Class;
    using UMethod = UnityResolve::Method;
    using UTYPE = UnityResolve::UnityType;

    class Camera : public UTYPE::Camera
    {
    public:
        auto GetFieldOfView() -> float
        {
            static UMethod *method;
            if (!method) method = Camera::GetUClass()->Get<UMethod>("get_fieldOfView");
            return method->Invoke<float>(this);
        }
        auto SetFieldOfView(float v) -> void
        {
            static UMethod *method;
            if (!method) method = Camera::GetUClass()->Get<UMethod>("set_fieldOfView");
            return method->Invoke<void>(this);
        }

    public:
        inline static auto GetUClass() -> UClass *
        {
            static UClass *klass;
            if (!klass) klass = UnityResolve::Get("UnityEngine.CoreModule.dll")->Get("Camera");
            return klass;
        }
    };
}
