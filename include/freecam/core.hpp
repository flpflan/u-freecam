#pragma once

#include "feature/free_camera.hpp"

namespace FreeCam
{
    class Core
    {
    public:
        static constexpr float DeltaTime_s = 0.0167f;
        static constexpr float DeltaTime_ms = DeltaTime_s * 1000;
        static constexpr float DeltaTime_us = DeltaTime_ms * 1000;

    private:
        inline static auto freeCam = Feature::FreeCamera();

    public:
        inline static bool UseMockLoop = false;
        static auto Update() -> void;
    };

}
