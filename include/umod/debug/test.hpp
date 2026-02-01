#pragma once

#include "logger.hpp"
#include "utype/unity_engine/core.hpp"

using UClass = UnityResolve::Class;
using UMethod = UnityResolve::Method;

namespace Debug
{
    namespace Test
    {
        inline void TestAllCamera()
        {
            int i = 0;
            for (const auto cam : UTYPE::Camera::GetAllCamera())
            {
                Logger::Debug("Cam Index: {}", i++);
                Logger::Debug("Name: {}", cam->GetName()->ToString());
                Logger::Debug("Tag: {}", cam->GetGameObject()->GetTag()->ToString());
                Logger::Debug("ActiveSelf: {}", cam->GetGameObject()->GetActiveSelf());
            }
        }
        inline void TestHierarchy(UTYPE::Transform *const target, const std::string prefix = "")
        {
            Logger::Debug("{}Target Name: {}, Tag: {}", prefix, target->GetName()->ToString(), target->GetTag()->ToString());
            const auto childCount = target->GetChildCount();
            for (auto i = 0; i < childCount; i++)
            {
                const auto child = target->GetChild(i);
                TestHierarchy(child, "--" + prefix);
            }
        }
        inline void TestComponents(UTYPE::Transform *const target, const std::string prefix = "")
        {
            static UMethod *method;
            if (!method) method = UnityResolve::Get("mscorlib.dll")->Get("Type", "System", "MemberInfo")->Get<UMethod>("get_NameOrDefault");

            const auto scripts = target->GetComponents<UTYPE::Component *>(UTYPE::Component::GetUClass());
            Logger::Debug("Found {} Components on GameObject {}", scripts.size(), target->GetName()->ToString());
            for (const auto script : scripts)
            {
                const auto ty = script->GetType();
                Logger::Debug("- {}", method->Invoke<UTYPE::String *>(ty)->ToString());
            }
        }
        inline void TestGameObject(UTYPE::Transform *const target)
        {
            TestHierarchy(target, "");
            TestComponents(target);
        }
    };
}
