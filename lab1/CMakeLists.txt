cmake_minimum_required(VERSION 3.6)
project(lab1)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_COMPILE_FEATURES -Wpedantic -Wall -Werror)

set(SOURCE_FILES solution.c solution.h)
add_executable(lab1 ${SOURCE_FILES})
