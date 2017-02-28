cmake_minimum_required(VERSION 3.6)
project(lab2)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_COMPILE_FEATURES -Wpedantic -Wall -Werror -pipe -Og -march=native -mtune=native)

set(SOURCE_FILES solution.c solution.h)
add_executable(lab2 ${SOURCE_FILES})
