cmake_minimum_required(VERSION 3.0.0)
project(Grass VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

set(GLFW_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/libs/glfw/include)
set(GLFW_LIB ${PROJECT_SOURCE_DIR}/libs/glfw/lib/x64/glfw3.lib)
include_directories(${GLFW_INCLUDE_DIR})

add_executable(Grass
        src/Main.cpp
        src/shader/Shader.cpp
        src/world/Ground.cpp
        src/world/Grass.cpp
        src/Camera.cpp
        src/PerlinNoise.hpp
)

add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glm)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glad)

target_link_libraries(Grass PRIVATE glm)
target_link_libraries(Grass PRIVATE ${GLFW_LIB})
target_link_libraries(Grass PRIVATE glad)