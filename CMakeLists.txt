cmake_minimum_required(VERSION 3.0)

project(Harmony)

#set(CMAKE_CXX_COMPILER g++)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
set(BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Build)
set(CMAKE_TEMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CMakeFiles)

file(GLOB_RECURSE SOURCE_FILES ${SOURCE_DIR}/*.cpp)

add_executable(Harmony ${SOURCE_FILES})

set_target_properties(Harmony PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR})

message("Cmake configuration is complite.")

