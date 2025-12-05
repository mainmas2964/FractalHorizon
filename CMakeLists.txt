cmake_minimum_required(VERSION 3.26)
project(FractalHorizon3DEngine VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)

add_subdirectory(${EXTERNAL_DIR}/GLFW)
add_subdirectory(${EXTERNAL_DIR}/glm)

# Все cpp/c файлы
file(GLOB_RECURSE SRC_FILES
    ${SRC_DIR}/*.cpp
    ${SRC_DIR}/*.c
)

add_executable(FractalHorizon3DEngine ${SRC_FILES})

target_include_directories(FractalHorizon3DEngine PRIVATE
    ${SRC_DIR}
    ${EXTERNAL_DIR}
    ${EXTERNAL_DIR}/GLFW/include
    ${EXTERNAL_DIR}/glm
)

target_link_libraries(FractalHorizon3DEngine PRIVATE glfw glm)

if (WIN32)
    target_compile_definitions(FractalHorizon3DEngine PRIVATE PLATFORM_WINDOWS)
elseif(UNIX)
    target_compile_definitions(FractalHorizon3DEngine PRIVATE PLATFORM_LINUX)
endif()

set_target_properties(FractalHorizon3DEngine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
