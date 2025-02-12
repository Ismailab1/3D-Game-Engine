cmake_minimum_required(VERSION 3.20)
project(3DGameEngine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(CMAKE_PREFIX_PATH "E:/VisualStudio/tools/vcpkg/installed/x64-windows")

if (MSVC)
    add_compile_options(/utf-8)
endif()


enable_testing()

# Add subdirectories
add_subdirectory(engine)
add_subdirectory(Sandbox)
add_subdirectory(tests)
