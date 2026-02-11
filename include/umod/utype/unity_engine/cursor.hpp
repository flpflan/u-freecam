#pragma once

#include "umod/utype/utype.hpp"

namespace umod::UTYPE::unity_engine
{
    enum class LockState
    {
        None,
        Locked,
        Confined
    };

    class Cursor
    {
        UNITY_CLASS_DECL("UnityEngine.CoreModule.dll", Cursor)
    public:
        UNITY_STATIC_METHOD(LockState, get_lockState, (void))
        UNITY_STATIC_METHOD(void, set_lockState, (LockState v), v)
        UNITY_STATIC_METHOD(bool, get_visible, (void))
        UNITY_STATIC_METHOD(void, set_visible, (bool v), v)
    };
}
