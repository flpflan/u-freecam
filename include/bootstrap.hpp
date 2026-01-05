#pragma once

#include "UnityResolve.hpp"

class Bootstrap
{
public:
    inline static bool Hardened = false;
    inline static long long SecondsBeforeInit = 5;

public:
    static void Run();
    static void Shutdown();

private:
    inline static void *IL2CPP_LIB_HANDLE{};
    static void bypassHardenedIL2CPP();
    static std::pair<void *, UnityResolve::Mode> getUnityBackend();
    static bool initializeUnity();
    template <auto UpdateFn>
    static bool attachToGameLoop();
};
