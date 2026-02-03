#pragma once

namespace umod::unity_runtime::helper
{
    namespace TimeUtils
    {
        auto getDeltaTime_s() -> float;
        inline auto getDeltaTime_ms() -> float { return getDeltaTime_s() * 1000; }
        inline auto getDeltaTime_us() -> float { return getDeltaTime_ms() * 1000; }
    }
}
