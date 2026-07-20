cmake_minimum_required(VERSION 3.20)

# Auto-discover or bootstrap vcpkg. Must come before project() so that the
# toolchain file is active when CMake detects the compiler and languages.
include(cmake/Vcpkg.cmake)

project(3DGameEngine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

enable_testing()

add_subdirectory(engine)
add_subdirectory(Sandbox)
add_subdirectory(tests)
