#pragma once

#include <optional>
#ifdef __ANDROID__

#include "KittyMemory/KittyInclude.hpp"
#include "xdl.h"
#include <dlfcn.h>
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

inline std::optional<Handle> A_get_handle(const char *path)
{
    void *handle{};
    // Emulator
    if ((handle = NativeBridgeLinker::dlopen(path, RTLD_LAZY))) return Handle{handle, Handle::Emulated};
    // Real device
    if ((handle = xdl_open(path, XDL_DEFAULT))) return Handle{handle, Handle::XDL};
    if ((handle = dlopen(path, RTLD_LAZY))) return Handle{handle, Handle::Native};
    return std::nullopt;
}

inline void *A_symbol_resolve(const Handle *handle, const char *sym_name)
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
    [[unlikely]] return nullptr;
}

inline std::optional<Handle> GetMoudleFromSymbol(const char *sym_name)
{
    for (auto &it : ElfScanner::findSymbolAll(sym_name, EScanElfType::Any, EScanElfFilter::App))
    {
        if (it.second.dynamic())
        {
            return Handle{it.second, Handle::Base};
        }
    }
    return std::nullopt;
}

#endif // __ANDROID__
