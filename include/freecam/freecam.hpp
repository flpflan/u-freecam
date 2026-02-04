#pragma once

#include "umod/runtime/helper/camera.hpp"
#include "umod/runtime/helper/transform.hpp"
#include "umod/utype/unity_engine/core.hpp"

#include <memory>

namespace freecam
{
    enum class Mode
    {
        Orignal,
        Depth,
        MainCamera
    };

    constexpr auto kFreeArchorName = "UE_Freecam_Archor";
    constexpr auto kFreeCameraName = "UE_Freecam";

    class FreeCamera
    {
    protected:
        FreeCamera() = default;

    public:
        static FreeCamera create(Mode mode)
        {
            FreeCamera freeCam{};
            freeCam.mode = mode;
            return freeCam;
        }

    public:
        bool enabled = false;
        Mode mode;

    public:
        auto enable() -> void;
        auto disable() -> void;
        auto update() -> void;

    protected:
        using CameraHelper = umod::unity_runtime::helper::CameraHelper;
        using TransformHelper = umod::unity_runtime::helper::TransformHelper;

        struct
        {
            bool ui_layer = false;
            bool zoom_mode = false;
            bool attach_mode = false;
        } kFlags;

        umod::UTYPE::unity_engine::Camera *freeCam_{};
        umod::UTYPE::unity_engine::Transform *anchorTrans_{};
        std::unique_ptr<CameraHelper> cameraHelper_;
        std::unique_ptr<TransformHelper> freeTransHelper_;
        std::unique_ptr<TransformHelper> anchorTransHelper_;

    protected:
        umod::UTYPE::unity_engine::Camera *origCamera_{};
        umod::UTYPE::unity_engine::Vector3 origPosition_{};
        umod::UTYPE::unity_engine::Quaternion origRotation_{};

        auto createCamera(Mode) -> umod::UTYPE::unity_engine::Camera *;
        auto backupOrigCamera() -> void;
        auto updateMove() -> void;
        auto updateRotate() -> void;
        auto updateRoll() -> void;
        auto updateZoom() -> void;
        auto updateAttachMode() -> void;

        auto enterAttachMode(umod::UTYPE::unity_engine::Transform *target) -> void;
        auto exitAttachMode() -> void;
    };
}
