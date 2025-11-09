#pragma once

#include "utype/utype.hpp"

namespace UTYPE
{
    class Time : public UTYPE::Time
    {
        UNITY_CLASS_DECL("UnityEngine.CoreModule.dll", Time)
    public:
        UNITY_STATIC_METHOD(int, get_frameCount, (void))
    };
}
