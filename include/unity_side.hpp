#pragma once

#include "UnityResolve.hpp"

namespace FreeCam {
    enum KeyCode
    {
        Backspace = 8,
        TAB = 9,
        Enter = 13,
        Space = 32,
        Minus = 45,
        Plus = 61,
        A = 97,
        B = 98,
        C = 99,
        D = 100,
        E = 101,
        F = 102,
        G = 103,
        H = 104,
        I = 105,
        J = 106,
        K = 107,
        L = 108,
        M = 109,
        N = 110,
        O = 111,
        P = 112,
        Q = 113,
        R = 114,
        S = 115,
        T = 116,
        U = 117,
        V = 118,
        W = 119,
        X = 120,
        Y = 121,
        Z = 122,
        UpArrow = 273,
        DownArrow = 274,
        RightArrow = 275,
        LeftArrow = 276,
        CTRL_R = 275,
        SHIFT_R = 303,
        SHIFT_L = 304,
        Ctrl_L = 306,
    };

    class UnityApi {
    public:
        using UMethod = UnityResolve::Method;
        using UTYPE = UnityResolve::UnityType;

        inline static auto GetKey(const KeyCode key) -> bool {
            static UMethod * method;
            if(!method)
                method = UnityResolve::Get("UnityEngine.InputLegacyModule.dll")
                            ->Get("Input")
                            ->Get<UnityResolve::Method>("GetKey");
            return method->Invoke<bool>(key);
        }
        inline static auto GetKeyDown(const KeyCode key) -> bool {
            static UMethod * method;
            if(!method)
                method = UnityResolve::Get("UnityEngine.InputLegacyModule.dll")
                            ->Get("Input")
                            ->Get<UnityResolve::Method>("GetKeyDown");
            return method->Invoke<bool>(key);
        }
        inline static auto GetAxis(const UTYPE::String *const axis) -> float {
            static UMethod * method;
            if (!method)
                method = UnityResolve::Get("UnityEngine.InputLegacyModule.dll")
                            ->Get("Input")
                            ->Get<UnityResolve::Method>("GetAxis");
            return method->Invoke<float>(axis);
        }
        inline static auto GetAxis(const std::string &axis) -> float {
            return GetAxis(UTYPE::String::New(axis));
        }
        inline static auto GetMouseButtonDown(const int key) -> bool {
            static UMethod *method;
            if (!method)
                method = UnityResolve::Get("UnityEngine.InputLegacyModule.dll")
                            ->Get("Input")
                            ->Get<UnityResolve::Method>("GetMouseButtonDown");
            return method->Invoke<bool>(key);
        }
        inline static auto SetTag(const UTYPE::GameObject *const go, const UTYPE::String *const  name) -> void {
            static UMethod *method;
            if (!method)
                method = UnityResolve::Get("UnityEngine.CoreModule.dll")
                            ->Get("GameObject")
                            ->Get<UnityResolve::Method>("set_tag");
            return method->Invoke<void>(go, name);
        }
        inline static auto SetTag(const UTYPE::GameObject *const go, const std::string &name) -> void {
            return SetTag(go, UTYPE::String::New(name));
        }
        template<typename T>
        inline static auto AddComponent(const UTYPE::GameObject *const go, const UTYPE::CsType *const ty) -> T * {
            static UMethod *method;
            if (!method)
                method = UnityResolve::Get("UnityEngine.CoreModule.dll")
                            ->Get("GameObject")
                            ->Get<UnityResolve::Method>("AddComponent", {"System.Type"});
            return method->Invoke<T *>(go, ty);

        }

    };
}
