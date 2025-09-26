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
        inline static auto GetFrameCount() -> int
        {

            static UMethod *method;
            if (!method) method = Time::GetUClass()->Get<UMethod>("get_frameCount");
            return method->Invoke<int>();
        }

    public:
        inline static auto GetUClass() -> UClass *
        {
            static UClass *klass;
            if (!klass) klass = UnityResolve::Get("UnityEngine.CoreModule.dll")->Get("Time");
            return klass;
        }
    };
}
