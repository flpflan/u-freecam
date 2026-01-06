#include "debug/logger.hpp"
#include "feature/free_camera.hpp"
#include "utype/unity_engine/physics.hpp"

namespace FreeCam::Feature
{
    auto FreeCamera::selectGameObject() -> UTYPE::Transform *
    {
        const auto screenCenter = UTYPE::Vector2(UTYPE::Screen::get_width() / 2.f, UTYPE::Screen::get_height() / 2.f);
        const auto cam = freeCam->GetUCamera();

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

    auto FreeCamera::getMaxDepthCamera() -> UTYPE::Camera *
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
        origCamera = getMaxDepthCamera();
        if (!origCamera) return;
        const auto origTransform = origCamera->GetTransform();
        origParent = static_cast<UTYPE::GameObject *>(origTransform->GetParent()->GetGameObject());
        origPosition = origTransform->GetPosition();
        origRotation = origTransform->GetRotation();
        origGObject = origCamera->GetGameObject();
        origGObject->SetActive(false);
        if (Mode == Mode::MainCamera) origGObject->SetTag("Untagged");
    }
    bool FreeCamera::IsCurrentFreeCamera()
    {
        const auto curCam = Mode == Mode::Depth ? getMaxDepthCamera() : UTYPE::Camera::GetMain();
        if (curCam == nullptr) return false;
        const auto curCamName = curCam->GetName()->ToString();
        if (curCamName == "UE_Freecam") return true;

        Debug::Logger::Debug("Not UE_Freecam: {}", curCamName);
        // NOTE: This must be `Camera -> GameObject` -> Tag and not `Camera -> Tag`，otherwise wired bug can happen in some games.
        Debug::Logger::Debug("Tag: {}", curCam->GetGameObject()->GetTag()->ToString());
        return false;
    }
}
