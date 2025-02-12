#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

/**
 * @class Logger
 * @brief A static utility class for managing different loggers
 *        (Engine, Client/App, and Profiling).
 */
class Logger {
public:
    /**
     * @brief Initializes all loggers (Engine, Client, Profile).
     *        Must be called before using any logger macros.
     */
    static void Init();

    /**
     * @brief Provides access to the Engine logger.
     * @return Reference to the shared pointer of the Engine logger.
     */
    static std::shared_ptr<spdlog::logger>& GetEngineLogger();

    /**
     * @brief Provides access to the Client (APP) logger.
     * @return Reference to the shared pointer of the Client logger.
     */
    static std::shared_ptr<spdlog::logger>& GetClientLogger();

    /**
     * @brief Provides access to the dedicated Profiling logger.
     * @return Reference to the shared pointer of the Profiling logger.
     */
    static std::shared_ptr<spdlog::logger>& GetProfileLogger();

private:
    // Private static members to hold the SPDLog logger instances.
    static std::shared_ptr<spdlog::logger> s_EngineLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
    static std::shared_ptr<spdlog::logger> s_ProfileLogger;
};

// ---------------------- Logging Macros ----------------------
// These macros provide a convenient interface for logging messages
// at various severity levels for the Engine, Client, and Profile logs.
//
// The usage is identical to spdlog formatting, e.g.:
//   LOG_ENGINE_INFO("Hello {}", "World");
//
// By default, all loggers are set to 'trace' level, so all messages are shown.

#define LOG_ENGINE_TRACE(...) Logger::GetEngineLogger()->trace(__VA_ARGS__)
#define LOG_ENGINE_INFO(...)  Logger::GetEngineLogger()->info(__VA_ARGS__)
#define LOG_ENGINE_WARN(...)  Logger::GetEngineLogger()->warn(__VA_ARGS__)
#define LOG_ENGINE_ERROR(...) Logger::GetEngineLogger()->error(__VA_ARGS__)

#define LOG_CLIENT_TRACE(...) Logger::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_CLIENT_INFO(...)  Logger::GetClientLogger()->info(__VA_ARGS__)
#define LOG_CLIENT_WARN(...)  Logger::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_CLIENT_ERROR(...) Logger::GetClientLogger()->error(__VA_ARGS__)

// Profiling logging macros
#define LOG_PROFILE_TRACE(...) Logger::GetProfileLogger()->trace(__VA_ARGS__)
#define LOG_PROFILE_INFO(...)  Logger::GetProfileLogger()->info(__VA_ARGS__)
#define LOG_PROFILE_WARN(...)  Logger::GetProfileLogger()->warn(__VA_ARGS__)
#define LOG_PROFILE_ERROR(...) Logger::GetProfileLogger()->error(__VA_ARGS__)
