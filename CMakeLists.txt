cmake_minimum_required(VERSION 3.10)
project(MyProject LANGUAGES CXX)

# Dichiara il file sorgente
add_executable(devtools 
    src/main.cpp
)

# Compilatore (C++17 o 20, ad esempio)
set(CMAKE_CXX_STANDARD 17)

# Pulisci le cartelle se vuoi fare un pulito prima
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(_DEBUG)
endif()