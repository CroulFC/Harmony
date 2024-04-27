cmake_minimum_required(VERSION 3.0)

project(Harmony)

set(CMAKE_CXX_STANDART 20)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
set(BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Bin)

file(GLOB WAVFILE_SOURCE ${SOURCE_DIR}/WavFile/*.cpp ${SOURCE_DIR}/WavFile/*.h)
add_library(WavFile STATIC ${WAVFILE_SOURCE})

file(GLOB DATAENCODER_SOURCE ${SOURCE_DIR}/DataEncoder/*.cpp ${SOURCE_DIR}/DataEncoder/*.h)
add_executable(DataEncoder ${DATAENCODER_SOURCE})

target_link_libraries(DataEncoder PRIVATE WavFile)

set_target_properties(WavFile PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${BINARY_DIR}
)
set_target_properties(DataEncoder PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR}
)

