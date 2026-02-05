#pragma once

#include "logger.hpp"
#include "umod/utype/unity_engine/core.hpp"

namespace umod::debug::test
{
    using UClass = UnityResolve::Class;
    using UMethod = UnityResolve::Method;

    using UCamera = UTYPE::unity_engine::Camera;
    using UTransform = UTYPE::unity_engine::Transform;
    using UComponent = UTYPE::unity_engine::Component;
    using UString = UTYPE::unity_engine::String;

    inline void testAllCamera()
    {
        int i = 0;
        for (const auto cam : UCamera::GetAllCamera())
        {
            logger::debug("Cam Index: {}", i++);
            logger::debug("Name: {}", cam->GetName()->ToString());
            logger::debug("Tag: {}", cam->GetGameObject()->GetTag()->ToString());
            logger::debug("ActiveSelf: {}", cam->GetGameObject()->GetActiveSelf());
        }
    }
    inline void testHierarchy(UTransform *const target, const std::string prefix = "")
    {
        logger::debug("{}Target Name: {}, Tag: {}", prefix, target->GetName()->ToString(),
                      target->GetTag()->ToString());
        const auto childCount = target->GetChildCount();
        for (auto i = 0; i < childCount; i++)
        {
            const auto child = target->GetChild(i);
            testHierarchy(child, "--" + prefix);
        }
    }
    inline void testComponents(UTransform *const target, const std::string prefix = "")
    {
        static UMethod *method;
        if (!method)
            method = UnityResolve::Get("mscorlib.dll")
                         ->Get("Type", "System", "MemberInfo")
                         ->Get<UMethod>("get_NameOrDefault");

        const auto scripts = target->GetComponents<UComponent *>(UComponent::GetUClass());
        logger::debug("Found {} Components on GameObject {}", scripts.size(), target->GetName()->ToString());
        for (const auto script : scripts)
        {
            const auto ty = script->GetType();
            logger::debug("- {}", method->Invoke<UString *>(ty)->ToString());
        }
    }
    inline void testGameObject(UTransform *const target)
    {
        testHierarchy(target, "");
        testComponents(target);
    }
}
