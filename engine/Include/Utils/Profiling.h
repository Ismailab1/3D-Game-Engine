#ifndef PROFILING_H
#define PROFILING_H

#include <chrono>
#include <string>
#include <unordered_map>
#include <mutex>
#include <spdlog/spdlog.h>

/**
 * @class Profiling
 * @brief A singleton utility class for basic frame timing (FPS),
 *        scoped/named timers, and memory usage logging.
 *
 * Typical usage:
 *  - At the start of each frame:   Profiling::StartFrame();
 *  - At the end of each frame:     Profiling::EndFrame();
 *  - (Optional) Start/stop timers for specific code blocks:
 *      Profiling::StartTimer("Physics");
 *      ... do physics ...
 *      Profiling::EndTimer("Physics");
 *  - Query MemoryManager stats:
 *      Profiling::LogMemoryUsage();
 */
class Profiling {
public:
    /**
     * @brief Get the singleton instance of the Profiling system.
     * @return A reference to the single Profiling object.
     */
    static Profiling& GetInstance();

    /**
     * @brief Record the current time as the 'start' of a frame.
     *        Typically called once per frame, at the beginning.
     */
    static void StartFrame();

    /**
     * @brief Record the end of a frame, calculate frame time and FPS,
     *        and log the information.
     */
    static void EndFrame();

    /**
     * @brief Begin timing a named code section.
     * @param name A unique name/identifier for this timer (e.g., "Physics", "AI", etc.)
     */
    static void StartTimer(const std::string& name);

    /**
     * @brief Stop the named timer and log the elapsed time in milliseconds.
     * @param name The name/identifier for the timer that was started.
     */
    static void EndTimer(const std::string& name);

    /**
     * @brief Logs memory usage stats (allocated, deallocated, current usage)
     *        by querying the MemoryManager.
     */
    static void LogMemoryUsage();

private:
    // Private constructor & destructor ensure singleton usage
    Profiling() = default;
    ~Profiling() = default;

    // Delete copy-constructor and copy-assignment to prevent copies
    Profiling(const Profiling&) = delete;
    Profiling& operator=(const Profiling&) = delete;

    // Alias for convenience
    using HighResClock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<HighResClock>;
    using Duration = std::chrono::duration<double>;
    using TimerMap = std::unordered_map<std::string, TimePoint>;

    // ------------------- STATIC MEMBERS -------------------

    /**
     * @brief Tracks the time when the last frame began.
     */
    static TimePoint      s_LastFrameTime;

    /**
     * @brief Duration (in seconds) of the most recent frame.
     */
    static double         s_FrameTime; // seconds

    /**
     * @brief The computed frames per second (FPS).
     */
    static double         s_FPS;

    /**
     * @brief A map from timer names to their start times,
     *        allowing multiple named timers.
     */
    static TimerMap       s_Timers;

    /**
     * @brief A mutex to guard shared data (s_Timers) in multi-threaded scenarios.
     */
    static std::mutex     s_Mutex;
};

#endif // PROFILING_H
