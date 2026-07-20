find_package(Catch2 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

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

# 3DGameEngine exposes engine headers as PUBLIC; no extra include_directories needed
target_link_libraries(3DGameEngineTests
    PRIVATE
        3DGameEngine
        Catch2::Catch2   # test_main.cpp provides its own main() via CATCH_CONFIG_RUNNER
        spdlog::spdlog
)

enable_testing()
add_test(NAME 3DGameEngineTests COMMAND 3DGameEngineTests)
