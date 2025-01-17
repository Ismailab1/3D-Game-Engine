#include "../../include/engine/MemoryAllocator.h"
#include <cstdlib>  // For aligned allocation functions
#include <iostream> // For debugging output

#ifdef _WIN32
#include <malloc.h> // For _aligned_malloc and _aligned_free
#endif

namespace Memory {

    // --------------------------- StackAllocator ---------------------------
    StackAllocator::StackAllocator(size_t size)
        : base(nullptr), totalSize(size), offset(0) {
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

        uintptr_t alignedAddress = currentAddress + adjustment;
        offset += adjustment + size;

        std::cout << "[Allocate] " << size << " bytes at " << reinterpret_cast<void*>(alignedAddress)
            << " (Alignment: " << alignment << ")\n";

        return reinterpret_cast<void*>(alignedAddress);
    }

    void StackAllocator::deallocate(void* ptr) {
        std::cerr << "[Warning] StackAllocator does not support deallocate for specific addresses.\n";
    }

    void StackAllocator::reset() {
        offset = 0;
        std::cout << "[Reset] Resetting allocator. All memory will be reclaimed.\n";
    }

    // --------------------------- PoolAllocator ---------------------------
    PoolAllocator::PoolAllocator(size_t blockSize, size_t blockCount)
        : base(nullptr), blockSize(blockSize), blockCount(blockCount), freeList(nullptr), usedBlocks(0) {
        base = std::malloc(blockSize * blockCount);
        if (!base) {
            throw std::bad_alloc();
        }

        freeList = base;
        void* current = freeList;

        for (size_t i = 1; i < blockCount; ++i) {
            void* next = reinterpret_cast<void*>(reinterpret_cast<char*>(current) + blockSize);
            *reinterpret_cast<void**>(current) = next;
            current = next;
        }
        *reinterpret_cast<void**>(current) = nullptr;

        std::cout << "[PoolAllocator] Initialized with block size: " << blockSize
            << " bytes, block count: " << blockCount << "\n";
    }

    PoolAllocator::~PoolAllocator() {
        if (usedBlocks > 0) {
            std::cerr << "[Memory Leak Detected] Not all blocks were deallocated. Used blocks: "
                << usedBlocks << "\n";
        }
        free(base);
        std::cout << "[PoolAllocator] Destroyed\n";
    }

    void* PoolAllocator::allocate() {
        if (!freeList) {
            throw std::bad_alloc();
        }

        void* block = freeList;
        freeList = *reinterpret_cast<void**>(freeList);
        ++usedBlocks;

        std::cout << "[Allocate] Block at " << block << "\n";
        return block;
    }

    void PoolAllocator::deallocate(void* ptr) {
        if (!ptr) return;

        // Validate if pointer belongs to the pool
        uintptr_t poolStart = reinterpret_cast<uintptr_t>(base);
        uintptr_t poolEnd = poolStart + (blockSize * blockCount);
        uintptr_t ptrAddr = reinterpret_cast<uintptr_t>(ptr);

        if (ptrAddr < poolStart || ptrAddr >= poolEnd || (ptrAddr - poolStart) % blockSize != 0) {
            std::cerr << "[Error] Pointer does not belong to the pool.\n";
            return;
        }

        *reinterpret_cast<void**>(ptr) = freeList;
        freeList = ptr;
        --usedBlocks;

        std::cout << "[Deallocate] Block at " << ptr << " returned to pool\n";
    }


    void PoolAllocator::reset() {
        freeList = base;
        void* current = freeList;

        for (size_t i = 1; i < blockCount; ++i) {
            void* next = reinterpret_cast<void*>(reinterpret_cast<char*>(current) + blockSize);
            *reinterpret_cast<void**>(current) = next;
            current = next;
        }
        *reinterpret_cast<void**>(current) = nullptr;

        usedBlocks = 0;
        std::cout << "[Reset] PoolAllocator reset\n";
    }

    // ---------------- FreeListAllocator ------------------
    FreeListAllocator::FreeListAllocator(size_t size)
        : base(nullptr), freeList(nullptr), totalSize(size), used(0) {
        base = std::malloc(size);
        if (!base) {
            throw std::bad_alloc();
        }
        freeList = static_cast<FreeBlock*>(base);
        freeList->size = size;
        freeList->next = nullptr;
    }

