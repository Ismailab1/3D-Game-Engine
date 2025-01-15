#include "../../include/engine/MemoryAllocator.h"
#include <cstdlib>  // For aligned allocation functions
#include <iostream> // For debugging output

#ifdef _WIN32
#include <malloc.h> // For _aligned_malloc and _aligned_free
#endif

StackAllocator::StackAllocator(size_t size)
    : base(nullptr),
    totalSize(size),
    offset(0) {
#ifdef _WIN32
    base = _aligned_malloc(size, alignof(std::max_align_t));
#else
    if (posix_memalign(&base, alignof(std::max_align_t), size) != 0) {
        base = nullptr;
    }
#endif
    if (!base) {
        throw std::bad_alloc();
    }
    std::cout << "[StackAllocator] Initialized with size: " << totalSize << " bytes\n";
}

StackAllocator::~StackAllocator() {
    if (offset > 0) {
        std::cerr << "[Memory Leak Detected] " << offset << " bytes still allocated\n";
    }
#ifdef _WIN32
    _aligned_free(base);
#else
    free(base);
#endif
    std::cout << "[StackAllocator] Destroyed\n";
}

void* StackAllocator::allocate(size_t size, size_t alignment) {
    if (alignment == 0) {
        alignment = alignof(std::max_align_t);
    }

    uintptr_t currentAddress = reinterpret_cast<uintptr_t>(base) + offset;
    uintptr_t adjustment = alignment - (currentAddress % alignment);

    if (adjustment == alignment) {
        adjustment = 0;
    }

    if (offset + adjustment + size > totalSize) {
        throw std::bad_alloc();
    }

    if (size == 0) {
        // Return a valid pointer for zero-size allocation
        uintptr_t alignedAddress = currentAddress + adjustment;
        std::cout << "[Allocate] 0 bytes at " << reinterpret_cast<void*>(alignedAddress)
            << " (Alignment: " << alignment << ")\n";
        return reinterpret_cast<void*>(alignedAddress);
    }

    uintptr_t alignedAddress = currentAddress + adjustment;
    offset += adjustment + size;

    std::cout << "[Allocate] " << size << " bytes at " << reinterpret_cast<void*>(alignedAddress)
        << " (Alignment: " << alignment << ")\n";

    return reinterpret_cast<void*>(alignedAddress);
}


void StackAllocator::deallocate(void* ptr) {
    // StackAllocator does not support deallocate for specific addresses
    std::cerr << "[Warning] StackAllocator does not support deallocate for specific addresses.\n";
}

void StackAllocator::reset() {
    offset = 0;
    std::cout << "[Reset] Resetting allocator. All memory will be reclaimed.\n";
}
