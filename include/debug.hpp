#pragma once

#include "unity_side.hpp"
#include <fstream>

inline static auto _f() -> std::ofstream& {
#if ANDROID_MODE
    static auto f = std::ofstream("/sdcard/Android/data/com.nexon.bluearchive/cache/debug.txt", std::ios::out | std::ios::trunc);
#else
    static auto f = std::ofstream("debug.txt", std::ios::out | std::ios::trunc);
#endif
    return f;
}
namespace Debug {
    template <typename... Args>
    void LOG(Args &&...args) {
        auto& f = _f();
        ((f << args << " "), ...);
        f << std::endl;
    }

    static void TestKey() {
        for (int i = 0; i <= 500; i++) {
            if (UnityApi::GetKey((KeyCode)i)) {
                LOG(i);
            }
        }
    }
}
