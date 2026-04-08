#[[

# build with static SDL3 library
cmake -B build -DCMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang++.exe" -DSDL_STATIC=ON -DSDL_SHARED=OFF -G "Ninja Multi-Config" 

# build with shared/DLL SDL3 library
cmake -B build -DCMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang++.exe" -DSDL_STATIC=OFF -DSDL_SHARED=ON -G "Ninja Multi-Config" 

cmake --build build
cmake --install build

cmake --build build --config Release
cmake --install build --config Release

# test
.\install\hello.exe

]]

cmake_minimum_required(VERSION 3.23)

project(hello)

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL) #git clone https://github.com/libsdl-org/SDL

add_executable(${PROJECT_NAME} WIN32 hello.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_LIST_DIR}/install)
install(TARGETS SDL3-shared DESTINATION ${CMAKE_CURRENT_LIST_DIR}/install)
