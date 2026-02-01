#include "platform/android.hpp"

#include "KittyMemory/KittyInclude.hpp"
#include "xdl.h"

#include <dlfcn.h>
#include <memory>
#include <variant>

namespace platform::android
{
    struct ASymbolQuery::Impl
    {
        enum TYPE
        {
            Hijacked,
            Emulated,
            Native,
            Base,
            XDL,
        };
        void *handle;
        TYPE ty;
        ~Impl()
        {
            if (ty == Hijacked)
            {
            }
            else if (ty == Emulated)
                NativeBridgeScanner::dlclose(handle);
            else if (ty == Native)
                dlclose(handle);
            else if (ty == Base)
                delete (static_cast<ElfScanner *>(handle));
            else if (ty == XDL)
                xdl_close(handle);
        }
    };

    static ASymbolQuery ASymbolQuery::fromModule(const char *path)
    {
        void *handle{};
        // Emulator
        if ((handle = NativeBridgeLinker::dlopen(path, RTLD_LAZY))) return {{handle, Impl::Emulated}};
        // Real device
        if ((handle = xdl_open(path, XDL_DEFAULT))) return {{handle, Impl::XDL}};
        if ((handle = dlopen(path, RTLD_LAZY))) return {{handle, Impl::Native}};
    }

    static ASymbolQuery ASymbolQuery::fromSymbol(const string_view &symName)
    {
        for (auto &it : ElfScanner::findSymbolAll(symName, EScanElfType::Any, EScanElfFilter::App))
        {
            if (it.second.dynamic())
            {
                return {{it.second, Impl::Base}};
            }
        }
    }

    static void *ASymbolQuery::resolve(const ASymbolQuery &query, const string_view &symName)
    {
        if (!symName) return nullptr;
        const auto impl = query.impl_;

        void *symbol{};
        switch (impl.ty)
        {
        case Impl::Hijacked:
            if ((symbol = dlsym(impl.handle, symName))) return symbol;
            break;
        case Impl::Emulated:
            if ((symbol = NativeBridgeLinker::dlsym(impl.handle, symName))) return symbol;
            break;
        case Impl::Native:
            if ((symbol = dlsym(impl.handle, symName))) return symbol;
            break;
        case Impl::Base:
            if ((symbol = (void *)static_cast<ElfScanner *>(impl.handle)->findSymbol(symName))) return symbol;
            break;
        case Impl::XDL:
            if ((symbol = xdl_sym(impl.handle, symName, NULL))) return symbol;
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
