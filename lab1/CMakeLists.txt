cmake_minimum_required(VERSION 3.9)
project(lab1 C)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -O3")

add_executable(lab1-seq lab1.c)
add_executable(lab1-par-2 lab1.c)
add_executable(lab1-par-4 lab1.c)
add_executable(lab1-par-5 lab1.c)
add_executable(lab1-par-8 lab1.c)

target_link_libraries(lab1-seq m)
target_link_libraries(lab1-par-2 m)
target_link_libraries(lab1-par-4 m)
target_link_libraries(lab1-par-5 m)
target_link_libraries(lab1-par-8 m)

SET_TARGET_PROPERTIES(lab1-par-2 PROPERTIES
        COMPILE_FLAGS "-floop-parallelize-all -ftree-parallelize-loops=2")
SET_TARGET_PROPERTIES(lab1-par-4 PROPERTIES
        COMPILE_FLAGS "-floop-parallelize-all -ftree-parallelize-loops=4")
SET_TARGET_PROPERTIES(lab1-par-5 PROPERTIES
        COMPILE_FLAGS "-floop-parallelize-all -ftree-parallelize-loops=5")
SET_TARGET_PROPERTIES(lab1-par-8 PROPERTIES
        COMPILE_FLAGS "-floop-parallelize-all -ftree-parallelize-loops=8")