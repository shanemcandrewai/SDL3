#[[
cmake -B build -DCMAKE_CXX_COMPILER=clang++ -DSDL_STATIC=ON -DSDL_SHARED=OFF -G "Ninja Multi-Config"
cmake -B build -DCMAKE_CXX_COMPILER=clang++ -DSDL_STATIC=OFF -DSDL_SHARED=ON -G "Ninja Multi-Config"

cmake --build build
cmake --install build

cmake --build build --config Release
cmake --install build --config Release

cmake --build build --config Release && cmake --install build --config Release && cd install && .\checkers.exe && cd ..
]]

cmake_minimum_required(VERSION 3.23)

project(checkers)

add_subdirectory(src)
