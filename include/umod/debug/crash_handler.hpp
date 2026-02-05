#pragma once

#if defined(__ANDROID__)
#if (__ANDROID_API__ >= 33)
#include "backward.hpp"

#include <csignal>

#include "logger.hpp"

namespace umod::debug::crash_handler
{
    inline void handleSignal(int signal)
    {
        const char *signal_name = "";
        switch (signal)
        {
        case SIGSEGV:
            signal_name = "SIGABRT";
            break;
        case SIGABRT:
            signal_name = "SIGABRT";
            break;
        case SIGFPE:
            signal_name = "SIGFPE";
            break;
        case SIGILL:
            signal_name = "SIGILL";
            break;
        default:
            signal_name = "UNKNOWN";
            break;
        }
        logger::critical("===== [Program Crashed:{}] =====", signal_name);
// #ifndef NDEBUG
/*
    INFO:
    We use backward-cpp, which relys on `backtrace_symbols` on Android, for stack trace.
    And `backtrace_symbols` only accessible for Android API >= 33
*/
#if defined(__ANDROID__) && (__ANDROID_API__ < 33)
#else
        // Stack Trace
        backward::StackTrace st;
        st.load_here(32);

        backward::Printer p;
        p.address = true;
        p.object = true;

        std::ostringstream oss;
        p.print(st, oss);
        logger::critical(oss.str());
#endif
        // #endif
        std::exit(signal);
    }

    inline void setup()
    {
        std::signal(SIGSEGV, handleSignal);
        std::signal(SIGABRT, handleSignal);
        std::signal(SIGFPE, handleSignal);
        std::signal(SIGILL, handleSignal);
    }
}
#else
namespace umod::debug::crash_handler
{
    inline void setup() {}
}
#endif
#endif
