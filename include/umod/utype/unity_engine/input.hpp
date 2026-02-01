#pragma once

#include "freecam/utype/utype.hpp"

namespace UTYPE
{
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
    enum class TouchPhase
    {
        Began = 2,
        Moved = 3,
        Stationary = 4,
        Ended = 5,
        Canceled = 6
    };
    struct Touch
    {
        UNITY_CLASS_DECL("UnityEngine.InputLegacyModule.dll", Touch)
    public:
        UNITY_METHOD(TouchPhase, get_phase, (void))
        UNITY_METHOD(UTYPE::Vector2, get_position, (void))
        UNITY_METHOD(UTYPE::Vector2, get_deltaPosition, (void))
    };

    class Input
    {
        UNITY_CLASS_DECL("UnityEngine.InputLegacyModule.dll", Input)
    public:
        UNITY_STATIC_METHOD(bool, GetKey, (const KeyCode key), key)
        UNITY_STATIC_METHOD(bool, GetKeyDown, (const KeyCode key), key)
        UNITY_STATIC_METHOD(bool, GetKeyUp, (const KeyCode key), key)
        UNITY_STATIC_METHOD(float, GetAxis, (const UTYPE::String *const axis), axis)
        inline static auto GetAxis(const std::string &axis) -> float { return GetAxis(UTYPE::String::New(axis)); }
        UNITY_STATIC_METHOD(bool, GetMouseButtonDown, (const int key), key)
        UNITY_STATIC_METHOD(Touch, GetTouch, (int v), v)
        UNITY_STATIC_METHOD(int, get_touchCount, (void))
    };
}
