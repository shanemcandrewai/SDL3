#[[

# build with shared prebuilt SDL3 library (DLL)

cmake -B build -DCMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang++.exe" -DSDL_STATIC=OFF -DSDL_SHARED=ON -G "Ninja Multi-Config" 
cmake -B build -DCMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang++.exe" -DSDL_STATIC=ON -DSDL_SHARED=OFF -G "Ninja Multi-Config" 

cmake --build build
.\build\Debug\checkers_clang.exe

cmake --build build --config Release
.\build\Release\checkers_clang.exe

]]

cmake_minimum_required(VERSION 3.5)

project(checkers_clang)

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/../blender/assets DESTINATION ${CMAKE_CURRENT_LIST_DIR})

add_executable(${PROJECT_NAME} WIN32 ../checkers.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
