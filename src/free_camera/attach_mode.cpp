#include "debug/logger.hpp"
#include "feature/free_camera.hpp"
#include "utype/unity_engine/input.hpp"

using enum UTYPE::KeyCode;
using UTYPE::Input;

namespace FreeCam::Feature
{
    auto FreeCamera::updateAttachMode() -> void
    {
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
}
