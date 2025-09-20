#include "freecam/camera_proxy.hpp"
#include "UnityResolve.hpp"
using UTYPE = UnityResolve::UnityType;
using UMethod = UnityResolve::Method;

template <typename T>
constexpr inline static T Clamp(T n, T bb, T bt)
{
    if (n < bb) return bb;
    if (n > bt) return bt;
    return n;
}

namespace FreeCam
{
    CameraProxy::CameraProxy(UTYPE::Camera *c)
    {
        camera = c;
        transform = camera->GetTransform();
        position = transform->GetPosition();
        const auto angles = transform->GetRotation().ToEuler();
        yaw = angles.y;
        pitch = angles.x;
        roll = angles.z;
    }
    bool CameraProxy::IsCurrentFreeCamera()
    {
        const auto curCam = UTYPE::Camera::GetMain();
        if (curCam == nullptr) return true;
        // const auto _n = std::wstring(curCam->GetName()->m_firstChar);
        // const auto name = std::string(_n.begin(), _n.end());
        // return name.starts_with("U_rea") || name == "UE_Freecam";
        return curCam->GetName()->ToString() == "UE_Freecam";
    }
    auto CameraProxy::Rotate(UTYPE::Vector2 input) -> void
    {
        yaw += input.x * rotationSpeed * FreeCam::DeltaTime_s;
        pitch -= input.y * rotationSpeed * FreeCam::DeltaTime_s;

        pitch = Clamp(pitch, -80.0f, 80.0f);
        const auto euler = UTYPE::Quaternion().Euler(pitch, yaw, roll);
        transform->SetRotation(euler);
    }
    auto CameraProxy::Move(UTYPE::Vector3 input, bool sprint = true) -> void
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

        position.y += move.y * speed * FreeCam::DeltaTime_s;
        position.z += move.z * speed * FreeCam::DeltaTime_s;
        position.x += move.x * speed * FreeCam::DeltaTime_s;
        transform->SetPosition(position);
    }
    auto CameraProxy::ZoomIn(float am) -> void
    {
        static UMethod *method;
        if (!method) method = UnityResolve::Get("UnityEngine.CoreModule.dll")->Get("Camera")->Get<UMethod>("set_fieldOfView");
        currentZoom -= am * zommSpeed * FreeCam::DeltaTime_s;
        return method->Invoke<void>(currentZoom);
    }
    auto CameraProxy::ZoomOut(float am) -> void
    {
        static UMethod *method;
        if (!method) method = UnityResolve::Get("UnityEngine.CoreModule.dll")->Get("Camera")->Get<UMethod>("get_fieldOfView");
        if (method->Invoke<float>() > defaultZoom) return;
        return ZoomIn(-am);
    }
    auto CameraProxy::ResetZoom() -> void
    {
        currentZoom = defaultZoom;
        return ZoomIn(0);
    }
}
