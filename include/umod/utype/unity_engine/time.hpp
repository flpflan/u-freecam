#pragma once

#include "umod/utype/utype.hpp"

namespace umod::UTYPE::unity_engine
{
    class Time : public UTYPE::Time
    {
        UNITY_CLASS_DECL("UnityEngine.CoreModule.dll", Time)
    public:
        UNITY_STATIC_METHOD(int, get_frameCount, (void))
    };
}
