#pragma once

#include <atomic>
#include "utype/camera.hpp"

using UTYPE = UnityResolve::UnityType;

namespace FreeCam
{
    class Core
    {
    public:
        inline static constexpr float DeltaTime_s = 0.0167f;
        inline static constexpr float DeltaTime_ms = DeltaTime_s * 1000;
        inline static constexpr float DeltaTime_us = DeltaTime_ms * 1000;

    private:
        std::atomic<bool> listenKeys = false;
        UType::Camera *freeCamera = nullptr;
        UTYPE::GameObject *freeCameraGObject = nullptr;
        UTYPE::Camera *originCamera = nullptr;
        UTYPE::GameObject *originGObject = nullptr;
        UTYPE::Vector3 originPosition;
        UTYPE::Quaternion originRotation;

        auto StartListenKeys() -> void;

    public:
        bool isFreeCamBegin = false;
        auto BeginFreeCam() -> void;
        auto EndFreeCam() -> void;
        static void Hack();
    };


}
