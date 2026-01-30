u-freecam
==========

[English](./README.md) | 中文

----------

## 这是什么
如其名所示，这是一个针对基于 Unity 引擎制作的游戏的自由镜头工具。\
此外，还附带了游戏内变速功能。

## 如何使用
通过任何手段, 将动态库注入目标进程/app即可[^1]。\
举例来说，Windows 上可通过 CE 自带的 dll 注入工具进行注入，而 Android 上可以使用 [XInjector](https://github.com/WindySha/XInjector) (非 Root 环境可以用 [Android-Virtual-Inject](https://github.com/reveny/Android-Virtual-Inject/releases/latest))

## 按键
> ** Android 上可使用外接键盘 ** \

| 自由镜头           | 键位                            |
| ------------------ | ------------------------------- |
| 打开/关闭自由镜头  | Enter                           |
| 移动               | WASD、Ctrl、空格、左Shift(加速) |
| 旋转镜头           | 鼠标 / 方向键 / 触屏（移动端）  |
| UI模式             | 鼠标中键 / U                    |
| 镜头缩放           | 按住Z + 鼠标滚轮/ X、C          |
| 镜头滚动           | Q / E                           |
| 镜头复位           | R                               |

> 初始状态下，锚点与镜头重合。\
> 任何情况下，镜头始终随着锚点移动，绕着锚点旋转。\
> 在锚点固定时，通过移动可改变镜头与锚点相对位置。\
> 所谓依附模式，即是将目标物体设为锚点。(默认情况下将屏幕中心所对物体选为目标)\

| 锚点         | 键位        |
| ------------ | ----------- |
| 固定锚点     | 按住M       |
| 复位到锚点   | 左Shift + M |
| 切换依附模式 | T           |

| 变速                    | 键位      |
| ----------------------- | --------- |
| 加速                    | +         |
| 减速                    | -         |
| 冻结速度 / 恢复正常速度 | Backspace |

## 已测试游戏
- [蔚蓝档案](https://www.bilibili.com/video/BV1XRpmz8EBW)
- 喵斯快跑
- 托兰异世录 (移动端)
- 明日方舟：终末地

## Special Thanks
- [UnityResolve.hpp](https://github.com/issuimo/UnityResolve.hpp)

[^1]: 对于部分魔改/加固的引擎，需要在游戏启动时一并注入。
