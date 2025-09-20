#include "core.hpp"
#include "debug/logger.hpp"
#include "proxy/camera.hpp"
#include "proxy/cursor.hpp"
#include "utype/input.hpp"

namespace FreeCam
{
    using FreeCamera = Proxy::Camera;
    using UType::Input;
    using enum UType::KeyCode;

    using namespace std::chrono_literals;

    auto Core::Hack() -> void
    {
        UnityResolve::ThreadAttach();

        Core freecam;
        std::thread(
            [&freecam]
            {
                while (true)
                {
                    if (freecam.isFreeCamBegin && !FreeCamera::IsCurrentFreeCamera()) freecam.EndFreeCam();
                    std::this_thread::sleep_for(1s);
                }
            })
            .detach();

        Debug::Logger::LOGI("Main loop start");
        while (true)
        {
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
                }
                std::this_thread::sleep_for(100ms);
            }
            if (Input::GetKeyDown(Plus))
            {
                Debug::Logger::LOGI("Set timescale: +=1");
                UTYPE::Time::SetTimeScale(UTYPE::Time::GetTimeScale() + 1);
                std::this_thread::sleep_for(100ms);
            }
            if (Input::GetKeyDown(Minus))
            {
                Debug::Logger::LOGI("Set timescale: -=1");
                UTYPE::Time::SetTimeScale(UTYPE::Time::GetTimeScale() - 1);
                std::this_thread::sleep_for(100ms);
            }
            if (Input::GetKeyDown(Enter))
            {
                if (freecam.isFreeCamBegin)
                {
                    freecam.EndFreeCam();
                }
                else
                {
                    freecam.BeginFreeCam();
                }
                std::this_thread::sleep_for(100ms);
            }

            std::this_thread::sleep_for(std::chrono::microseconds((int)DeltaTime_us));
        }
        UnityResolve::ThreadDetach();
    }

    auto Core::BeginFreeCam() -> void
    {
        Debug::Logger::LOGI("Start freecam");
        if (isFreeCamBegin) return;

        isFreeCamBegin = true;

        // Backup
        originCamera = UType::Camera::GetMain();
        const auto originTransform = originCamera->GetTransform();
        originPosition = originTransform->GetPosition();
        originRotation = originTransform->GetRotation();
        originGObject = originCamera->GetGameObject();
        originGObject->SetActive(false);

        // Setup free camera
        if (!freeCamera)
        {
            const auto go = UTYPE::GameObject::Create("UE_Freecam");
            freeCameraGObject = go;
            UTYPE::GameObject::DontDestroyOnLoad(go);

            freeCamera = go->AddComponent<UType::Camera *>(UType::Camera::GetUClass());
        }

        freeCameraGObject->SetTag("MainCamera");
        freeCameraGObject->SetActive(true);

        static UTYPE::Transform *freeTransform;
        if (!freeTransform) freeTransform = freeCamera->GetTransform();
        freeTransform->SetPosition(originPosition);
        freeTransform->SetRotation(originRotation);

        // Set Cursor
        Proxy::Cursor::DisableCursor();

        // Start Listen Keys
        std::thread(
            [this]
            {
                this->listenKeys = true;
                this->StartListenKeys();
            })
            .detach();
    }

    auto Core::EndFreeCam() -> void
    {
        Debug::Logger::LOGI("End freecam");
        if (!isFreeCamBegin) return;

        isFreeCamBegin = false;

        if (freeCamera)
        {
            freeCameraGObject->SetActive(false);
            const auto freeTransform = freeCamera->GetTransform();
            freeTransform->SetPosition(originPosition);
            freeTransform->SetRotation(originRotation);

            freeCameraGObject->SetTag("Bulabula");
        }

        if (originGObject)
        {
            const auto curCam = UTYPE::Camera::GetMain();
            if (FreeCamera::IsCurrentFreeCamera())
            {
                originGObject->SetActive(true);
                const auto originTransform = originCamera->GetTransform();
                originTransform->SetPosition(originPosition);
                originTransform->SetRotation(originRotation);
            }
        }

        Proxy::Cursor::EnableCursor();
        listenKeys = false;
    }
    auto Core::StartListenKeys() -> void
    {
        Debug::Logger::LOGI("Start listen keys");

        FreeCamera camera(freeCamera);

        auto ui_layer = false;
        // auto zoom_mode = false;
        while (listenKeys)
        {
            if (!ui_layer)
            {
                // const bool toZoom = Input::GetKey(Z);
                // if (!toZoom && zoom_mode) camera.ResetZoom();
                // zoom_mode = toZoom;
                // if (zoom_mode)
                // {
                //     const float mouseCenter = Input::GetAxis("Mouse ScrollWheel");
                //     if (mouseCenter < 0)
                //     {
                //         camera.ZoomOut(mouseCenter);
                //     }
                //     else
                //     {
                //         camera.ZoomIn(mouseCenter);
                //     };
                // }

                UTYPE::Vector3 toMove(0, 0, 0);
                // toMove.x = UnityApi::GetAxis("Horizontal");
                // toMove.y = UnityApi::GetAxis("Vertical");
                if (Input::GetKey(Space)) toMove.z = 1;
                if (Input::GetKey(Ctrl_L)) toMove.z = -1;
                if (Input::GetKey(W)) toMove.y = 1;
                if (Input::GetKey(S)) toMove.y = -1;
                if (Input::GetKey(A)) toMove.x = -1;
                if (Input::GetKey(D)) toMove.x = 1;
                if (toMove.x || toMove.y || toMove.z) camera.Move(toMove, Input::GetKey(SHIFT_L));

                UTYPE::Vector2 toRotate(0, 0);
                toRotate.x = Input::GetAxis("Mouse X");
                toRotate.y = Input::GetAxis("Mouse Y");
                if (Input::GetKey(UpArrow)) toRotate.y = 1;
                if (Input::GetKey(DownArrow)) toRotate.y = -1;
                if (Input::GetKey(RightArrow)) toRotate.x = 1;
                if (Input::GetKey(LeftArrow)) toRotate.x = -1;
                if (toRotate.x || toRotate.y) camera.Rotate(toRotate);
            }
            if (Input::GetMouseButtonDown(2) || Input::GetKeyDown(U))
            {
                Proxy::Cursor::ToggleCursor();
                ui_layer = !ui_layer;
                std::this_thread::sleep_for(100ms);
            }

            std::this_thread::sleep_for(std::chrono::microseconds((int)DeltaTime_us));
        }
    }
}
