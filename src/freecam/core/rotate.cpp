#include "freecam/freecam.hpp"
#include "umod/utype/unity_engine/input.hpp"

#include "user/config.hpp"

using namespace umod::UTYPE::unity_engine;

using enum user_config::freecam::keybind::KeyCode;

namespace freecam
{
    auto FreeCamera::updateRotate() -> void
    {
        using namespace user_config::freecam;

        Vector2 toRotate(0, 0);
        toRotate.x = Input::GetAxis("Mouse X") * property::RotationSpeed;
        toRotate.y = Input::GetAxis("Mouse Y") * property::RotationSpeed;
        if (Input::GetKey(keybind::RotateUp)) toRotate.y = property::RotationSpeed;
        if (Input::GetKey(keybind::RotateDown)) toRotate.y = -property::RotationSpeed;
        if (Input::GetKey(keybind::RotateRight)) toRotate.x = property::RotationSpeed;
        if (Input::GetKey(keybind::RotateLeft)) toRotate.x = -property::RotationSpeed;
        if (toRotate.x || toRotate.y) anchorTransHelper_->rotate(toRotate);
    }
    auto FreeCamera::updateRoll() -> void
    {
        using namespace user_config::freecam;

        if (Input::GetKey(keybind::RollLeft))
            anchorTransHelper_->roll(Input::GetKey(keybind::SpeedUp) ? -property::RollSpeed * 2 : -property::RollSpeed);
        if (Input::GetKey(keybind::RollRight))
            anchorTransHelper_->roll(Input::GetKey(keybind::SpeedUp) ? property::RollSpeed * 2 : property::RollSpeed);
        if (Input::GetKeyDown(keybind::ResetRoll)) anchorTransHelper_->resetRoll();
    }
}
