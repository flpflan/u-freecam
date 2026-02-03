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
        else if (ty == Base)
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
        if ((handle = dlopen(path.c_str(), RTLD_LAZY))) return ASymbolQuery{Native, handle};
        return std::nullopt;
    }

    std::optional<ASymbolQuery> ASymbolQuery::fromSymbol(const std::string &symName)
    {
        for (auto &it : ElfScanner::findSymbolAll(symName, EScanElfType::Any, EScanElfFilter::App))
        {
            if (it.second.dynamic())
            {
                return ASymbolQuery{Base, new ElfScanner{it.second}};
            }
        }
        return std::nullopt;
    }

    void *ASymbolQuery::resolve(const std::string &symName) const
    {
        if (symName.empty()) return nullptr;

        void *symbol{};
        switch (ty)
        {
        case Hijacked:
            if ((symbol = dlsym(this->handle, symName.c_str()))) return symbol;
            break;
        case Emulated:
            if ((symbol = NativeBridgeLinker::dlsym(this->handle, symName))) return symbol;
            break;
        case Native:
            if ((symbol = dlsym(this->handle, symName.c_str()))) return symbol;
            break;
        case Base:
            if ((symbol = (void *)static_cast<ElfScanner *>(this->handle)->findSymbol(symName))) return symbol;
            break;
        case XDL:
            if ((symbol = xdl_sym(this->handle, symName.c_str(), NULL))) return symbol;
            break;
        }
    }

#include <jni.h>

    inline JavaVM *sJavaVM;

    template <auto &&CALL_ORIGNAL>
    inline jint _Hook_JNI_OnLoad(JavaVM *vm, void *reserved)
    {
        sJavaVM = vm;
        return CALL_ORIGNAL(vm, reserved);
    }
}
