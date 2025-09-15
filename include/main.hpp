#pragma once

#define __ANDROID__ 1
#define WINDOWS_MODE 0
#define ANDROID_MODE 1
#define LINUX_MODE 0

#include "UnityResolve.hpp"

class Main
{
public:
    static void run();
    static void shutdown();

private:
    static std::pair<void*, UnityResolve::Mode> getUnityBackend();
    static bool initializeUnity();
    static std::pair<uintptr_t, int> getUnityVersionMajor();
};