    FreeListAllocator::~FreeListAllocator() {
        if (used > 0) {
            std::cerr << "[Memory Leak Detected] " << used << " bytes still allocated\n";
        }
        std::free(base);
    }

    void* FreeListAllocator::allocate(size_t size, size_t alignment) {
        FreeBlock* prevBlock = nullptr;
        FreeBlock* currentBlock = freeList;

        while (currentBlock) {
            uintptr_t currentAddress = reinterpret_cast<uintptr_t>(currentBlock);
            uintptr_t adjustment = alignment - (currentAddress % alignment);
            if (adjustment == alignment) adjustment = 0;

            size_t totalSize = size + adjustment;
            if (currentBlock->size >= totalSize) {
                uintptr_t alignedAddress = currentAddress + adjustment;
                size_t remainingSize = currentBlock->size - totalSize;

                if (remainingSize > sizeof(FreeBlock)) {
                    FreeBlock* newBlock = reinterpret_cast<FreeBlock*>(alignedAddress + size);
                    newBlock->size = remainingSize;
                    newBlock->next = currentBlock->next;

                    if (prevBlock) {
                        prevBlock->next = newBlock;
                    }
                    else {
                        freeList = newBlock;
                    }
                }
                else {
                    if (prevBlock) {
                        prevBlock->next = currentBlock->next;
                    }
                    else {
                        freeList = currentBlock->next;
                    }
                }

                used += totalSize;
                return reinterpret_cast<void*>(alignedAddress);
            }

            prevBlock = currentBlock;
            currentBlock = currentBlock->next;
        }

        throw std::bad_alloc();
    }

    void FreeListAllocator::deallocate(void* ptr) {
        if (!ptr) return;

        uintptr_t baseAddr = reinterpret_cast<uintptr_t>(base);
        uintptr_t ptrAddr = reinterpret_cast<uintptr_t>(ptr);

        if (ptrAddr < baseAddr || ptrAddr >= baseAddr + totalSize) {
            std::cerr << "[Error] Pointer does not belong to the allocator.\n";
            return;
        }

        FreeBlock* block = static_cast<FreeBlock*>(ptr);
        FreeBlock** current = &freeList;

        while (*current && (*current < block)) {
            current = &(*current)->next;
        }

        if (*current && reinterpret_cast<char*>(block) + block->size == reinterpret_cast<char*>(*current)) {
            block->size += (*current)->size;
            block->next = (*current)->next;
        }
        else {
            block->next = *current;
        }

        if (current != &freeList && reinterpret_cast<char*>(*current) + (*current)->size == reinterpret_cast<char*>(block)) {
            (*current)->size += block->size;
            (*current)->next = block->next;
        }
        else {
            *current = block;
        }

        used -= block->size;
        std::cout << "[Deallocate] Block at " << ptr << " returned to free list\n";
    }


    void FreeListAllocator::reset() {
        freeList = static_cast<FreeBlock*>(base);
        freeList->size = totalSize;
        freeList->next = nullptr;
        used = 0;
        std::cout << "[Reset] FreeListAllocator reset.\n";
    }

    // ---------------- LinearAllocator ------------------
    LinearAllocator::LinearAllocator(size_t size)
        : base(nullptr), totalSize(size), offset(0) {
        base = std::malloc(size);
        if (!base) {
            throw std::bad_alloc();
        }
    }

    LinearAllocator::~LinearAllocator() {
        if (offset > 0) {
            std::cerr << "[Memory Leak Detected] " << offset << " bytes still allocated\n";
        }
        std::free(base);
    }

    void* LinearAllocator::allocate(size_t size, size_t alignment) {
        uintptr_t currentAddress = reinterpret_cast<uintptr_t>(base) + offset;
        uintptr_t adjustment = alignment - (currentAddress % alignment);
        if (adjustment == alignment) adjustment = 0;

        if (offset + adjustment + size > totalSize) {
            throw std::bad_alloc();
        }

        uintptr_t alignedAddress = currentAddress + adjustment;
        offset += adjustment + size;
        return reinterpret_cast<void*>(alignedAddress);
    }

    void LinearAllocator::reset() {
        offset = 0;
        std::cout << "[Reset] LinearAllocator reset.\n";
    }

} // namespace Memory
