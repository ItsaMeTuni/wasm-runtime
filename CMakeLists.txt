cmake_minimum_required(VERSION 3.21)
project(wasm_runtime CXX)

set(CMAKE_CXX_STANDARD 23)

include_directories(.)

add_executable(wasm_runtime
        bytecode.cpp
        bytecode.h
        main.cpp
        runtime.cpp
        runtime.h
        ModuleParser.cpp
        ModuleParser.h
        Module.h
        Module.cpp
    )
