#include "console_logger.h"

namespace NKVStore::NCore::NLogger
{

TConsoleLogger::TConsoleLogger()
{
    spdlog::init_thread_pool(32768, 2); // 32KB queue, 2 background threads
    
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::debug);
    console_sink->set_pattern("[%n] [%Y-%m-%d %H:%M:%S.%e] [%^%L%$] [th %t] [%s:%#] %v");
    
    _logger = std::make_shared<spdlog::async_logger>(
        "default_console",
        console_sink, 
        spdlog::thread_pool(),
        spdlog::async_overflow_policy::block);

    _logger->set_level(spdlog::level::debug);
    _logger->flush_on(spdlog::level::err);

    spdlog::register_logger(_logger);
}

} // namespace NKVStore::NCore::NLogger
