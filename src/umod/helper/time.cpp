#include "umod/runtime/helper/time.hpp"
#include "umod/core.hpp"
#include "umod/utype/unity_engine/time.hpp"

#include "user/config.hpp"

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
