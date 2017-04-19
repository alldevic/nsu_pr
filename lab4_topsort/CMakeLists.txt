cmake_minimum_required(VERSION 3.6)
project(lab4)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpedantic -Wall -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pipe -Og")
set(SOURCE_FILES solution.c solution.h)

add_executable(lab4 ${SOURCE_FILES})

include(cotire)
cotire(lab4)
