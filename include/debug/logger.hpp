#pragma once

#include <spdlog/spdlog.h>
#ifdef __ANDROID__
#include "spdlog/sinks/android_sink.h"
#else
#include "spdlog/sinks/basic_file_sink.h"
#endif

#include <string_view>
#if defined(__ANDROID__) && !defined(NDEBUG)
#include <csignal>
#endif

using namespace std::chrono_literals;

namespace Debug
{
    class Logger
    {
    private:
#ifdef __ANDROID__
        inline static constexpr auto TAG = "freecam";
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
            spdlog::flush_every(1s);
            spdlog::flush_on(spdlog::level::warn);
#if defined(__ANDROID__) && !defined(NDEBUG)
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
            logger->debug(fmt::runtime(fmt), std::forward<Args>(args)...);
        }
        template <typename... Args>
        inline static void LOGI(std::string_view fmt, Args &&...args)
        {
            logger->info(fmt::runtime(fmt), std::forward<Args>(args)...);
        }
        template <typename... Args>
        inline static void LOGW(std::string_view fmt, Args &&...args)
        {
            logger->warn(fmt::runtime(fmt), std::forward<Args>(args)...);
        }
        template <typename... Args>
        inline static void LOGE(std::string_view fmt, Args &&...args)
        {
            logger->error(fmt::runtime(fmt), std::forward<Args>(args)...);
        }
        template <typename... Args>
        inline static void LOGC(std::string_view fmt, Args &&...args)
        {
            logger->critical(fmt::runtime(fmt), std::forward<Args>(args)...);
        }

    private:
#if defined(__ANDROID__) && !defined(NDEBUG)
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
#endif
    };
}
