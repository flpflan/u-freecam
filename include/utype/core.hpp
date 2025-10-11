#pragma once

#include "UnityResolve.hpp"

namespace UType
{
    using UClass = UnityResolve::Class;
    using UMethod = UnityResolve::Method;
    using UTYPE = UnityResolve::UnityType;

    class MonoBehavior : public UTYPE::MonoBehaviour
    {
    public:
        inline static auto GetUClass() -> UClass *
        {
            static UClass *klass;
            if (!klass) klass = UnityResolve::Get("UnityEngine.CoreModule.dll")->Get("MonoBehaviour");
            return klass;
        }
    };

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
