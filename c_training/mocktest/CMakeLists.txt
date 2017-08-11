cmake_minimum_required (VERSION 2.6)
project (mocktest)

set (mocktest_VERSION_MAJOR 1)
set (mocktest_VERSION_MINOR 0)

file(GLOB SOURCES "app/*.c" "fat/*.c" "hal/*.c" "util/*.c")

include_directories("${PROJECT_BINARY_DIR}")

# add the executable
add_executable(mocktest ${SOURCES})
