cmake_minimum_required(VERSION 3.10)
project(FractalHorizon3DEngine VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE SRC_FILES
    ${SRC_DIR}/*.cpp
    ${SRC_DIR}/*.c
)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/GLFW)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glm)


add_executable(FractalHorizon3DEngine ${SRC_FILES})

target_include_directories(FractalHorizon3DEngine PRIVATE
    ${SRC_DIR}
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}/external/GLFW/include
    ${CMAKE_SOURCE_DIR}/external/glm
)
set_target_properties(FractalHorizon3DEngine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
target_link_libraries(FractalHorizon3DEngine PRIVATE glfw)
target_link_libraries(FractalHorizon3DEngine PRIVATE glm)
if (WIN32)
    target_compile_definitions(FractalHorizon3DEngine PRIVATE PLATFORM_WINDOWS)
elseif(UNIX)
    target_compile_definitions(FractalHorizon3DEngine PRIVATE PLATFORM_LINUX)
endif()
