#pragma once

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <optional>

namespace json
{
    struct Json
    {
        using Object = std::unordered_map<std::string, Json>;
        using Array = std::vector<Json>;

        template <typename T>
        std::optional<T> as() const
        {
            if (const auto v = std::get_if<T>(&value_)) return *v;
            return std::nullopt;
        }

    private:
        using Value = std::variant<std::nullptr_t, bool, double, std::string, Array, Object>;
        Value value_;

    public:
        Json(Value value) : value_(value) {}
    };

    std::optional<Json> parse(const std::string &);
}
