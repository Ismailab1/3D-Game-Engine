#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Include your MemoryManager and Logger
#include "../../engine/include/Memory/MemoryManager.h"
#include "../../engine/include/Utils/Logger.h"

// Optional: spdlog sink includes (for capturing log output)
#include <spdlog/sinks/ostream_sink.h>
#include <sstream>

TEST_CASE("MemoryManager Singleton", "[memory]") {
    MemoryManager& mm1 = MemoryManager::GetInstance();
    MemoryManager& mm2 = MemoryManager::GetInstance();
    REQUIRE(&mm1 == &mm2); // Ensure it's a singleton instance
}

TEST_CASE("Memory Allocation & Deallocation", "[memory]") {
    MemoryManager& mm = MemoryManager::GetInstance();

    SECTION("Allocate and Free Memory") {
        void* block = mm.Allocate(128, "TestBlock");
        REQUIRE(block != nullptr);
        mm.Deallocate(block);
        REQUIRE_FALSE(mm.HasMemoryLeaks()); // Ensure no leaks after cleanup
    }

    SECTION("Detect Memory Leak (and Fix It)") {
        void* leakedBlock = mm.Allocate(256, "LeakedBlock");
        REQUIRE(leakedBlock != nullptr); // Ensure allocation was successful

        // Fix: Deallocate the leaked block before checking for leaks
        mm.Deallocate(leakedBlock);
        REQUIRE_FALSE(mm.HasMemoryLeaks()); // No leaks after proper cleanup
    }

    SECTION("Multiple Allocations and Deallocation") {
        void* block1 = mm.Allocate(64);
        void* block2 = mm.Allocate(256);
        REQUIRE(block1 != nullptr);
        REQUIRE(block2 != nullptr);

        mm.Deallocate(block1);
        mm.Deallocate(block2); // Fix: Deallocate block2 before checking leaks

        REQUIRE_FALSE(mm.HasMemoryLeaks()); // Should be clean now
    }
}

TEST_CASE("Memory Deallocation Edge Cases", "[memory]") {
    MemoryManager& mm = MemoryManager::GetInstance();

    SECTION("Double Free") {
        void* block = mm.Allocate(128);
        mm.Deallocate(block);
        // Should handle gracefully (log warning instead of crashing)
        REQUIRE_NOTHROW(mm.Deallocate(block));
    }

    SECTION("Deallocate Nullptr") {
        // Should safely ignore null pointer
        REQUIRE_NOTHROW(mm.Deallocate(nullptr));
    }
}

TEST_CASE("Out of Memory Handling", "[memory]") {
    MemoryManager& mm = MemoryManager::GetInstance();

    // Try to allocate an extremely large block
    void* largeBlock = mm.Allocate(SIZE_MAX);
    REQUIRE(largeBlock == nullptr); // Should return nullptr on failure
}

TEST_CASE("Memory Profiling Logs", "[memory][profiling]") {
    // 1) Create a sink that writes log messages into a string stream.
    std::ostringstream oss;
    auto sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(oss);

    // 2) Attach this sink to the PROFILE logger so we can intercept logs.
    //    If you want to capture ENGINE or APP logs, push_back to those instead.
    Logger::GetProfileLogger()->sinks().push_back(sink);

    // 3) Perform a memory operation that triggers profiling logs.
    MemoryManager& mm = MemoryManager::GetInstance();
    void* temp = mm.Allocate(128, "ProfileTest");
    mm.Deallocate(temp);

    // 4) Flush any remaining log messages to be safe (sometimes spdlog is async).
    Logger::GetProfileLogger()->flush();

    // 5) Extract the logs from the string stream.
    std::string logOutput = oss.str();

    // 6) Now test if the expected profiling messages exist in the logs.
    REQUIRE(logOutput.find("[MemoryManager] Allocated 128 bytes at") != std::string::npos);
    REQUIRE(logOutput.find("[MemoryManager] Deallocated 128 bytes at") != std::string::npos);

    // 7) (Optional) Remove our test sink to avoid polluting subsequent tests.
    Logger::GetProfileLogger()->sinks().pop_back();
}

