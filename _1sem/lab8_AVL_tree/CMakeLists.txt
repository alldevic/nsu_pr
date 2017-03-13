cmake_minimum_required(VERSION 3.6)
project(lab8)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra -Wall -Wpedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -ansi")

set(SOURCE_FILES solution.c)
add_executable(lab8 ${SOURCE_FILES})