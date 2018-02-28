cmake_minimum_required(VERSION 3.9)
project(lab1 C)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -lm -O3")

add_executable(lab1 lab1.c)

target_link_libraries(lab1 m)