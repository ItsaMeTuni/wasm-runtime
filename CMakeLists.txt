cmake_minimum_required(VERSION 3.21)
project(wasm_runtime C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(wasm_runtime
        bytecode.c
        bytecode.h
        common.h
        main.c
        runtime.c
        runtime.h)
