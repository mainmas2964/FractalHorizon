cmake_minimum_required(VERSION 3.10)
project(FractalHorizon3DEngine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
file(GLOB_RECURSE SRC_FILES ${SRC_DIR}/*.cpp)

add_executable(FractalHorizon3DEngine ${SRC_FILES})
target_include_directories(FractalHorizon3DEngine PRIVATE ${SRC_DIR})
set_target_properties(FractalHorizon3DEngine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


