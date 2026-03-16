#https://cmake.org/cmake/help/book/mastering-cmake
# cmake -S . -B build
# cmake --build build
cmake_minimum_required(VERSION 3.20)
project(hello)
add_executable(hello hello.c)

