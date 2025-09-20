#pragma once

#include "UnityResolve.hpp"

namespace UType
{
    using UClass = UnityResolve::Class;
    using UMethod = UnityResolve::Method;
    using UTYPE = UnityResolve::UnityType;

    class Cursor
    {
    public:
        public:
        
        template <typename T>
        class Property_lockState
        {
        public:
            Property_lockState() = default;

            inline Property_lockState &operator=(const T &v)
            {
                set(v);
                return *this;
            }
            inline operator const T &() { return get(); }
        private:
            auto set(const T value) -> void
            {
                static UMethod *method;
                if (!method) method = Cursor::GetUClass()->Get<UMethod>("set_lockState");
                return method->Invoke<void>(value);
            }
            auto get() -> const T
            {
                static UMethod *method;
                if (!method) method = Cursor::GetUClass()->Get<UMethod>("get_lockState");
                return method->Invoke<T>();
            }
        };
        inline static Property_lockState<int> lockState;
        
        template <typename T>
        class Property_visible
        {
        public:
            Property_visible() = default;

            inline Property_visible &operator=(const T &v)
            {
                set(v);
                return *this;
            }
            inline operator const T &() { return get(); }
        private:
            auto set(const T value) -> void
            {
                static UMethod *method;
                if (!method) method = Cursor::GetUClass()->Get<UMethod>("set_visible");
                return method->Invoke<void>(value);
            }
            auto get() -> const T
            {
                static UMethod *method;
                if (!method) method = Cursor::GetUClass()->Get<UMethod>("get_visible");
                return method->Invoke<T>();
            }
        };
        inline static Property_visible<bool> visible;
        public:
        public:
        public:
        inline static auto GetUClass() -> UClass *
        {
            static UClass *klass;
            if(!klass) klass = UnityResolve::Get("UnityEngine.CoreModule.dll")->Get("Cursor");
            return klass;
        }
    };
    }