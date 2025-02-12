#include "Utils/Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

// Define the static members declared in Logger.h
std::shared_ptr<spdlog::logger> Logger::s_EngineLogger;
std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;
std::shared_ptr<spdlog::logger> Logger::s_ProfileLogger;

/**
 * @brief Initializes the Engine, Client, and Profile loggers.
 *        Here we set a common pattern and attach color console sinks.
 */
void Logger::Init() {
    // Define a log pattern: "[time] [log-level] message"
    // %^ and %$ are spdlog color markers for highlight.
    spdlog::set_pattern("[%T] [%^%l%$] %v");

    // 1) ENGINE logger
    s_EngineLogger = spdlog::stdout_color_mt("ENGINE");
    s_EngineLogger->set_level(spdlog::level::trace);

    // 2) CLIENT logger
    s_ClientLogger = spdlog::stdout_color_mt("APP");
    s_ClientLogger->set_level(spdlog::level::trace);

    // 3) PROFILE logger
    s_ProfileLogger = spdlog::stdout_color_mt("PROFILE");
    s_ProfileLogger->set_level(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger>& Logger::GetEngineLogger() {
    return s_EngineLogger;
}

std::shared_ptr<spdlog::logger>& Logger::GetClientLogger() {
    return s_ClientLogger;
}

std::shared_ptr<spdlog::logger>& Logger::GetProfileLogger() {
    return s_ProfileLogger;
}
