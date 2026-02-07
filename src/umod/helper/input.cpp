#include "umod/runtime/helper/input.hpp"
#include "umod/runtime/UnityResolve.hpp"
#include "umod/utype/unity_engine/input.hpp"
#include "umod/utype/unity_engine/legacy_input.hpp"

#define MAP_KEYCODE(keycode)                                                                                           \
    case InputUtils::Key::keycode:                                                                                     \
        return UTYPE::unity_engine::KeyCode::keycode

#define MAP_KEY(keycode)                                                                                               \
    case InputUtils::Key::keycode:                                                                                     \
        return UTYPE::unity_engine::Key::keycode

namespace umod::unity_runtime::helper::InputUtils
{
    namespace
    {
        static constexpr UTYPE::unity_engine::KeyCode toRuntimeKeyCode(const InputUtils::Key key)
        {
            switch (key)
            {
                MAP_KEYCODE(A);
                MAP_KEYCODE(B);
                MAP_KEYCODE(C);
                MAP_KEYCODE(D);
                MAP_KEYCODE(E);
                MAP_KEYCODE(F);
                MAP_KEYCODE(G);
                MAP_KEYCODE(H);
                MAP_KEYCODE(I);
                MAP_KEYCODE(J);
                MAP_KEYCODE(K);
                MAP_KEYCODE(L);
                MAP_KEYCODE(M);
                MAP_KEYCODE(N);
                MAP_KEYCODE(O);
                MAP_KEYCODE(P);
                MAP_KEYCODE(Q);
                MAP_KEYCODE(R);
                MAP_KEYCODE(S);
                MAP_KEYCODE(T);
                MAP_KEYCODE(U);
                MAP_KEYCODE(V);
                MAP_KEYCODE(W);
                MAP_KEYCODE(X);
                MAP_KEYCODE(Y);
                MAP_KEYCODE(Z);
                MAP_KEYCODE(UpArrow);
                MAP_KEYCODE(DownArrow);
                MAP_KEYCODE(RightArrow);
                MAP_KEYCODE(LeftArrow);
                MAP_KEYCODE(Shift_L);
                MAP_KEYCODE(Shift_R);
                MAP_KEYCODE(Ctrl_L);
                MAP_KEYCODE(Ctrl_R);
                MAP_KEYCODE(Backspace);
                MAP_KEYCODE(Tab);
                MAP_KEYCODE(Enter);
                MAP_KEYCODE(Space);
                MAP_KEYCODE(Minus);
                MAP_KEYCODE(Equals);
                MAP_KEYCODE(Esc);
            }
        }
        static constexpr UTYPE::unity_engine::Key toRuntimeKey(const InputUtils::Key key)
        {
            switch (key)
            {
                MAP_KEY(A);
                MAP_KEY(B);
                MAP_KEY(C);
                MAP_KEY(D);
                MAP_KEY(E);
                MAP_KEY(F);
                MAP_KEY(G);
                MAP_KEY(H);
                MAP_KEY(I);
                MAP_KEY(J);
                MAP_KEY(K);
                MAP_KEY(L);
                MAP_KEY(M);
                MAP_KEY(N);
                MAP_KEY(O);
                MAP_KEY(P);
                MAP_KEY(Q);
                MAP_KEY(R);
                MAP_KEY(S);
                MAP_KEY(T);
                MAP_KEY(U);
                MAP_KEY(V);
                MAP_KEY(W);
                MAP_KEY(X);
                MAP_KEY(Y);
                MAP_KEY(Z);
                MAP_KEY(UpArrow);
                MAP_KEY(DownArrow);
                MAP_KEY(RightArrow);
                MAP_KEY(LeftArrow);
                MAP_KEY(Shift_L);
                MAP_KEY(Shift_R);
                MAP_KEY(Ctrl_L);
                MAP_KEY(Ctrl_R);
                MAP_KEY(Backspace);
                MAP_KEY(Tab);
                MAP_KEY(Enter);
                MAP_KEY(Space);
                MAP_KEY(Minus);
                MAP_KEY(Equals);
                MAP_KEY(Esc);
            }
        }

        enum Backend
        {
            None,
            Input,
            LegacyInput
        };

        static Backend getBackend()
        {
            static Backend backend = []
            {
                if (const auto module = UnityResolve::Get("UnityEngine.InputLegacyModule.dll"))
                    if (module->Get("Input")) return LegacyInput;
                if (const auto module = UnityResolve::Get("Unity.InputSystem.dll"))
                    if (module->Get("Keyboard")) return LegacyInput;
                return None;
            }();
            return backend;
        }
    }

    bool GetKey(const Key key)
    {
        // if (getBackend() == LegacyInput)
        return UTYPE::unity_engine::Input::GetKey(toRuntimeKeyCode(key));
        // else
        //     return UTYPE::unity_engine::Keyboard::get_current()->get_Item(toRuntimeKey(key))->get_isPressed();
    }
    bool GetKeyUp(const Key key)
    {
        // if (getBackend() == LegacyInput)
        return UTYPE::unity_engine::Input::GetKeyUp(toRuntimeKeyCode(key));
        // else
        //     return UTYPE::unity_engine::Keyboard::get_current()
        //         ->get_Item(toRuntimeKey(key))
        //         ->get_wasReleasedThisFrame();
    }
    bool GetKeyDown(const Key key)
    {
        // if (getBackend() == LegacyInput)
        return UTYPE::unity_engine::Input::GetKeyDown(toRuntimeKeyCode(key));
        // else
        //     return UTYPE::unity_engine::Keyboard::get_current()->get_Item(toRuntimeKey(key))->get_wasPressedThisFrame();
    }
    float GetAxis(const std::string &axis) { return UTYPE::unity_engine::Input::GetAxis(axis); }
}
