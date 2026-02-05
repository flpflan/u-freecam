#ifdef __ANDROID__

#include "umod/platform/android.hpp"

#include "KittyMemory/KittyInclude.hpp"
#include "xdl.h"

#include <dlfcn.h>

namespace umod::platform::android
{
    using enum ASymbolQuery::TYPE;

    ASymbolQuery::~ASymbolQuery()
    {
        if (ty == Hijacked)
        {
        }
        else if (ty == Emulated)
        {
            // NativeBridgeLinker::dlclose(handle);
        }
        else if (ty == Native)
            dlclose(handle);
        else if (ty == Scanner)
            delete (static_cast<ElfScanner *>(handle));
        else if (ty == XDL)
            xdl_close(handle);
    }

    std::optional<ASymbolQuery> ASymbolQuery::fromModule(const std::string &path)
    {
        void *handle{};
        // Emulator
        if ((handle = NativeBridgeLinker::dlopen(path, RTLD_LAZY))) return ASymbolQuery{Emulated, handle};
        // Real device
        if ((handle = xdl_open(path.c_str(), XDL_DEFAULT))) return ASymbolQuery{XDL, handle};
        // if ((handle = dlopen(path.c_str(), RTLD_LAZY))) return ASymbolQuery{Native, handle};
        return std::nullopt;
    }

    std::optional<ASymbolQuery> ASymbolQuery::fromSymbol(const std::string &symName)
    {
        for (auto &it : ElfScanner::findSymbolAll(symName, EScanElfType::Any, EScanElfFilter::App))
        {
            if (it.second.dynamic())
            {
                return ASymbolQuery{Scanner, new ElfScanner{it.second}};
            }
        }
        return std::nullopt;
    }

    ASymbolQuery ASymbolQuery::fromHijacked(void *handle) { return ASymbolQuery{Hijacked, handle}; }

    void *ASymbolQuery::resolve(const std::string &symName) const
    {
        if (!handle || symName.empty()) return nullptr;

        void *symbol{};
        switch (ty)
        {
        case Hijacked:
            if ((symbol = dlsym(handle, symName.c_str()))) return symbol;
            break;
        case Emulated:
            if ((symbol = NativeBridgeLinker::dlsym(handle, symName))) return symbol;
            break;
        case Native:
            if ((symbol = dlsym(handle, symName.c_str()))) return symbol;
            break;
        case Scanner:
            if ((symbol = (void *)static_cast<ElfScanner *>(handle)->findSymbol(symName))) return symbol;
            break;
        case XDL:
            if ((symbol = xdl_sym(handle, symName.c_str(), NULL))) return symbol;
            break;
        }
        [[unlikely]] return nullptr;
    }
}

#endif // __ANDROID__
