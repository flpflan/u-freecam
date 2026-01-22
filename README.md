## 这是什么 (What's This)
如其名所示，这是一个针对基于 Unity 引擎制作的游戏的自由镜头工具。\
此外，还附带了游戏内变速功能。

As the name suggests, this a free camera tool for games built on the Unity engine. \
It also comes with an in-game speed change feature.

## 如何使用 (How to Use)
通过任何手段, 将动态库注入目标进程/app即可[^1]。\
举例来说，Windows 上可通过 CE 自带的 dll 注入工具进行注入，而 Android 上可以使用 [XInjector](https://github.com/WindySha/XInjector) (非 Root 环境可以用 [Android-Virtual-Inject](https://github.com/reveny/Android-Virtual-Inject/releases/latest))

By any means, inject this dynamic library into the target process or application. \
For example, on Windows, you can use the DLL injection tool that comes with Cheat Engine (CE), while on Android, you can use [XInjector](https://github.com/WindySha/XInjector) (Non-root environments can use [Android-Virtual-Inject](https://github.com/reveny/Android-Virtual-Inject/releases/latest)).

## 按键 (Keybindings)
> ** Android 上可使用外接键盘 ** \
> ** You can use an external keyboard on Android. **

|自由镜头|Free Camera|
| - | - |
|打开/关闭自由镜头 -> Enter|Toggle FreeCam -> Enter|
|移动 -> WASD、Ctrl、空格、左Shift(加速)|Movement -> WASD, Ctrl, Space, and Shift_L (Sprint)|
|旋转镜头 -> 鼠标 / 方向键 / 触屏（移动端）|Rotation -> Mouse / Arrow keys / Touch screen (Moblie)|
|UI模式 -> 鼠标中键 / U|UI Mode -> Mouse middle button / U|
|镜头缩放 -> 按住Z + 鼠标滚轮/ X、C|Zoom -> Hold Z + Mouse Wheel / X or C|
|镜头滚动 -> Q/E|Roll Camera -> Q/E|
|镜头复位 -> R|Reset Camera -> R|

> 初始状态下，锚点与镜头重合。\
> 任何情况下，镜头始终随着锚点移动，绕着锚点旋转。\
> 在锚点固定时，通过移动可改变镜头与锚点相对位置。\
> 所谓依附模式，即是将目标物体设为锚点。(默认情况下将屏幕中心所对物体选为目标)\
> Initially, the Anchor coincides with the Camera.\
> In all cases, the Camera moves with and rotates around the Anchor.\
> With the Anchor fixed, the relative position between Camera and Anchor can be changed through movement.\
> Attach Mode sets the target object as Anchor. (By default, the object corresponding to the center of the screen is selected as the target.)

|锚点|Anchor|
| - | - |
|固定锚点 -> 按住M|Pin Anchor -> Hold M|
|复位到锚点 -> 左Shift + M|Reset to Anchor -> Shift_L + M|
|切换依附模式 -> T|Toggle Attach Mode -> T|

|变速|Speed Hack|
| - | - |
|加速 -> +|Speed up -> +|
|减速 -> -|Speed down -> -|
|冻结速度 / 恢复正常速度 -> Backspace|Freeze speed / Resume speed -> Backspace|

## 已测试游戏 (Tested Game)
- [蔚蓝档案](https://www.bilibili.com/video/BV1XRpmz8EBW) / [Blue Archive](https://youtu.be/40Od_dHH5oY)
- 喵斯快跑 / Muse Dash
- 托兰异世录 (移动端) / Toram Online (Moblie)
- 明日方舟：终末地 / Arknights: Endfield

## Special Thanks
- [UnityResolve.hpp](https://github.com/issuimo/UnityResolve.hpp)

[^1]: 对于部分魔改/加固的引擎，需要在游戏启动时一并注入。
