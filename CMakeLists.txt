#https://cmake.org/cmake/help/book/mastering-cmake
# cls && cmake -S . -B build && cmake --build build && build\Debug\hello.exe
cmake_minimum_required(VERSION 3.20)

project(hello)

add_library(SDL3 SHARED IMPORTED)
set_target_properties(SDL3 PROPERTIES
INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/SDL3-3.4.2/include"
IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/SDL3-3.4.2/lib/x64/SDL3.lib"
)

add_executable(hello)
target_sources(hello PRIVATE hello.cpp)
target_link_libraries(hello SDL3)