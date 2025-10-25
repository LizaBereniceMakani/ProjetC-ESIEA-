cmake_minimum_required(VERSION 3.20)
project(ProjetC_ESIEA C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(ProjetC_ESIEA
        src/main.c
        src/grandentier.c
)
