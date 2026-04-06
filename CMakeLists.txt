#[[

# build with shared prebuilt SDL3 library (DLL)

cmake -B build -G "Ninja Multi-Config" 

cmake --build build
build\Debug\hello.exe

cmake --build build --config Release
build\Release\hello.exe

]]

cmake_minimum_required(VERSION 3.20)

project(hello)

set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang++.exe")


add_library(SDL3 SHARED IMPORTED)
set_target_properties(SDL3 PROPERTIES
INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/SDL3-3.4.2/include"
IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/SDL3-3.4.2/lib/x64/SDL3.lib" # SHARED
)

add_executable(${PROJECT_NAME} hello.cpp)
target_link_libraries(${PROJECT_NAME} SDL3)

#set(ENV{CMAKE_GENERATOR} Ninja)
#add_subdirectory(emscripten)
