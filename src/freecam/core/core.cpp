#include "freecam/freecam.hpp"
#include "freecam/helper.hpp"

#include "umod/debug/logger.hpp"
#include "umod/runtime/helper/cursor.hpp"
#include "umod/utype/unity_engine/core.hpp"
#include "user/config.hpp"

using namespace umod::UTYPE::unity_engine;
using namespace umod::unity_runtime::helper;
using namespace umod::debug;

namespace freecam
{
    namespace
    {
        static auto createOrignalCamera(Camera *origCamera)
        {
            return static_cast<Camera *>(UnityObject::Instantiate(origCamera));
        }
        static auto createDepthCamera()
        {
            const auto go = GameObject::Create(kFreeCameraName);
            const auto camera = go->AddComponent<Camera *>(Camera::GetUClass());
            camera->SetDepth(999);
            return camera;
        }
        static auto createMainCamera()
        {
            const auto go = GameObject::Create(kFreeCameraName);
            go->SetTag("MainCamera");
            return go->AddComponent<Camera *>(Camera::GetUClass());
        }

        static Transform *createAnchor() { return GameObject::Create(kFreeArchorName)->GetTransform(); }
    }

    auto FreeCamera::createCamera(Mode mode) -> Camera *
    {
        if (mode == Mode::Orignal)
            return createOrignalCamera(origCamera_);
        else if (mode == Mode::Depth)
            return createDepthCamera();
        else if (mode == Mode::MainCamera)
            return createMainCamera();
        [[unlikely]] return nullptr;
    }

    auto FreeCamera::enable() -> void
    {

        logger::info("Start freecam");

        // Backup
        backupOrigCamera();

        // Setup free camera
        anchorTrans_ = createAnchor();
        GameObject::DontDestroyOnLoad(anchorTrans_);

        freeCam_ = createCamera(user_config::freecam::Mode);
        cameraHelper_ = std::make_unique<CameraHelper>(*freeCam_);
        GameObject::DontDestroyOnLoad(freeCam_->GetGameObject());
        static_cast<Transform *>(freeCam_->GetTransform())->SetParent(anchorTrans_);

        anchorTransHelper_ = std::make_unique<TransformHelper>(anchorTrans_);
        freeTransHelper_ = std::make_unique<TransformHelper>(static_cast<Transform *>(freeCam_->GetTransform()));

        if (origCamera_) anchorTransHelper_->copyState(*static_cast<Transform *>(origCamera_->GetTransform()));
        freeTransHelper_->setLocalPosition({0, 0, 0});
        freeTransHelper_->setLocalRotation({0, 0, 0, 1});
        freeTransHelper_->setLocalScale({1, 1, 1});

        // Set Cursor
#ifndef __ANDROID__
        CursorUtils::backup();
#endif
    }

    auto FreeCamera::disable() -> void
    {
        logger::info("End freecam");
        if (const auto freeGO = freeCam_->GetGameObject(); freeGO && !freeGO->IsDestoryed())
        {
            GameObject::Destroy(freeGO);
        }
        if (const auto anchorGO = static_cast<GameObject *>(anchorTrans_->GetGameObject());
            anchorGO && !anchorGO->IsDestoryed())
        {
            GameObject::Destroy(anchorGO);
        }
        if (const auto origGO = origCamera_->GetGameObject(); origGO && !origGO->IsDestoryed())
        {
            // if (FreeCamera::IsCurrentFreeCamera())
            // {
            // if (origParent && !origParent->IsDestoryed())
            //     origGObject->GetTransform()->SetParent(origParent->GetTransform());
            if (user_config::freecam::DisableOrigCam) origGO->SetActive(true);
            // if (Mode == Mode::MainCamera)
            // {
            // origGObject->SetTag("MainCamera");
            // const auto originTransform = origCamera->GetTransform();
            // originTransform->SetPosition(origPosition);
            // originTransform->SetRotation(origRotation);
            // }
            // }
        }
#ifndef __ANDROID__
        CursorUtils::resume();
#endif
    }

    auto FreeCamera::update() -> void
    {
        // TODO: Disabled by guard
        // if (Input::GetKeyDown(F))
        //     suspend();
        // else if (Input::GetKeyUp(F)) // FIXME: GetKeyUp / GetKey may crash
        //     resume();

        if (!kFlags.ui_layer)
        {
            updateZoom();
            updateMove();
            updateRotate();
            updateRoll();
            updateAttachMode();
        }
#ifndef __ANDROID__
        if (InputUtils::GetMouseButtonDown(2) || InputUtils::GetKeyDown(user_config::freecam::keybind::UIMode))
        {
            CursorUtils::toggleCursor();
            kFlags.ui_layer = !kFlags.ui_layer;
        }
#endif
    }

    auto FreeCamera::backupOrigCamera() -> void
    {

        // Get original camera or return
        do
        {
            if ((origCamera_ = Camera::GetMain())) break;
            if ((origCamera_ = helper::getMaxDepthCamera())) break;
            if (!origCamera_) return;
        } while (0);

        const auto origTransform = origCamera_->GetTransform();
        // if (const auto parent = origTransform->GetParent())
        //     origParent = static_cast<UTYPE::GameObject *>(parent->GetGameObject());
        origPosition_ = origTransform->GetPosition();
        origRotation_ = origTransform->GetRotation();
        // if (Mode == Mode::MainCamera) origGObject->SetTag("Untagged");
        if (user_config::freecam::DisableOrigCam) origCamera_->GetGameObject()->SetActive(false);
    }

    // // TODO: merge enable/resume, disable/suspend
    // auto FreeCamera::suspend() -> void
    // {
    //     if (!freeGO->IsDestoryed())
    //     {
    //         freeGO->SetActive(false);
    //         if (Mode == Mode::MainCamera)
    //         {
    //             freeGO->SetTag("Untagged");
    //         }
    //     }
    //     if (!origGObject->IsDestoryed())
    //     {
    //         origGObject->SetActive(true);
    //         if (Mode == Mode::MainCamera)
    //         {
    //             origGObject->SetTag("MainCamera");
    //         }
    //     }
    // }
    // auto FreeCamera::resume() -> void
    // {
    //     if (!freeGO->IsDestoryed())
    //     {
    //         freeGO->SetActive(true);
    //         if (Mode == Mode::MainCamera)
    //         {
    //             freeGO->SetTag("MainCamera");
    //         }
    //         if (!origGObject->IsDestoryed())
    //         {
    //             origGObject->SetActive(false);
    //             if (Mode == Mode::MainCamera)
    //             {
    //                 origGObject->SetTag("Untagged");
    //             }
    //         }
    //     }
    // }
}
