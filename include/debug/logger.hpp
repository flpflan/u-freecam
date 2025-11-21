#pragma once

#include <spdlog/spdlog.h>
#ifdef __ANDROID__
#include "spdlog/sinks/android_sink.h"
#else
#include "spdlog/sinks/basic_file_sink.h"
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
        }
        inline static void ShutDown() { spdlog::shutdown(); }
        template <typename... Args>
        inline static void Debug(const fmt::format_string<Args...> &fmt, Args &&...args)
        {
            logger->debug(fmt, std::forward<Args>(args)...);
        }
        template <typename T>
        inline static void Debug(const T &fmt)
        {
            logger->debug(fmt);
        }
        template <typename... Args>
        inline static void Info(const fmt::format_string<Args...> &fmt, Args &&...args)
        {
            logger->info(fmt, std::forward<Args>(args)...);
        }
        template <typename T>
        inline static void Info(const T &fmt)
        {
            logger->info(fmt);
        }
        template <typename... Args>
        inline static void Warn(const fmt::format_string<Args...> &fmt, Args &&...args)
        {
            logger->warn(fmt, std::forward<Args>(args)...);
        }
        template <typename T>
        inline static void Warn(const T &fmt)
        {
            logger->warn(fmt);
        }
        template <typename... Args>
        inline static void Error(const fmt::format_string<Args...> &fmt, Args &&...args)
        {
            logger->error(fmt, std::forward<Args>(args)...);
        }
        template <typename T>
        inline static void Error(const T &fmt)
        {
            logger->error(fmt);
        }
        template <typename... Args>
        inline static void Critical(const fmt::format_string<Args...> &fmt, Args &&...args)
        {
            logger->critical(fmt, std::forward<Args>(args)...);
        }
        template <typename T>
        inline static void Critical(const T &fmt)
        {
            logger->critical(fmt);
        }
    };
}
