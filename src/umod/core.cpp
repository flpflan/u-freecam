#include "core.hpp"
#include "debug/logger.hpp"
#include "proxy/camera.hpp"
#include "proxy/time.hpp"
#include "utype/unity_engine/core.hpp"
#include "utype/unity_engine/input.hpp"
#include "utype/unity_engine/time.hpp"
#include "utype/unity_engine/ui.hpp"

namespace FreeCam
{
    using FreeCamera = Proxy::Camera;
    using UTYPE::Input;
    using enum UTYPE::KeyCode;

    using namespace std::chrono_literals;

    auto Core::Update() -> void
    {
        // if (freeCam.Enabled && !freeCam.IsCurrentFreeCamera()) return freeCam.Disable();

        static UnityResolve::Method *method;
        if (!method)
            method = UnityResolve::Get("mscorlib.dll")
                         ->Get("Type", "System", "MemberInfo")
                         ->Get<UnityResolve::Method>("get_NameOrDefault");
        const auto dump_camera = [](UTYPE::Camera *cam)
        {
            Debug::Logger::Debug("type: {}", method->Invoke<UTYPE::String *>(cam->GetType())->ToString());
            if (const auto name = cam->GetName()) Debug::Logger::Debug("name: {}", name->ToString());
            if (const auto tag = cam->GetTag()) Debug::Logger::Debug("tag: {}", tag->ToString());
            Debug::Logger::Debug("fov: {}", cam->GetFoV());
            Debug::Logger::Debug("depth: {}", cam->GetDepth());
            Debug::Logger::Debug("clear flags: {}", (int)(cam->get_clearFlags()));
            Debug::Logger::Debug("orthographic: {}", cam->get_orthographic());
            const auto pos = cam->GetTransform()->GetPosition();
            Debug::Logger::Debug("position: x: {}, y:{}, z: {}\n", pos.x, pos.y, pos.z);
        };
        if (Input::GetKeyDown(P))
        {
            const auto cam = Feature::FreeCamera::GetMaxDepthCamera();
            cam->set_orthographic(!cam->get_orthographic());
            dump_camera(cam);
        }
        // TODO:
        // {
        //     const auto zoom_in = [](float am)
        //     {
        //         auto currentZoom = Feature::FreeCamera::GetMaxDepthCamera()->get_orthographicSize();
        //         currentZoom -= am * 4.f * Proxy::Time::GetDeltaTime_s();
        //         Feature::FreeCamera::GetMaxDepthCamera()->set_orthographicSize(currentZoom);
        //     };
        //     const auto zoom_out = [&](float am) { return zoom_in(-am); };
        //     static bool zoom_mode = false;
        //     static float backupZoom = 0;
        //     if (Input::GetKeyDown(Z)) backupZoom = Feature::FreeCamera::GetMaxDepthCamera()->get_orthographicSize();
        //     const bool toZoom = Input::GetKey(Z);
        //     if (!toZoom && zoom_mode) Feature::FreeCamera::GetMaxDepthCamera()->set_orthographicSize(backupZoom);
        //     zoom_mode = toZoom;
        //     if (zoom_mode)
        //     {
        //         if (Input::GetKey(X)) zoom_in(1);
        //         if (Input::GetKey(C)) zoom_out(1);
        //     }
        // }
        if (Input::GetKeyDown(J))
        {
            const auto cam = Feature::FreeCamera::GetMaxDepthCamera();
            cam->set_clearFlags((UTYPE::Camera::CameraClearFlags)(((int)cam->get_clearFlags() % 4) + 1));
            dump_camera(cam);
        }
        if (Input::GetKeyDown(H))
        {
            for (const auto cam : UTYPE::Camera::GetAllCamera())
            {
                dump_camera((UTYPE::Camera *)cam);
            }
        }
        if (Input::GetKeyDown(K))
        {
            for (const auto cam : UTYPE::Camera::GetAllCamera())
            {
                if (cam->GetName()->ToString() != "UE_Freecam") cam->GetGameObject()->SetActive(false);
                Debug::Logger::Debug("disable builtin cameraes");
            }
        }
        if (Input::GetKeyDown(L))
        {
            const auto canvases = UTYPE::Canvas::GetUClass()->FindObjectsByType<UTYPE::Canvas *>();
            for (const auto canvas : canvases)
            {
                Debug::Logger::Debug("canvas: {}", canvas->GetName()->ToString());
                Debug::Logger::Debug("render mode: {}", (int)canvas->get_renderMode());
            }
        }
        if (Input::GetKeyDown(Backspace))
        {
            if (UTYPE::Time::GetTimeScale() <= 0)
            {
                Debug::Logger::Info("Set timescale: 1");
                UTYPE::Time::SetTimeScale(1);
            }
            else
            {
                Debug::Logger::Info("Set timescale: 0");
                UTYPE::Time::SetTimeScale(0);
                Core::UseMockLoop =
                    true; // A bit tricky, but this ensure camera can still move even when timescale is 0
            }
            std::this_thread::sleep_for(100ms); // For MockLoop use
        }
        if (Input::GetKeyDown(Plus))
        {
            Debug::Logger::Info("Set timescale: +=1");
            UTYPE::Time::SetTimeScale(UTYPE::Time::GetTimeScale() + 1);
            std::this_thread::sleep_for(100ms); // For MockLoop use
        }
        if (Input::GetKeyDown(Minus))
        {
            Debug::Logger::Info("Set timescale: -=1");
            UTYPE::Time::SetTimeScale(UTYPE::Time::GetTimeScale() - 1);
            if (UTYPE::Time::GetTimeScale() <= 0)
                Core::UseMockLoop =
                    true; // A bit tricky, but this ensure camera can still move even when timescale is 0

            std::this_thread::sleep_for(100ms); // For MockLoop use
        }
        if (Input::GetKeyDown(Enter))
        {
            return freeCam.Enabled ? freeCam.Disable() : freeCam.Enable();
        }

        if (freeCam.Enabled) freeCam.Update();
    }
}
