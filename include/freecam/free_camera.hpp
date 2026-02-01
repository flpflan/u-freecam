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
        UTYPE::GameObject *freeGO{};
        std::unique_ptr<Proxy::Camera> freeCam{};
        std::unique_ptr<Proxy::Transform> freeTrans{};
        UTYPE::GameObject *anchorGO{};
        std::unique_ptr<Proxy::Transform> anchorTrans{};

        // Properties for Rotate
#ifdef __ANDROID__
        constexpr static float rotationSpeed = 100.f;
#else
        constexpr static float rotationSpeed = 300.f;
#endif
        constexpr static float rollSpeed = 100.f;
        // Properties for Move
        constexpr static float baseMoveSpeed = 1.f;
        constexpr static float maxMoveSpeed = 50.f;

    public:
        enum class Mode
        {
            Depth,
            MainCamera
        };
        Mode Mode{Mode::Depth};
        bool Enabled = false;
        bool DisableOrigCam = false;

    public:
        auto Enable() -> void;
        auto Disable() -> void;
        auto Update() -> void;

    public:
        bool IsCurrentFreeCamera();
        static auto GetMaxDepthCamera() -> UTYPE::Camera *;
        static auto SelectGameObject() -> UTYPE::Transform *;

    private:
        UTYPE::Camera *origCamera{};
        UTYPE::Vector3 origPosition{};
        UTYPE::Quaternion origRotation{};
        UTYPE::GameObject *origGObject{};
        UTYPE::GameObject *origParent{};

        auto backupOrigCamera() -> void;

        auto updateMove() -> void;
        auto updateRotate() -> void;
        auto updateRoll() -> void;
        auto updateZoom() -> void;
        auto updateAttachMode() -> void;

        auto suspend() -> void;
        auto resume() -> void;

        auto enterAttachMode(UTYPE::Transform *target) -> void;
        auto exitAttachMode() -> void;
    };
}
