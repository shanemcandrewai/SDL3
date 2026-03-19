#https://cmake.org/cmake/help/book/mastering-cmake
# cmake -S . -B build
# cmake --build build
cmake_minimum_required(VERSION 3.20)

project(hello)

# add_subdirectory(SDL3-3.4.2 EXCLUDE_FROM_ALL)
target_link_libraries(hello PRIVATE SDL3-3.4.2/lib/x64/SDL3.lib)

target_include_directories(hello PUBLIC "${PROJECT_SOURCE_DIR}/SDL3-3.4.2/lib/x64/SDL3"
                          )

add_executable(hello)
target_sources(hello
  PRIVATE
    hello.c
)
