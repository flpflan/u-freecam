#pragma once

#include "umod/feature.hpp"

namespace freecam
{
    void load();
    void unload();

    inline constexpr umod::feature::Module::Desc kDesc{.name = "FreeCam", .load = load, .unload = unload};
}
