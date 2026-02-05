#include "freecam/module.hpp"
#include "freecam/freecam.hpp"
#include "freecam/helper.hpp"

#include "umod/core.hpp"
#include "umod/debug/logger.hpp"
#include "umod/utype/unity_engine/input.hpp"
#include "umod/utype/unity_engine/time.hpp"
#include "user/config.hpp"

#include <optional>

using namespace std::chrono_literals;

namespace freecam
{
    using namespace umod::UTYPE::unity_engine;
    using namespace umod::debug;

    using namespace user_config;

    using enum user_config::freecam::keybind::KeyCode;

    namespace
    {
        static umod::core::player_loop::Handle playerLoop{};
        static std::optional<FreeCamera> freeCam{};
    }

    static void run()
    {
        // if (freeCam && !helper::isCurrentFreeCamera())
        // {
        //     freeCam->disable();
        //     freeCam = std::nullopt;
        // }

        if (Input::GetKeyDown(speed_gear::keybind::ToggleFreeze))
        {
            if (Time::GetTimeScale() <= 0)
            {
                logger::info("Set timescale: 1");
                Time::SetTimeScale(1);
            }
            else
            {
                logger::info("Set timescale: 0");
                Time::SetTimeScale(0);
            }
            std::this_thread::sleep_for(100ms); // For MockLoop use
        }
        if (Input::GetKeyDown(speed_gear::keybind::SpeedUp))
        {
            logger::info("Set timescale: +=1");
            Time::SetTimeScale(Time::GetTimeScale() + 1);
            std::this_thread::sleep_for(100ms); // For MockLoop use
        }
        if (Input::GetKeyDown(speed_gear::keybind::SpeedDown))
        {
            logger::info("Set timescale: -=1");
            Time::SetTimeScale(Time::GetTimeScale() - 1);

            std::this_thread::sleep_for(100ms); // For MockLoop use
        }
        if (Input::GetKeyDown(user_config::freecam::keybind::ToggleFreeCam))
        {
            if (freeCam)
            {
                freeCam->disable();
                freeCam = std::nullopt;
            }
            else
            {
                freeCam = FreeCamera::create(user_config::freecam::Mode);
                freeCam->enable();
            }
            return;
        }

        if (freeCam) freeCam->update();
    }

    void load() { playerLoop = umod::core::player_loop::attach(run); }

    void unload() { playerLoop.dettach(); }
}
