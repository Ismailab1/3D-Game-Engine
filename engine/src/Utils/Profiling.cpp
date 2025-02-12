#include "Utils/Profiling.h"
#include "Utils/Logger.h"         // For logging macros
#include "Memory/MemoryManager.h" // For querying memory stats

#include <chrono>
#include <thread>
#include <spdlog/spdlog.h>

// Use C++ chrono in a shorter form
using namespace std::chrono;

// ------------------ Define static members ------------------

// Initialize static data members from Profiling.h
Profiling::TimePoint Profiling::s_LastFrameTime = high_resolution_clock::now();
double               Profiling::s_FrameTime = 0.0;
double               Profiling::s_FPS = 0.0;
Profiling::TimerMap  Profiling::s_Timers = {};
std::mutex           Profiling::s_Mutex;

// ----------------------------------------------------------
// Singleton instance retrieval
// ----------------------------------------------------------
Profiling& Profiling::GetInstance() {
    static Profiling instance;
    return instance;
}

// ----------------------------------------------------------
// FRAME TIMING
// ----------------------------------------------------------

/**
 * @brief Mark the start of a new frame by recording the current time.
 *        Called at the beginning of a render/update cycle.
 */
void Profiling::StartFrame() {
    // Acquire the lock in case other profiling methods are running concurrently
    std::lock_guard<std::mutex> lock(s_Mutex);
    s_LastFrameTime = high_resolution_clock::now();
}

/**
 * @brief Mark the end of a frame and calculate the frame duration + FPS.
 *        Logs the timing information.
 */
void Profiling::EndFrame() {
    // Acquire the lock to safely read/write the shared data
    std::lock_guard<std::mutex> lock(s_Mutex);

    auto now = high_resolution_clock::now();
    // Frame time in seconds
    double frameTimeSec = duration<double>(now - s_LastFrameTime).count();

    s_FrameTime = frameTimeSec;
    // Calculate frames per second
    s_FPS = (frameTimeSec > 0.0) ? (1.0 / frameTimeSec) : 0.0;

    // Example log message: "Frame Time: 16.667 ms, FPS: 60.00"
    // Using direct spdlog here, or you could use LOG_PROFILE_* macros
    spdlog::info("[Profiling] Frame Time: {:.3f} ms, FPS: {:.2f}",
        frameTimeSec * 1000.0, s_FPS);
}

// ----------------------------------------------------------
// NAMED TIMERS
// ----------------------------------------------------------

/**
 * @brief Start timing a custom/named section of code.
 * @param name A unique name or identifier for the timer.
 */
void Profiling::StartTimer(const std::string& name) {
    std::lock_guard<std::mutex> lock(s_Mutex);
    // Record the time at which this section started
    s_Timers[name] = high_resolution_clock::now();
}

/**
 * @brief End the named timer started by StartTimer() and log the result.
 * @param name The same name used in StartTimer.
 */
void Profiling::EndTimer(const std::string& name) {
    std::lock_guard<std::mutex> lock(s_Mutex);
    // Check if we actually have a timer with this name
    auto it = s_Timers.find(name);
    if (it != s_Timers.end()) {
        // Calculate elapsed time in milliseconds
        auto durationMs = duration_cast<milliseconds>(high_resolution_clock::now() - it->second).count();
        spdlog::info("[Profiling] '{}' took {} ms", name, durationMs);

        // Remove it from the map
        s_Timers.erase(it);
    }
    else {
        // If no timer is found for the given name, warn the user
        spdlog::warn("[Profiling] Timer '{}' not found!", name);
    }
}

// ----------------------------------------------------------
// MEMORY USAGE
// ----------------------------------------------------------

/**
 * @brief Query the MemoryManager for total allocated vs. deallocated
 *        and log the current memory usage.
 */
void Profiling::LogMemoryUsage() {
    // Acquire the MemoryManager
    MemoryManager& mm = MemoryManager::GetInstance();

    // You might implement these accessors in MemoryManager:
    //    size_t GetTotalAllocated() const;
    //    size_t GetTotalDeallocated() const;
    size_t totalAllocated = mm.GetTotalAllocated();   // Implement these in your MemoryManager
    size_t totalDeallocated = mm.GetTotalDeallocated(); // if not already done
    size_t currentUsage = totalAllocated - totalDeallocated;

    // Log the usage stats
    spdlog::info("[Profiling] Memory Usage - Allocated: {} bytes, Deallocated: {} bytes, Current: {} bytes",
        totalAllocated, totalDeallocated, currentUsage);
}
