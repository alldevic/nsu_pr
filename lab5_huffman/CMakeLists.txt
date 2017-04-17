cmake_minimum_required(VERSION 3.7)
project(lab5)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_COMPILE_FEATURES -Wall -Werror -Wpedantic -pipe -Og)
set(SOURCE_FILES solution.c solution.h)
add_executable(lab5 ${SOURCE_FILES})
