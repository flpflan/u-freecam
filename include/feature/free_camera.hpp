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
        UTYPE::GameObject *freeGO = nullptr;
        std::unique_ptr<Proxy::Camera> freeCam = nullptr;
        std::unique_ptr<Proxy::Transform> freeTrans = nullptr;
        UTYPE::GameObject *anchorGO = nullptr;
        std::unique_ptr<Proxy::Transform> anchorTrans = nullptr;

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
