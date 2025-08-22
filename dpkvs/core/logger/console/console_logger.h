#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace NKVStore::NCore::NLogger
{

class TConsoleLogger {
public:
    TConsoleLogger();

    TConsoleLogger(const TConsoleLogger&) = delete;
    TConsoleLogger& operator=(const TConsoleLogger&) = delete;

    TConsoleLogger(TConsoleLogger&&) = default;
    TConsoleLogger& operator=(TConsoleLogger&&) = default;

    ~TConsoleLogger() = default;

    template<class... Args>
    void Debug(spdlog::format_string_t<Args...>fmt, Args&&... args) {
        _logger->debug(fmt, std::forward<Args>(args)...);
    }

    template<class... Args>
    void Info(spdlog::format_string_t<Args...>fmt, Args&&... args) {
        _logger->info(fmt, std::forward<Args>(args)...);
    }

    template<class... Args>
    void Warning(spdlog::format_string_t<Args...>fmt, Args&&... args) {
        _logger->warn(fmt, std::forward<Args>(args)...);
    }

    template<class... Args>
    void Error(spdlog::format_string_t<Args...>fmt, Args&&... args) {
        _logger->error(fmt, std::forward<Args>(args)...);
    }

private:
    std::shared_ptr<spdlog::logger> _logger;
};

} // namespace NKVStore::NCore::NLogger
