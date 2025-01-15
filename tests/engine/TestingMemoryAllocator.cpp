#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "../../include/engine/MemoryAllocator.h"

TEST_CASE("StackAllocator: Basic Allocation and Alignment", "[StackAllocator]") {
    constexpr size_t allocatorSize = 1024; // 1 KB stack
    constexpr size_t alignment = 16;

    StackAllocator allocator(allocatorSize);

    SECTION("Allocation works within bounds") {
        void* ptr1 = allocator.allocate(128, alignment);
        REQUIRE(ptr1 != nullptr);

        void* ptr2 = allocator.allocate(256, alignment);
        REQUIRE(ptr2 != nullptr);

        REQUIRE(static_cast<char*>(ptr2) > static_cast<char*>(ptr1));
    }

    SECTION("Alignment is respected") {
        void* ptr = allocator.allocate(64, alignment);
        REQUIRE(ptr != nullptr);
        REQUIRE(reinterpret_cast<uintptr_t>(ptr) % alignment == 0);
    }

    SECTION("Cannot allocate beyond stack size") {
        REQUIRE_THROWS_AS(allocator.allocate(allocatorSize + 1, alignment), std::bad_alloc);
    }
}

TEST_CASE("StackAllocator: Reset and Reuse", "[StackAllocator]") {
    constexpr size_t allocatorSize = 512; // 512 bytes
    constexpr size_t alignment = 8;

    StackAllocator allocator(allocatorSize);

    SECTION("Reset reclaims all memory") {
        void* ptr1 = allocator.allocate(128, alignment);
        REQUIRE(ptr1 != nullptr);

        allocator.reset();

        void* ptr2 = allocator.allocate(256, alignment);
        REQUIRE(ptr2 != nullptr);
        REQUIRE(static_cast<char*>(ptr2) <= static_cast<char*>(ptr1));
    }
}

TEST_CASE("StackAllocator: Edge Cases", "[StackAllocator]") {
    constexpr size_t allocatorSize = 256; // 256 bytes
    constexpr size_t alignment = 32;

    StackAllocator allocator(allocatorSize);

    SECTION("Zero-size allocation returns valid pointer") {
        void* ptr = allocator.allocate(0, alignment);
        REQUIRE(ptr != nullptr);
    }

    SECTION("Alignment larger than allocation size") {
        StackAllocator allocator(256);
        void* ptr = allocator.allocate(16, 64);
        REQUIRE(ptr != nullptr);
        // Validate alignment
        REQUIRE(reinterpret_cast<uintptr_t>(ptr) % 64 == 0);
        allocator.reset(); // Clean up
    }

    SECTION("Double free does not crash") {
        void* ptr = allocator.allocate(64, alignment);
        REQUIRE(ptr != nullptr);

        allocator.deallocate(ptr); // Should not throw or crash.
        allocator.deallocate(ptr); // No-op in stack allocator.
    }
}
