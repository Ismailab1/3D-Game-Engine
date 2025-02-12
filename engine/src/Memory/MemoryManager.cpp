#include "Memory/MemoryManager.h"
#include "Utils/Logger.h" // For LOG_ENGINE_INFO, LOG_PROFILE_TRACE, etc.

MemoryManager::MemoryManager() {
    // We can log once here to indicate the MemoryManager has been created.
    // Make sure Logger::Init() is called before MemoryManager is accessed.
    LOG_ENGINE_INFO("[MemoryManager] Initialized.");
}

MemoryManager::~MemoryManager() {
    // If there are still items in m_Allocations, they were never freed.
    if (!m_Allocations.empty()) {
        LOG_ENGINE_WARN("[MemoryManager] Memory leaks detected at shutdown:");
        for (const auto& [ptr, info] : m_Allocations) {
            // Logging each leaked allocation: address, size, tag, and how many seconds ago it was allocated.
            LOG_ENGINE_WARN(
                "  Leak: Address={}, Size={} bytes, Tag='{}', Time={}s",
                ptr, info.size, info.tag,
                std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::steady_clock::now() - info.timestamp
                ).count()
            );
        }
    }

    // Log overall memory stats (total allocated vs total deallocated).
    LOG_ENGINE_INFO(
        "[MemoryManager] Shutdown. Total Allocated: {} bytes, Total Deallocated: {} bytes.",
        m_TotalAllocated, m_TotalDeallocated
    );
}

MemoryManager& MemoryManager::GetInstance() {
    // The static instance is created once (the first time this function is called)
    // and destroyed automatically on program exit.
    static MemoryManager instance;
    return instance;
}

void* MemoryManager::Allocate(size_t size, const char* tag) {
    // Start profiling (timing) for allocation
    auto startTime = std::chrono::steady_clock::now();

    // Lock for thread-safety
    std::lock_guard<std::mutex> lock(m_Mutex);

    // Allocate memory using std::malloc
    void* ptr = std::malloc(size);
    if (ptr) {
        // Insert into our tracking map
        m_Allocations[ptr] = { size, tag, std::chrono::steady_clock::now() };
        m_TotalAllocated += size;

        // Log the allocation
        spdlog::info("[MemoryManager] Allocated {} bytes at {}, Tag='{}'", size, ptr, tag);
    }
    else {
        // If allocation fails, log an error
        spdlog::error("[MemoryManager] Allocation failed for {} bytes!", size);
    }

    // End profiling and log the duration in microseconds
    auto endTime = std::chrono::steady_clock::now();
    auto durationMicro = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    LOG_PROFILE_TRACE("[MemoryManager][Profiling] Allocate {} bytes took {}us (Tag='{}')",
        size, durationMicro, tag);

    return ptr;
}

void MemoryManager::Deallocate(void* ptr) {
    // Start profiling (timing) for deallocation
    auto startTime = std::chrono::steady_clock::now();

    // Lock for thread-safety
    std::lock_guard<std::mutex> lock(m_Mutex);

    // If it's nullptr, do nothing (safe to call free on null in C/C++, but we skip the logs)
    if (ptr == nullptr) {
        return;
    }

    // Find the pointer in our allocation map
    auto it = m_Allocations.find(ptr);
    if (it != m_Allocations.end()) {
        // Retrieve the allocation info
        size_t size = it->second.size;
        std::string tag = it->second.tag;

        // Update counters
        m_TotalDeallocated += size;

        // Erase from the map before actually freeing
        m_Allocations.erase(it);

        // Free the memory
        std::free(ptr);

        // Log successful deallocation
        spdlog::info("[MemoryManager] Deallocated {} bytes at {}, Tag='{}'", size, ptr, tag);
    }
    else {
        // Attempt to free something not found in our map
        spdlog::warn("[MemoryManager] Attempted to free unknown or already freed pointer {}", ptr);
    }

    // End profiling and log the duration in microseconds
    auto endTime = std::chrono::steady_clock::now();
    auto durationMicro = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    LOG_PROFILE_TRACE("[MemoryManager][Profiling] Deallocate took {}us (Ptr={})", durationMicro, ptr);
}

void MemoryManager::PrintMemoryUsage() const {
    // Acquire the lock to safely iterate over allocations
    std::lock_guard<std::mutex> lock(m_Mutex);

    // Log a header
    spdlog::info("[MemoryManager] Current Memory Usage:");

    // For each pointer tracked, log its info
    for (const auto& [ptr, info] : m_Allocations) {
        spdlog::info("  Address: {}, Size: {} bytes, Tag: '{}'", ptr, info.size, info.tag);
    }
}

bool MemoryManager::HasMemoryLeaks() const {
    // Acquire the lock for thread safety
    std::lock_guard<std::mutex> lock(m_Mutex);

    // If m_Allocations is empty, then no leaks
    return !m_Allocations.empty();
}
