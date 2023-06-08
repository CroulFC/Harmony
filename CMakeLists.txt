cmake_minimum_required(VERSION 3.0)

project(Harmony)

set(SOURCE_DIR ${CMAKE_CURENT)SOURCE_DIR}/Source)
set(BINARY_DIR ${CMAKE_CURENT)SOURCE_DIR}/Build)
set(CMAKE_TEMP_DIR ${CMAKE_CURENT)SOURCE_DIR}/CMakeFiles)

file(GLOB_RECURSE SOURCE_FILES ${SOURCE_DIR}/*.cpp)

add_executable(Harmony ${SOURCE_FILES})

set_target_properties(Harmony PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR})

message("CMake configuration is complite")

