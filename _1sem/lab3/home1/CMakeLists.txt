cmake_minimum_required(VERSION 3.6)
project(home1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

set(SOURCE_FILES main.c)
add_executable(home1 ${SOURCE_FILES})