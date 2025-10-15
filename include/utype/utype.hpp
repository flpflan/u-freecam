#pragma once

#include "UnityResolve.hpp"

namespace UTYPE
{
    using UClass = UnityResolve::Class;
    using UMethod = UnityResolve::Method;
    using UTYPE = UnityResolve::UnityType;
}

// template <typename CLS, const bool IsStatic, typename RET, typename... ARGS>
// struct UnityMethod;
//
// template <typename CLS, typename RET, typename... ARGS>
// struct UnityMethod<CLS, false, RET, ARGS...>
// {
// private:
//     UMethod *method;
//     CLS &instance;
//
// public:
//     inline RET Call(ARGS... args) const
//     {
//         if (!method) method = CLS::GetUClass()->template Get<UMethod>(NAME);
//         method->Invoke<RET>(instance, args...);
//     }
//     inline RET operator()(ARGS... args) const { Call(args...); }
// };
// template <typename CLS, typename RET, typename... ARGS>
// struct UnityMethod<CLS, true, RET, ARGS...>
// {
//     UMethod *method;
//     inline RET Call(ARGS... args) { method->Invoke<RET>(args...); }
//     inline RET operator()(ARGS... args) const { Call(args...); }
// };

#define UNITY_CLASS_DECL(MODULE, CLS)                                                                                                                                                                  \
private:                                                                                                                                                                                               \
    inline static constexpr auto MODULE_NAME = MODULE;                                                                                                                                                 \
    inline static constexpr auto CLS_NAME = #CLS;                                                                                                                                                      \
                                                                                                                                                                                                       \
public:                                                                                                                                                                                                \
    inline static auto GetUClass() -> UClass *                                                                                                                                                         \
    {                                                                                                                                                                                                  \
        static UClass *klass;                                                                                                                                                                          \
        if (!klass) klass = UnityResolve::Get(MODULE_NAME)->Get(CLS_NAME);                                                                                                                             \
        return klass;                                                                                                                                                                                  \
    }

// #define UNITY_FIELD(FIELD_TY, FIELD_NAME)                                                                                                                                                              \
// public:                                                                                                                                                                                                \
//     auto Get##FIELD_NAME() -> FIELD_TY                                                                                                                                                                 \
//     {                                                                                                                                                                                                  \
//         static UMethod *method;                                                                                                                                                                        \
//         if (!method) method = GetUClass()->Get<UMethod>("get_collider");                                                                                                                               \
//         return method->Invoke<FIELD_TY>(this);                                                                                                                                                         \
//     }

#define UNITY_METHOD(RET_TY, METHOD_NAME, PARAMS, ...)                                                                                                                                                 \
public:                                                                                                                                                                                                \
    auto METHOD_NAME PARAMS->RET_TY                                                                                                                                                                    \
    {                                                                                                                                                                                                  \
        static UMethod *method;                                                                                                                                                                        \
        if (!method) method = GetUClass()->Get<UMethod>(#METHOD_NAME);                                                                                                                                 \
        return method->Invoke<RET_TY>(this, #__VA_ARGS__);                                                                                                                                             \
    }
#define UNITY_STATIC_METHOD(RET_TY, METHOD_NAME, PARAMS, ...)                                                                                                                                          \
public:                                                                                                                                                                                                \
    inline static auto METHOD_NAME PARAMS->RET_TY                                                                                                                                                      \
    {                                                                                                                                                                                                  \
        static UMethod *method;                                                                                                                                                                        \
        if (!method) method = GetUClass()->Get<UMethod>(#METHOD_NAME);                                                                                                                                 \
        return method->Invoke<RET_TY>(#__VA_ARGS__);                                                                                                                                                   \
    }
