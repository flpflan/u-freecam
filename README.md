u-freecam
==========

English | [中文](./README.zh-CN.md)

----------

## What's This

As the name suggests, this a free camera tool for games built on the Unity engine. \
It also comes with an in-game speed change feature.

## Build (Windows)

### Requirement
- Visual Studio (With C++ toolchain and Cmake)

### Compile
Open this project directly in Visual Studio, select the `freecam-AMD64` target, and then generate it.

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

By any means, inject this dynamic library into the target process or application[^1]. \
For example, on Windows, you can use the DLL injection tool that comes with Cheat Engine (CE), while on Android, you can use [XInjector](https://github.com/WindySha/XInjector) (Non-root environments can use [Android-Virtual-Inject](https://github.com/reveny/Android-Virtual-Inject/releases/latest)).

## Keybindings

> ** You can use an external keyboard on Android. **

| Free Camera    | Keybind                                    |
| -------------- | ------------------------------------------ |
| Toggle FreeCam | Enter                                      |
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

[^1]:For some modified/hardened engines, this need to be injected at the time of game startup. 
