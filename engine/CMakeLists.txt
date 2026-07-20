# Set CMake version and project name
cmake_minimum_required(VERSION 3.20)
project(3DGameEngine LANGUAGES CXX)

# Define the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages actually used by implemented code
find_package(glfw3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# Create static library for 3DGameEngine
add_library(3DGameEngine STATIC
    src/Core/Application.cpp Include/Core/Application.h
    src/Core/Window.cpp      Include/Core/Window.h
    src/Core/Input.cpp       Include/Core/Input.h
    src/Memory/MemoryManager.cpp Include/Memory/MemoryManager.h
    src/Threading/JobSystem.cpp  Include/Threading/JobSystem.h
    src/Renderer/Renderer.cpp    Include/Renderer/Renderer.h
    src/Physics/Physics.cpp      Include/Physics/Physics.h
    src/IO/FileSystem.cpp        Include/IO/FileSystem.h
    src/Utils/Logger.cpp         Include/Utils/Logger.h
    src/Utils/Profiling.cpp      Include/Utils/Profiling.h
)

# Public so consumers (Sandbox, tests) get engine headers automatically
target_include_directories(3DGameEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
)

target_link_libraries(3DGameEngine PRIVATE
    glfw
    spdlog::spdlog
)
