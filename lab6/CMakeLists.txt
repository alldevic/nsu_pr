cmake_minimum_required(VERSION 3.6)
project(lab6)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi -Wall -Wpedantic -Werror")

set(SOURCE_FILES solution.c solution.h)
add_executable(lab6 ${SOURCE_FILES})
