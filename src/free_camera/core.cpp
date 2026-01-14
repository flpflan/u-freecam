#include "debug/logger.hpp"
#include "feature/free_camera.hpp"
#include "proxy/cursor.hpp"
#include "utype/unity_engine/input.hpp"

using enum UTYPE::KeyCode;
using UTYPE::Input;

namespace FreeCam::Feature
{
    auto FreeCamera::Enable() -> void
    {
        if (Enabled) return;
        Enabled = true;
        Debug::Logger::Info("Start freecam");

        // Backup
        backupOrigCamera();

        // Setup free camera
        anchorGO = UTYPE::GameObject::Create("UE_Freecam_Anchor");
        anchorGO->SetActive(true);
        UTYPE::GameObject::DontDestroyOnLoad(anchorGO);

        freeGO = UTYPE::GameObject::Create("UE_Freecam");
        UTYPE::GameObject::DontDestroyOnLoad(freeGO);
        static_cast<UTYPE::Transform *>(freeGO->GetTransform())->SetParent(anchorGO->GetTransform());
        // freeGO->SetTag("MainCamera");
        freeGO->SetActive(true);

        anchorTrans = std::make_unique<Proxy::Transform>(anchorGO->GetTransform());
        freeTrans = std::make_unique<Proxy::Transform>(freeGO->GetTransform());
        freeCam = std::make_unique<Proxy::Camera>(freeGO->AddComponent<UTYPE::Camera *>(UTYPE::Camera::GetUClass()));

        if (origGObject) anchorTrans->CopyState(*origGObject->GetTransform());
        freeTrans->SetLocalPosition({0, 0, 0});
        freeTrans->SetLocalRotation({0, 0, 0, 1});
        freeTrans->SetLocalScale({1, 1, 1});

        if (Mode == Mode::Depth)
            freeCam->GetUCamera()->SetDepth(999);
        else
            freeGO->SetTag("MainCamera");

        // Attach orignal camera to free camera
        // origGObject->GetTransform()->SetParent(freeGO->GetTransform());

        // Set Cursor
        Proxy::Cursor::DisableCursor();

        // Init Status
        ui_layer = false;
        zoom_mode = false;
        attach_mode = false;
    }
    auto FreeCamera::Disable() -> void
    {
        if (!Enabled) return;
        Enabled = false;
        Debug::Logger::Info("End freecam");
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
            if (origParent && !origParent->IsDestoryed()) origGObject->GetTransform()->SetParent(origParent->GetTransform());
            origGObject->SetActive(true);
            // origGObject->SetTag("MainCamera");
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
            updateZoom();
            updateMove();
            updateRotate();
            updateRoll();
            updateAttachMode();
        }
        if (Input::GetMouseButtonDown(2) || Input::GetKeyDown(U))
        {
            Proxy::Cursor::ToggleCursor();
            ui_layer = !ui_layer;
        }
    }
}
