#pragma once

#include "UnityResolve.hpp"

class Bootstrap
{
public:
    static void Run();
    static void Shutdown();

private:
    static std::pair<void *, UnityResolve::Mode> getUnityBackend();
    static bool initializeUnity();
    static std::pair<uintptr_t, int> getUnityVersionMajor();
    template <auto UpdateFn>
    static bool attachToGameLoop();
};
