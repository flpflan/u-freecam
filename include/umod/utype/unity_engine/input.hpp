#pragma once

#include "umod/utype/utype.hpp"

namespace umod::UTYPE::unity_engine
{
    enum class Key
    {
        None,
        Space,
        Enter,
        Tab,
        Backquote,
        Quote,
        Semicolon,
        Comma,
        Period,
        Slash,
        Backslash,
        LeftBracket,
        RightBracket,
        Minus,
        Equals,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        Digit1,
        Digit2,
        Digit3,
        Digit4,
        Digit5,
        Digit6,
        Digit7,
        Digit8,
        Digit9,
        Digit0,
        Shift_L,
        Shift_R,
        Alt_L,
        Alt_R,
        AltGr = 54,
        Ctrl_L,
        Ctrl_R,
        Meta_L,
        Meta_R,
        Windows_L = 57,
        Windows_R,
        Apple_L = 57,
        Apple_R,
        Command_L = 57,
        Command_R,
        ContextMenu,
        Esc,
        LeftArrow,
        RightArrow,
        UpArrow,
        DownArrow,
        Backspace,
        PageDown,
        PageUp,
        Home,
        End,
        Insert,
        Delete,
        CapsLock,
        NumLock,
        PrintScreen,
        ScrollLock,
        Pause,
        NumpadEnter,
        NumpadDivide,
        NumpadMultiply,
        NumpadPlus,
        NumpadMinus,
        NumpadPeriod,
        NumpadEquals,
        Numpad0,
        Numpad1,
        Numpad2,
        Numpad3,
        Numpad4,
        Numpad5,
        Numpad6,
        Numpad7,
        Numpad8,
        Numpad9,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        OEM1,
        OEM2,
        OEM3,
        OEM4,
        OEM5,
        IMESelected
    };
    class AxisControl : public UTYPE::Object
    {
        UNITY_CLASS_DECL("Unity.InputSystem.dll", AxisControl)
    public:
    };
    class ButtonControl : public AxisControl
    {
        UNITY_CLASS_DECL("Unity.InputSystem.dll", ButtonControl)
    public:
        UNITY_METHOD(bool *, get_isPressed, ())
        UNITY_METHOD(bool *, get_wasPressedThisFrame, ())
        UNITY_METHOD(bool *, get_wasReleasedThisFrame, ())
    };
    class KeyControl : public ButtonControl
    {
        UNITY_CLASS_DECL("Unity.InputSystem.dll", KeyControl)
    public:
    };

    class Keyboard
    {
        UNITY_CLASS_DECL("Unity.InputSystem.dll", Keyboard)
    public:
        UNITY_STATIC_METHOD(void, set_current, (const Keyboard *keyboard), keyboard)
        UNITY_STATIC_METHOD(Keyboard *, get_current, ())
        UNITY_METHOD(KeyControl *, get_Item, (Key key), key)
    };

    class DeltaControl
    {
        UNITY_CLASS_DECL("Unity.InputSystem.dll", DeltaControl)
    public:
    };
    class Pointer
    {
        UNITY_CLASS_DECL("Unity.InputSystem.dll", Pointer)
    public:
        UNITY_METHOD(void, set_delta, (DeltaControl * delta), delta)
        UNITY_METHOD(DeltaControl *, get_delta, ())
    };

    class Mouse : public Pointer
    {
        UNITY_CLASS_DECL("Unity.InputSystem.dll", Mouse)
    public:
        UNITY_STATIC_METHOD(void, set_current, (const Mouse *mouse), mouse)
        UNITY_STATIC_METHOD(Mouse *, get_current, ())
    };
}
