cmake_minimum_required(VERSION 3.9)
project(lab2 C)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -O3")

add_executable(lab2-seq lab2.c)
add_executable(lab2-par lab2.c)

target_link_libraries(lab2-seq m)
target_link_libraries(lab2-par m)