#pragma once

#include <functional>
#include <vector>

namespace umod::utils::hook_registry
{
    inline const static std::vector<std::function<void()>> NEXT_FRAME = {};
}
