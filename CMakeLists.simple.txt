# https://github.com/libsdl-org/SDL/blob/main/docs/INTRO-mingw.md
# MSYS2 UCRT64 prompt
# cmake -S . -B build
# cmake --build build
# copy SDL3.dll from https://github.com/libsdl-org/SDL/releases/tag/release-3.4.2 to build
cmake_minimum_required(VERSION 3.26)
project(hello C CXX)

find_package(SDL3 REQUIRED)

add_executable(hello)

target_sources(hello
PRIVATE
    hello.c
)

target_link_libraries(hello SDL3::SDL3)