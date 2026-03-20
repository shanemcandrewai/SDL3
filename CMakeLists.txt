#https://cmake.org/cmake/help/book/mastering-cmake
# cmake -S . -B build
# cmake --build build
cmake_minimum_required(VERSION 3.20)

project(hello)
message("dddd")
include_directories(SDL3-3.4.2/include)
# add_subdirectory(SDL3-3.4.2/cmake)
add_library(SDL3.lib SHARED  IMPORTED)
set_target_properties(SDL3.lib PROPERTIES IMPORTED_LOCATION SDL3-3.4.2/lib/x64/SDL3.lib)

add_executable(hello WIN32 hello.c)
target_sources(hello
  PRIVATE
    hello.c
)

target_link_libraries(hello SDL3.lib)