#pragma once

#include "UnityResolve.hpp"

namespace UType
{
    using UClass = UnityResolve::Class;
    using UMethod = UnityResolve::Method;
    using UTYPE = UnityResolve::UnityType;

    enum class KeyCode
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
    class Input
    {
    public:
        inline static auto GetKey(const KeyCode key) -> bool
        {
            static UMethod *method;
            if (!method) method = GetUClass()->Get<UMethod>("GetKey");
            return method->Invoke<bool>(key);
        }
        inline static auto GetKeyDown(const KeyCode key) -> bool
        {
            static UMethod *method;
            if (!method) method = GetUClass()->Get<UMethod>("GetKeyDown");
            return method->Invoke<bool>(key);
        }
        inline static auto GetAxis(const UTYPE::String *const axis) -> float
        {
            static UMethod *method;
            if (!method) method = GetUClass()->Get<UMethod>("GetAxis");
            return method->Invoke<float>(axis);
        }
        inline static auto GetAxis(const std::string &axis) -> float { return GetAxis(UTYPE::String::New(axis)); }
        inline static auto GetMouseButtonDown(const int key) -> bool
        {
            static UMethod *method;
            if (!method) method = GetUClass()->Get<UMethod>("GetMouseButtonDown");
            return method->Invoke<bool>(key);
        }

    public:
        inline static auto GetUClass() -> UClass *
        {
            static UClass *klass;
            if (!klass) klass = UnityResolve::Get("UnityEngine.InputLegacyModule.dll")->Get("Input");
            return klass;
        }
    };
}
