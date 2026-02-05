#pragma once

#include <chrono>
namespace umod::unity_runtime::helper
{
    template <class Rep, class Period>
    constexpr float to_seconds(std::chrono::duration<Rep, Period> d) noexcept
    {
        return std::chrono::duration<float>(d).count();
    }

    namespace TimeUtils
    {
        auto getDeltaTime_s() -> float;
        inline auto getDeltaTime_ms() -> float { return getDeltaTime_s() * 1000; }
        inline auto getDeltaTime_us() -> float { return getDeltaTime_ms() * 1000; }
    }
}
