set(Catch2_DIR "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/catch2")
find_package(Catch2 CONFIG REQUIRED)
find_package(spdlog REQUIRED)

add_executable(3DGameEngineTests
    test_main.cpp
    test_Window.cpp
    test_Input.cpp
    test_Logger.cpp
    test_Application.cpp
    test_Memory.cpp
    test_JobSystem.cpp
    test_Renderer.cpp
)

# If your engine's public headers are in /engine/include, add that path:
target_include_directories(3DGameEngineTests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/engine/include
)

target_link_libraries(3DGameEngineTests
    PRIVATE
        3DGameEngine            # The engine library (already links SPDLog)
        Catch2::Catch2WithMain  # If using the "WithMain" variant
        spdlog::spdlog
)

enable_testing()
add_test(NAME 3DGameEngineTests COMMAND 3DGameEngineTests)
