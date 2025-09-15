#pragma once

#include "unity_side.hpp"
#include <fstream>

namespace FreeCam::Debug {

    template <typename... Args>
    static void LOG(Args &&...args) {
        static auto f = std::fstream();
        if(!f.is_open())
            f.open("/sdcard/Android/data/com.nexon.bluearchive/cache/debug.txt", std::ios::out | std::ios::app);
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

} // namespace FreeCamDebug
