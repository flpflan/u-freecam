#include "feature/free_camera.hpp"
#include "debug/logger.hpp"
#include "proxy/camera.hpp"
#include "proxy/cursor.hpp"
#include "proxy/transform.hpp"
#include "utype/input.hpp"
#include "utype/transform.hpp"
#include <memory>

using UTYPE = UnityResolve::UnityType;
using enum UType::KeyCode;
using UType::Input;

// template <typename T>
// T Abs(T n)
// {
//     return n >= 0 ? n : -n;
// }
// float getPinchDelta()
// {
//     static float pinchDelta = 0.f;
//     static float lastDistance = 0.f;
//     static int lastCount = 0;
//     pinchDelta = 0.f;
//     if (Input::GetTouchCount() != 2)
//     {
//         lastCount = 0;
//         return pinchDelta;
//     }
//
//     auto t0 = Input::GetTouch(0);
//     auto t1 = Input::GetTouch(1);
//
//     float currentDistance = t0.GetPosition().Distance(t1.GetPosition());
//
//     if (lastCount != 2)
//     {
//         lastDistance = currentDistance;
//     }
//     else
//     {
//         pinchDelta = currentDistance - lastDistance;
//         lastDistance = currentDistance;
//     }
//     lastCount = 2;
//     return pinchDelta;
// }

namespace FreeCam::Feature
{
    auto FreeCamera::resetState() -> void { ui_layer = false; }
    auto FreeCamera::backupOrigCamera() -> void
    {
        origCamera = UTYPE::Camera::GetMain();
        const auto origTransform = origCamera->GetTransform();
        origPosition = origTransform->GetPosition();
        origRotation = origTransform->GetRotation();
        origGObject = origCamera->GetGameObject();
        origGObject->SetActive(false);
    }
    bool FreeCamera::IsCurrentFreeCamera()
    {
        const auto curCam = UTYPE::Camera::GetMain();
        if (curCam == nullptr) return true;
        // const auto _n = std::wstring(curCam->GetName()->m_firstChar);
        // const auto name = std::string(_n.begin(), _n.end());
        // return name.starts_with("U_rea") || name == "UE_Freecam";
        return curCam->GetName()->ToString() == "UE_Freecam";
    }
    auto FreeCamera::Enable() -> void
    {
        if (Enabled) return;
        Enabled = true;
        Debug::Logger::LOGI("Start freecam");

        // Backup
        backupOrigCamera();

        // Setup free camera
        // if (!freeCamera)
        //{
        anchorGO = UTYPE::GameObject::Create("UE_Freecam_Anchor");
        anchorGO->SetActive(true);

        freeGO = UTYPE::GameObject::Create("UE_Freecam");
        static_cast<UType::Transform *>(freeGO->GetTransform())->SetParent(anchorGO->GetTransform());
        freeGO->SetTag("MainCamera");
        freeGO->SetActive(true);
        // UTYPE::GameObject::DontDestroyOnLoad(go);

        anchorTrans = std::make_unique<Proxy::Transform>(anchorGO->GetTransform());
        freeTrans = std::make_unique<Proxy::Transform>(freeGO->GetTransform());
        freeCam = std::make_unique<Proxy::Camera>(freeGO->AddComponent<UType::Camera *>(UType::Camera::GetUClass()));
        //}
        anchorTrans->CopyState(*origGObject->GetTransform());
        freeTrans->CopyState(*origGObject->GetTransform());
        freeTrans->SetLocalPosition(UTYPE::Vector3(0, 0, 0));
        freeTrans->SetLocalRotation(UTYPE::Quaternion(0, 0, 0, 0));

        // Set Cursor
        Proxy::Cursor::DisableCursor();

        // Reset Self State
        resetState();
    }
    auto FreeCamera::Disable() -> void
    {
        if (!Enabled) return;
        Enabled = false;
        Debug::Logger::LOGI("End freecam");
        if (freeCam)
        {
            // freeGO->SetActive(false);
            // freeGO->SetTag("Bulabula");
            anchorGO->SetActive(false);
            anchorTrans->CopyState(*origGObject->GetTransform());
        }
        if (origGObject)
        {
            const auto curCam = UTYPE::Camera::GetMain();
            if (FreeCamera::IsCurrentFreeCamera())
            {
                origGObject->SetActive(true);
                const auto originTransform = origCamera->GetTransform();
                originTransform->SetPosition(origPosition);
                originTransform->SetRotation(origRotation);
            }
        }
        Proxy::Cursor::EnableCursor();
    }
    auto FreeCamera::Update() -> void
    {
        if (!ui_layer)
        {
            const bool toZoom = Input::GetKey(Z);
            if (!toZoom && zoom_mode) freeCam->ResetZoom();
            zoom_mode = toZoom;
            if (zoom_mode)
            {
#ifdef __ANDROID__
                // float delta = getPinchDelta();
                // if (Abs(delta) > 0.01f) freeCamera->ZoomIn(delta);
                if (Input::GetKey(X)) freeCam->ZoomIn(1);
                if (Input::GetKey(C)) freeCam->ZoomOut(1);
#else
                const float mouseCenter = Input::GetAxis("Mouse ScrollWheel");
                if (mouseCenter < 0)
                {
                    freeCamera->ZoomOut(-mouseCenter);
                }
                else
                {
                    freeCamera->ZoomIn(mouseCenter);
                };
#endif
            }

            UTYPE::Vector3 toMove(0, 0, 0);
            // toMove.x = UnityApi::GetAxis("Horizontal");
            // toMove.y = UnityApi::GetAxis("Vertical");
            if (Input::GetKey(Space)) toMove.z = 1;
            if (Input::GetKey(Ctrl_L)) toMove.z = -1;
            if (Input::GetKey(W)) toMove.y = 1;
            if (Input::GetKey(S)) toMove.y = -1;
            if (Input::GetKey(A)) toMove.x = -1;
            if (Input::GetKey(D)) toMove.x = 1;
            if (toMove.x || toMove.y || toMove.z)
            {
                if (Input::GetKey(M))
                {
                    freeTrans->Move(toMove, Input::GetKey(SHIFT_L));
                }
                else
                {
                    anchorTrans->Move(toMove, Input::GetKey(SHIFT_L));
                }
            }
            if (Input::GetKeyDown(M) && Input::GetKey(SHIFT_L)) freeTrans->SetLocalPosition(UTYPE::Vector3(0, 0, 0));

            UTYPE::Vector2 toRotate(0, 0);
            toRotate.x = Input::GetAxis("Mouse X");
            toRotate.y = Input::GetAxis("Mouse Y");
            if (Input::GetKey(UpArrow)) toRotate.y = 1;
            if (Input::GetKey(DownArrow)) toRotate.y = -1;
            if (Input::GetKey(RightArrow)) toRotate.x = 1;
            if (Input::GetKey(LeftArrow)) toRotate.x = -1;
            if (toRotate.x || toRotate.y) anchorTrans->Rotate(toRotate);
        }
        if (Input::GetMouseButtonDown(2) || Input::GetKeyDown(U))
        {
            Proxy::Cursor::ToggleCursor();
            ui_layer = !ui_layer;
        }
    }

}
