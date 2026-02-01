#pragma once

#include <optional>
#ifdef __ANDROID__
#include <memory>
#include <string_view>

namespace platform::android
{
    struct ASymbolQuery
    {
    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
        ASymbolQuery() = delete;

    public:
        static std::optional<ASymbolQuery> fromModule(const std::string_view &symName);
        static std::optional<ASymbolQuery> fromSymbol(const std::string_view &symName);
        static void* resolve(const ASymbolQuery &query, const std::string_view &symName);
    };
}
#endif // __ANDROID__
