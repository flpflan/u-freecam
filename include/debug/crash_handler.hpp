#pragma once

#if defined(__ANDROID__) && (__ANDROID_API__ < 33)
#else
#include "backward.hpp"
#endif
#include <csignal>

#include "logger.hpp"

namespace Debug
{
    class CrashHandler
    {
    public:
        inline static void Init()
        {
            // Setup Crash Handler
            std::signal(SIGSEGV, crachHandler);
            std::signal(SIGABRT, crachHandler);
            std::signal(SIGFPE, crachHandler);
            std::signal(SIGILL, crachHandler);
        }

    private:
        inline static void crachHandler(int signal)
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
            Logger::Critical("===== [Program Crashed:{}] =====", signal_name);
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
            Logger::Critical(oss.str());
#endif
// #endif
            Logger::ShutDown();
            std::exit(signal);
        }
    };
}
