#pragma once

#include "UnityResolve.hpp"

namespace UType
{
    using UClass = UnityResolve::Class;
    using UMethod = UnityResolve::Method;
    using UTYPE = UnityResolve::UnityType;

    class Cursor
    {
    public:
        inline static auto GetLockState() -> int
        {
            static UMethod *method;
            if (!method) method = Cursor::GetUClass()->Get<UMethod>("get_lockState");
            return method->Invoke<int>();
        }
        inline static auto SetLockState(int v) -> void
        {
            static UMethod *method;
            if (!method) method = Cursor::GetUClass()->Get<UMethod>("set_lockState");
            return method->Invoke<void>(v);
        }
        inline static auto GetVisible() -> bool
        {

            static UMethod *method;
            if (!method) method = Cursor::GetUClass()->Get<UMethod>("get_visible");
            return method->Invoke<bool>();
        }
        inline static auto SetVisible(bool v) -> void
        {

            static UMethod *method;
            if (!method) method = Cursor::GetUClass()->Get<UMethod>("set_visible");
            return method->Invoke<void>(v);
        }

    public:
        inline static auto GetUClass() -> UClass *
        {
            static UClass *klass;
            if (!klass) klass = UnityResolve::Get("UnityEngine.CoreModule.dll")->Get("Cursor");
            return klass;
        }
    };
}
