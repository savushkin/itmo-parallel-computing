cmake_minimum_required(VERSION 3.9)
project(itmo-parallel-computing C)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -O3")

add_executable(lab1 src/lab1.c)
add_executable(lab2 src/lab2.c)
add_executable(lab3 src/lab3.c)
add_executable(lab4 src/lab4.c)