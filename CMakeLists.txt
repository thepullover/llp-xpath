cmake_minimum_required(VERSION 3.20)
project(lab2_llp C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(lab2_llp include/structure.h src/structure.c src/parser.c src/io_handler.c src/main.c)