cmake_minimum_required(VERSION 3.20)

# Project configuration
project(3DGameEngine VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# (Optional) Vcpkg toolchain file – for other libraries
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Enable testing
enable_testing()

#
# -- Explicitly set each library's directory before find_package() --
#
set(glfw3_DIR   "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/glfw3")
set(GLEW_DIR    "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/glew")
set(glm_DIR     "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/glm")
set(assimp_DIR  "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/assimp")
set(Catch2_DIR  "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/catch2")
set(TBB_DIR     "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/tbb")
set(SDL2_DIR    "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/sdl2")
set(imgui_DIR   "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/imgui")
set(Bullet_DIR "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/bullet3")
set(nlohmann_json_DIR "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/nlohmann_json")
set(fmt_DIR "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/fmt")
set(spdlog_DIR  "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/spdlog")

find_package(glfw3   CONFIG REQUIRED)
find_package(GLEW    CONFIG REQUIRED)
find_package(glm     CONFIG REQUIRED)
find_package(assimp  CONFIG REQUIRED)
find_package(Catch2  CONFIG REQUIRED)
find_package(TBB     CONFIG REQUIRED)
find_package(SDL2    CONFIG REQUIRED)
find_package(imgui   CONFIG REQUIRED)
find_package(Bullet CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog  CONFIG REQUIRED)

# Source and header files
file(GLOB_RECURSE ENGINE_SOURCES src/engine/*.cpp)
file(GLOB_RECURSE ENGINE_HEADERS include/engine/*.h)

# Include the Game.cpp file explicitly as the entry point
set(GAME_MAIN_SOURCE src/game/Game.cpp)

# Include all other game-related source files
file(GLOB_RECURSE GAME_OTHER_SOURCES src/game/*.cpp)
list(REMOVE_ITEM GAME_OTHER_SOURCES ${GAME_MAIN_SOURCE})

file(GLOB_RECURSE GAME_HEADERS include/game/*.h)

# Main executable
add_executable(3DGameEngine
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
    ${GAME_MAIN_SOURCE}
    ${GAME_OTHER_SOURCES}
    ${GAME_HEADERS}
 "include/engine/SceneManagement.h" "src/engine/SceneManagement.cpp" "include/engine/PhysicsSystem.h" "include/engine/InputSystem.h" "include/engine/DebuggingSystem.h" "src/engine/InputSystem.cpp" "src/engine/PhysicsSystem.cpp" "src/engine/DebuggingSystem.cpp" "tests/engine/TestingAssetManager.cpp" "tests/engine/TestingDebuggingSystem.cpp" "tests/engine/TestingInputSystem.cpp" "tests/engine/TestingMemoryAllocator.cpp" "tests/engine/TestingSceneManagement.cpp" "tests/engine/TestingPhysicsSystem.cpp")

# Link dependencies to the main executable
target_link_libraries(3DGameEngine PRIVATE 
    glfw 
    GLEW::GLEW 
    TBB::tbb 
    SDL2::SDL2 
    SDL2::SDL2main 
    imgui::imgui 
    nlohmann_json::nlohmann_json 
    spdlog::spdlog 
    BulletDynamics 
    BulletCollision 
    LinearMath)

# Add Assimp explicitly (via full .lib path)
target_link_libraries(3DGameEngine PRIVATE "E:/VisualStudio/tools/vcpkg/installed/x64-windows/lib/assimp-vc143-mt.lib")

# Include GLM headers explicitly
target_include_directories(3DGameEngine PRIVATE "$ENV{VCPKG_ROOT}/installed/x64-windows/include")

# Test executable
file(GLOB TEST_SOURCES tests/*.cpp)
add_executable(3DGameEngineTests ${TEST_SOURCES} "include/engine/SceneManagement.h" "src/engine/SceneManagement.cpp" "include/engine/PhysicsSystem.h" "include/engine/InputSystem.h" "include/engine/DebuggingSystem.h" "src/engine/InputSystem.cpp" "src/engine/PhysicsSystem.cpp" "src/engine/DebuggingSystem.cpp" "tests/engine/TestingAssetManager.cpp" "tests/engine/TestingDebuggingSystem.cpp" "tests/engine/TestingInputSystem.cpp" "tests/engine/TestingMemoryAllocator.cpp" "tests/engine/TestingSceneManagement.cpp" "tests/engine/TestingPhysicsSystem.cpp")

# Link dependencies to the test executable
target_link_libraries(3DGameEngineTests PRIVATE 
    glfw 
    GLEW::GLEW 
    TBB::tbb 
    Catch2::Catch2WithMain 
    SDL2::SDL2 
    SDL2::SDL2main 
    imgui::imgui 
    nlohmann_json::nlohmann_json 
    spdlog::spdlog 
    BulletDynamics 
    BulletCollision 
    LinearMath)
target_link_libraries(3DGameEngineTests PRIVATE "E:/VisualStudio/tools/vcpkg/installed/x64-windows/lib/assimp-vc143-mt.lib")
target_include_directories(3DGameEngineTests PRIVATE "$ENV{VCPKG_ROOT}/installed/x64-windows/include")

# Discover Catch2 tests
include(Catch)
catch_discover_tests(3DGameEngineTests)
