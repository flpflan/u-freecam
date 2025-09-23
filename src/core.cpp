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

    auto Core::Update() -> void
    {
        if (IsFreeCamBegin && !FreeCamera::IsCurrentFreeCamera()) return EndFreeCam();

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
            if (IsFreeCamBegin)
            {
                return EndFreeCam();
            }
            else
            {
                return BeginFreeCam();
            }
        }

        if (IsFreeCamBegin) freeCamera->Update();
    }

    auto Core::BeginFreeCam() -> void
    {
        Debug::Logger::LOGI("Start freecam");
        if (IsFreeCamBegin) return;

        IsFreeCamBegin = true;

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

            freeCamera = go->AddComponent<FreeCamera *>(FreeCamera::GetUClass());
        }

        freeCameraGObject->SetTag("MainCamera");
        freeCameraGObject->SetActive(true);

        static UTYPE::Transform *freeTransform;
        if (!freeTransform) freeTransform = freeCamera->GetTransform();
        freeTransform->SetPosition(originPosition);
        freeTransform->SetRotation(originRotation);

        // Set Cursor
        Proxy::Cursor::DisableCursor();
    }

    auto Core::EndFreeCam() -> void
    {
        Debug::Logger::LOGI("End freecam");
        if (!IsFreeCamBegin) return;

        IsFreeCamBegin = false;

        if (freeCamera)
        {
            freeCameraGObject->SetActive(false);
            const auto freeTransform = freeCamera->GetTransform();
            freeTransform->SetPosition(originPosition);
            freeTransform->SetRotation(originRotation);

            freeCameraGObject->SetTag("Bulabula");

            freeCamera->Disable();
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
    }
}
