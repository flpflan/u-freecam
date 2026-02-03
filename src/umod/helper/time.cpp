#include "umod/runtime/helper/time.hpp"
#include "umod/core.hpp"
#include "umod/utype/unity_engine/time.hpp"

#include "user/config.hpp"

template <class Rep, class Period>
constexpr float to_seconds(std::chrono::duration<Rep, Period> d) noexcept
{
    return std::chrono::duration<float>(d).count();
}

namespace umod::unity_runtime::helper
{
    using namespace umod::UTYPE::unity_engine;

    namespace TimeUtils
    {
        auto getDeltaTime_s() -> float
        {
            if (core::player_loop::isMockLoop()) return to_seconds(user::config::core::MockLoopDeltaTime);
            return Time::GetDeltaTime();
        }
    }
}
