#https://cmake.org/cmake/help/book/mastering-cmake
# cls && cmake -S . -B build && cmake --build build && build\Debug\hello.exe
# cls && cmake -S . -B build && cmake --build build --config Release && build\Release\hello.exe
# build static SDL library
# cls && cmake -S vendored\SDL -B build -DSDL_STATIC=ON && cmake --build build

cmake_minimum_required(VERSION 3.20)

project(hello)

include_directories("${CMAKE_SOURCE_DIR}/vendored/SDL/include" "${CMAKE_SOURCE_DIR}/vendored/SDL/include/build_config")
add_library(SDL3 STATIC "${CMAKE_SOURCE_DIR}/vendored/SDL/src/SDL.c")
# set_target_properties(SDL3 PROPERTIES
# LINKER_LANGUAGE CXX
# INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/vendored/SDL/include/SDL3"
# IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/SDL3-3.4.2/lib/x64/SDL3.lib"
# IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/SDLinstall/lib/SDL3-static.lib"
# )

add_executable(${PROJECT_NAME} hello.cpp)
target_link_libraries(${PROJECT_NAME} SDL3)