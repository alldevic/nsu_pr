cmake_minimum_required(VERSION 3.6)
project(lab9)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_COMPILE_FEATURES -Wall -Wpedantic -Werror)

set(SOURCE_FILES solution.c solution.h)
add_executable(lab9 ${SOURCE_FILES})
