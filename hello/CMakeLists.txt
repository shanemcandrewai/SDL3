#[[

cmake -B build -DCMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang++.exe" -G "Ninja Multi-Config" 

cmake --build build
.\build\Debug\hello.exe

cmake --build build --config Release
.\build\Release\hello.exe

]]

cmake_minimum_required(VERSION 3.5)

project(hello)

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_executable(${PROJECT_NAME} WIN32 hello.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
