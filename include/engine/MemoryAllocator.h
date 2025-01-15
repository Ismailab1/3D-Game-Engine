#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <cstddef> // For size_t

class StackAllocator {
private:
    void* base;          // Base address of the allocated memory block
    size_t totalSize;    // Total size of the allocator
    size_t offset;       // Current offset from the base address

public:
    explicit StackAllocator(size_t size);
    ~StackAllocator();

    void* allocate(size_t size, size_t alignment = alignof(std::max_align_t));
    void deallocate(void* ptr);
    void reset();        // Reset the allocator, reclaiming all memory
};

#endif // MEMORY_ALLOCATOR_H
