#pragma once

#include "logger.hpp"
#include "utype/core.hpp"

using UClass = UnityResolve::Class;
using UMethod = UnityResolve::Method;
using UTYPE = UnityResolve::UnityType;

namespace Debug
{
    class Test
    {
    public:
        inline static void TestGameObject(UTYPE::Transform *const target)
        {
            TestHierarchy(target, "");
            TestComponents(target);
        }
        inline static void TestHierarchy(UTYPE::Transform *const target, const std::string prefix = "")
        {
            Logger::LOGD("{}Target Name: {}", prefix, target->GetName()->ToString());
            const auto childCount = target->GetChildCount();
            for (auto i = 0; i < childCount; i++)
            {
                const auto child = target->GetChild(i);
                TestHierarchy(child, "--" + prefix);
            }
        }
        inline static void TestComponents(UTYPE::Transform *const target, const std::string prefix = "")
        {
            static UMethod *method;
            if (!method) method = UnityResolve::Get("mscorlib.dll")->Get("Type", "System", "MemberInfo")->Get<UMethod>("get_NameOrDefault");

            const auto scripts = target->GetComponentsInChildren<UType::MonoBehavior *>(UType::MonoBehavior::GetUClass());
            Debug::Logger::LOGD("Found {} MonoBehavior on GameObject {}", scripts.size(), target->GetName()->ToString());
            for (const auto script : scripts)
            {
                const auto ty = script->GetType();
                Logger::LOGD("- {}", method->Invoke<UTYPE::String *>(ty)->ToString());
            }
        }
    };
}
