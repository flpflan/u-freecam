#include "umod/core.hpp"
#include "umod/debug/logger.hpp"
#include "umod/memory/hook.hpp"
#include "umod/runtime/UnityResolve.hpp"
#include "umod/utils/container.hpp"
#include "umod/utype/unity_engine/time.hpp"

#include "user/config.hpp"

#include <memory>

namespace umod::core::player_loop
{
    namespace
    {
        static utils::container::CowSeq<void (*)()> kUpdateLoops;
        // static CowSeq<void (*)()> kFixedUpdateLoops;
        // static CowSeq<void (*)()> kLateUpdateLoops;

        // static void runAttached(Index index)
        // {
        //     const auto loops = index == Index::Update        ? kUpdateLoops.snapshot()
        //                        : index == Index::FixedUpdate ? kFixedUpdateLoops.snapshot()
        //                                                      : kLateUpdateLoops.snapshot();
        //     for (const auto &loop : *loops)
        //     {
        //         loop();
        //     }
        // }

        static void runAttached()
        {
            for (const auto &loop : *kUpdateLoops.snapshot())
            {
                loop();
            }
        }

        template <auto &&CALL_ORIGNAL>
        static void detour_update(void *obj, Index index)
        {
            CALL_ORIGNAL(obj, index);

            static int lastFrame = -1;
            const int curFrame = umod::UTYPE::unity_engine::Time::get_frameCount();
            if (curFrame == lastFrame || index != Index::Update) return;
            // runAttached(index);
            runAttached();
            lastFrame = curFrame;
        }
    }

    namespace manager
    {
        static void (*kPlayerLoop)(void *, Index){};
        static bool Attached{};
        static bool isMockLoop_{};
        static void attach()
        {
            if (Attached) return;
            debug::logger::info("Try attaching to internal player loop");
            if (Hook(kPlayerLoop, (detour_update<CALL_ORIGNAL>)))
                debug::logger::info("Hooking success");
            else
            {
                debug::logger::warn("Failed attaching to internal loop, fallback to mock loop");
                isMockLoop_ = true;
                std::thread(
                    []
                    {
                        UnityResolve::ThreadAttach();
                        debug::logger::info("Mock loop started");
                        do
                        {
                            runAttached();
                            std::this_thread::sleep_for(user::config::core::MockLoopDeltaTime);
                        } while (1);
                        UnityResolve::ThreadDetach();
                    })
                    .detach();
            }
        }
        static void dettach()
        {
            if (!Attached) return;
            debug::logger::info("Dettached from internal player loop");
            UnHook(kPlayerLoop);
        }
    }

    // void Handle::dettach()
    // {
    //     if (index == Index::Update)
    //         kUpdateLoops.remove(id);
    //     else if (index == Index::FixedUpdate)
    //         kFixedUpdateLoops.remove(id);
    //     else
    //         kLateUpdateLoops.remove(id);
    // }
    void Handle::dettach()
    {
        kUpdateLoops.remove(id);
        // INFO: Dettach from PlayerLoop if no update hook
        if (kUpdateLoops.empty()) manager::dettach();
    }

    // Handle attach(Index index, void (*target)())
    // {
    //     if (index == Index::Update)
    //         kUpdateLoops.push(target);
    //     else if (index == Index::FixedUpdate)
    //         kFixedUpdateLoops.push(target);
    //     else
    //         kLateUpdateLoops.push(target);
    //     return {target, index};
    // }
    Handle attach(void (*target)())
    {
        // INFO: Attach to PlayerLoop if not yet
        if (!manager::Attached) manager::attach();
        kUpdateLoops.push(target);
        return {target, Index::Update};
    }

    const bool isMockLoop() { return manager::isMockLoop_; }

    void init(void *playerLoop) { manager::kPlayerLoop = reinterpret_cast<void (*)(void *, Index)>(playerLoop); }
}
