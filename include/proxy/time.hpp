#pragma once

#include "core.hpp"
#include "utype/unity_engine/time.hpp"

namespace FreeCam::Proxy
{
    class Time
    {
    public:
        inline static auto GetDeltaTime_s() -> float
        {
            if (Core::UseMockLoop) return Core::DeltaTime_s;
            return UTYPE::Time::GetDeltaTime();
        }
        inline static auto GetDeltaTime_ms() -> float { return GetDeltaTime_s() * 1000; }
        inline static auto GetDeltaTime_us() -> float { return GetDeltaTime_ms() * 1000; }
    };
}
