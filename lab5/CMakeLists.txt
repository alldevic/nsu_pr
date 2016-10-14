cmake_minimum_required(VERSION 3.6)
project(lab5)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi -Wpedantic -Wall -Werror")

set(SOURCE_FILES solution.c solution.h)
add_executable(lab4 ${SOURCE_FILES})