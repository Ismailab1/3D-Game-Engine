#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "../../include/engine/MemoryAllocator.h"

TEST_CASE("StackAllocator: Basic Operations", "[StackAllocator]") {
    constexpr size_t allocatorSize = 1024;
    constexpr size_t alignment = 16;

    Memory::StackAllocator allocator(allocatorSize);

    SECTION("Allocation and alignment") {
        void* ptr1 = allocator.allocate(128, alignment);
        REQUIRE(ptr1 != nullptr);

        void* ptr2 = allocator.allocate(256, alignment);
        REQUIRE(ptr2 != nullptr);
        REQUIRE(reinterpret_cast<uintptr_t>(ptr2) % alignment == 0);

        REQUIRE(static_cast<char*>(ptr2) > static_cast<char*>(ptr1));
    }

    SECTION("Reset and reuse") {
        allocator.allocate(512, alignment);
        allocator.reset();
        REQUIRE_NOTHROW(allocator.allocate(1024, alignment));
    }

    SECTION("Allocation beyond capacity throws") {
        REQUIRE_THROWS_AS(allocator.allocate(2048, alignment), std::bad_alloc);
    }
}

TEST_CASE("PoolAllocator: Functionality", "[PoolAllocator]") {
    constexpr size_t blockSize = 64;
    constexpr size_t blockCount = 10;

    Memory::PoolAllocator allocator(blockSize, blockCount);

    SECTION("Basic allocation and deallocation") {
        void* ptr1 = allocator.allocate();
        void* ptr2 = allocator.allocate();

        REQUIRE(ptr1 != ptr2);
        allocator.deallocate(ptr1);
        allocator.deallocate(ptr2);
    }

    SECTION("Reset reclaims memory") {
        for (size_t i = 0; i < blockCount; ++i) {
            allocator.allocate();
        }
        allocator.reset();
        REQUIRE_NOTHROW(allocator.allocate());
    }

    SECTION("Deallocate invalid pointer") {
        void* invalidPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(0xDEADBEEF));

        allocator.deallocate(invalidPtr); // Should not crash
    }

    SECTION("Deallocate and reuse") {
        void* ptr1 = allocator.allocate();
        allocator.deallocate(ptr1);

        void* ptr2 = allocator.allocate();
        REQUIRE(ptr1 == ptr2);
    }
}

TEST_CASE("FreeListAllocator: Advanced Scenarios", "[FreeListAllocator]") {
    constexpr size_t allocatorSize = 1024;

    Memory::FreeListAllocator allocator(allocatorSize);

    SECTION("Basic allocation and deallocation") {
        void* ptr1 = allocator.allocate(128);
        REQUIRE(ptr1 != nullptr);

        void* ptr2 = allocator.allocate(256);
        REQUIRE(ptr2 != nullptr);

        allocator.deallocate(ptr1);
        allocator.deallocate(ptr2);
    }

    SECTION("Memory reuse") {
        void* ptr1 = allocator.allocate(128);
        allocator.deallocate(ptr1);

        void* ptr2 = allocator.allocate(128);
        REQUIRE(ptr1 == ptr2);
    }

    SECTION("Memory coalescing") {
        void* ptr1 = allocator.allocate(128);
        void* ptr2 = allocator.allocate(256);

        allocator.deallocate(ptr1);
        allocator.deallocate(ptr2);

        void* ptr3 = allocator.allocate(384); // Should coalesce
        REQUIRE(ptr3 != nullptr);
        REQUIRE(ptr3 == ptr1);
    }

    SECTION("Deallocate invalid pointer") {
        void* invalidPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(0xBADCAFE));
        allocator.deallocate(invalidPtr); // Should not crash
    }
}

TEST_CASE("LinearAllocator: Operations", "[LinearAllocator]") {
    constexpr size_t allocatorSize = 1024;

    Memory::LinearAllocator allocator(allocatorSize);

    SECTION("Basic allocation") {
        void* ptr1 = allocator.allocate(128);
        REQUIRE(ptr1 != nullptr);

        void* ptr2 = allocator.allocate(256);
        REQUIRE(ptr2 != nullptr);

        REQUIRE(static_cast<char*>(ptr2) > static_cast<char*>(ptr1));
    }

    SECTION("Reset and reuse") {
        allocator.allocate(512);
        allocator.reset();
        REQUIRE_NOTHROW(allocator.allocate(1024));
    }

    SECTION("Allocation beyond capacity throws") {
        REQUIRE_THROWS_AS(allocator.allocate(2048), std::bad_alloc);
    }
}

TEST_CASE("Mixed Allocator Usage and Memory Management", "[Allocators]") {
    constexpr size_t stackSize = 1024;
    constexpr size_t poolBlockSize = 64;
    constexpr size_t poolBlockCount = 20;
    constexpr size_t freeListSize = 2048;
    constexpr size_t linearAllocatorSize = 1024;

    Memory::StackAllocator stackAllocator(stackSize);
    Memory::PoolAllocator poolAllocator(poolBlockSize, poolBlockCount);
    Memory::FreeListAllocator freeListAllocator(freeListSize);
    Memory::LinearAllocator linearAllocator(linearAllocatorSize);

    SECTION("Simultaneous allocation and deallocation across allocators") {
        void* stackPtr1 = stackAllocator.allocate(128, 16);
        REQUIRE(stackPtr1 != nullptr);

        void* poolPtr1 = poolAllocator.allocate();
        REQUIRE(poolPtr1 != nullptr);

        void* freeListPtr1 = freeListAllocator.allocate(256, 32);
        REQUIRE(freeListPtr1 != nullptr);

        void* linearPtr1 = linearAllocator.allocate(128, 8);
        REQUIRE(linearPtr1 != nullptr);

        stackAllocator.reset();
        REQUIRE_NOTHROW(stackAllocator.allocate(512));

        poolAllocator.reset();
        REQUIRE_NOTHROW(poolAllocator.allocate());

        freeListAllocator.reset();
        REQUIRE_NOTHROW(freeListAllocator.allocate(512));

        linearAllocator.reset();
        REQUIRE_NOTHROW(linearAllocator.allocate(512));
    }

    SECTION("Exceeding allocator limits") {
        REQUIRE_THROWS_AS(stackAllocator.allocate(stackSize + 1), std::bad_alloc);
        for (size_t i = 0; i < poolBlockCount; ++i) {
            REQUIRE_NOTHROW(poolAllocator.allocate());
        }
        REQUIRE_THROWS_AS(poolAllocator.allocate(), std::bad_alloc);

        REQUIRE_THROWS_AS(freeListAllocator.allocate(freeListSize + 1), std::bad_alloc);
        REQUIRE_THROWS_AS(linearAllocator.allocate(linearAllocatorSize + 1), std::bad_alloc);
    }

    SECTION("Mixed allocation and reuse") {
        void* poolPtr = poolAllocator.allocate();
        REQUIRE(poolPtr != nullptr);
        poolAllocator.deallocate(poolPtr);
        REQUIRE_NOTHROW(poolAllocator.allocate());

        void* freeListPtr1 = freeListAllocator.allocate(256, 16);
        REQUIRE(freeListPtr1 != nullptr);
        freeListAllocator.deallocate(freeListPtr1);

        void* freeListPtr2 = freeListAllocator.allocate(256, 16);
        REQUIRE(freeListPtr2 == freeListPtr1);

        linearAllocator.allocate(512, 16);
        linearAllocator.reset();
        REQUIRE_NOTHROW(linearAllocator.allocate(512, 16));
    }
}
