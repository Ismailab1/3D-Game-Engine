#pragma once

#include <memory>
#include <spdlog/spdlog.h>

class Logger {
public:
    static void Init();
    static std::shared_ptr<spdlog::logger>& GetEngineLogger();
    static std::shared_ptr<spdlog::logger>& GetClientLogger();

private:
    static std::shared_ptr<spdlog::logger> s_EngineLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

// Logging macros
#define LOG_ENGINE_TRACE(...) Logger::GetEngineLogger()->trace(__VA_ARGS__)
#define LOG_ENGINE_INFO(...)  Logger::GetEngineLogger()->info(__VA_ARGS__)
#define LOG_ENGINE_WARN(...)  Logger::GetEngineLogger()->warn(__VA_ARGS__)
#define LOG_ENGINE_ERROR(...) Logger::GetEngineLogger()->error(__VA_ARGS__)

#define LOG_CLIENT_TRACE(...) Logger::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_CLIENT_INFO(...)  Logger::GetClientLogger()->info(__VA_ARGS__)
#define LOG_CLIENT_WARN(...)  Logger::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_CLIENT_ERROR(...) Logger::GetClientLogger()->error(__VA_ARGS__)
