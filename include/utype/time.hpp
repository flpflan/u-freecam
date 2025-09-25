#pragma once

#include "UnityResolve.hpp"

namespace UType
{
    using UClass = UnityResolve::Class;
    using UMethod = UnityResolve::Method;
    using UTYPE = UnityResolve::UnityType;

    class Time : public UTYPE::Time
    {
    public:
        template <typename T>
        class Property_frameCount
        {
        public:
            Property_frameCount() = default;

            inline Property_frameCount &operator=(const T &v)
            {
                set(v);
                return *this;
            }
            inline operator const T() { return get(); }

        private:
            auto get() -> const T
            {
                static UMethod *method;
                if (!method) method = Time::GetUClass()->Get<UMethod>("get_frameCount");
                return method->Invoke<T>();
            }
        };
        inline static Property_frameCount<int> frameCount;

    public:
        inline static auto GetUClass() -> UClass *
        {
            static UClass *klass;
            if (!klass) klass = UnityResolve::Get("UnityEngine.CoreModule.dll")->Get("Time");
            return klass;
        }
    };
}
