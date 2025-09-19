#pragma once

#include <spdlog/spdlog.h>
#include <string_view>
#if ANDROID_MODE
#include "spdlog/sinks/android_sink.h"
#else
#include "spdlog/sinks/basic_file_sink.h"
#endif

#ifndef NDEBUG
#include <csignal>
#endif

namespace Debug
{
    class Logger
    {
    private:
        inline static constexpr auto TAG = "freecam";
#if ANDROID_MODE
        inline static auto logger = spdlog::android_logger_mt("freecam_logger", TAG);
#else
        inline static auto logger = spdlog::basic_logger_mt("freecam_logger", "logs/log.txt");
#endif

    public:
        inline static void Init()
        {
            // Setup Logger
#ifdef NDEBUG
            logger->set_level(spdlog::level::info);
#else
            logger->set_level(spdlog::level::debug);
#endif
            spdlog::set_default_logger(logger);
            spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
#ifndef NDEBUG
            // Setup Crash Handler
            std::signal(SIGSEGV, crachHandler);
            std::signal(SIGABRT, crachHandler);
            std::signal(SIGFPE, crachHandler);
            std::signal(SIGILL, crachHandler);
#endif
        }
        template <typename... Args>
        inline static void LOGD(std::string_view fmt, Args &&...args)
        {
            logger->debug(fmt::runtime(fmt), args...);
        }
        template <typename... Args>
        inline static void LOGI(std::string_view fmt, Args &&...args)
        {
            logger->info(fmt::runtime(fmt), args...);
        }
        template <typename... Args>
        inline static void LOGW(Args &&...args)
        {
            logger->warn(args...);
        }
        template <typename... Args>
        inline static void LOGE(Args &&...args)
        {
            logger->error(args...);
        }
        template <typename... Args>
        inline static void LOGC(Args &&...args)
        {
            logger->critical(args...);
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
            logger->critical("=== Program Crashed ===");
            logger->critical("Signal: {}", signal_name);
            logger->critical("Stack Trace:");
            // TODO:
            // backward::StackTrace st;
            // st.load_here(32);
            //
            // backward::Printer p;
            // p.address = true;
            // p.object = true;
            //
            // std::ostringstream oss;
            // p.print(st, oss);
            // logger->critical(oss.str());

            spdlog::shutdown();
            std::exit(signal);
        }
    };
}
