#include "proxy/camera.hpp"
#include "core.hpp"
#include "proxy/cursor.hpp"
#include "utype/input.hpp"

template <typename T>
constexpr inline static T Clamp(T n, T bb, T bt)
{
    if (n < bb) return bb;
    if (n > bt) return bt;
    return n;
}

namespace FreeCam::Proxy
{
    auto Camera::Enable() -> void { ui_layer = false; }
    auto Camera::Disable() -> void { ui_layer = false; }
    auto Camera::Update() -> void
    {
        using UType::Input;
        using enum UType::KeyCode;
        if (!ui_layer)
        {
            // const bool toZoom = Input::GetKey(Z);
            // if (!toZoom && zoom_mode) camera.ResetZoom();
            // zoom_mode = toZoom;
            // if (zoom_mode)
            // {
            //     const float mouseCenter = Input::GetAxis("Mouse ScrollWheel");
            //     if (mouseCenter < 0)
            //     {
            //         camera.ZoomOut(mouseCenter);
            //     }
            //     else
            //     {
            //         camera.ZoomIn(mouseCenter);
            //     };
            // }

            UTYPE::Vector3 toMove(0, 0, 0);
            // toMove.x = UnityApi::GetAxis("Horizontal");
            // toMove.y = UnityApi::GetAxis("Vertical");
            if (Input::GetKey(Space)) toMove.z = 1;
            if (Input::GetKey(Ctrl_L)) toMove.z = -1;
            if (Input::GetKey(W)) toMove.y = 1;
            if (Input::GetKey(S)) toMove.y = -1;
            if (Input::GetKey(A)) toMove.x = -1;
            if (Input::GetKey(D)) toMove.x = 1;
            if (toMove.x || toMove.y || toMove.z) Move(toMove, Input::GetKey(SHIFT_L));

            UTYPE::Vector2 toRotate(0, 0);
            toRotate.x = Input::GetAxis("Mouse X");
            toRotate.y = Input::GetAxis("Mouse Y");
            if (Input::GetKey(UpArrow)) toRotate.y = 1;
            if (Input::GetKey(DownArrow)) toRotate.y = -1;
            if (Input::GetKey(RightArrow)) toRotate.x = 1;
            if (Input::GetKey(LeftArrow)) toRotate.x = -1;
            if (toRotate.x || toRotate.y) Rotate(toRotate);
        }
        if (Input::GetMouseButtonDown(2) || Input::GetKeyDown(U))
        {
            Cursor::ToggleCursor();
            ui_layer = !ui_layer;
        }
    }
    Camera::Camera(UType::Camera *c)
    {
        camera = c;
        transform = camera->GetTransform();
        position = transform->GetPosition();
        const auto angles = transform->GetRotation().ToEuler();
        yaw = angles.y;
        pitch = angles.x;
        roll = angles.z;
    }
    bool Camera::IsCurrentFreeCamera()
    {
        const auto curCam = UTYPE::Camera::GetMain();
        if (curCam == nullptr) return true;
        // const auto _n = std::wstring(curCam->GetName()->m_firstChar);
        // const auto name = std::string(_n.begin(), _n.end());
        // return name.starts_with("U_rea") || name == "UE_Freecam";
        return curCam->GetName()->ToString() == "UE_Freecam";
    }
    auto Camera::Rotate(UTYPE::Vector2 input) -> void
    {
        yaw += input.x * rotationSpeed * Core::DeltaTime_s;
        pitch -= input.y * rotationSpeed * Core::DeltaTime_s;

        pitch = Clamp(pitch, -80.0f, 80.0f);
        const auto euler = UTYPE::Quaternion().Euler(pitch, yaw, roll);
        transform->SetRotation(euler);
    }
    auto Camera::Move(UTYPE::Vector3 input, bool sprint = true) -> void
    {
        const auto speed = sprint ? moveSpeed * moveSpeedMultiplier : moveSpeed;

        const auto forward = transform->GetForward();
        const auto right = transform->GetRight();
        const auto up = transform->GetUp();
        // forward.y = 0;
        // right.y = 0;
        // forward.Normalize();
        // right.Normalize();

        // clang-format off
        const auto move = UTYPE::Vector3(right.x * input.x + forward.x * input.y + up.x * input.z,
                                         right.y * input.x + forward.y * input.y + up.y * input.z,
                                         right.z * input.x + forward.z * input.y + up.z * input.z);
        // clang-format on

        position.y += move.y * speed * Core::DeltaTime_s;
        position.z += move.z * speed * Core::DeltaTime_s;
        position.x += move.x * speed * Core::DeltaTime_s;
        transform->SetPosition(position);
    }
    // auto Camera::ZoomIn(float am) -> void
    // {
    //     currentZoom -= am * zommSpeed * Core::DeltaTime_s;
    //     currentZoom = Clamp(currentZoom, 1.f, 179.f);
    //     fieldOfView = currentZoom;
    // }
    // auto Camera::ZoomOut(float am) -> void
    // {
    //     if (fieldOfView > defaultZoom) return;
    //     return ZoomIn(-am);
    // }
    // auto Camera::ResetZoom() -> void
    // {
    //     currentZoom = defaultZoom;
    //     return ZoomIn(0);
    // }
}
