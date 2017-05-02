cmake_minimum_required(VERSION 3.6)
set(PROJECT_NAME "lab5")

project(${PROJECT_NAME})

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../CMake")
include(cotire OPTIONAL)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpedantic -Wall -Wextra -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pipe")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mtune=native -march=native")

set(SOURCE_FILES main.c main.h)
set(SOURCE_FILES ${SOURCE_FILES} list.c list.h)
set(SOURCE_FILES ${SOURCE_FILES} errors.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

if (COMMAND cotire)
    set(COTIRE_MAXIMUM_NUMBER_OF_UNITY_INCLUDES "-j4")
    cotire(${PROJECT_NAME})
endif()

