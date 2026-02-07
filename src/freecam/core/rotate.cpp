#include "freecam/freecam.hpp"
#include "umod/runtime/helper/input.hpp"

#include "user/config.hpp"

using namespace umod::UTYPE::unity_engine;
using namespace umod::unity_runtime::helper;

using enum user_config::freecam::keybind::Key;

namespace freecam
{
    auto FreeCamera::updateRotate() -> void
    {
        using namespace user_config::freecam;

        Vector2 toRotate(0, 0);
        toRotate.x = InputUtils::GetAxis("Mouse X") * property::RotationSpeed;
        toRotate.y = InputUtils::GetAxis("Mouse Y") * property::RotationSpeed;
        if (InputUtils::GetKey(keybind::RotateUp)) toRotate.y = property::RotationSpeed;
        if (InputUtils::GetKey(keybind::RotateDown)) toRotate.y = -property::RotationSpeed;
        if (InputUtils::GetKey(keybind::RotateRight)) toRotate.x = property::RotationSpeed;
        if (InputUtils::GetKey(keybind::RotateLeft)) toRotate.x = -property::RotationSpeed;
        if (toRotate.x || toRotate.y) anchorTransHelper_->rotate(toRotate);
    }
    auto FreeCamera::updateRoll() -> void
    {
        using namespace user_config::freecam;

        if (InputUtils::GetKey(keybind::RollLeft))
            anchorTransHelper_->roll(InputUtils::GetKey(keybind::SpeedUp) ? -property::RollSpeed * 2
                                                                          : -property::RollSpeed);
        if (InputUtils::GetKey(keybind::RollRight))
            anchorTransHelper_->roll(InputUtils::GetKey(keybind::SpeedUp) ? property::RollSpeed * 2
                                                                          : property::RollSpeed);
        if (InputUtils::GetKeyDown(keybind::ResetRoll)) anchorTransHelper_->resetRoll();
    }
}
