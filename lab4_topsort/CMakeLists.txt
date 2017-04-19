cmake_minimum_required(VERSION 3.6)
project(lab4)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpedantic -Wall -Wextra -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mtune=native -march=native")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pipe")


set(SOURCE_FILES solution.c solution.h)
    
add_executable(lab4 ${SOURCE_FILES})

include(cotire)
cotire(lab4)
