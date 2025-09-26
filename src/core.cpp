#include "core.hpp"
#include "debug/logger.hpp"
#include "proxy/camera.hpp"
#include "utype/input.hpp"
#include "utype/time.hpp"

namespace FreeCam
{
    using FreeCamera = Proxy::Camera;
    using UType::Input;
    using enum UType::KeyCode;

    using namespace std::chrono_literals;

    auto Core::Update() -> void
    {
        if (freeCam->Enabled && !freeCam->IsCurrentFreeCamera()) return freeCam->Disable();

        if (Input::GetKeyDown(Backspace))
        {
            if (UTYPE::Time::GetTimeScale() <= 0)
            {
                Debug::Logger::LOGI("Set timescale: 1");
                UTYPE::Time::SetTimeScale(1);
            }
            else
            {
                Debug::Logger::LOGI("Set timescale: 0");
                UTYPE::Time::SetTimeScale(0);
                Core::UseMockLoop = true; // A bit tricky, but this ensure camera can still move even when timescale is 0
            }
            std::this_thread::sleep_for(100ms); // For MockLoop use
        }
        if (Input::GetKeyDown(Plus))
        {
            Debug::Logger::LOGI("Set timescale: +=1");
            UTYPE::Time::SetTimeScale(UTYPE::Time::GetTimeScale() + 1);
            std::this_thread::sleep_for(100ms);// For MockLoop use
        }
        if (Input::GetKeyDown(Minus))
        {
            Debug::Logger::LOGI("Set timescale: -=1");
            UTYPE::Time::SetTimeScale(UTYPE::Time::GetTimeScale() - 1);
            std::this_thread::sleep_for(100ms);// For MockLoop use
            if (UType::Time::GetTimeScale() <= 0) Core::UseMockLoop = true; // A bit tricky, but this ensure camera can still move even when timescale is 0
        }
        if (Input::GetKeyDown(Enter))
        {
            return freeCam->Enabled ? freeCam->Disable() : freeCam->Enable();
        }

        if (freeCam->Enabled) freeCam->Update();
    }
}
