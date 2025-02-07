set(Catch2_DIR "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/catch2")
find_package(Catch2 CONFIG REQUIRED)


add_executable(3DGameEngineTests
    test_main.cpp
    test_Memory.cpp
    test_JobSystem.cpp
    test_Renderer.cpp
)

target_link_libraries(3DGameEngineTests PRIVATE 3DGameEngine Catch2::Catch2WithMain)

enable_testing()
add_test(NAME 3DGameEngineTests COMMAND 3DGameEngineTests)

