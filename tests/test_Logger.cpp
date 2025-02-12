#include <catch2/catch_all.hpp>
#include "Utils/Logger.h"

/*
 * Tests for the Logger class. We'll ensure initialization
 * doesn't crash and that logger pointers are valid.
 */

TEST_CASE("Logger Initialization", "[logger]") {

    auto& engineLogger = Logger::GetEngineLogger();
    auto& clientLogger = Logger::GetClientLogger();
    auto& profileLogger = Logger::GetProfileLogger();

    REQUIRE(engineLogger != nullptr);
    REQUIRE(clientLogger != nullptr);
    REQUIRE(profileLogger != nullptr);

    // Simple log test
    REQUIRE_NOTHROW(LOG_ENGINE_INFO("Engine logger test message"));
    REQUIRE_NOTHROW(LOG_CLIENT_INFO("Client logger test message"));
    REQUIRE_NOTHROW(LOG_PROFILE_INFO("Profile logger test message"));
}
TEST_CASE("Logger Levels", "[logger]") {
	// Set log levels
	Logger::GetEngineLogger()->set_level(spdlog::level::info);
	Logger::GetClientLogger()->set_level(spdlog::level::warn);
	Logger::GetProfileLogger()->set_level(spdlog::level::err);
	// Test logging at various levels
	REQUIRE_NOTHROW(LOG_ENGINE_TRACE("This should not be logged"));
	REQUIRE_NOTHROW(LOG_ENGINE_INFO("This should be logged"));
	REQUIRE_NOTHROW(LOG_CLIENT_TRACE("This should not be logged"));
	REQUIRE_NOTHROW(LOG_CLIENT_WARN("This should be logged"));
	REQUIRE_NOTHROW(LOG_PROFILE_TRACE("This should not be logged"));
	REQUIRE_NOTHROW(LOG_PROFILE_ERROR("This should be logged"));
}