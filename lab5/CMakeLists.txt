cmake_minimum_required(VERSION 3.23)
project(lab5 C)

set(CMAKE_C_STANDARD 99)

add_executable(lab5 main.c graph.c graph.h userFunc.c userFunc.h checkFunc.c checkFunc.h error.h hashTable.c hashTable.h func.h)
