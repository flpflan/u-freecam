#pragma once

#include "proxy/camera.hpp"
#include "proxy/transform.hpp"
#include <memory>

namespace FreeCam::Feature
{
    class FreeCamera
    {
    private:
        bool ui_layer = false;
        bool zoom_mode = false;
        bool attach_mode = false;
        UTYPE::GameObject *freeGO = nullptr;
        std::unique_ptr<Proxy::Camera> freeCam = nullptr;
        std::unique_ptr<Proxy::Transform> freeTrans = nullptr;
        UTYPE::GameObject *anchorGO = nullptr;
        std::unique_ptr<Proxy::Transform> anchorTrans = nullptr;

        // Properties for Rotate
#ifdef __ANDROID__
        constexpr static float rotationSpeed = 100.f;
#else
        constexpr static float rotationSpeed = 500.f;
#endif
        constexpr static float rollSpeed = 100.f;
        // Properties for Move
        const float moveSpeed = 1.f;
        const float moveSpeedMultiplier = 5.f;

    public:
        bool Enabled = false;

    public:
        auto Enable() -> void;
        auto Disable() -> void;
        auto Update() -> void;

    public:
        bool IsCurrentFreeCamera();

    private:
        UTYPE::Camera *origCamera;
        UTYPE::Vector3 origPosition;
        UTYPE::Quaternion origRotation;
        UTYPE::GameObject *origGObject;
        auto backupOrigCamera() -> void;
        auto selectGameObject() -> UTYPE::Transform *;
    };
}
