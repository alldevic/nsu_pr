cmake_minimum_required(VERSION 3.6)
project(lab3_adj)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_COMPILE_FEATURES -Wpedantic -Wall -Werror -pipe -Ofast -mtune=native
        -march=native)

set(SOURCE_FILES solution.c solution.h)
add_executable(lab3_adj ${SOURCE_FILES})
