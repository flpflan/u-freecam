#pragma once

#include <spdlog/spdlog.h>
#ifdef __ANDROID__
#include "spdlog/sinks/android_sink.h"
#else
#include "spdlog/sinks/basic_file_sink.h"
#endif

namespace umod::debug::logger
{
    using spdlog::format_string_t;

    // Setup logger (Thread Safe)
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
    inline void shutdown() { spdlog::shutdown(); }
    template <typename... Args>
    void trace(const format_string_t<Args...> &fmt, Args &&...args)
    {
        logger()->trace(fmt, std::forward<Args>(args)...);
    }
    template <typename T>
    void trace(T &&fmt)
    {
        logger()->trace(std::forward<T>(fmt));
    }
    template <typename... Args>
    void debug(const format_string_t<Args...> &fmt, Args &&...args)
    {
        logger()->debug(fmt, std::forward<Args>(args)...);
    }
    template <typename T>
    void debug(T &&fmt)
    {
        logger()->debug(std::forward<T>(fmt));
    }
    template <typename... Args>
    void info(const format_string_t<Args...> &fmt, Args &&...args)
    {
        logger()->info(fmt, std::forward<Args>(args)...);
    }
    template <typename T>
    void info(T &&fmt)
    {
        logger()->info(std::forward<T>(fmt));
    }
    template <typename... Args>
    void warn(const format_string_t<Args...> &fmt, Args &&...args)
    {
        logger()->warn(fmt, std::forward<Args>(args)...);
    }
    template <typename T>
    void warn(T &&fmt)
    {
        logger()->warn(std::forward<T>(fmt));
    }
    template <typename... Args>
    void error(const format_string_t<Args...> &fmt, Args &&...args)
    {
        logger()->error(fmt, std::forward<Args>(args)...);
    }
    template <typename T>
    void error(T &&fmt)
    {
        logger()->error(std::forward<T>(fmt));
    }
    template <typename... Args>
    void critical(const format_string_t<Args...> &fmt, Args &&...args)
    {
        logger()->critical(fmt, std::forward<Args>(args)...);
    }
    template <typename T>
    void critical(T &&fmt)
    {
        logger()->critical(std::forward<T>(fmt));
    }
}
