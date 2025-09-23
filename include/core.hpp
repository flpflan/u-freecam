#pragma once

#include "proxy/camera.hpp"

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
        inline static Proxy::Camera *freeCamera = nullptr;
        inline static UTYPE::GameObject *freeCameraGObject = nullptr;
        inline static UTYPE::Camera *originCamera = nullptr;
        inline static UTYPE::GameObject *originGObject = nullptr;
        inline static UTYPE::Vector3 originPosition;
        inline static UTYPE::Quaternion originRotation;

    public:
        inline static bool IsFreeCamBegin = false;
        static auto BeginFreeCam() -> void;
        static auto EndFreeCam() -> void;
        static auto Update() -> void;
    };

}
