#include "Utils/Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

// Static members definition
std::shared_ptr<spdlog::logger> Logger::s_EngineLogger;
std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;

void Logger::Init() {
    spdlog::set_pattern("[%T] [%^%l%$] %v");

    s_EngineLogger = spdlog::stdout_color_mt("ENGINE");
    s_EngineLogger->set_level(spdlog::level::trace);

    s_ClientLogger = spdlog::stdout_color_mt("APP");
    s_ClientLogger->set_level(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger>& Logger::GetEngineLogger() { return s_EngineLogger; }
std::shared_ptr<spdlog::logger>& Logger::GetClientLogger() { return s_ClientLogger; }
