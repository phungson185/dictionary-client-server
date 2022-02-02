cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(dict)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Wall")

add_subdirectory(src)
add_subdirectory(src/client)
add_subdirectory(src/server)