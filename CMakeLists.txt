cmake_minimum_required(VERSION 3.0)

project(Harmony)

set(CMAKE_CXX_STANDART 20)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
set(BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Bin)

file(GLOB_RECURSE SOURCE_FILES ${SOURCE_DIR}/*.cpp ${SOURCE_DIR}/*.h)

add_executable(Harmony ${SOURCE_FILES})

set_target_properties(Harmony PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR})

