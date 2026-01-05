#pragma once

#include "KittyMemory/KittyInclude.hpp"
#include "xdl.h"
#include <dlfcn.h>

inline void *A_dlopen(const char *path, int flags)
{
    void *handle;
    // Emulator
    if ((handle = NativeBridgeLinker::dlopen(path, flags))) return handle;
    // Real device
    if ((handle = xdl_open(path, XDL_DEFAULT))) return handle;
}

inline void *A_dlsym(void *handle, const char *sym_name)
{
    void *symbol;
    // Emulator
    if ((symbol = NativeBridgeLinker::dlsym(handle, sym_name))) return symbol;
    // Real device
    if ((symbol = xdl_sym(handle, sym_name, NULL))) return symbol;
}

#include <jni.h>

inline JavaVM *sJavaVM;

template <auto &&CALL_ORIGNAL>
inline jint _Hook_JNI_OnLoad(JavaVM *vm, void *reserved)
{
    sJavaVM = vm;
    return CALL_ORIGNAL(vm, reserved);
}

inline void *GetMoudleFromSymbol(const char *sym_name)
{
    for (auto &it : ElfScanner::findSymbolAll(sym_name, EScanElfType::Any, EScanElfFilter::App))
    {
        if (it.second.dynamic())
        {
            return (void *)it.first;
        }
    }
}
