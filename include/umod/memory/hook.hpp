#pragma once

namespace umod::memory
{
    bool hook(void *target, void *detour, void *const *orig);

    template <typename RTYPE, typename... ARGS>
    auto hook(RTYPE (*target)(ARGS...), RTYPE (*detour)(ARGS...), RTYPE (*const *orig)(ARGS...)) -> bool
    {
        return hook((void *)target, (void *)detour, (void **)orig);
    }

    bool unhook(void *target);

    template <typename RTYPE, typename... ARGS>
    bool unhook(RTYPE (*target)(ARGS...))
    {
        return unhook(reinterpret_cast<void *>(target));
    }

    constexpr void(*__init_orig__(void *)) { return nullptr; }
    template <typename RTYPE, typename... ARGS>
    constexpr RTYPE (*__init_orig__(RTYPE (*)(ARGS...)))(ARGS...)
    {
        return nullptr;
    }
}

/*
 * Usage(1):
 * bool target(int x) { print(x); return x == 1; }
 * const bool hook_success = Hook(target, +[](int x){ CALL_ORIGNAL(x); return true; })
 *
 * Usage(2):
 * bool target(int x) { return x == 1; }
 * template <auto&& CALL_ORIGNAL>
 * bool detour(int x) { CALL_ORIGNAL(x); return true; }
 * const bool hook_success = Hook(target, detour<CALL_ORIGNAL>);
 */
#ifndef _MSC_VER
#define Hook(TARGET, DETOUR)                                                                                           \
    ({                                                                                                                 \
        const static auto CALL_ORIGNAL = ::umod::memory::__init_orig__(TARGET);                                        \
        ::umod::memory::hook(TARGET, DETOUR, &CALL_ORIGNAL);                                                           \
    })

#else
#define Hook(TARGET, DETOUR)                                                                                           \
    (                                                                                                                  \
        [=]                                                                                                            \
        {                                                                                                              \
            static auto CALL_ORIGNAL = ::umod::memory::__init_orig__(TARGET);                                          \
            return ::umod::memory::hook(TARGET, DETOUR, &CALL_ORIGNAL);                                                \
        })()
#endif

#define UnHook(TARGET) ::umod::memory::unhook(TARGET)
