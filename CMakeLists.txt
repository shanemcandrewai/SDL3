#https://cmake.org/cmake/help/book/mastering-cmake
# cmake -S . -B build
# cmake --build build
cmake_minimum_required(VERSION 3.20)

project(hello)
include_directories(SDL3-3.4.2/include)
add_library(SDL3::SDL3-shared SHARED IMPORTED)
set_target_properties(SDL3::SDL3-shared PROPERTIES
IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/SDL3-3.4.2/lib/x64/SDL3.lib"
)

add_executable(hello hello.cpp)
target_sources(hello PRIVATE hello.cpp)
target_link_libraries(hello PRIVATE SDL3::SDL3-shared)