# Set CMake version and project name
cmake_minimum_required(VERSION 3.20)
project(3DGameEngine LANGUAGES CXX)

# Define the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Explicitly define library directories
set(GLFW3_DIR             "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/glfw3")
set(GLEW_DIR              "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/glew")
set(TBB_DIR               "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/tbb")
set(SDL2_DIR              "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/sdl2")
set(IMGUI_DIR             "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/imgui")
set(NLOHMANN_JSON_DIR     "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/nlohmann_json")
set(SPDLOG_DIR            "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/spdlog")
set(BULLET_DIR            "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/bullet3")
set(ASSIMP_DIR            "E:/VisualStudio/tools/vcpkg/installed/x64-windows/share/assimp")

# Find required packages
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Bullet CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

# Create static library for 3DGameEngine
add_library(3DGameEngine STATIC
    src/Core/Application.cpp include/Core/Application.h
    src/Memory/MemoryManager.cpp include/Memory/MemoryManager.h
    src/Threading/JobSystem.cpp include/Threading/JobSystem.h
    src/Renderer/Renderer.cpp include/Renderer/Renderer.h
    src/Physics/Physics.cpp include/Physics/Physics.h
    src/IO/FileSystem.cpp include/IO/FileSystem.h
    src/Utils/Logger.cpp include/Utils/Logger.h
)

# Ensure all include directories are properly set
target_include_directories(3DGameEngine PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    "E:/VisualStudio/tools/vcpkg/installed/x64-windows/include"
)

# Explicitly link required libraries
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
    LinearMath
    assimp::assimp
)
