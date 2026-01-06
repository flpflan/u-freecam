#pragma once

#include "KittyMemory/KittyInclude.hpp"
#include "xdl.h"
#include <dlfcn.h>
#include <memory>
#include <variant>

struct Handle
{
    enum TYPE
    {
        Hijacked,
        Emulated,
        Native,
        Base,
        XDL,
    };
    std::variant<void *, ElfScanner> handle;
    TYPE ty;
};

inline std::unique_ptr<Handle> A_get_handle(const char *path)
{
    void *handle{};
    // Emulator
    if ((handle = NativeBridgeLinker::dlopen(path, RTLD_NOW))) return std::make_unique<Handle>(Handle{handle, Handle::Emulated});
    // Real device
    if ((handle = xdl_open(path, XDL_DEFAULT))) return std::make_unique<Handle>(Handle{handle, Handle::XDL});
    if ((handle = dlopen(path, RTLD_NOW))) return std::make_unique<Handle>(Handle{handle, Handle::Native});
}

inline void *A_symbol_resolve(Handle *handle, const char *sym_name)
{
    void *symbol{};
    if (!handle || !sym_name) return nullptr;
    switch (handle->ty)
    {
    case Handle::Hijacked:
        if ((symbol = dlsym(std::get<void *>(handle->handle), sym_name))) return symbol;
        break;
    case Handle::Emulated:
        if ((symbol = NativeBridgeLinker::dlsym(std::get<void *>(handle->handle), sym_name))) return symbol;
        break;
    case Handle::Native:
        if ((symbol = dlsym(std::get<void *>(handle->handle), sym_name))) return symbol;
        break;
    case Handle::Base:
        if ((symbol = (void *)std::get<ElfScanner>(handle->handle).findSymbol(sym_name))) return symbol;
        break;
    case Handle::XDL:
        if ((symbol = xdl_sym(std::get<void *>(handle->handle), sym_name, NULL))) return symbol;
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

inline std::unique_ptr<Handle> GetMoudleFromSymbol(const char *sym_name)
{
    for (auto &it : ElfScanner::findSymbolAll(sym_name, EScanElfType::Any, EScanElfFilter::App))
    {
        if (it.second.dynamic())
        {
            return std::make_unique<Handle>(Handle{it.second, Handle::Base});
        }
    }
}
