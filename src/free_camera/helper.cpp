#include "debug/logger.hpp"
#include "feature/free_camera.hpp"
#include "utype/unity_engine/physics.hpp"
#include "debug/test.hpp"

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
        origGObject->SetTag("Untagged");
    }
    bool FreeCamera::IsCurrentFreeCamera()
    {
        const auto curCam = UTYPE::Camera::GetMain();
        if (curCam == nullptr) return false;
        const auto curCamName = curCam->GetName()->ToString();
        if (curCamName == "UE_Freecam") return true;

        Debug::Logger::LOGD("Not UE_Freecam: {}", curCamName);
        // NOTE: This must be `Camera -> GameObject` -> Tag and not `Camera -> Tag`，otherwise wired bug can happen in some games.
        Debug::Logger::LOGD("Tag: {}", curCam->GetGameObject()->GetTag()->ToString());
        return false;
    }
}
