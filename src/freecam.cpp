#include "freecam.hpp"
#include "freecam/camera_proxy.hpp"
#include "freecam/cursor_proxy.hpp"
#include "unity_side.hpp"
#include "debug.hpp"

using namespace std::chrono_literals;

namespace FreeCam
{
    auto FreeCam::Hack() -> void
    {
        UnityResolve::ThreadAttach();

        FreeCam freecam;
        std::thread(
            [&freecam]
            {
                while (true)
                {
                    if (freecam.isFreeCamBegin && !CameraProxy::IsCurrentFreeCamera()) freecam.EndFreeCam();
                    std::this_thread::sleep_for(1s);
                }
            })
            .detach();

        Debug::LOG("main loop start");
        while (true)
        {
            if (UnityApi::GetKeyDown(Backspace))
            {
                if (UTYPE::Time::GetTimeScale() <= 0)
                {
                    Debug::LOG("set timescale: 1");
                    UTYPE::Time::SetTimeScale(1);
                }
                else
                {
                    Debug::LOG("set timescale: 0");
                    UTYPE::Time::SetTimeScale(0);
                }
                std::this_thread::sleep_for(100ms);
            }
            if (UnityApi::GetKeyDown(Plus))
            {
                Debug::LOG("set timescale: +=1");
                UTYPE::Time::SetTimeScale(UTYPE::Time::GetTimeScale() + 1);
                std::this_thread::sleep_for(100ms);
            }
            if (UnityApi::GetKeyDown(Minus))
            {
                Debug::LOG("set timescale: -=1");
                UTYPE::Time::SetTimeScale(UTYPE::Time::GetTimeScale() - 1);
                std::this_thread::sleep_for(100ms);
            }
            if (UnityApi::GetKeyDown(Enter))
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

    auto FreeCam::BeginFreeCam() -> void
    {
        Debug::LOG("start freecam");
        if (isFreeCamBegin) return;

        isFreeCamBegin = true;

        // Backup
        originCamera = UTYPE::Camera::GetMain();
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

            freeCamera = UnityApi::AddComponent<UTYPE::Camera>(go, originCamera->GetType());
        }

        UnityApi::SetTag(freeCameraGObject, "MainCamera");
        freeCameraGObject->SetActive(true);

        static UTYPE::Transform *freeTransform;
        if (!freeTransform) freeTransform = freeCamera->GetTransform();
        freeTransform->SetPosition(originPosition);
        freeTransform->SetRotation(originRotation);

        // Set Cursor
        CursorProxy::DisableCursor();

        // Start Listen Keys
        std::thread(
            [this]
            {
                this->listenKeys = true;
                this->StartListenKeys();
            })
            .detach();
    }

    auto FreeCam::EndFreeCam() -> void
    {
        Debug::LOG("end freecam");
        if (!isFreeCamBegin) return;

        isFreeCamBegin = false;

        if (freeCamera)
        {
            freeCameraGObject->SetActive(false);
            const auto freeTransform = freeCamera->GetTransform();
            freeTransform->SetPosition(originPosition);
            freeTransform->SetRotation(originRotation);

            UnityApi::SetTag(freeCameraGObject, "Bulabula");
        }

        if (originGObject)
        {
            const auto curCam = UTYPE::Camera::GetMain();
            if (CameraProxy::IsCurrentFreeCamera())
            {
                originGObject->SetActive(true);
                const auto originTransform = originCamera->GetTransform();
                originTransform->SetPosition(originPosition);
                originTransform->SetRotation(originRotation);
            }
        }

        CursorProxy::EnableCursor();
        listenKeys = false;
    }
    auto FreeCam::StartListenKeys() -> void
    {
        Debug::LOG("start listen keys");

        CameraProxy camera(freeCamera);

        auto ui_layer = false;
        while (listenKeys)
        {
            if (!ui_layer)
            {
                UTYPE::Vector3 toMove(0, 0, 0);
                // toMove.x = UnityApi::GetAxis("Horizontal");
                // toMove.y = UnityApi::GetAxis("Vertical");
                if (UnityApi::GetKey(Space)) toMove.z = 1;
                if (UnityApi::GetKey(Ctrl_L)) toMove.z = -1;
                if (UnityApi::GetKey(W)) toMove.y = 1;
                if (UnityApi::GetKey(S)) toMove.y = -1;
                if (UnityApi::GetKey(A)) toMove.x = -1;
                if (UnityApi::GetKey(D)) toMove.x = 1;
                if (toMove.x || toMove.y || toMove.z) camera.Move(toMove, UnityApi::GetKey(SHIFT_L));

                UTYPE::Vector2 toRotate(0, 0);
                toRotate.x = UnityApi::GetAxis("Mouse X");
                toRotate.y = UnityApi::GetAxis("Mouse Y");
                if (UnityApi::GetKey(UpArrow)) toRotate.y = 1;
                if (UnityApi::GetKey(DownArrow)) toRotate.y = -1;
                if (UnityApi::GetKey(RightArrow)) toRotate.x = 1;
                if (UnityApi::GetKey(LeftArrow)) toRotate.x = -1;
                if (toRotate.x || toRotate.y) camera.Rotate(toRotate);
            }
            if (UnityApi::GetMouseButtonDown(2)||UnityApi::GetKeyDown(U))
            {
                CursorProxy::ToggleCursor();
                ui_layer = !ui_layer;
                std::this_thread::sleep_for(100ms);
            }

            std::this_thread::sleep_for(std::chrono::microseconds((int)DeltaTime_us));
        }
    }
}
