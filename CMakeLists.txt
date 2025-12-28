cmake_minimum_required(VERSION 3.20)

project(barrel_level
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Core solver library
add_library(barrel_level
    src/Barrel.cpp
    src/VolumeSolver.cpp
)

target_include_directories(barrel_level
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Compiler warnings
if (MSVC)
    target_compile_options(barrel_level PRIVATE /W4)
else()
    target_compile_options(barrel_level PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Command-line application
add_executable(barrel_cli
    apps/barrel_cli.cpp
)

target_link_libraries(barrel_cli
    PRIVATE barrel_level
)

# Tests
enable_testing()

add_executable(barrel_tests
    tests/test_volume.cpp
)

target_link_libraries(barrel_tests
    PRIVATE barrel_level
)

add_test(
    NAME barrel_tests
    COMMAND barrel_tests
)

