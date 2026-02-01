#include "debug/logger.hpp"
#include "feature/free_camera.hpp"
#include "utype/unity_engine/physics.hpp"

namespace FreeCam::Feature
{
    auto FreeCamera::SelectGameObject() -> UTYPE::Transform *
    {
        const auto screenCenter = UTYPE::Vector2(UTYPE::Screen::get_width() / 2.f, UTYPE::Screen::get_height() / 2.f);
        const auto cam = GetMaxDepthCamera();

        const auto ray = cam->ScreenPointToRay(screenCenter);
        const auto hit = std::make_unique<UTYPE::RaycastHit>();
        if (UTYPE::Physics::Raycast(ray, &*hit, 100.f))
        {
            const auto target = hit->get_collider()->GetGameObject();
            // if (const auto animator = target->GetComponentInChildren<UTYPE::Animator *>(UTYPE::Animator::GetUClass()))
            // {
            //     Debug::Logger::Debug("Found Animator");
            //     if (const auto head = animator->GetBoneTransform(UTYPE::Animator::HumanBodyBones::Head))
            //     {
            //         Debug::Logger::Debug("Found Head");
            //         return head;
            //     }
            // }
            return static_cast<UTYPE::Transform *>(target->GetTransform());
        }
        return nullptr;
    }

    auto FreeCamera::GetMaxDepthCamera() -> UTYPE::Camera *
    {
        UTYPE::Camera *max{};
        for (const auto cam : UTYPE::Camera::GetAllCamera())
        {
            if (!max)
                max = static_cast<UTYPE::Camera *>(cam);
            else if (max->GetDepth() < cam->GetDepth())
                max = static_cast<UTYPE::Camera *>(cam);
        }
        return max;
    }
    auto FreeCamera::backupOrigCamera() -> void
    {
        // Get original camera or return
        do
        {
            if ((origCamera = UTYPE::Camera::GetMain())) break;
            if ((origCamera = GetMaxDepthCamera())) break;
            if (!origCamera) return;
        } while (0);

        const auto origTransform = origCamera->GetTransform();
        if (const auto parent = origTransform->GetParent())
            origParent = static_cast<UTYPE::GameObject *>(parent->GetGameObject());
        origPosition = origTransform->GetPosition();
        origRotation = origTransform->GetRotation();
        origGObject = origCamera->GetGameObject();
        // if (Mode == Mode::MainCamera) origGObject->SetTag("Untagged");
        if (DisableOrigCam) origGObject->SetActive(false);
    }
    bool FreeCamera::IsCurrentFreeCamera()
    {
        const auto curCam = Mode == Mode::Depth ? GetMaxDepthCamera() : UTYPE::Camera::GetMain();
        if (curCam == nullptr) return false;
        const auto curCamName = curCam->GetName()->ToString();
        if (curCamName == "UE_Freecam") return true;

        Debug::Logger::Debug("Not UE_Freecam: {}", curCamName);
        // NOTE: This must be `Camera -> GameObject` -> Tag and not `Camera -> Tag`，otherwise wired bug can happen in some games.
        Debug::Logger::Debug("Tag: {}", curCam->GetGameObject()->GetTag()->ToString());
        return false;
    }
}
