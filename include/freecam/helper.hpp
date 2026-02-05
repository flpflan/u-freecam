#pragma once

#include "umod/utype/unity_engine/core.hpp"

namespace freecam::helper
{
    auto isCurrentFreeCamera() -> bool;
    auto selectGameObject() -> umod::UTYPE::unity_engine::Transform *;
    auto getMaxDepthCamera() -> umod::UTYPE::unity_engine::Camera *;
}
