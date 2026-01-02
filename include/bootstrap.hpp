#pragma once

#include "UnityResolve.hpp"

class Bootstrap
{
public:
    static void Run();
    static void Shutdown();

private:
    inline static void *IL2CPP_LIB_HANDLE = nullptr;
    static void bypassHardenedIL2CPP();
    static std::pair<void *, UnityResolve::Mode> getUnityBackend();
    static bool initializeUnity();
    template <auto UpdateFn>
    static bool attachToGameLoop();
};
