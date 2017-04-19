cmake_minimum_required(VERSION 3.6)
project(lab5)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
include(cotire)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpedantic -Wall -Wextra -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mtune=native -march=native")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pipe")


set(SOURCE_FILES solution.c solution.h)

add_executable(lab5 ${SOURCE_FILES})


cotire(lab5)
