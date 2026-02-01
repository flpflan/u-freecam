#pragma once

#include "freecam/UnityResolve.hpp"

namespace UTYPE
{
    using UClass = UnityResolve::Class;
    using UMethod = UnityResolve::Method;
    using UTYPE = UnityResolve::UnityType;
}

#define UNITY_CLASS_DECL(MODULE, CLS)                                                                                                                                                                  \
private:                                                                                                                                                                                               \
    inline static constexpr auto MODULE_NAME = MODULE;                                                                                                                                                 \
    inline static constexpr auto CLS_NAME = #CLS;                                                                                                                                                      \
                                                                                                                                                                                                       \
public:                                                                                                                                                                                                \
    inline static auto __ctor__(CLS *self) -> void                                                                                                                                                     \
    {                                                                                                                                                                                                  \
        static UMethod *method;                                                                                                                                                                        \
        if (!method) method = GetUClass()->Get<UMethod>(".ctor");                                                                                                                                      \
        return method->Invoke<void>(self);                                                                                                                                                             \
    }                                                                                                                                                                                                  \
    inline static auto GetUClass() -> UClass *                                                                                                                                                         \
    {                                                                                                                                                                                                  \
        static UClass *klass;                                                                                                                                                                          \
        if (!klass) klass = UnityResolve::Get(MODULE_NAME)->Get(CLS_NAME);                                                                                                                             \
        return klass;                                                                                                                                                                                  \
    }

// #define UNITY_FIELD(FIELD_TY, FIELD_NAME)                                                                                                                                                              \
//     auto Get##FIELD_NAME() -> FIELD_TY                                                                                                                                                                 \
//     {                                                                                                                                                                                                  \
//         static UMethod *method;                                                                                                                                                                        \
//         if (!method) method = GetUClass()->Get<UMethod>("get_collider");                                                                                                                               \
//         return method->Invoke<FIELD_TY>(this);                                                                                                                                                         \
//     }

#define UNITY_METHOD(RET_TY, METHOD_NAME, PARAMS, ...)                                                                                                                                                 \
    auto METHOD_NAME PARAMS->RET_TY                                                                                                                                                                    \
    {                                                                                                                                                                                                  \
        static UMethod *method;                                                                                                                                                                        \
        if (!method) method = GetUClass()->Get<UMethod>(#METHOD_NAME);                                                                                                                                 \
        return method->Invoke<RET_TY>(this __VA_OPT__(, ) __VA_ARGS__);                                                                                                                                \
    }
#define UNITY_STATIC_METHOD(RET_TY, METHOD_NAME, PARAMS, ...)                                                                                                                                          \
    inline static auto METHOD_NAME PARAMS->RET_TY                                                                                                                                                      \
    {                                                                                                                                                                                                  \
        static UMethod *method;                                                                                                                                                                        \
        if (!method) method = GetUClass()->Get<UMethod>(#METHOD_NAME);                                                                                                                                 \
        return method->Invoke<RET_TY>(__VA_ARGS__);                                                                                                                                                    \
    }
