add_executable(Sandbox Sandbox.cpp)

# Ensure Sandbox can include the engine headers
target_include_directories(Sandbox PRIVATE 
    ${CMAKE_SOURCE_DIR}/engine/include
)

# Link Sandbox with the 3DGameEngine library
target_link_libraries(Sandbox PRIVATE 3DGameEngine)
