#pragma once

#include <spdlog/spdlog.h>
#ifdef __ANDROID__
#include "spdlog/sinks/android_sink.h"
#else
#include "spdlog/sinks/basic_file_sink.h"
#endif

namespace Debug::Logger
{
    using spdlog::format_string_t;

    // Setup Logger (Thread Safe)
    inline auto logger()
    {
        const static auto _logger = []
        {
#ifdef __ANDROID__
            const auto _logger = spdlog::android_logger_mt("freecam_logger", "freecam");
#else
            const auto _logger = spdlog::basic_logger_mt("freecam_logger", "logs/log.txt");
#endif
#ifdef NDEBUG
            _logger->set_level(spdlog::level::info);
#else
            _logger->set_level(spdlog::level::trace);
#endif
            spdlog::set_default_logger(_logger);
            spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
            spdlog::flush_every(std::chrono::seconds(1));
            spdlog::flush_on(spdlog::level::warn);
            return _logger;
        }();
        return _logger;
    }
    inline void ShutDown() { spdlog::shutdown(); }
    template <typename... Args>
    void Trace(const format_string_t<Args...> &fmt, Args &&...args)
    {
        logger()->trace(fmt, std::forward<Args>(args)...);
    }
    template <typename T>
    void Trace(const T &fmt)
    {
        logger()->trace(fmt);
    }
    template <typename... Args>
    void Debug(const format_string_t<Args...> &fmt, Args &&...args)
    {
        logger()->debug(fmt, std::forward<Args>(args)...);
    }
    template <typename T>
    void Debug(const T &fmt)
    {
        logger()->debug(fmt);
    }
    template <typename... Args>
    void Info(const format_string_t<Args...> &fmt, Args &&...args)
    {
        logger()->info(fmt, std::forward<Args>(args)...);
    }
    template <typename T>
    void Info(const T &fmt)
    {
        logger()->info(fmt);
    }
    template <typename... Args>
    void Warn(const format_string_t<Args...> &fmt, Args &&...args)
    {
        logger()->warn(fmt, std::forward<Args>(args)...);
    }
    template <typename T>
    void Warn(const T &fmt)
    {
        logger()->warn(fmt);
    }
    template <typename... Args>
    void Error(const format_string_t<Args...> &fmt, Args &&...args)
    {
        logger()->error(fmt, std::forward<Args>(args)...);
    }
    template <typename T>
    void Error(const T &fmt)
    {
        logger()->error(fmt);
    }
    template <typename... Args>
    void Critical(const format_string_t<Args...> &fmt, Args &&...args)
    {
        logger()->critical(fmt, std::forward<Args>(args)...);
    }
    template <typename T>
    void Critical(const T &fmt)
    {
        logger()->critical(fmt);
    }
}
