#include "freecam/freecam.hpp"
#include "freecam/helper.hpp"

#include "umod/debug/logger.hpp"
#include "umod/utype/unity_engine/input.hpp"

#include "user/config.hpp"

using namespace umod::UTYPE::unity_engine;
using namespace umod::debug;

using namespace user::config::freecam;
using enum user::config::freecam::keybind::KeyCode;

namespace freecam
{
    auto FreeCamera::enterAttachMode(Transform *target) -> void
    {
        anchorTrans_->SetParent(target);
        anchorTrans_->SetLocalPosition(Vector3(0, 0, 0));
        anchorTrans_->SetLocalRotation(Quaternion(0, 0, 0, 1));
        anchorTrans_->SetLocalScale(Vector3(1, 1, 1));
        freeTransHelper_->setLocalPosition(Vector3(0, 0, 0));
        freeTransHelper_->setLocalRotation(Quaternion(0, 0, 0, 1));
        freeTransHelper_->setLocalScale(Vector3(1, 1, 1));
        logger::info("Anchor attached to GameObject: {}", target->GetName()->ToString());
        kFlags.attach_mode = true;
    }

    auto FreeCamera::exitAttachMode() -> void
    {
        anchorTrans_->SetParent(nullptr);
        anchorTrans_->SetLocalScale(Vector3(1, 1, 1));
        freeTransHelper_->setLocalPosition(Vector3(0, 0, 0));
        freeTransHelper_->setLocalRotation(Quaternion(0, 0, 0, 1));
        freeTransHelper_->setLocalScale(Vector3(1, 1, 1));
        logger::info("Anchor detached");
        kFlags.attach_mode = false;
    }

    auto FreeCamera::updateAttachMode() -> void
    {
        if (kFlags.attach_mode)
        {
            // FIXME:
            // If parent got destroyed，free camera GameObject would be destroyed too.
            // Then we will never enter this branch since FreeCamera would be disabled by the guard.
            // If free camera is the only camera exists in the scene, after being destroyed, IsCurrentFreeCamera return false since there is no camera at all.
            const auto destroyed = [&]
            {
                const auto parent = anchorTrans_->GetParent();
                if (!parent) return true;
                const auto parentGo = static_cast<GameObject *>(parent->GetGameObject());
                if (!parentGo) return true;
                return parentGo->IsDestoryed();
            }();
            if (destroyed)
            {
                logger::debug("Attach mode parent was destroyed");
                exitAttachMode();
            }
        }
        if (Input::GetKeyDown(keybind::AttachMode))
        {
            if (!kFlags.attach_mode && anchorTrans_->GetParent() == nullptr)
            {
                if (const auto target = helper::selectGameObject())
                    enterAttachMode(target);
                else
                    logger::info("Target not found");
            }
            else
            {
                exitAttachMode();
            }
        }
    }
}
