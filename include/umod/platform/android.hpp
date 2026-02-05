#pragma once

#ifdef __ANDROID__
#include <optional>
#include <string>

namespace umod::platform::android
{
    struct ASymbolQuery
    {
        enum class TYPE
        {
            Hijacked,
            Emulated,
            Native,
            Scanner,
            XDL,
        };
        TYPE ty;
        void *handle;

        ~ASymbolQuery();

        static std::optional<ASymbolQuery> fromModule(const std::string &symName);
        static std::optional<ASymbolQuery> fromSymbol(const std::string &symName);
        static ASymbolQuery fromHijacked(void *);
        void *resolve(const std::string &symName) const;
    };
}
#endif // __ANDROID__
