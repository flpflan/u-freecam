# Build

## Windows

### Requirements

- Visual Studio (with C++ workload and CMake support)

### Build

Open the project in Visual Studio and build the `freecam-x86_64-windows` target.

## Android

### Requirements

- Android NDK 27+
- Cmake 3.22+
- Ninja (Optional)

### Build

```shell
export ANDROID_NDK=/path/to/ndk
export GENERATOR=Ninja

make release-aarch64
# or: release-x86_64, debug-aarch64, debug-x86_64
```

output: `build/release-android-aarch64/libfreecam-aarch64-android.so`

