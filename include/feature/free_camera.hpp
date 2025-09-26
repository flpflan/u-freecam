#pragma once

#include "proxy/camera.hpp"
#include <memory>

namespace FreeCam::Feature
{
    class FreeCamera
    {
    private:
        bool ui_layer = false;
        bool zoom_mode = false;
        std::unique_ptr<Proxy::Camera> freeCamera = nullptr;
        UTYPE::GameObject *freeGObject = nullptr;

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
        auto resetState() -> void;
    };
}
