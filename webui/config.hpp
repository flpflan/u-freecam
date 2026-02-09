#pragma once

#include "json.hpp"

#include <string>

namespace user_config::webui
{
    inline int Port = 23333;
}

namespace user_config
{
    std::string schema();

    namespace loader
    {
        std::string to_json();
        void from_json(const json::Json &data);
    }

    namespace core
    {
        std::string to_json();
        void from_json(const json::Json &data);
    }

    namespace freecam
    {
        namespace property
        {
            std::string to_json();
            void from_json(const json::Json &data);
        }

        namespace keybind
        {
            std::string to_json();
            void from_json(const json::Json &data);
        }

        std::string to_json();
        void from_json(const json::Json &data);
    }

    namespace speed_gear
    {
        namespace keybind
        {
            std::string to_json();
            void from_json(const json::Json &data);
        }

        std::string to_json();
        void from_json(const json::Json &data);
    }

    std::string to_json();
    void from_json(const json::Json &data);
}
