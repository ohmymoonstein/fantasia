project(fantasia)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wfatal-errors -fvisibility=hidden -pedantic -Wl,--no-undefined -fPIC -Wall -Wextra -Wconversion -Werror=return-type")

if (ENABLE_PROFILING)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -O3")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O3")
endif()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0")

add_executable(assembler
    "assembler/util.cc"
    "assembler/main.cc"
    "assembler/semantic.cc"
    "assembler/lexer.cc"
    "assembler/debugging.cc"
    "assembler/parser.cc")
set_target_properties(assembler PROPERTIES
    OUTPUT_NAME "fasm"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" )
