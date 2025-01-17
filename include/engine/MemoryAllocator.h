#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <cstddef>  // For size_t
#include <cstdint>  // For uintptr_t
#include <cstdlib>  // For std::malloc and std::free
#include <new>      // For std::align
#include <cassert>  // For assert
#include <iostream> // For debugging

namespace Memory {

    // ----------------------- Stack Allocator -----------------------
    class StackAllocator {
    private:
        void* base;          // Base address of the allocated memory block
        size_t totalSize;    // Total size of the allocator
        size_t offset;       // Current offset from the base address

    public:
        explicit StackAllocator(size_t size);
        ~StackAllocator();

        void* allocate(size_t size, size_t alignment = alignof(std::max_align_t));
        void deallocate(void* ptr); // Warning: StackAllocator does not support individual deallocation
        void reset();               // Reset the allocator, reclaiming all memory
    };

    // ----------------------- Pool Allocator -----------------------
    class PoolAllocator {
    private:
        void* base;           // Base address of the pool
        size_t blockSize;     // Size of each block
        size_t blockCount;    // Total number of blocks
        void* freeList;       // Pointer to the free list
        size_t usedBlocks;    // Number of currently allocated blocks

    public:
        PoolAllocator(size_t blockSize, size_t blockCount);
        ~PoolAllocator();

        void* allocate();     // Allocate a single block
        void deallocate(void* ptr); // Deallocate a specific block
        void reset();         // Reset the allocator to reclaim all blocks
    };

    // -------------------- Free-List Allocator ---------------------
    class FreeListAllocator {
    protected:
        struct FreeBlock {
            size_t size;       // Size of the free block
            FreeBlock* next;   // Pointer to the next free block
        };

    private:
        FreeBlock* freeList;   // Head of the free list
        void* base;            // Base address of memory
        size_t totalSize;      // Total memory size
        size_t used;           // Currently used memory

    public:
        explicit FreeListAllocator(size_t size);
        ~FreeListAllocator();

        void* allocate(size_t size, size_t alignment = alignof(std::max_align_t));
        void deallocate(void* ptr); // Add the block back to the free list
        void reset();               // Reset the allocator, reclaiming all memory
    };

    // ---------------------- Linear Allocator ----------------------
    class LinearAllocator {
    private:
        void* base;          // Base address of memory
        size_t totalSize;    // Total memory size
        size_t offset;       // Current allocation offset

    public:
        explicit LinearAllocator(size_t size);
        ~LinearAllocator();

        void* allocate(size_t size, size_t alignment = alignof(std::max_align_t));
        void reset();        // Reset the allocator, reclaiming all memory
    };

} // namespace Memory

#endif // MEMORY_ALLOCATOR_H
