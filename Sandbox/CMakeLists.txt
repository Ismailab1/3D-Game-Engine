add_executable(Sandbox Sandbox.cpp)

find_package(spdlog CONFIG REQUIRED)

# 3DGameEngine exposes engine headers as PUBLIC; also link spdlog for Logger.h
target_link_libraries(Sandbox PRIVATE
    3DGameEngine
    spdlog::spdlog
)
