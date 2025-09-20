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
        
        template <typename T>
        class Property_fieldOfView
        {
            Camera *k;
        public:
            Property_fieldOfView(Camera *k) : k(k) {};

            inline Property_fieldOfView &operator=(const T &v)
            {
                set(v);
                return *this;
            }
            inline operator const T &() { return get(); }
        private:
            auto set(const T value) -> void
            {
                static UMethod *method;
                if (!method) method = Camera::GetUClass()->Get<UMethod>("set_fieldOfView");
                return method->Invoke<void>(k, value);
            }
            auto get() -> const T
            {
                static UMethod *method;
                if (!method) method = Camera::GetUClass()->Get<UMethod>("get_fieldOfView");
                return method->Invoke<T>(k);
            }
        };
        Property_fieldOfView<float> fieldOfView = Property_fieldOfView<float>(this);
        public:
        public:
        public:
        public:
        inline static auto GetUClass() -> UClass *
        {
            static UClass *klass;
            if(!klass) klass = UnityResolve::Get("UnityEngine.CoreModule.dll")->Get("Camera");
            return klass;
        }
    };
    }