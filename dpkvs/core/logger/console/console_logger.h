#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>
#include <string>
#include <utility>

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
    void Debug(spdlog::format_string_t<Args...>fmt, Args&&... args) const {
        _logger->debug(fmt, std::forward<Args>(args)...);
    }

    template<class... Args>
    void Info(spdlog::format_string_t<Args...>fmt, Args&&... args) const {
        _logger->info(fmt, std::forward<Args>(args)...);
    }

    template<class... Args>
    void Warning(spdlog::format_string_t<Args...>fmt, Args&&... args) const {
        _logger->warn(fmt, std::forward<Args>(args)...);
    }

    template<class... Args>
    void Error(spdlog::format_string_t<Args...>fmt, Args&&... args) const {
        _logger->error(fmt, std::forward<Args>(args)...);
    }

private:
    static constexpr std::string _loggerName = "default_console";

    std::shared_ptr<spdlog::logger> _logger;
};

} // namespace NKVStore::NCore::NLogger
