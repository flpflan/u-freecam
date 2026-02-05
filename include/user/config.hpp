#pragma once

#include "freecam/freecam.hpp"
#include "umod/utype/unity_engine/input.hpp"

#include <array>

namespace user_config
{
    // Loader
    namespace loader
    {
        using namespace std::chrono_literals;
        inline bool Hardened = true;
        inline auto WaitBeforeInit = 5s;
    };

    namespace core
    {
        using namespace std::chrono_literals;
        inline auto MockLoopDeltaTime = 16.7ms;
        inline auto EnabledModules = std::array{"FreeCam"};
    }

    namespace freecam
    {
        inline ::freecam::Mode Mode = ::freecam::Mode::Orignal; // 相机模式
        inline bool DisableOrigCam = false;                     // 禁用原相机

        // 镜头属性
        namespace property
        {
#ifdef __ANDROID__
            inline float RotationSpeed = 100.f; // 镜头旋转速度
#else
            inline float RotationSpeed = 300.f; // 镜头旋转速度
#endif
            inline float RollSpeed = 100.f;     // 镜头滚动速度
            inline float BaseMoveSpeed = 100.f; // 基础移动速度
            inline float MaxMoveSpeed = 100.f;  // 最大移动速度
        };

        // 键位
        namespace keybind
        {
            using umod::UTYPE::unity_engine::KeyCode;
            using enum KeyCode;

            inline KeyCode ToggleFreeCam = Enter; // 开启/关闭自由镜头

            // 移动
            inline KeyCode Up = Space;        // 上升
            inline KeyCode Down = Ctrl_L;     // 下降
            inline KeyCode Forward = W;       // 前
            inline KeyCode Back = S;          // 后
            inline KeyCode Left = A;          // 左
            inline KeyCode Right = D;         // 右
            inline KeyCode SpeedUp = SHIFT_L; // 加速

            // 转动
            inline KeyCode RotateUp = UpArrow;       // 镜头向上转动
            inline KeyCode RotateDown = DownArrow;   // 镜头向下转动
            inline KeyCode RotateLeft = LeftArrow;   // 镜头向左转动
            inline KeyCode RotateRight = RightArrow; // 镜头向右转动
            inline KeyCode RollLeft = Q;             // 镜头逆时针滚动
            inline KeyCode RollRight = E;            // 镜头顺时针滚动
            inline KeyCode ResetRoll = R;            // 复位镜头滚动

            // 镜头缩放
            inline KeyCode ZoomMode = Z; // 镜头缩放模式
            inline KeyCode ZoomIn = X;   // 放大
            inline KeyCode ZoomOut = C;  // 缩小

            inline KeyCode PinAnchor = M; //固定锚点

            inline KeyCode UIMode = U;         // 开启/关闭 UI 模式
            inline KeyCode AttachMode = T; // 开启/退出依附模式
        };
    };

    namespace speed_gear
    {
        namespace keybind
        {
            using umod::UTYPE::unity_engine::KeyCode;
            using enum KeyCode;

            inline KeyCode ToggleFreeze = Backspace; // 开启/关闭时间流动
            inline KeyCode SpeedUp = Plus;           // 加速时间流动
            inline KeyCode SpeedDown = Minus;        // 减速时间流动
        }
    }
}
