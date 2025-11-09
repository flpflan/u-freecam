#pragma once

#include "utype/utype.hpp"

namespace UTYPE
{
    using Vector2 = UnityResolve::UnityType::Vector2;
    using Vector3 = UnityResolve::UnityType::Vector3;
    using Vector4 = UnityResolve::UnityType::Vector4;
    using Quaternion = UnityResolve::UnityType::Quaternion;
    using String = UnityResolve::UnityType::String;
    using Screen = UnityResolve::UnityType::Screen;

    enum class Space
    {
        World = 0x0,
        Self = 0x1
    };

    class Transform : public UTYPE::Transform
    {
        UNITY_CLASS_DECL("UnityEngine.CoreModule.dll", Transform)
    public:
        UNITY_METHOD(Transform *, GetParent, (void))
        UNITY_METHOD(void, SetParent, (UTYPE::Transform *const parent), parent)
        inline auto GetChild(const int index) -> Transform * { return static_cast<Transform *>(UTYPE::Transform::GetChild(index)); }

    public:
        inline auto Rotate(Vector3 axis, float angle, Space relativeTo) -> void
        {

            static UMethod *method;
            if (!method) method = GetUClass()->Get<UMethod>("Rotate", {"UnityEngine.Vector3", "System.Single", "UnityEngine.Space"});
            return method->Invoke<void>(this, axis, angle, relativeTo);
        }
    };

    class GameObject : public UTYPE::GameObject
    {
        UNITY_CLASS_DECL("UnityEngine.CoreModule.dll", GameObject)
    public:
        inline static auto Create(const std::string &name) -> GameObject * { return static_cast<GameObject *>(UTYPE::GameObject::Create(name)); }
        inline auto GetTransform() -> Transform * { return static_cast<Transform *>(UTYPE::GameObject::GetTransform()); }
        inline auto IsDestoryed() -> bool
        {
            static UMethod *method;
            if (!method) method = UnityResolve::Get("UnityEngine.CoreModule.dll")->Get("Object")->Get<UMethod>("op_Equality");
            return method->Invoke<bool>(this, nullptr);
        }
    };

    class MonoBehaviour : public UTYPE::MonoBehaviour
    {
        UNITY_CLASS_DECL("UnityEngine.CoreModule.dll", MonoBehaviour)
    };

    class Camera : public UTYPE::Camera
    {
        UNITY_CLASS_DECL("UnityEngine.CoreModule.dll", Camera)
    public:
        inline static auto GetMain() -> Camera * { return static_cast<Camera *>(UTYPE::Camera::GetMain()); }
        inline auto GetGameObject() -> GameObject * { return static_cast<GameObject *>(UTYPE::Camera::GetGameObject()); }
    };

    // struct Quaternion : public UTYPE::Quaternion
    // {
    //     UNITY_CLASS_DECL("UnityEngine.CoreModule.dll", Quaternion)
    // public:
    //     inline static auto AngleAxis(float angle, Vector3 axis) -> Quaternion
    //     {
    //         static UMethod *method;
    //         if (!method) method = GetUClass()->Get<UMethod>("AngleAxis", {"System.Single", "UnityEngine.Vector3"});
    //         return method->Invoke<Quaternion>(angle, axis);
    //     }
    // };
}
