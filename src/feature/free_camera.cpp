#include "feature/free_camera.hpp"
#include "UnityResolve.hpp"
#include "debug/logger.hpp"
#include "proxy/camera.hpp"
#include "proxy/cursor.hpp"
#include "utype/input.hpp"

using enum UType::KeyCode;
using UTYPE = UnityResolve::UnityType;

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
        if (!freeCamera)
        {
            const auto go = UTYPE::GameObject::Create("UE_Freecam");
            freeGObject = go;
            UTYPE::GameObject::DontDestroyOnLoad(go);

            const auto c = go->AddComponent<UType::Camera *>(UType::Camera::GetUClass());
            freeCamera = new Proxy::Camera(c);
        }
        freeGObject->SetTag("MainCamera");
        freeGObject->SetActive(true);
        freeGObject->GetTransform()->SetPosition(origPosition);
        freeGObject->GetTransform()->SetRotation(origRotation);

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
        if (freeCamera)
        {
            freeGObject->SetActive(false);
            const auto freeTransform = freeCamera->GetTransform();
            freeTransform->SetPosition(origPosition);
            freeTransform->SetRotation(origRotation);
            freeGObject->SetTag("Bulabula");
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
            if (toMove.x || toMove.y || toMove.z) freeCamera->Move(toMove, Input::GetKey(SHIFT_L));

            UTYPE::Vector2 toRotate(0, 0);
            toRotate.x = Input::GetAxis("Mouse X");
            toRotate.y = Input::GetAxis("Mouse Y");
            if (Input::GetKey(UpArrow)) toRotate.y = 1;
            if (Input::GetKey(DownArrow)) toRotate.y = -1;
            if (Input::GetKey(RightArrow)) toRotate.x = 1;
            if (Input::GetKey(LeftArrow)) toRotate.x = -1;
            if (toRotate.x || toRotate.y) freeCamera->Rotate(toRotate);
        }
        if (Input::GetMouseButtonDown(2) || Input::GetKeyDown(U))
        {
            Proxy::Cursor::ToggleCursor();
            ui_layer = !ui_layer;
        }
    }
}
