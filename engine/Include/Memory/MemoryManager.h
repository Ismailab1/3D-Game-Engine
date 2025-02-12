#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <cstdlib>   // For malloc/free
#include <unordered_map>
#include <mutex>
#include <chrono>
#include <string>

/**
 * @class MemoryManager
 * @brief A singleton class that tracks allocations and deallocations to detect leaks
 *        and provide basic profiling/timing info.
 */
class MemoryManager {
public:
    /**
     * @brief Gets the single instance of the MemoryManager.
     * @return A reference to the singleton MemoryManager instance.
     */
    static MemoryManager& GetInstance();

    /**
     * @brief Allocates a block of memory of the given size, optionally tagged for logging/profiling.
     * @param size The size (in bytes) to allocate.
     * @param tag  An optional string tag to label the allocation.
     * @return A pointer to the allocated memory, or nullptr if allocation failed.
     */
    void* Allocate(size_t size, const char* tag = "Unknown");

    /**
     * @brief Deallocates a previously allocated block of memory.
     * @param ptr Pointer to the memory block to free.
     */
    void Deallocate(void* ptr);

    /**
     * @brief Logs all current allocations with their addresses, sizes, and tags.
     */
    void PrintMemoryUsage() const;

    /**
     * @brief Checks if there are any unfreed allocations.
     * @return True if there are memory leaks, otherwise false.
     */
    bool HasMemoryLeaks() const;

private:
    // Private constructor and destructor for singleton pattern
    MemoryManager();
    ~MemoryManager();

    // Disable copy/move
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;

    /**
     * @struct AllocationInfo
     * @brief Keeps record of a single allocation, including size, tag, and allocation timestamp.
     */
    struct AllocationInfo {
        size_t size;  // Number of bytes allocated
        std::string tag;  // Descriptive tag, e.g. "Texture", "Buffer", etc.
        std::chrono::time_point<std::chrono::steady_clock> timestamp;  // Time the memory was allocated
    };

    // A map from the pointer address to its allocation info
    std::unordered_map<void*, AllocationInfo> m_Allocations;
    // Mutex to guard m_Allocations and total allocation counters
    mutable std::mutex m_Mutex;

    // Track total allocated and deallocated bytes
    size_t m_TotalAllocated = 0;
    size_t m_TotalDeallocated = 0;
};

#endif // MEMORY_MANAGER_H
