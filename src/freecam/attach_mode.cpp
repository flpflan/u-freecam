#include "debug/logger.hpp"
#include "feature/free_camera.hpp"
#include "utype/unity_engine/input.hpp"

using enum UTYPE::KeyCode;
using UTYPE::Input;

namespace FreeCam::Feature
{
    auto FreeCamera::enterAttachMode(UTYPE::Transform *target) -> void
    {
        anchorTrans->SetParent(target);
        anchorTrans->SetLocalPosition(UTYPE::Vector3(0, 0, 0));
        anchorTrans->SetLocalRotation(UTYPE::Quaternion(0, 0, 0, 1));
        anchorTrans->SetLocalScale(UTYPE::Vector3(1, 1, 1));
        freeTrans->SetLocalPosition(UTYPE::Vector3(0, 0, 0));
        freeTrans->SetLocalRotation(UTYPE::Quaternion(0, 0, 0, 1));
        freeTrans->SetLocalScale(UTYPE::Vector3(1, 1, 1));
        Debug::Logger::Info("Anchor attached to GameObject: {}", target->GetName()->ToString());
        attach_mode = true;
    }

    auto FreeCamera::exitAttachMode() -> void
    {
        anchorTrans->SetParent(nullptr);
        anchorTrans->SetLocalScale(UTYPE::Vector3(1, 1, 1));
        freeTrans->SetLocalPosition(UTYPE::Vector3(0, 0, 0));
        freeTrans->SetLocalRotation(UTYPE::Quaternion(0, 0, 0, 1));
        freeTrans->SetLocalScale(UTYPE::Vector3(1, 1, 1));
        Debug::Logger::Info("Anchor detached");
        attach_mode = false;
    }

    auto FreeCamera::updateAttachMode() -> void
    {
        if (attach_mode)
        {
            // FIXME:
            // If parent got destroyed，free camera GameObject would be destroyed too.
            // Then we will never enter this branch since FreeCamera would be disabled by the guard.
            // If free camera is the only camera exists in the scene, after being destroyed, IsCurrentFreeCamera return false since there is no camera at all.
            const auto destroyed = [&]
            {
                const auto parent = anchorTrans->GetParent();
                if (!parent) return true;
                const auto parentGo = static_cast<UTYPE::GameObject *>(parent->GetGameObject());
                if (!parentGo) return true;
                return parentGo->IsDestoryed();
            }();
            if (destroyed)
            {
                Debug::Logger::Debug("Attach mode parent was destroyed");
                exitAttachMode();
            }
        }
        if (Input::GetKeyDown(T))
        {
            if (!attach_mode && anchorTrans->GetParent() == nullptr)
            {
                if (const auto target = SelectGameObject())
                    enterAttachMode(target);
                else
                    Debug::Logger::Info("Target not found");
            }
            else
            {
                exitAttachMode();
            }
        }
    }
}
