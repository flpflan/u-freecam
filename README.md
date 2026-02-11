u-freecam
==========

<div align="center">
  <h3>Runtime Freecam for Unity games</h3>
  <p>
    <span>English</span> |
    <a href="./README.zh-CN.md">简体中文</a>  
  </p>

[![Release](https://img.shields.io/github/v/release/flpflan/u-freecam)](https://github.com/flpflan/u-freecam/releases/latest)
![Unity](https://img.shields.io/badge/Unity-Mono%20%7C%20IL2CPP-green.svg)
![Platform](https://img.shields.io/badge/Platform-Windows_|_Android-2376E6)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

</div>

----------

## What's This

As the name suggests, this a freecam tool for games built on the Unity engine. 

It also comes with an in-game speed change feature.

## Download

Pre-built versions can be downloaded from the [Release](https://github.com/flpflan/u-freecam/releases) page.

If you want the latest development version, you can also download it from the [CI](https://github.com/flpflan/u-freecam/actions) build artifacts.

## Build (Windows)

### Requirement

- Visual Studio (With C++ toolchain and Cmake)

### Compile

Open this project directly in Visual Studio, select the `freecam-x86_64-windows` target, and then generate it.

## Build (Android)

### Requirement

- Android NDK 27+
- Cmake 3.22+
- Ninja (Optional)

### Compile

```shell
export ANDROID_NDK=/path/to/ndk # Set the path to Android NDK
export GENERATOR=Ninja
make release-aarch64 # Or the other target, e.g. release-x86_64
```

output: `build/release-android-aarch64/libfreecam-aarch64-android.so`

## How to Use

### General methods

By any means, inject this dynamic library into the target process or application[^1]. 

For example, on Windows, you can use the DLL injection tool that comes with Cheat Engine (CE), while on Android, you can use [XInjector](https://github.com/WindySha/XInjector) (Non-root environments can use [Android-Virtual-Inject](https://github.com/reveny/Android-Virtual-Inject/releases/latest)).

> [!IMPORTANT]
> u-freecam has three different operating modes, and their behavior can vary greatly depending on the game.\
> Depending on the game, some modes may not work correctly or may even cause the game to crash.
> You can try out all three modes and choose the one that works best for you.

You can switch between modes in the [WebUI](#Configuration).

### Bypassing XignCode3

> Note: Currently, bypass support for XignCode3 is only available on Windows 64-bit systems.
> You must handle file integrity checks and other protection mechanisms on your own.\
> If you have any questions, please submit an Issue.

If the target process uses __XignCode3__ anti-cheat protection, locate the `x3_x64.xem` file in the game directory and rename it to `x3_x64.xem.bak`.
Then place the DLL in the same directory and rename it to `x3_x64.xem`.\
After that, launch the game as usual. u-freecam will load automatically.

## Configuration

After injected to the process , a WebUI interface will be started on the local port __23333__. This interface can be accessed via a browser to adjust various program parameters.

To access it locally, simply open http://localhost:23333.

## Keybindings

> [!TIP]
> These keybindings are configurable through [WebUI](#Configuration).

> [!TIP]
> You can use an external keyboard on Android.

| Freecam        | Keybind                                    |
| -------------- | ------------------------------------------ |
| Toggle Freecam | Enter                                      |
| Movement       | WASD, Ctrl, Space, and Shift_L (Sprint)    |
| Rotation       | Mouse / Arrow keys / Touch screen (Moblie) |
| UI Mode        | Mouse middle button / U                    |
| Zoom           | Hold Z + Mouse Wheel / X or C              |
| Roll Camera    | Q/E                                        |
| Reset Camera   | R                                          |

> Initially, the Anchor coincides with the Camera.\
> In all cases, the Camera moves with and rotates around the Anchor.\
> With the Anchor fixed, the relative position between Camera and Anchor can be changed through movement.\
> Attach Mode sets the target object as Anchor. (By default, the object corresponding to the center of the screen is selected as the target.)

| Anchor             | Keybind     |
| ------------------ | ----------- |
| Pin Anchor         | Hold M      |
| Reset to Anchor    | Shift_L + M |
| Toggle Attach Mode | T           |

| Speed Hack                  | Keybind   |
| --------------------------- | -         |
| Speed up                    | +         |
| Speed down                  | -         |
| Freeze speed / Resume speed | Backspace |

## Tested Game

- [Blue Archive](https://youtu.be/40Od_dHH5oY)
- Muse Dash
- Toram Online (Moblie)
- Arknights: Endfield

## Special Thanks

- [UnityResolve.hpp](https://github.com/issuimo/UnityResolve.hpp)

## FAQ

### Crashes after injection with CE

Ensure that the directory containing the `freecam-x86_86-windows.dll` file is in a directory with only English characters.

### The game crashes on the first attempt

This is normal; please try again. If it still doesn't work, please submit an issue.

[^1]:For some modified/hardened engines, this need to be injected at the time of game startup. 
