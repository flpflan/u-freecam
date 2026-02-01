#include "feature/free_camera.hpp"
#include "utype/unity_engine/input.hpp"

using enum UTYPE::KeyCode;
using UTYPE::Input;

namespace FreeCam::Feature
{
    auto FreeCamera::updateRotate() -> void
    {
        UTYPE::Vector2 toRotate(0, 0);
        toRotate.x = Input::GetAxis("Mouse X") * rotationSpeed;
        toRotate.y = Input::GetAxis("Mouse Y") * rotationSpeed;
        if (Input::GetKey(UpArrow)) toRotate.y = rotationSpeed;
        if (Input::GetKey(DownArrow)) toRotate.y = -rotationSpeed;
        if (Input::GetKey(RightArrow)) toRotate.x = rotationSpeed;
        if (Input::GetKey(LeftArrow)) toRotate.x = -rotationSpeed;
        if (toRotate.x || toRotate.y) anchorTrans->Rotate(toRotate);
    }
    auto FreeCamera::updateRoll() -> void
    {
        if (Input::GetKey(Q)) anchorTrans->Roll(Input::GetKey(SHIFT_L) ? -rollSpeed * 2 : -rollSpeed);
        if (Input::GetKey(E)) anchorTrans->Roll(Input::GetKey(SHIFT_L) ? rollSpeed * 2 : rollSpeed);
        if (Input::GetKeyDown(R)) anchorTrans->ResetRoll();
    }
}
