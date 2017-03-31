cmake_minimum_required(VERSION 3.6)
project(lab3_adj)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_COMPILE_FEATURES -Wpedantic -Wall -Werror -pipe -Og)

set(SOURCE_FILES solution.c solution.h)
add_executable(lab3 ${SOURCE_FILES})
