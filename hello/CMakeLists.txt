#[[

# build all

cmake -B build -G Ninja
cmake --build build

# test hello_clang
.\hello_clang\build\hello_clang.exe

# testing hello_emscripten
cd hello_emscripten
python -m http.server 8000
http://localhost:8000/

]]

cmake_minimum_required(VERSION 3.25)

project(hello_all)

add_subdirectory(hello_clang)
add_subdirectory(hello_emscripten)
