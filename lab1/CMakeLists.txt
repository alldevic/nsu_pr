cmake_minimum_required(VERSION 3.6)
project(lab1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra -Wall -Wpedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -ansi")
set(SOURCE_FILES solution.c solution.h)
add_executable(lab1 ${SOURCE_FILES})
