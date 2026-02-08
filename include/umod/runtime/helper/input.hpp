#pragma once

#include <string>
namespace umod::unity_runtime::helper
{
    namespace InputUtils
    {
        enum class Key
        {
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
            UpArrow,
            DownArrow,
            RightArrow,
            LeftArrow,
            Shift_L,
            Shift_R,
            Ctrl_L,
            Ctrl_R,
            Backspace,
            Tab,
            Enter,
            Space,
            Minus,
            // Plus,
            Equals,
            Esc
        };
        bool GetKey(const Key);
        bool GetKeyUp(const Key);
        bool GetKeyDown(const Key);
        float GetAxis(const std::string &);
        bool GetMouseButtonDown(const int);
    }
}
