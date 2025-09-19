#pragma once

#include "UnityResolve.hpp"
#include "logger.hpp"
#include "unity_side.hpp"
#include <thread>

using UTYPE = UnityResolve::UnityType;
using UMethod = UnityResolve::Method;

namespace Debug
{
    static std::string WStringToString(const std::wstring &wstr)
    {
        std::string str;
        for (auto wc : wstr)
        {
            if (wc <= 0x7F)
            {
                str.push_back(static_cast<char>(wc));
            }
            else if (wc <= 0x7FF)
            {
                str.push_back(static_cast<char>(0xC0 | ((wc >> 6) & 0x1F)));
                str.push_back(static_cast<char>(0x80 | (wc & 0x3F)));
            }
            else if (wc <= 0xFFFF)
            {
                str.push_back(static_cast<char>(0xE0 | ((wc >> 12) & 0x0F)));
                str.push_back(static_cast<char>(0x80 | ((wc >> 6) & 0x3F)));
                str.push_back(static_cast<char>(0x80 | (wc & 0x3F)));
            }
            else
            {
                str.push_back(static_cast<char>(0xF0 | ((wc >> 18) & 0x07)));
                str.push_back(static_cast<char>(0x80 | ((wc >> 12) & 0x3F)));
                str.push_back(static_cast<char>(0x80 | ((wc >> 6) & 0x3F)));
                str.push_back(static_cast<char>(0x80 | (wc & 0x3F)));
            }
        }
        return str;
    }
    static void TestKey()
    {
        for (int i = 0; i <= 500; i++)
        {
            if (UnityApi::GetKey((KeyCode)i))
            {
                Logger::LOGD("Keycode: {}", i);
            }
        }
    }
    class Test
    {
    public:
        inline static auto Start() -> void { std::thread(run).detach(); }

    private:
        inline static auto run() -> void
        {
            using namespace std::chrono_literals;
            auto kScreen = UnityResolve::Get("UnityEngine.CoreModule.dll")->Get("Screen");
            auto getScreenWidth = kScreen->Get<UMethod>("get_width")->Cast<int>();
            auto getScreenHeight = kScreen->Get<UMethod>("get_height")->Cast<int>();
            auto getCollider = UnityResolve::Get("UnityEngine.PhysicsModule.dll")->Get("RaycastHit")->Get<UMethod>("get_collider")->Cast<UTYPE::Collider *, UTYPE::RaycastHit *>();
            UTYPE::Vector2 screenCenter(getScreenWidth() / 2.f, getScreenHeight() / 2.f);
            auto cam = UTYPE::Camera::GetMain();
            auto ray = cam->ScreenPointToRay(screenCenter);
            while (true)
            {
                UTYPE::RaycastHit hit;
                if (UTYPE::Physics::Raycast(ray, &hit, 100.f))
                {
                    auto go = getCollider(&hit)->GetGameObject();
                    Logger::LOGD("Gameobject Detected: {}", WStringToString(go->GetName()->m_firstChar));
                    Logger::LOGD("1");
                    dumpGameobject(go);
                    Logger::LOGD("e");

                    std::this_thread::sleep_for(1s);
                }
                std::this_thread::sleep_for(17ms);
            }
        }
        inline static auto dumpGameobject(UTYPE::GameObject *go) -> void
        {
            auto components = go->GetComponents<UTYPE::Component>(UnityResolve::Get("UnityEngine.CoreModule.dll")->Get("Component"));
            Logger::LOGD("2");
            for (auto c : components)
            {
                Logger::LOGD("3");
                auto x = c.GetType();
                Logger::LOGD("4");
                auto y = x->GetFullName();
                Logger::LOGD("c is null: {}", y == nullptr);
                if (!y) continue;
                Logger::LOGD("c name: {}", WStringToString(y->m_firstChar));
            }
        }
    };
}
