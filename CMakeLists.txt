cmake_minimum_required(VERSION 4.0)
project(Project_C_ESIEA_ C)

set(CMAKE_C_STANDARD 11)

add_executable(Project_C_ESIEA_ src/main.c
        include/grandentier.h
        src/grandentier.c)
