u-freecam
==========

<div align="center">
  <h3>Unity 引擎自由镜头（运行时）</h3>
  <p>
    <a href="./README.md">English</a>  
    <span>简体中文</span> |
  </p>

[![Release](https://img.shields.io/github/v/release/flpflan/u-freecam)](https://github.com/flpflan/u-freecam/releases/latest)
![Unity](https://img.shields.io/badge/Unity-Mono%20%7C%20IL2CPP-green.svg)
![Platform](https://img.shields.io/badge/Platform-Windows_|_Android-2376E6)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

</div>

----------

## 这是什么

如其名所示，这是一个针对基于 Unity 引擎制作的游戏的自由镜头工具。

此外，还附带了游戏内变速功能。

## 下载

可在 [Release](https://github.com/flpflan/u-freecam/releases) 页面下载预构建版本。
如果你想获取最新的开发版本，也可从 [CI](https://github.com/flpflan/u-freecam/actions) 构建产物中下载。

## 构建 (Windows)

### 前置需求

- Visual Studio (包含 C++ 工具链和 Cmake)

### 编译

用 Visual Studio 打开本项目，选择 `freecam-x86_64-windows` 目标，然后生成。

## 构建 (Android)

### 前置需求

- Android NDK 27+
- Cmake 3.22+
- Ninja (可选)

### 编译

```shell
export ANDROID_NDK=/path/to/ndk # 设置 NDK 路径
export GENERATOR=Ninja
make release-aarch64 # 或者其它目标，如 release-x86_64
```

输出: `build/release-android-aarch64/libfreecam-aarch64-android.so`

## 如何使用

通过任何手段, 将动态库注入目标进程/app即可[^1]。

举例来说，Windows 上可通过 CE 自带的 dll 注入工具进行注入，而 Android 上可以使用 [XInjector](https://github.com/WindySha/XInjector) (非 Root 环境可以用 [Android-Virtual-Inject](https://github.com/reveny/Android-Virtual-Inject/releases/latest))

## 配置

注入进程之后，程序会在本地 __23333__ 端口启动一个 WebUI 配置界面，可通过浏览器访问此界面来调整程序的各项参数。

如果是本机访问，打开 http://localhost:23333 即可。

## 模式
u-freecam 具有三种不同的运行模式，效果根据游戏的不同效果会有很大差异。\
根据游戏的不同，一些模式可能不能正常运作，甚至导致游戏直接崩溃，请尝试三种模式后选择合适的那个。

模式的切换可在 [WebUI](#配置) 中进行。

## 按键

> ** Android 上可使用外接键盘 **

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
> 所谓依附模式，即是将目标物体设为锚点。(默认情况下将屏幕中心所对物体选为目标)

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

## 特别鸣谢

- [UnityResolve.hpp](https://github.com/issuimo/UnityResolve.hpp)

## 常见问题
 
### CE 注入后游戏崩溃

确保 `freecam-x86_86-windows.dll` 文件所在路径为纯英文，不包含中文及特殊字符。

[^1]: 对于部分魔改/加固的引擎，需要在游戏启动时一并注入。
