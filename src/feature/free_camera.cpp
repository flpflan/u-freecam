#include "feature/free_camera.hpp"
#include "debug/logger.hpp"
#include "debug/test.hpp"
#include "proxy/camera.hpp"
#include "proxy/cursor.hpp"
#include "proxy/transform.hpp"
#include "utype/unity_engine/animation.hpp"
#include "utype/unity_engine/core.hpp"
#include "utype/unity_engine/input.hpp"
#include "utype/unity_engine/physics.hpp"
#include <memory>

using enum UTYPE::KeyCode;
using UTYPE::Input;

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
    auto FreeCamera::selectGameObject() -> UTYPE::Transform *
    {
        const auto screenCenter = UTYPE::Vector2(UTYPE::Screen::get_width() / 2.f, UTYPE::Screen::get_height() / 2.f);

        const auto ray = UTYPE::Camera::GetMain()->ScreenPointToRay(screenCenter);
        const auto hit = std::make_unique<UTYPE::RaycastHit>();
        if (UTYPE::Physics::Raycast(ray, &*hit, 100.f))
        {
            const auto target = hit->get_collider()->GetGameObject();
            // if (const auto animator = target->GetComponentInChildren<UTYPE::Animator *>(UTYPE::Animator::GetUClass()))
            // {
            //     Debug::Logger::LOGD("Found Animator");
            //     if (const auto head = animator->GetBoneTransform(UTYPE::Animator::HumanBodyBones::Head))
            //     {
            //         Debug::Logger::LOGD("Found Head");
            //         return head;
            //     }
            // }
            return static_cast<UTYPE::Transform *>(target->GetTransform());
        }
        return nullptr;
    }
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
        anchorGO = UTYPE::GameObject::Create("UE_Freecam_Anchor");
        anchorGO->SetActive(true);
        UTYPE::GameObject::DontDestroyOnLoad(anchorGO);

        freeGO = UTYPE::GameObject::Create("UE_Freecam");
        UTYPE::GameObject::DontDestroyOnLoad(freeGO);
        static_cast<UTYPE::Transform *>(freeGO->GetTransform())->SetParent(anchorGO->GetTransform());
        freeGO->SetTag("MainCamera");
        freeGO->SetActive(true);

        anchorTrans = std::make_unique<Proxy::Transform>(anchorGO->GetTransform());
        freeTrans = std::make_unique<Proxy::Transform>(freeGO->GetTransform());
        freeCam = std::make_unique<Proxy::Camera>(freeGO->AddComponent<UTYPE::Camera *>(UTYPE::Camera::GetUClass()));

        anchorTrans->CopyState(*origGObject->GetTransform());
        freeTrans->SetLocalPosition(UTYPE::Vector3(0, 0, 0));
        freeTrans->SetLocalRotation(UTYPE::Quaternion(0, 0, 0, 1));
        freeTrans->SetLocalScale(UTYPE::Vector3(1, 1, 1));

        // Set Cursor
        Proxy::Cursor::DisableCursor();
    }
    auto FreeCamera::Disable() -> void
    {
        if (!Enabled) return;
        Enabled = false;
        Debug::Logger::LOGI("End freecam");
        if (!freeGO->IsDestoryed())
        {
            UTYPE::GameObject::Destroy(freeGO);
        }
        if (!anchorGO->IsDestoryed())
        {
            UTYPE::GameObject::Destroy(anchorGO);
        }
        if (!origGObject->IsDestoryed())
        {
            // if (FreeCamera::IsCurrentFreeCamera())
            // {
                origGObject->SetActive(true);
                origGObject->SetTag("MainCamera");
                const auto originTransform = origCamera->GetTransform();
                originTransform->SetPosition(origPosition);
                originTransform->SetRotation(origRotation);
            // }
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
                if (Input::GetKey(X)) freeCam->ZoomIn(1);
                if (Input::GetKey(C)) freeCam->ZoomOut(1);
                const float mouseCenter = Input::GetAxis("Mouse ScrollWheel");
                if (mouseCenter < 0)
                {
                    freeCam->ZoomOut(10);
                }
                else if (mouseCenter > 0)
                {
                    freeCam->ZoomIn(10);
                };
            }

            UTYPE::Vector3 toMove(0, 0, 0);
            // toMove.x = UnityApi::GetAxis("Horizontal");
            // toMove.y = UnityApi::GetAxis("Vertical");
            if (Input::GetKey(Space)) toMove.z = moveSpeed;
            if (Input::GetKey(Ctrl_L)) toMove.z = -moveSpeed;
            if (Input::GetKey(W)) toMove.y = moveSpeed;
            if (Input::GetKey(S)) toMove.y = -moveSpeed;
            if (Input::GetKey(A)) toMove.x = -moveSpeed;
            if (Input::GetKey(D)) toMove.x = moveSpeed;
            if (toMove.x || toMove.y || toMove.z)
            {
                if (Input::GetKey(M))
                {
                    freeTrans->Move(Input::GetKey(SHIFT_L) ? toMove * moveSpeedMultiplier : toMove);
                }
                else
                {
                    if (!attach_mode)
                    {
                        anchorTrans->Move(Input::GetKey(SHIFT_L) ? toMove * moveSpeedMultiplier : toMove);
                    }
                    else
                    {
                        freeTrans->Move(Input::GetKey(SHIFT_L) ? toMove * moveSpeedMultiplier : toMove);
                    }
                }
            }
            if (Input::GetKeyDown(M) && Input::GetKey(SHIFT_L)) freeTrans->SetLocalPosition(UTYPE::Vector3(0, 0, 0));

            UTYPE::Vector2 toRotate(0, 0);
            toRotate.x = Input::GetAxis("Mouse X");
            toRotate.y = Input::GetAxis("Mouse Y");
            if (Input::GetKey(UpArrow)) toRotate.y = rotationSpeed;
            if (Input::GetKey(DownArrow)) toRotate.y = -rotationSpeed;
            if (Input::GetKey(RightArrow)) toRotate.x = rotationSpeed;
            if (Input::GetKey(LeftArrow)) toRotate.x = -rotationSpeed;
            if (toRotate.x || toRotate.y) anchorTrans->Rotate(toRotate);

            if (Input::GetKey(Q)) anchorTrans->Roll(Input::GetKey(SHIFT_L) ? -rollSpeed * 2 : -rollSpeed);
            if (Input::GetKey(E)) anchorTrans->Roll(Input::GetKey(SHIFT_L) ? rollSpeed * 2 : rollSpeed);
            if (Input::GetKeyDown(R)) anchorTrans->ResetRoll();

            if (attach_mode)
            {
                if (const auto parent = anchorTrans->GetParent(); !parent || !parent->GetGameObject())
                {
                    Debug::Logger::LOGD("Parent Destroyed");
                    anchorTrans->SetParent(nullptr);
                    attach_mode = false;
                }
            }
            if (Input::GetKeyDown(T))
            {
                if (!attach_mode && anchorTrans->GetParent() == nullptr)
                {
                    const auto target = selectGameObject();
                    if (target)
                    {
                        anchorTrans->SetParent(target);
                        anchorTrans->SetLocalPosition(UTYPE::Vector3(0, 0, 0));
                        anchorTrans->SetLocalRotation(UTYPE::Quaternion(0, 0, 0, 1));
                        anchorTrans->SetLocalScale(UTYPE::Vector3(1, 1, 1));
                        freeTrans->SetLocalPosition(UTYPE::Vector3(0, 0, 0));
                        freeTrans->SetLocalRotation(UTYPE::Quaternion(0, 0, 0, 1));
                        freeTrans->SetLocalScale(UTYPE::Vector3(1, 1, 1));
                        Debug::Logger::LOGI("Anchor attached to GameObject: {}", target->GetName()->ToString());
                        attach_mode = true;
                    }
                    else
                    {
                        Debug::Logger::LOGI("Target not found");
                    }
                }
                else
                {
                    anchorTrans->SetParent(nullptr);
                    anchorTrans->SetLocalScale(UTYPE::Vector3(1, 1, 1));
                    freeTrans->SetLocalPosition(UTYPE::Vector3(0, 0, 0));
                    freeTrans->SetLocalRotation(UTYPE::Quaternion(0, 0, 0, 1));
                    freeTrans->SetLocalScale(UTYPE::Vector3(1, 1, 1));
                    Debug::Logger::LOGI("Anchor detached");
                    attach_mode = false;
                }
            }
        }
        if (Input::GetMouseButtonDown(2) || Input::GetKeyDown(U))
        {
            Proxy::Cursor::ToggleCursor();
            ui_layer = !ui_layer;
        }
    }

}
