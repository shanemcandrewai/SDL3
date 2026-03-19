#https://cmake.org/cmake/help/book/mastering-cmake
# cmake -S . -B build
# cmake --build build
cmake_minimum_required(VERSION 3.20)

project(hello)

include_directories(SDL3-3.4.2/include/SDL3)
# add_subdirectory(SDL3-3.4.2 EXCLUDE_FROM_ALL)
add_library(SDL3.lib SDL3-3.4.2/lib/x64/)


add_executable(hello)
target_sources(hello
  PRIVATE
    hello.c
)
